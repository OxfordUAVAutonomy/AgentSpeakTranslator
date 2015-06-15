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
 * @file agent_translator.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_AGENT_TRANSLATOR_H
#define AGENTSPEAK_AGENT_TRANSLATOR_H

#include <map>
#include <iostream>
#include <ostream>
#include <string>

#include "agent_analysis.h"
#include "agentspeak_syntax.h"
#include "trigger_translator.h"

/** \TODO these macros are potentially dangerous */
/** apply F to all basic functions */
#define forall_basic_functions(F) \
  for (auto it = agent.atom_table.begin(); \
       it != agent.atom_table.end();\
       it++) \
  { \
    atomt &atom = *(it->second); \
    if (!config.is_basic_function(atom)) continue; \
    F(atom); \
  }

/** apply F to all believables, i.e., atoms that may occur as elements of the belief base */
#define forall_believables(F) \
  for (auto it = agent.atom_table.begin(); \
       it != agent.atom_table.end();\
       it++) \
  { \
    atomt &atom = *(it->second); \
    if (!analysis.believables.is_believable(atom)) continue; \
    F(atom); \
  }

/** apply F to all settables, i.e., elements of the belief base that can be set by the environment */
#define forall_settables(F) \
  for (auto it = agent.atom_table.begin(); \
       it != agent.atom_table.end();\
       it++) \
  { \
    atomt &atom = *(it->second); \
    if (!config.is_settable(atom)) continue; \
    F(atom); \
  }

/** apply F to all plans */
#define forall_plans(F) \
  for (std::size_t i = 0; \
       i < agent.get_number_of_plans(); \
       i++) \
  { \
    plant &plan = agent.get_plan(i); \
    F(plan); \
  }

/**
 * @brief Coordinates the overall translation of an agent to C. After construction, call agent_translatort::translate() in order generate files.
 *
 * \TODO Maybe make split version with and without event queue into two separate classes, sharing the common methods in some base class?
 */
class agent_translatort
{
  const agentt &agent; /**< @brief The agent to be translated. */
  const agent_configt &config; /**< @brief The configuration for the translation. */
  const agent_analysist analysis; /**< @brief Used for storing various types of analysis. */
  trigger_translatort trigger_translator; /**< @brief The trigger translator associated with this agent translator. */
  std::ostream &strm; /**< @brief The output stream for the generated C file. */
  std::ostream &h_strm; /**< @brief The output stream for the generated header file. */
  std::ostream &b_strm; /**< @brief The output stream for the generated header file for the basic actions. */

public:
  /**
   * @brief Store given parameters and analyse given agent.
   */
  agent_translatort(
    const agentt &_agent,
    const agent_configt &_config,
    std::ostream &_strm,
    std::ostream &_h_strm,
    std::ostream &_b_strm)
    : agent(_agent),
    config(_config),
    analysis(_agent, _config),
    trigger_translator(_config, analysis, _strm, _h_strm),
    strm(_strm),
    h_strm(_h_strm),
    b_strm(_b_strm)
  {
  }

  /**
   * @brief Translate the given agent, creating the three generated files.
   *
   * This method uses
   * - agent_translatort::emit_header_file()
   * - agent_translatort::emit_basics_file()
   * - agent_translatort::emit_code_file()
   */
  void translate()
  {
    emit_header_file();
    emit_basics_file();
    emit_code_file();
  }

private:
  /**
   * @brief Creates a notice indicating that the given file was created by the AgentSpeak translator.
   */
  std::string translation_notice(const std::string &filename);

  /**
   * @brief Creates the header file for the generated C code. This is mostly the function declarations for the ''publicly'' accessible methods.
   */
  void emit_header_file(void);

  /**
   * @brief Creates the header file for the basic actions to be provided by the environment as given by the configuration file 
   */
  void emit_basics_file(void);

  /**
   * @brief Generates the actual C code. 
   */
  void emit_code_file(void);

  /**
   * @brief Emit the declaration of the basic function corresponding to the given atom.
   */
  void emit_basic_function_declaration(atomt &atom);

  /**
   * @brief Emit the declaration of a belief base setter corresponding to the given atom.
   */
  void emit_belief_base_setter_declaration(atomt &atom);

  /**
   * @brief Emit the list of includes as given in the configuration file, plus possibly, includes for the generated files.
   *
   * @param ostrm The output stream to write the includes to.
   * @param include_generated Indicates whether the generated header files should also be listed. This is necessary for the generated C code, but obviously wrong for the generated header files.
   */
  void emit_includes(std::ostream &ostrm, bool include_generated);
 
  /**
   * @brief Emit variables and setters for belief base, see agent_translatort::emit_belief_base_variables and agent_translatort::emit_belief_base_setter.
   */
  void emit_belief_base(void);

  /**
   * @brief Emit variables for the belief base. This includes one flag per element of the belief base to indicate whether it is set or not plus one variable for each parameter of the elements of the belief base. */
  void emit_belief_base_variables(atomt &atom);

  /**
   * @brief Emits a setter for the given element of the belief base.
   */
  void emit_belief_base_setter(atomt &atom);

  /**
   * @brief Emits the code for event handling.
   *
   * The required functions are
   * - void init_events(void) for initialising the events (used internally by the generated code),
   * - bool can_add_event(void) for the environment in order to check whether the agent is ready to handle an event,
   * - bool add_event_new_focus(eventt) to add a new focus event (used internally by the generated code), returns true upon successfully adding the event,
   * - bool add_event_to_complete(eventt) to add a run-to-completion event (used internally by the generated code), returns true upon successfully adding the event,
   * - bool add_external_event(eventt) for the environment to add an event, returns true upon successfully adding the event, 
   * - eventt get_next_event(void) for the agent to obtain the next event.
   *
   * This code is static and generated from event_handling_with_queue.c or event_handling_without_queue.c using @c make_event_handling.sh
   */
  void emit_event_handling(void);

  /**
   * @brief Emits the list of the given formula's parameters (or nothing, if empty). Used by agent_translatort::emit_init() and agent_translatort::emit_clear() to set the initial goal.
   *
   * \TODO Some duplicated code with the plan translatort, maybe move into some shared part.
   */
  void emit_goal_parameters(formulat &goal);

  /**
   * @brief Emits the init() function, which initialises the event handling, sets initial beliefs and sets the initial goal. */
  void emit_init(void);

  /**
   * @brief Emits the clear() function, which resets the agent into its initial state.
   */
  void emit_clear(void);
};

#endif /* AGENTSPEAK_AGENT_TRANSLATOR_H */

