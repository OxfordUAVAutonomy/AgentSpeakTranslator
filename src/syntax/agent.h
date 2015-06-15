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
 * @file agent.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_AGENT_H
#define AGENTSPEAK_AGENT_H

#include <memory>
#include <ostream>

#include "agent_config.h"
#include "formula.h"
#include "plan.h"
#include "symbol_table.h"
#include "term.h"
#include "util.h"

class agent_buildert;
class agent_translatort;

/**
 * @brief Represents an agent consisting of a set of initial beliefs, an initial goal (optional), and a list of plans */
class agentt
{
  /** \TODO Agent name is currently not used. */
  std::string name; /**< @brief The name of the agent (optional) */

  beliefst beliefs; /**< @brief The agent's initial beliefs. */
  std::unique_ptr<formulat> initial_goal; /**< @brief The agent's inital goal (optional). */
  planst plans; /**< @brief The list of the agent's plans. */

  friend class agent_buildert;
  friend std::ostream& operator<<(std::ostream&, const agentt&);

public:
  atom_tablet atom_table; /**< @brief A symbol table of all atoms used by the agent. */

  /** \TODO It might be nicer if this constructor is private and only used by the agent builder. */
  /** 
   * @brief Create an empty agent.
   */
  agentt()
  {
  }

  /** \TODO It might be nicer if this constructor is private and only used by the agent builder. */
  /** @brief Create an empty agent with given name. */
  agentt(const std::string _name)
    : name(_name)
  {
  }

  /** 
   * @brief Create an agent with the given initial beliefs, initial goal, and list of plans.
   */
  agentt(beliefst *_beliefs, formulat *_initial_goal, planst *_plans)
    : beliefs(std::move(*_beliefs)),
    initial_goal(_initial_goal),
    plans(std::move(*_plans))
  {
    switch (_initial_goal->get_type())
    {
      /** \TODO achieve_new_focus would be possible, but doesn't make much sense, also, the more elaborate form of test goals could be possible in the future, but doesn't make much sense in the current simple form */
      case formulatypet::achieve:
        // nothing to do
        break;
      default:
        ASSERT(false, "initial goal " << _initial_goal << " is not an achievement goal");
        break;
    }
  }

  /** 
   * @brief Create an agent with the give initial beliefs and list of plans.
   */
  agentt(beliefst *_beliefs, planst *_plans)
    : beliefs(std::move(*_beliefs)),
    plans(std::move(*_plans))
  {
  }

  bool has_initial_goal() const
  {
    return (bool)initial_goal;
  }

  std::size_t get_number_of_beliefs() const
  {
    return beliefs.size();
  }

  literalt& get_belief(std::size_t index) const
  {
    ASSERT(index < get_number_of_beliefs(), "belief index out of bounds");
    return *(beliefs[index]);
  }

  std::size_t get_number_of_plans() const
  {
    return plans.size();
  }

  plant& get_plan(std::size_t index) const
  {
    ASSERT(index < get_number_of_plans(), "plan index out of bounds");
    return *(plans[index]);
  }

  formulat& get_initial_goal() const
  {
    ASSERT(has_initial_goal(), "agent does not have an initial goal");
    return *initial_goal;
  }
};

#endif /* AGENTSPEAK_AGENT_H */
