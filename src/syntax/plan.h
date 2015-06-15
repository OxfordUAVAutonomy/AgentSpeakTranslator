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
 * @file plan.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_PLAN_H
#define AGENTSPEAK_PLAN_H

#include <memory>
#include <ostream>
#include <vector>

#include "condition.h"
#include "formula.h"
//#include "trigger.h"
#include "util.h"

class agent_buildert;
class triggert;

/**
 * @brief Represent an agent's plan, consisting of a triggering event, a context (pre-condition), and a plan body.
 */
class plant
{
  std::unique_ptr<triggert> trigger; /**< @brief The triggering event of the plan. */
  contextt context;  /**< @brief The context of the plan, a list of conditions. */
  bodyt body;   /**< @brief The plan body, a list of formulae. */

  friend class agent_buildert;
  friend std::ostream& operator<<(std::ostream&, const plant&);

  /**
   * @brief Constructor used by agent builder to build up plan incrementally.
   */
  plant(
    triggert *_trigger)
    : trigger(_trigger)
  {
  }

public:
  /**
   * @brief The plans ''regular'' constructor taking all the necessary parts.
   */
  plant(triggert* _trigger, contextt* _context, bodyt* _body)
    : trigger(_trigger),
    context(std::move(*_context)),
    body(std::move(*_body))
  {
  }

  triggert& get_trigger() const
  {
    return *trigger;
  }

  std::size_t get_context_size() const
  {
    return context.size();
  }

  conditiont& get_condition(std::size_t index) const
  {
    ASSERT(index < get_context_size(), "condition index out of bounds");
    return *(context[index]);
  }

  std::size_t get_body_size() const
  {
    return body.size();
  }

  formulat& get_formula(std::size_t index) const
  {
    ASSERT(index < get_body_size(), "formula index out of bounds");
    return *(body[index]);
  }
};

/** @brief Convenience abbreviation for list of (pointers to) plans. */
typedef std::vector<std::unique_ptr<plant> > planst;

#endif /* AGENTSPEAK_PLAN_H */
