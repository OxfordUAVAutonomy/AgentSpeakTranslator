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
 * @file plan_usage.cpp
 *
 * @author Samuel Bucheli
 */

#include <set>

#include "agentspeak_syntax.h"
#include "plan_usage.h"
#include "util.h"

void plan_usaget::collect_usage()
{
  for (std::size_t i = 0;
       i < agent.get_number_of_plans();
       i++)
  {
    plant &plan = agent.get_plan(i);
    for (std::size_t j = 0;
         j < plan.get_body_size();
         j++)
    {
      const formulat &formula = plan.get_formula(j);
      const atomt &atom = formula.get_atom();
      formulatypet type = formula.get_type();
      switch (type)
      {
        case formulatypet::achieve:
        case formulatypet::add:
        case formulatypet::del:
        case formulatypet::test:
          // only the last formula in a plan needs an internal achieve method
          if (j == plan.get_body_size() - 1)
          {
            usage[&atom].insert(type);
          }
          break;
        case formulatypet::achieve_new_focus:
          // new focus needs an internal achieve regardless of its position in the plan
          usage[&atom].insert(type);
          break;
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

bool plan_usaget::is_used_as(const atomt &atom, formulatypet type) const
{
  if (usage.find(&atom) != usage.end())
  {
    return usage.at(&atom).find(type) != usage.at(&atom).end();
  }
  return false;
}
