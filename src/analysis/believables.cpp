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
 * @file believables.cpp
 *
 * @author Samuel Bucheli
 */

#include <set>

#include "agentspeak_syntax.h"
#include "believables.h"
#include "util.h"

void believablest::collect_believables()
{
  for (std::size_t i = 0;
       i < agent.get_number_of_plans();
       i++)
  {
    const plant &plan = agent.get_plan(i);
    const triggert &trigger = plan.get_trigger();
    const atomt &atom = trigger.get_atom();
    switch (trigger.get_type())
    {
      case triggertypet::percept:
        add_to_set(atom, believable);
        break;
      case triggertypet::achieve:
      case triggertypet::test:
        // nothing to do
        break;
      default:
        ASSERT(false, "trigger " << trigger << " has unknown type");
        break;
    }
    for (std::size_t j = 0;
         j < plan.get_context_size();
         j++)
    {
      const conditiont &condition = plan.get_condition(j);
      const atomt &atom = condition.get_atom();
      // conditions with return types are basic functions, hence not elements of the belief base
      if (!atom.has_return_type())
      {
        add_to_set(atom, believable);
      }
    }
    for (std::size_t j = 0;
         j < plan.get_body_size();
         j++)
    {
      const formulat &formula = plan.get_formula(j);
      const atomt &atom = formula.get_atom();
      switch (formula.get_type())
      {
        case formulatypet::add:
        case formulatypet::del:
          add_to_set(atom, believable);
          break;
        case formulatypet::achieve:
        case formulatypet::achieve_new_focus:
        case formulatypet::test:
        case formulatypet::action:
        case formulatypet::assign:
          // nothing to do
          break;
        default:
          ASSERT(false, "formula " << formula << " has unknown type");
          break;
      }
    }
  }
}

bool believablest::is_believable(const atomt &atom) const
{
  return is_in_set(atom, believable);
}
