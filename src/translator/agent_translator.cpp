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
 * @file agent_translator.cpp
 *
 * @author Samuel Bucheli
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#include "agentspeak_syntax.h"
#include "agent_translator.h"
#include "name_translation.h"
#include "trigger_translator.h"

std::string agent_translatort::translation_notice(const std::string &filename)
{
  std::stringstream ss;
  ss << "/* ";
  ss << "File " << filename << " generated from " << config.get_input_name() << " using AgentSpeakTranslator";
  ss << " */\n\n";
  return ss.str();
}

void agent_translatort::emit_header_file()
{
  h_strm << translation_notice(config.get_output_h_name());
  h_strm << get_begin_header_guard(config.get_output_h_name());
  
  emit_includes(h_strm, false);

  /** \TODO this part of the translator is event queue size-dependent. */
  if (config.needs_event_queue())
  {
    h_strm << "\n#define MAX_EVENTS " << config.get_max_events() << "\n";
  }

  /** \TODO make these names configureable */
  h_strm << "\n/* \"public\" interface */\n";
  h_strm << "void init(void);\n";
  h_strm << "void next_step(void);\n";
  h_strm << "bool can_add_event(void);\n";
  h_strm << "void clear(void);\n";
  h_strm << "void updateBeliefs(void);\n";

  h_strm << "\n";
  trigger_translator.emit_trigger_function_declarations();

  h_strm << "\n /* setters for belief base */\n";
  forall_settables(emit_belief_base_setter_declaration);

  h_strm << get_end_header_guard(config.get_output_h_name());
}

void agent_translatort::emit_belief_base_setter_declaration(atomt &atom)
{
  std::string name = atom.get_name();

  h_strm << "void set_" << name << "(";
  h_strm << get_type_list(atom);
  h_strm << ");\n" ;
}

void agent_translatort::emit_basics_file()
{
  b_strm << translation_notice(config.get_basics_h_name());
  b_strm << get_begin_header_guard(config.get_basics_h_name());
  emit_includes(b_strm, false);
  b_strm << "\n/* basic functions */\n";
  forall_basic_functions(emit_basic_function_declaration);
  b_strm << get_end_header_guard(config.get_basics_h_name());
}

void agent_translatort::emit_basic_function_declaration(atomt &atom)
{
  std::string name = atom.get_name();
  b_strm << atom.get_return_type() << " ";
  b_strm << name << "(";
  b_strm << get_type_list(atom);
  b_strm << ");\n" ;
}

/**
 * The main steps are as follows.
 * It makes heavy use of @ref trigger_translatort as much of the code to be generated is dependent on the triggering events occurring in the AgentSpeak code.
 */
void agent_translatort::emit_code_file(void)
{
  strm << translation_notice(config.get_output_c_name());
  emit_includes(strm, true);
  /** - create variables for belief base (using agent_translatort::emit_belief_base()), */
  emit_belief_base();
  /** - create code for event structures (using trigger_translatort::emit_trigger_enum(), etc. */
  trigger_translator.emit_trigger_enum();
  trigger_translator.emit_event_struct();
  trigger_translator.emit_is_triggering();
  /** - create code for handling percept events (using trigger_translatort::emit_handle_percept()), */
  trigger_translator.emit_handle_percept();
  /** - create code for handling events in general: add event, get event, etc. using agent_translatort::emit_event_handling(), */
  emit_event_handling();
  /** - create code for creating events using trigger_translatort::emit_trigger_functions(), */
  trigger_translator.emit_trigger_functions();
  /** - create code for the actual plans using trigger_translatort::emit_plans(), */
  trigger_translator.emit_plans();
  /** - create code for init, clear, and next step using agent_translatort::emit_init(), agent_translatort::emit_clear(), trigger_translatort::emit_next_step(). */
  emit_init();
  emit_clear();
  trigger_translator.emit_next_step();
}

void agent_translatort::emit_includes(std::ostream &ostrm, bool include_generated)
{
  for (std::size_t i = 0;
       i < config.get_number_of_includes();
       i++)
  {
    ostrm << "#include " << config.get_include(i) << "\n";
  }
  if (include_generated)
  {
    ostrm << "#include \"" << config.get_output_h_name() << "\"\n";
    ostrm << "#include \"" << config.get_basics_h_name() << "\"\n";
  }
  ostrm << "\n";
}

void agent_translatort::emit_belief_base()
{
  strm << "/* belief base */\n";
  forall_believables(emit_belief_base_variables);
  forall_settables(emit_belief_base_setter);
  strm << "\n";
}

void agent_translatort::emit_belief_base_variables(atomt &atom)
{
  std::string name = atom.get_name();
  strm << "static bool " << name << "_set = false;\n";
  for (std::size_t i = 0; 
       i < atom.get_arity(); 
       i++)
  {
    strm << "static " << atom.get_type(i) << " " << get_param_name(name, i) << ";\n";
  }
  strm << "\n";
}

void agent_translatort::emit_belief_base_setter(atomt &atom)
{
  std::string name = atom.get_name();
  strm << "void set_" << name << "(";
  if (atom.get_arity() > 0)
  {
    strm << get_param_list(atom);
  }
  strm << ")\n{\n";
  strm << "  " << name << "_set = true;\n";
  for (std::size_t i = 0; 
       i < atom.get_arity(); 
       i++)
  {
    strm << "  " << get_param_name(name, i) << " = " << "param" << i << ";\n";
  }
  strm << "}\n";
}

void agent_translatort::emit_event_handling()
{
  /** \TODO this part of the translator is event queue size-dependent. */
  if (config.needs_event_queue())
  {
    /** \TODO this is somewhat crude... */
    #include "event_handling_with_queue.inc"
  }
  else
  {
    /** \TODO this is somewhat crude... */
    #include "event_handling_without_queue.inc"
  }
}

void agent_translatort::emit_goal_parameters(formulat &goal)
{
  /** \TODO check these are just values */
  if (goal.get_number_of_terms() == 0) return;
  for (std::size_t i = 0;
       i < goal.get_number_of_terms()-1;
       i++)
  {
    termt &term = goal.get_term(i);
    strm << term << ", ";
  }
  strm << goal.get_term(goal.get_number_of_terms());
}

void agent_translatort::emit_init()
{
  strm << "/* init */\n";
  strm << "void init(void)\n";
  strm << "{\n";
  strm << "  // initialise event queue" << "\n";
  strm << "  init_events();\n";
  strm << "  // set initial beliefs" << "\n";
  for (std::size_t i = 0;
       i < agent.get_number_of_beliefs();
       i++)
  {
    literalt &initial_belief = agent.get_belief(i);
    const atomt &atom = initial_belief.get_atom();
    strm << "  " << atom.get_name() << "_set = true;\n";
    for (std::size_t i = 0;
         i < atom.get_arity();
         i++)
    {
      /** \TODO check terms are values */
      strm << "  " << get_param_name(atom.get_name(), i) << " = " << initial_belief.get_term(i) << ";\n";
    }
  }
  if (agent.has_initial_goal())
  {
    formulat &initial_goal = agent.get_initial_goal();
    strm << "  // initial goal: " <<  initial_goal << "\n";
    strm << "  " << "add_" << get_trigger_function_name(initial_goal, false) << "(";
    emit_goal_parameters(initial_goal);
    strm << ");\n";
  }
  strm << "}\n\n";
}

/** \TODO what should clear actually do? Probably the best solution is that the user should define a plan for !clearCommands */
void agent_translatort::emit_clear()
{
  strm << "/* clear */\n";
  strm << "void clear(void)\n";
  strm << "{\n";
  strm << "  // clear event queue" << "\n";
  strm << "  init_events();\n";
  /** \TODO should this happen? */
  if (agent.has_initial_goal())
  {
    formulat &initial_goal = agent.get_initial_goal();
    strm << "  // restart with initial goal: " <<  initial_goal << "\n";
    strm << "  " << "add_" << get_trigger_function_name(initial_goal, false) << "(";
    emit_goal_parameters(initial_goal);
    strm << ");\n";
  }
  strm << "}\n\n";
}
