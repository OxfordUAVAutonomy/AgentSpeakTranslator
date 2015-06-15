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
 * @file trigger_translator.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_TRIGGER_TRANSLATOR_H
#define AGENTSPEAK_TRIGGER_TRANSLATOR_H

#include <ostream>

#include "agent_analysis.h"
#include "agentspeak_syntax.h"

/** Apply F to all triggers (either occuring as the triggering event of a plan or given in the configuration file). */
#define forall_triggers(F) \
  for (auto it = analysis.trigger_collector.begin(); \
       it != analysis.trigger_collector.end(); \
       it++) \
  { \
    F(**it); \
  }

/** Apply F to all triggering triggers, i.e., triggers occuring as the triggering event of a plan. */
#define forall_triggering_triggers(F) \
  for (auto it = analysis.trigger_collector.begin(); \
       it != analysis.trigger_collector.end(); \
       it++) \
  { \
    const triggert &trigger = **it; \
    if (!analysis.trigger_graph.is_triggering(trigger.get_atom())) continue; \
    F(trigger); \
  }

/** Apply F to all triggers that are percepts, i.e., either addition or deletion of a belief. */
#define forall_percept_triggers(F) \
  for (auto it = analysis.trigger_collector.begin(); \
       it != analysis.trigger_collector.end(); \
       it++) \
  { \
    const triggert &trigger = **it; \
    if (!trigger.is_percept()) continue; \
    F(trigger); \
  }

/** Apply F to all triggerables as given by the configuration file. */
#define forall_triggerables(F) \
  for (auto it = config.triggerables_begin(); \
       it != config.triggerables_end(); \
       it++) \
  { \
    F(**it); \
  }

/**
 * @brief  Takes care of all translation tasks that are required per trigger. See @ref agent_translatort::translate() for how it is used.
 *
 * \TODO This class is getting quite large, is there a way to break it up into smaller, well-defined parts?
 */
class trigger_translatort
{
  const agent_configt &config; /**< @brief The given configuration (handed down from agent_translatort::agent_translatort). */
  const agent_analysist &analysis; /**< @brief The analysis (created and handed down by agent_translatort::agent_translatort). */

  std::ostream &strm; /**< @brief The output stream for the C code to be generated (handed down from agent_translatort::agent_translatort). */
  std::ostream &h_strm; /**< @brief The output stream for the header file to the C code to be generated (handed down from agent_translatort::agent_translatort). */

public:
  /**
   * @brief Stores the given parameters.
   */
  trigger_translatort(
    const agent_configt &_config,
    const agent_analysist &_analysis,
    std::ostream &_strm,
    std::ostream &_h_strm)
    : config(_config),
      analysis(_analysis),
      strm(_strm),
      h_strm(_h_strm)
  {
  }

  /**
   * @brief A convenience method retrieving the list of plans having the given trigger as triggering events. It just redirects the call to trigger_collectort::get_plans() of agent_analysist::trigger_collector.
   */
  const std::vector<const plant*>& get_plans(const triggert &trigger) const;

  /**
   * @brief Emits an enum containing all possible triggers (plus NONE for the case when no event is given).
   */
  void emit_trigger_enum(void);

  /**
   * @brief Emits a struct for events, containing a field for the trigger type as emitted by trigger_translatort::emit_trigger_enum() plus fields for storing all parameters of all triggers.
   */
  void emit_event_struct(void);

  /**
   * @brief Emits a function that returns true if a the given trigger is triggering.
   */
  void emit_is_triggering(void);

  /**
   * @brief Emits a function handling the percept-related aspects of the given event, i.e., adding or deleting a belief from the belief base.
   */
  void emit_handle_percept(void);

  /**
   * @brief Emits the functions for adding specific events. These includes both the functions that are used for internally generated events as well as the ''public'' functions for events created by the environment.
   */
  void emit_trigger_functions(void);

  /**
   * @brief The declarations of the ''public'' trigger functions as generated in trigger_translatort::emit_trigger_functions().
   */
  void emit_trigger_function_declarations(void);

  /**
   * @brief Emits methods for applicable plan selection via trigger_translatort::emit_applicable_plan_selection() and then uses @ref plan_translatort in order to generate the translated plans. 
   */
  void emit_plans(void);

  /**
   * @brief Emits the next step function which corresponds to one deliberation cycle of the agent.
   */
  void emit_next_step(void);

private:
  /**
   * @brief Used in trigger_translatort::emit_trigger_enum() to get a list of names for the enum. 
   */
  void emit_enum_name(const triggert &trigger);

  /**
   * @brief Used in trigger_translatort::emit_event_struct() to get a list of the trigger parameters.
   */
  void emit_event_params(const triggert &trigger);

  /**
   * @brief Used in trigger_translatort::emit_is_triggering() to get one case of the switch statement.
   */
  void emit_is_triggering_case(const triggert &trigger);

  /**
   * @brief Used in trigger_translatort::emit_handle_percept() to get one case of the switch statemtn.
   */
  void emit_handle_percept_case(const triggert &trigger);

  /**
   * @brief Checks whether the given trigger is used as a regular achievement goal or a new focus goal (or both), and emits internal trigger functions accordingly using trigger_translatort::emit_internal_trigger_function(const triggert&, bool).
   */
  void emit_internal_trigger_function(const triggert &trigger);

  /**
   * @brief Emits a function generating an internal event for the given trigger using trigger_translatort::emit_trigger_function(const triggert&, bool, bool).
   *
   * @param trigger The trigger for which an event is to be generated.
   * @param new_focus Indicate whether to use the new focus variant for adding the event.
   */
  void emit_internal_trigger_function(const triggert &trigger, bool new_focus);

  /**
   * @brief Emits a trigger function generating an external event for the given trigger using using trigger_translatort::emit_trigger_function(const triggert&, bool, bool).
   */
  void emit_public_trigger_function(const triggert &trigger);

  /**
   * @brief Emits a function creating an event for the given trigger with the options outlined below.
   *
   * @param trigger The trigger for which an event is to be generated.
   * @param internal Indicates whether the event is generated internally or externally.
   * @param new_focus Indicates whether the event is generated with new focus or needs to be run to completion.
   */
  void emit_trigger_function(const triggert &trigger, bool internal, bool new_focus);

  /**
   * @brief Used in trigger_translatort::emit_trigger_function_declarations(void) to emit one declaration.
   */
  void emit_public_trigger_function_declaration(const triggert &trigger);

  /**
   * @brief Emits the declaration for applicable plan selection. This is necessary as translated plans might call applicable plan selection for a given event directly (in case of non-recursively triggering events), but applicable selection needs to be able to call translated plans.
   */
  void emit_applicable_plan_selection_declaration(const triggert &trigger);

  /**
   * @brief Emits the function for selecting the applicable plan. This is done by trying all translated plans in textual order until one returns true, thus indicating being applicable and having successfully run.
   */
  void emit_applicable_plan_selection(const triggert &trigger);

  /**
   * @brief Emits all plans associated with having the given trigger as triggering event.
   */
  void emit_plans(const triggert &trigger);

  /**
   * @brief Used in trigger_translatort::emit_next_step(void) to get one case in the switch statement for calling the appropriate applicable plan selection.
   */
  void emit_plan_call(const triggert &trigger);

  /**
   * @brief Used in trigger_translatort::emit_next_step(void) to get the code for handling percept related events. If the given event is just changing the belief base, but not triggering, we get the next event.
   */
  void emit_percept_handling(void);
};

#endif /* AGENTSPEAK_TRIGGER_TRANSLATOR_H */

