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
 * @file trigger_graph.cpp
 *
 * @author Samuel Bucheli
 */

#include <iostream>

#include <algorithm>
#include <deque>
#include <map>
#include <set>

#include "agentspeak_syntax.h"
#include "tarjan_scc.h"
#include "trigger_graph.h"
#include "util.h"

void trigger_grapht::create_graph()
{
  for (std::size_t i = 0;
       i < agent.get_number_of_plans();
       i++)
  {
    plant &plan = agent.get_plan(i);
    triggert &trigger = plan.get_trigger();
    const atomt &trigger_atom = trigger.get_atom();
    graph[&trigger_atom]; // init empty set for every triggering atom
    for (std::size_t j = 0;
         j < plan.get_body_size();
         j++)
    {
      formulat &formula = plan.get_formula(j);
      const atomt &atom = formula.get_atom();
      switch (formula.get_type())
      {
        case formulatypet::achieve:
        case formulatypet::achieve_new_focus:
        case formulatypet::test:
        case formulatypet::add:
        case formulatypet::del:
          graph[&trigger_atom].insert(&atom);
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

void trigger_grapht::remove_non_triggering()
{
  for (auto triggers = graph.begin();
       triggers != graph.end();
       ++triggers)
  {
    const atomt *trigger = triggers->first;
    for (auto triggereds = triggers->second.begin(); 
         triggereds != triggers->second.end();
         ++triggereds)
    {
      const atomt *triggered = *triggereds;
      if (!is_triggering(*triggered))
      {
        graph[trigger].erase(triggered);
      }
    }
  }
}

void trigger_grapht::print_graph()
{
  for (auto triggers = graph.begin();
       triggers != graph.end();
       ++triggers)
  {
    const atomt &trigger = *(triggers->first);
    std::cout << trigger << " -> ";
    auto triggered = triggers->second;
    for (auto triggereds = triggered.begin(); 
         triggereds != triggered.end();
         ++triggereds)
    {
      std::cout << **triggereds << ", ";
    }
    std::cout << std::endl;
  }
}

void trigger_grapht::get_recursives()
{
  // get strongly connected component
  tarjan_scct<const atomt*> tarjan(graph);
  std::set<std::set<const atomt*> > sccs = tarjan.get_scc();
  
  // find all recursive calls
  std::set<const atomt*> non_recursives;
  for (auto component = sccs.begin();
       component != sccs.end();
       ++component)
  {
    if (component->size() > 1) 
    {
      // indirect recursion
      recursives.insert(component->begin(), component->end());
    }
    else
    {
      const atomt *trigger = *(component->begin());
      // direct recursion
      if (graph[trigger].size() > 0)
      {
        recursives.insert(trigger);
      }
      else
      {
        non_recursives.insert(trigger);
      }
    }
  }

  // propagate recursiveness to calling methods
  bool change = true;
  while(change)
  {
    change = false;
    auto candidates = non_recursives.begin();
    while (candidates != non_recursives.end())
    {
      /** \TODO is this the right way to make the erase portable (?) */
      std::set<const atomt*>::iterator current = candidates++;
      const atomt *candidate = *current;
      for (auto triggereds = graph[candidate].begin();
           triggereds != graph[candidate].end(); 
           ++triggereds)
      {
        const atomt *triggered = *triggereds;
        if (recursives.find(triggered) != recursives.end())
        {
          recursives.insert(candidate);
          non_recursives.erase(current);
          change = true;
        }
      }
    }
  }
}

bool trigger_grapht::is_triggering(const atomt &atom) const
{
  return (graph.find(&atom) != graph.end());
}

bool trigger_grapht::is_recursive(const atomt &atom) const
{
  return (recursives.find(&atom) != recursives.end());
}
