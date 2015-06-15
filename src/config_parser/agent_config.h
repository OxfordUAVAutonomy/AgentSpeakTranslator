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
 * @file agent_config.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_AGENT_CONFIG_H
#define AGENTSPEAK_AGENT_CONFIG_H

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "atom.h"
#include "has_atom_set.h"
#include "symbol_table.h"
#include "trigger.h"
#include "util.h"

typedef std::set<const triggert*, trigger_comparatort> triggerables_sett;

/**
 * @brief Stores the (given) configuration of an agent for translation to C.
 */
class agent_configt : public has_atom_set_and_tablet
{
  std::string input_name; /**< name of the input AgentSpeak file */
  std::string output_c_name; /**< name of the output C file to be created from AgentSpeak code */
  std::string output_h_name; /**< name of the output header file to be created from AgentSpeak code */
  std::string basics_h_name; /**< name of the header file with all basic actions to be created */
  std::size_t max_events; /**< Maximum size of the event queue. Values == 0 will create an event queue-less version, this is also the default value. */
  std::vector<std::string> includes; /**< Additional includes to be inserted in generated C code */

  std::set<const atomt*> basic_functions; /**< basic functions as listed in config file */
  std::set<const atomt*> settables; /**< atoms listed as settable in config file */
  
  triggerables_sett triggerables; /**< externally triggerable events (represented as triggers) */

public:
  agent_configt(atom_tablet &_atom_table)
    : has_atom_set_and_tablet(_atom_table),
      max_events(0)
  {
  }

  ~agent_configt()
  {
    /** \TODO The current version requires manual memory management, it would be better to use unique pointers, as the config file owns these triggers. */
    for(auto it = triggerables.begin();
        it != triggerables.end();
        it++)
    {
      const triggert *trigger = *it;
      delete trigger;
    }
  }

  std::size_t get_number_of_includes() const
  {
    return includes.size();
  }

  std::string get_include(std::size_t index) const
  {
    return includes[index];
  }  

  void add_include(std::string include)
  {
    includes.push_back(include);
  }

  void set_input_name(const std::string name)
  {
    input_name = name;
  }

  std::string get_input_name() const
  {
    return input_name;
  }

  void set_output_c_name(const std::string name)
  {
    output_c_name = name;
  }

  std::string get_output_c_name() const
  {
    return output_c_name;
  }

  void set_output_h_name(const std::string name)
  {
    output_h_name = name;
  }

  std::string get_output_h_name() const
  {
    return output_h_name;
  }

  void set_basics_h_name(const std::string name)
  {
    basics_h_name = name;
  }

  std::string get_basics_h_name() const
  {
    return basics_h_name;
  }

  void set_max_events(std::size_t max)
  {
    ASSERT(max > 0, "event queue needs to have size > 0");
    max_events = max;
  }

  std::size_t get_max_events() const
  {
    ASSERT(max_events > 0, "max events not available for event queue-less version");
    return max_events;
  }

  bool needs_event_queue() const
  {
    /** \TODO replace with better solution... */
    // Warning: Don't use get_max_events(), as else this is only to be used for the version with event queues
    return (max_events > 0);
  }

  void make_settable(const std::string &name)
  {
    add_atom_to_set(name, settables);
  }

  bool is_settable(const atomt &atom) const
  {
    return is_in_set(atom, settables);
  }

  void make_triggerable(const std::string &name, triggeropt op, triggertypet type)
  {
    atomt &atom = get_atom(name);
    /** \TODO Maybe emit warning in case the triggerable is defined more than once? */
    triggerables.insert(new triggert(atom, op, type));
  }

  bool is_triggerable(const triggert &trigger) const
  {
    return (triggerables.find(&trigger) != triggerables.end());
  }

  triggerables_sett::const_iterator triggerables_begin(void) const
  {
    return triggerables.cbegin();
  }

  triggerables_sett::const_iterator triggerables_end(void) const
  {
    return triggerables.cend();
  }

  void make_basic_function(const std::string &name)
  {
    add_atom_to_set(name, basic_functions);
  }

  bool is_basic_function(const atomt &atom) const
  {
    return is_in_set(atom, basic_functions);
  }
};

#endif /* AGENTSPEAK_AGENT_CONFIG_H */
