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
 * @file agent_builder.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_AGENT_BUILDER_H
#define AGENTSPEAK_AGENT_BUILDER_H

#include "agent.h"
#include "condition.h"
#include "plan.h"
#include "util.h"

/**
 * @brief Auxiliary class employed by parser in order to facilitate the building of agents.
 */
class agent_buildert
{
  agentt &agent; /**< @brief The agent to be built */

  plant *plan; /**< @brief Used to store the plan currently being built */

public:
  /**
   * @brief Store given agent.
   */
  agent_buildert(
    agentt &_agent)
    : agent(_agent)
  {
  }

  void add_initial_belief(literalt *belief)
  {
    agent.beliefs.push_back(std::unique_ptr<literalt>(belief));
  }

  void set_initial_goal(formulat *initial_goal)
  {
    agent.initial_goal.reset(initial_goal);
  }

  /**
   * @brief Start the creation of a new plan with the given trigger.
   */
  void add_plan(triggert *trigger)
  {
    plan = new plant(trigger);
  }

  /**
   * @brief Add the given condition at the end of the context of the plan currently being built.
   */
  void add_condition(conditiont *condition)
  {
    ASSERT(plan != NULL, "no plan is currently being built");
    plan->context.push_back(std::unique_ptr<conditiont>(condition));
  }
 
  /**
   * @brief Add the given formula at the end of the body of the plan currently being built.
   */
  void add_formula(formulat *formula)
  {
    ASSERT(plan != NULL, "no plan is currently being built");
    plan->body.push_back(std::unique_ptr<formulat>(formula));
  }

  /**
   * @brief Finish the creation of the plan and add it to the agent.
   */
  void plan_finished()
  {
    agent.plans.push_back(std::unique_ptr<plant>(plan));
    plan = NULL;
  }

  /**
   * @brief Retrieve an atom from the agent's symbol table and check whether the arity is consitent with previous information.
   */
  atomt& get_atom(const std::string &name, std::size_t arity=0)
  {
    ASSERT(agent.atom_table.contains(name), "no types for atom " << name << " defined");
    atomt &atom = agent.atom_table.get(name);
    ASSERT(atom.get_arity() == arity, "arity of " << name << " given as " << arity << " conflicting with previously stored arity " << atom.get_arity());
    return atom;
  }
};


#endif /* AGENTSPEAK_AGENT_BUILDER_H */
