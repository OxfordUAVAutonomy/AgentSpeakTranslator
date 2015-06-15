/*
This file is part of AgentSpeakTranslator.

Copyright (c) 2015, Samuel Bucheli, Department of Computer Science, University
of Oxford. All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * @file agentspeak_main.cpp
 *
 * @brief Main entry point of the AgentSpeak translator plus some helper methods.
 *
 * @author Samuel Bucheli
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <utility>

#include "agentspeak_parser_driver.h"
#include "config_parser_driver.h"
#include "agent_translator.h"

#include "trigger_graph.h"

#include "agentspeak_syntax.h"

/**
 * @brief Splits a full path to a file into the filenmae and the base path.
 */
std::pair<std::string, std::string> split_path(std::string path)
{
  std::string base_path;
  std::string file_name;
  size_t split_point = path.find_last_of("/");
  if (split_point != std::string::npos)
    base_path = path.substr(0, split_point+1);
  file_name = path.substr(split_point+1);
  return std::pair<std::string, std::string>(base_path, file_name);
}

/**
 * @brief Creates a streambuffer for the given file_name.
 *
 * @param file_name The name of the file to be opened.
 * @param file The file stream for the file to be opened in.
 * @param description Can be used to give a custom description of the desired file in case of a missing file name.
 */
std::streambuf* getBuf(const std::string &file_name, std::ofstream &file, const std::string description)
{
  std::streambuf* buf;
  if (file_name != "")
  {
    file.open(file_name);
    if (!file.good())
    {
      std::cerr << "Could not open file: " << file_name << std::endl;
      return 0;
    }
    buf = file.rdbuf();
  }
  else
  {
    std::cerr << "No " << description << " provided!" << std::endl;
    return 0;
  }
  return buf;
}

/**
 * @brief Main entry point of the translator.
 *
 * Supports the following command line arguments:
 * - `-c filename` input path for the configuration file (required)
 * - `-i filename` input path for AgentSpeak code (overrides value given in config file)
 * - `-o filename` output path for generated C code (overrides value given in config file)
 * - `-b filename` output path for generated basic actions header file (overrides value given in config file)
 * - `-h filename` output path for generated header file (overrides value given in config file)
 *
 * This code does the following steps:
 */
int main(int argc, char *argv[])
{
  std::string basics_path;
  std::string config_path;
  std::string header_path;
  std::string input_path;
  std::string output_path;
  int c;
  opterr = 0;

  /** - read command line arguments */
  /** \TODO Add usage notes printed on command line */
  while ((c = getopt(argc, argv, "b:c:h:i:o:")) != -1)
  {
    switch(c)
    {
      case 'b':
        basics_path = optarg;
        break;
      case 'c':
        config_path = optarg;
        break;
      case 'h':
        header_path = optarg;
        break;
      case 'i':
        input_path = optarg;
        break;
      case 'o': 
        output_path = optarg; 
        break;
    }
  }

  agentt agent;
  agent_configt config(agent.atom_table);

  /* - open and parse config file */
  if (config_path != "")
  {
    std::ifstream configfile(config_path);
    if (!configfile.good())
    {
      std::cerr << "Could not open file: " << config_path << std::endl;
      return EXIT_FAILURE;
    }
    config_parser_drivert config_driver(&configfile, config, agent.atom_table);
    bool result = config_driver.parse();
    if (!result)
    {
      std::cerr << "Parsing config file failed!" << std::endl;
      return EXIT_FAILURE;
    } 
    else
    {
      /*agent.atom_table.print(std::cout);
      std::cout << std::endl;*/
    }
  }
  else
  {
    std::cerr << "No config file provided!" << std::endl;
    return EXIT_FAILURE;
  }

  /** - determine input file */
  /** \TODO at the moment the config file only accepts filenames for input, etc.
   * the generated files are created in the same place as the config file */
  std::string base_path;
  size_t split_point = config_path.find_last_of("/");
  if (split_point != std::string::npos)
    base_path = config_path.substr(0, split_point+1);

  /** \TODO there's lots of duplicated code like this, get rid of it! */
  std::string input_name;
  // command line option overrides config file
  if (input_path != "")
  {
    auto p = split_path(input_path);
    input_name = p.second;
    config.set_input_name(input_name);
  }
  else
  {
    input_path = base_path + config.get_input_name();
  }

  /** - parse AgentSpeak code */
  if (input_path != "")
  {
    std::ifstream infile(input_path);
    if (!infile.good())
    {
      std::cerr << "Could not open file: " << input_path << std::endl;
      return EXIT_FAILURE;
    }
    agentspeak_parser_drivert driver(&infile, agent);
    bool result = driver.parse();
    if (!result)
    {
      std::cerr << "Parsing AgentSpeak program failed!" << std::endl;
      return EXIT_FAILURE;
    }
    else
    {
      /*std::cout << agent << std::endl;
      agent.symbol_table.print(std::cout);
      std::cout << std::endl;*/
    }
  }
  else
  {
    std::cerr << "No input file provided!" << std::endl;
    return EXIT_FAILURE;
  }

  /** - set up the output files */
  /** \TODO there's lots of duplicated code like this, get rid of it! */
  std::string output_name;
  // command line option overrides config file
  if (output_path != "")
  {
    auto p = split_path(output_path);
    output_name = p.second;
    config.set_output_c_name(output_name);
  }
  else
  {
    output_path = base_path + config.get_output_c_name();
  }
  std::ofstream out_c_file;
  std::ostream out_c_stream(getBuf(output_path, out_c_file, "output c file"));

  /** \TODO there's lots of duplicated code like this, get rid of it! */
  std::string header_name;
  // command line option overrides config file
  if (header_path != "")
  {
    auto p = split_path(header_path);
    header_name = p.second;
    config.set_output_h_name(header_name);
  }
  else
  {
    header_path = base_path + config.get_output_h_name();
  }
  std::ofstream out_h_file;
  std::ostream out_h_stream(getBuf(header_path, out_h_file, "output header file"));

  /** \TODO there's lots of duplicated code like this, get rid of it! */
  std::string basics_name;
  // command line option overrides config file
  if (basics_path != "")
  {
    auto p = split_path(basics_path);
    basics_name = p.second;
    config.set_basics_h_name(basics_name);
  }
  else
  {
    basics_path = base_path + config.get_basics_h_name();
  }
  std::ofstream out_b_file;
  std::ostream out_b_stream(getBuf(basics_path, out_b_file, "actions header file"));

  /** - translate the code */
  agent_translatort translator(agent, config, out_c_stream, out_h_stream, out_b_stream);
  translator.translate();

  return EXIT_SUCCESS;
}
