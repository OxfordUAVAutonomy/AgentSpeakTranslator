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
 * @file name_translation.cpp
 *
 * @author Samuel Bucheli
 */

#include <algorithm>
#include <sstream>
#include <string>

#include "agentspeak_syntax.h"
#include "name_translation.h"
#include "util.h"

std::string get_plan_name(const triggert &trigger)
{
  std::stringstream s;
  switch (trigger.get_op())
  {
    case triggeropt::add:
      s << "add_";
      break;
    case triggeropt::del:
      s << "del_";
      break;
    default:
      ASSERT(false, "operation of trigger " << trigger << " is unknown");
      break;
  }
  switch (trigger.get_type())
  {
    case triggertypet::percept:
      s << "percept_";
      break;
    case triggertypet::achieve:
      s << "achieve_";
      break;
    case triggertypet::test:
      s << "test_";
      break;
    default:
      ASSERT(false, "type of trigger " << trigger << " is unknown");
      break;
  }
  s << trigger.get_atom().get_name();
  return s.str();
}

void add_or_del(std::stringstream &s, bool add)
{
  if (add)
  {
    s << "add_";
  }
  else
  {
    s << "del_";
  }
}

std::string get_plan_name(const atomt &atom, formulatypet type, bool add)
{
  std::stringstream s;
  switch (type)
  {
    case formulatypet::achieve:
    case formulatypet::achieve_new_focus:
      add_or_del(s, add);
      s << "achieve_";
      break;
    case formulatypet::add:
      s << "add_percept_";
      break;
    case formulatypet::del:
      s << "del_percept_";
    case formulatypet::test:
      add_or_del(s, add);
      s << "test_";
    case formulatypet::action:
    case formulatypet::assign:
      ASSERT(false, "plan name is only defined for percepts, achievement, or test goals");
      break;
    default:
      ASSERT(false, "formulatype unknown");
      break;
  }
  s << atom.get_name();
  return s.str();
}

std::string get_plan_name(const formulat &formula)
{
  return get_plan_name(formula.get_atom(), formula.get_type(), true);
}

std::string get_trigger_function_name(const atomt &atom, formulatypet type, bool internal)
{
  std::stringstream s;
  if (internal)
  {
    s << "internal_";
  }
  switch (type)
  {
    case formulatypet::achieve: 
      s << "achieve_";
      break;
    case formulatypet::achieve_new_focus:
       s << "achieve_new_focus_";
      break;
    case formulatypet::test: 
      s << "test_";   
      break;
    case formulatypet::add:     
      s << "add_";
      break;
    case formulatypet::del: 
      s << "del_";
      break;
    default:
      ASSERT(false, "unknown formulatype for atom " << atom);
      break;
  }
  s << atom.get_name();
  return s.str();
}

std::string get_trigger_function_name(const formulat &formula, bool internal)
{
  return get_trigger_function_name(formula.get_atom(), formula.get_type(), internal);
}

/** \TODO: Handle translation between formulatypes and triggertypes/ops in a more graceful manner... */
std::string get_trigger_function_name(const triggert &trigger, bool internal, bool new_focus)
{
  std::stringstream s;
  switch (trigger.get_type())
  {
    case triggertypet::achieve:
      ASSERT(trigger.get_op() == triggeropt::add, "trigger function name only defined for triggeropt add in " << trigger);
      if (new_focus)
      {
        s << get_trigger_function_name(trigger.get_atom(), formulatypet::achieve_new_focus, internal);
      }
      else
      {
        s << get_trigger_function_name(trigger.get_atom(), formulatypet::achieve, internal);
      }
      break;
    case triggertypet::percept:
      switch (trigger.get_op())
      {
        case triggeropt::add:
          s << get_trigger_function_name(trigger.get_atom(), formulatypet::add, internal);
          break;
        case triggeropt::del:
          s << get_trigger_function_name(trigger.get_atom(), formulatypet::del, internal);
          break;
        default:
          ASSERT(false, "unknown trigger op in trigger " << trigger);
          break;
      }
      break;
    case triggertypet::test:
      ASSERT(trigger.get_op() == triggeropt::add, "trigger function name only defined for triggeropt add in " << trigger);
      s << get_trigger_function_name(trigger.get_atom(), formulatypet::test, internal);
      break;
    default:
      ASSERT(false, "unknown trigger type in trigger " << trigger);
      break;
  }
  return s.str();
}

std::string get_param_name(std::string name, std::size_t i)
{
  std::stringstream s;
  s << name << "_param" << i;
  return s.str();
}

/** \TODO Use this everywhere possible */
std::string get_param_name(const triggert &trigger, std::size_t i)
{
  return get_param_name(trigger.get_atom().get_name(), i);
}

std::string get_enum_name(const triggert &trigger)
{
  std::string enum_name = get_plan_name(trigger);
  std::transform(enum_name.begin(), enum_name.end(), enum_name.begin(), ::toupper);
  return enum_name;
}

std::string get_param_list(const atomt &atom, bool with_types)
{
  std::stringstream s;
  if (atom.get_arity() == 0)
    s << "void";
  else
  {
    std::size_t n = atom.get_arity()-1;
    for (std::size_t i=0; 
         i < n; 
         i++)
    {
      if (with_types)
        s << atom.get_type(i) << " ";
      s << "param" << i << ", ";
    }

    if (with_types)
      s << atom.get_type(n) << " ";
    s << "param" << n;
  }
  return s.str();
}

std::string get_type_list(const atomt &atom)
{
  std::stringstream s;
  if (atom.get_arity() == 0)
     s << "void";
  else
  {
     std::size_t n = atom.get_arity()-1;
     for (std::size_t i=0;
          i < n;
          i++)
     {
       s << atom.get_type(i) << ", ";
     }
     
     s << atom.get_type(n);
  }
  return s.str();
}

std::string get_guard_name(std::string name)
{
  std::string guard;
  size_t split_point = name.find_last_of(".");
  if (split_point != std::string::npos)
    guard = name.substr(0, split_point);

  std::transform(guard.begin(), guard.end(), guard.begin(), ::toupper);
  return "__" + guard + "_H";
}

std::string get_begin_header_guard(std::string name)
{
  std::stringstream s;
  s << "#ifndef " << get_guard_name(name) << "\n";
  s << "#define " << get_guard_name(name) << "\n";
  return s.str();
}

std::string get_end_header_guard(std::string name)
{
  std::stringstream s;
  s << "#endif /* " << get_guard_name(name) << " */\n";
  return s.str();
}

