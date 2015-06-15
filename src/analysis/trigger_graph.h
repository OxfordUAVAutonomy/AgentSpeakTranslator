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
 * @file trigger_graph.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_TRIGGER_GRAPH_H
#define AGENTSPEAK_TRIGGER_GRAPH_H

#include <deque>
#include <map>
#include <set>
#include <vector>

#include "agentspeak_syntax.h"

/**
 * @brief Simple analysis of the trigger graph, gives information on which atoms are triggering (i.e., which ones occur as the triggering event of a plan) and which ones are recursively triggering (i.e., which ones may directly or indirectly trigger themselves). Note that this is an overapproximation (as most call-like graph analyses).
 *
 */
class trigger_grapht
{
private:
  const agentt &agent; /**< @brief The agent for which this analysis is to be conducted. */

  std::map<const atomt*, std::set<const atomt*>> graph; /**< @brief The trigger graph stored as adjacency list. */
  std::set<const atomt*> recursives; /**< @brief The set of recursively triggering atoms to be determined. */
  
  /**
   * @brief Create the trigger graph by traversing the agent's plans.
   */
  void create_graph();
  
  /**
   * @brief Remove non-triggering atoms (nodes without successors) from graph.
   */
  void remove_non_triggering();

  /**
   * @brief Determine the set of recursively triggering atoms.
   */ 
  void get_recursives();

  /**
   * @brief Convenience method for output, mostly used for debugging.
   */
  void print_graph();

public:
  /**
   * @brief Store given parameters and determine recursively-triggering atoms.
   */
  trigger_grapht(
    const agentt &_agent)
    : agent(_agent)
  {
    create_graph();
    remove_non_triggering();
    get_recursives();
  }
 
  /**
   * @brief Check whether the given atom is triggering.
   */
  bool is_triggering(const atomt&) const;

  /**
   * @brief Check whether the given atom is recursively triggering.
   */
  bool is_recursive(const atomt&) const;
};

#endif /* AGENTSPEAK_TRIGGER_GRAPH_H */

