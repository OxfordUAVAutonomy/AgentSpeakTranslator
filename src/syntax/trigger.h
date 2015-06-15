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
 * @file trigger.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_TRIGGER_H
#define AGENTSPEAK_TRIGGER_H

#include <ostream>
#include <vector>

#include "formula.h"
#include "literal_based.h"
#include "plan.h"
#include "term.h"
#include "util.h"

/** 
 * @brief The type of the triggering event of a plan is either a change in belief, an achievement goal, or a test goal 
 */
enum class triggertypet { percept, achieve, test};

/** 
 * @brief A triggering event corresponds to either the addition or the deletion of a goal or a belief 
 */
enum class triggeropt { add, del };

/**
 * @brief Represents the triggering event of a plan
 */
class triggert : public literal_basedt<triggert>
{
  triggeropt op; /**< @brief The operation, either addition or deletion, the event corresponds to, @sa triggeropt*/
  triggertypet type; /**< @brief The type of the triggering event, @sa triggertypet */

  friend std::ostream& operator<<(std::ostream&, const triggert&);

public:
  /**
   * @brief Construct the trigger corresponding to the given formula, i.e., the triggering event that would be created if the given formula was executed.
   */
  triggert(
    formulat *_formula,
    triggeropt _op = triggeropt::add )
    : literal_basedt(_formula),
      op(_op)
  {
    switch(_formula->get_type())
    {
      case formulatypet::achieve: 
        type = triggertypet::achieve;
        break;
      case formulatypet::achieve_new_focus:
        ASSERT(false, "formula " << *_formula << " can not be used as triggering event");
        break;
      case formulatypet::add:
        /** \TODO Check that _op == triggeropt::add ? */  
        type = triggertypet::percept;
        op = triggeropt::add;
        break;
      case formulatypet::del: 
        /** \TODO Check that _op == triggeropt::del ? */  
        type = triggertypet::percept;
        op = triggeropt::del;
        break;
      case formulatypet::test:    
        type = triggertypet::test;
        break;
      default: 
        ASSERT(false, "formula " << *_formula << " has unknown type");
        break;
    }
  }

  /**
   * @brief Build a trigger from the given atom, used by @ref trigger_collectort as triggerables in the configuration are listed without terms.
   *
   * \TODO See also the discussion in literalt::literalt(const atomt&, termst*).
   */
  triggert(
    atomt &_atom,
    triggeropt _op,
    triggertypet _type)
    : literal_basedt(_atom),
      op(_op),
      type(_type)
  {
  }

  triggeropt get_op() const
  {
    return op;
  }

  triggertypet get_type() const
  {
    return type;
  }

  bool is_percept() const
  {
    return type == triggertypet::percept;
  }

  bool is_goal() const
  {
    return !is_percept();
  }

  /**
   * @brief Compares the (address of the) atom, then op, then type in lexiographic order, i.e., terms are not relevant for comparison. Used in trigger_collector.h to ensure uniqueness of triggers with respect to this notion of equality.
   */
  bool operator<(const triggert &other) const
  {
    if (&(this->get_atom()) < &(other.get_atom()))
    {
      return true;
    }
    if (this->get_op() < other.get_op())
    {
      return true;
    }
    if (this->get_type() < other.get_type())
    {
      return true;
    }
    return false;
  }
};

/**
 * @brief Define a custom comparator for trigger pointers in order to ensure that we get each trigger only once and store things in the correct place in the map. Used in @ref trigger_collectort and @ref agent_configt.
 */
struct trigger_comparatort
{
  bool operator()(const triggert *lhs, const triggert *rhs) const
  {
    return *lhs < *rhs;
  }
};


#endif /* AGENTSPEAK_TRIGGER_H */
