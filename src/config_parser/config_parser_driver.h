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
 * @file config_parser_driver.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_CONFIG_PARSER_DRIVER_H
#define AGENTSPEAK_CONFIG_PARSER_DRIVER_H

#include <istream>
#include <stack>

#include "config_parser.h"

#include "generic_parser_driver.h"

#include "agent_config.h"
#include "agentspeak_syntax.h"
#include "atom_builder.h"
#include "symbol_table.h"

/**
 * @brief Driver for config file parser, @sa generic_parser_drivert.
 */
class config_parser_drivert
  : public generic_parser_drivert<class config_parser_drivert>
{
public:
  atom_buildert atom_builder;
  agent_configt &config;

  config_parser_drivert(std::istream *_is, agent_configt &_config, atom_tablet &_atom_table)
    : generic_parser_drivert(_is), 
    atom_builder(_atom_table),
    config(_config)
  {
    init_scanner();
  }

  virtual ~config_parser_drivert()
  {
    destroy_scanner();
  }

  bool parse();

protected:
  virtual void init_scanner();	
  virtual void destroy_scanner();
};

int config_parser_parse(config_parser_drivert *driver);

#endif // AGENTSPEAK_CONIG_PARSER_DRIVER_H

