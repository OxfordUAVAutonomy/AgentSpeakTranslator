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
 * @file trigger_translator.cpp
 *
 * @author Samuel Bucheli
 */

#include <sstream>
#include <string>
#include <vector>

#include "agentspeak_syntax.h"
#include "name_translation.h"
#include "plan_translator.h"
#include "trigger_translator.h"

const std::vector<const plant*>& trigger_translatort::get_plans(const triggert &trigger) const
{
  return analysis.trigger_collector.get_plans(trigger);
}

void trigger_translatort::emit_trigger_enum()
{
  strm << "/* triggers */\n";
  strm << "typedef enum\n";
  strm << "{\n";
  strm << "  NONE";
  forall_triggers(emit_enum_name);
  strm << "\n} triggert;\n\n";
}

void trigger_translatort::emit_event_struct()
{
  strm << "/* events */\n";
  strm << "typedef struct\n"; 
  strm << "{\n";
  strm << "  triggert trigger;\n";
  forall_triggers(emit_event_params);
  strm << "} eventt;\n\n";
}

void trigger_translatort::emit_is_triggering(void)
{
  strm << "/* returns true if the event is triggering */\n";
  strm << "static bool is_triggering(eventt event)\n";
  strm << "{\n";
  strm << "  bool triggering = false;\n";
  strm << "  switch (event.trigger)\n";
  strm << "  {\n";
  forall_triggering_triggers(emit_is_triggering_case);
  strm << "    default:  // nothing to do\n";
  strm << "      break;\n";
  strm << "  }\n";
  strm << "  return triggering;\n";
  strm << "}\n\n";
}

void trigger_translatort::emit_handle_percept(void)
{
  /** \TODO Implement emit_handle_percept() */
  strm << "/* changes belief base according to triggering event */\n";
  strm << "static void handle_percept(eventt event)\n";
  strm << "{\n";
  strm << "  switch (event.trigger)\n";
  strm << "  {\n";
  forall_percept_triggers(emit_handle_percept_case);
  strm << "    default:  // nothing to do\n";
  strm << "      break;\n";
  strm << "  }\n";
  strm << "}\n\n";
}

void trigger_translatort::emit_is_triggering_case(const triggert &trigger)
{
  std::string enum_name = get_enum_name(trigger);
  strm << "    case " << enum_name << ":\n";
  strm << "      triggering = true;\n";
  strm << "      break;\n";
}

void trigger_translatort::emit_handle_percept_case(const triggert &trigger)
{
  std::string enum_name = get_enum_name(trigger);
  const atomt &atom = trigger.get_atom();
  strm << "    case " << enum_name << ":\n";
  /** \TODO Some duplicated code with formula add/del */
  switch(trigger.get_op())
  {
    case triggeropt::add:
      strm << "      " << atom.get_name() << "_set = true;\n";
      for (std::size_t i = 0;
        i < atom.get_arity();
        i++)
      {
        strm << "      " << get_param_name(atom.get_name(), i) << " = " << "event." << get_param_name(trigger, i) << ";\n";
      }
      break;
    case triggeropt::del:
      strm << "      " << atom.get_name() << "_set = false;\n";
      break;
    default:
      ASSERT(false, "trigger " << trigger << " has unknown operation");
      break;
  }
  strm << "      break;\n";
}

void trigger_translatort::emit_trigger_functions(void)
{
  strm << "/* internal trigger functions */\n";
  forall_triggering_triggers(emit_internal_trigger_function);
  strm << "/* \"public\" trigger functions */\n";
  forall_triggerables(emit_public_trigger_function);
}

void trigger_translatort::emit_trigger_function_declarations(void)
{
  h_strm << "/* \"public\" trigger functions */\n";
  forall_triggerables(emit_public_trigger_function_declaration);
}

void trigger_translatort::emit_plans(void)
{
  strm << "/* applicable plan selection declarations */\n";
  forall_triggering_triggers(emit_applicable_plan_selection_declaration);
  strm << "/* plans */\n";
  forall_triggering_triggers(emit_plans);
  strm << "/* applicable plan selection */\n";
  forall_triggering_triggers(emit_applicable_plan_selection);
}

void trigger_translatort::emit_next_step(void)
{
  strm << "/* do one step */\n";
  strm << "void next_step(void)\n";
  strm << "{\n";
  /** - use updateBeliefs() to allow environment to set belief base, if necessary, */
  strm << "  updateBeliefs();\n";
  /** - get the next event to be handled, */
  strm << "  eventt event = get_next_event();\n";
  /** - handle the percept-related aspects of the next event, i.e., change belief base if necessary, */
  emit_percept_handling();
  /** - select the relevant plans and execute the applicable plan selection for the given event. */
  strm << "  switch (event.trigger)\n";
  strm << "  {\n";
  forall_triggering_triggers(emit_plan_call);
  /* MISRA C Rule 15.3 requires a default case in switch statements (which also needs a break!). */
  strm << "    default:  // nothing to do\n";
  strm << "      " << "break;\n";
  strm << "  }\n";
  strm << "}\n";
}

void trigger_translatort::emit_enum_name(const triggert &trigger)
{
  strm << ",\n  " << get_enum_name(trigger);
}

void trigger_translatort::emit_event_params(const triggert &trigger)
{
  const atomt &atom = trigger.get_atom();
  std::string name = atom.get_name();
  for (std::size_t i = 0; 
       i < atom.get_arity(); 
       i++)
    strm << "  " << atom.get_type(i) << " " << get_param_name(name,i) << ";\n";
}

void trigger_translatort::emit_internal_trigger_function(const triggert &trigger)
{
  const atomt &atom = trigger.get_atom();
  if (analysis.plan_usage.is_used_as(atom, formulatypet::achieve_new_focus))
  {
    emit_internal_trigger_function(trigger, true);
  }
  if (analysis.plan_usage.is_used_as(atom, formulatypet::achieve))
  {
    emit_internal_trigger_function(trigger, false);
  }
}

void trigger_translatort::emit_public_trigger_function(const triggert &trigger)
{
  emit_trigger_function(trigger, false, false);
}

void trigger_translatort::emit_internal_trigger_function(const triggert &trigger, bool new_focus)
{
  emit_trigger_function(trigger, true, new_focus);
}

/** This function proceeds as follows: */
void trigger_translatort::emit_trigger_function(const triggert &trigger, bool internal, bool new_focus)
{
  const atomt &atom =trigger.get_atom();
  std::string name = atom.get_name();  
  std::string enum_name = get_enum_name(trigger);
  /** - trigger functions for internal events can be @c static, */
  if (internal)
  {
    strm << "static ";
  }
  strm << "void ";
  /** - create the appropriate name with the given options, */
  strm << get_trigger_function_name(trigger, internal, new_focus);
  strm << "(";
  if (atom.get_arity() == 0)
    strm << "void";
  else
  {
    strm << get_param_list(atom);
  }
  strm << ")\n";
  strm << "{\n";
  /** - create event and set given event parameters, */
  strm << "  eventt event;\n";
  strm << "  event.trigger = " << enum_name << ";\n";
  for (std::size_t i = 0; i < atom.get_arity(); i++)
  {
    strm << "  event." << name << "_param" << i << " = " << "param" << i << ";\n";
  }
  /** - add created event using the appropriate event handling method w.r.t. the given options. */
  /** \TODO What shall we do in case adding the event fails? (This should not happen) */
  if (!internal)
  {
    strm << "  add_external_event(event);\n";
  }  
  else if (new_focus)
  {
    strm << "  add_event_new_focus(event);\n";
  }
  else
  {
    strm << "  add_event_to_complete(event);\n";
  }

  strm << "}\n\n";
}

void trigger_translatort::emit_public_trigger_function_declaration(const triggert &trigger)
{
  const atomt &atom = trigger.get_atom();
  h_strm << "void ";
  h_strm << get_trigger_function_name(trigger, false, false);
  h_strm << "(";
  h_strm << get_type_list(atom);
  h_strm << ");\n";
}

void trigger_translatort::emit_applicable_plan_selection_declaration(const triggert &trigger)
{
  const atomt &atom = trigger.get_atom();
  std::string name = atom.get_name();
  strm << "static void " << get_plan_name(trigger) << "(";
  strm << get_type_list(atom);
  strm << ");\n\n";
}

void trigger_translatort::emit_applicable_plan_selection(const triggert &trigger)
{
  std::string plan_name = get_plan_name(trigger);
  const atomt &atom = trigger.get_atom();
  std::string name = atom.get_name();
  strm << "static void " << plan_name << "(";
  strm << get_param_list(atom);
  strm << ")" << "\n";
  strm << "{\n";
  for (std::size_t i = 0;
       i < get_plans(trigger).size();
       i++)
  {
    /** \TODO This results in a deviation from the (arguably not very popular) MISRA C Rule 14.7: "A function shall have a single point of exit at the end of the function." */
    strm << "  if (" << plan_name << "_plan" << i << "(";
    if (atom.get_arity() > 0)
      strm << get_param_list(atom, false);
    strm << ")" << ")" << "\n";
    strm << "  {\n    return;\n  }\n";
  }
  /** \TODO What to do in case no plan is applicable? */
  strm << "  // TODO: Handle the case where no plan is applicable\n";
  strm << "  return;\n";
  strm << "}\n";
  strm << "\n";
}

void trigger_translatort::emit_plans(const triggert &trigger)
{
  const std::vector<const plant*> &plans = get_plans(trigger);
  for (std::size_t i = 0; i < plans.size(); i++)
  {
    const plant &plan = *(plans[i]);
    plan_translatort plan_translator(plan, config, analysis, i, strm);
    plan_translator.emit_plan();
  }
}

void trigger_translatort::emit_plan_call(const triggert &trigger)
{
  std::string plan_name = get_plan_name(trigger);
  std::string enum_name = get_enum_name(trigger);
  strm << "    case " << enum_name << ":\n";
  strm << "      " << plan_name << "(";
  const atomt &atom = trigger.get_atom();
  if (atom.get_arity() > 0)
  {
    std::string name = atom.get_name();
    for (std::size_t i = 0; 
         i < atom.get_arity()-1; 
         i++)
      strm << "event." << get_param_name(name,i) << ", ";
    strm << "event." << get_param_name(name, atom.get_arity()-1);
  }
  strm << ");\n";
  strm << "      " << "break;\n";
}

void trigger_translatort::emit_percept_handling(void)
{
  strm << "  handle_percept(event);\n";
  /** \TODO This code depends on whether there is an event queue. */
  if (config.needs_event_queue())
  {
    strm << "  while (!is_triggering(event))\n";
  }
  else
  {
    strm << "  if (!is_triggering(event))\n";
  }
  strm << "  {\n";
  strm << "    event = get_next_event();\n";
  strm << "    handle_percept(event);\n";
  strm << "  }\n";
}


