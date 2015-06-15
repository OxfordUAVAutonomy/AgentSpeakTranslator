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
 * @file formula.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_FORMULA_H
#define AGENTSPEAK_FORMULA_H

#include <memory>
#include <ostream>
#include <vector>

#include "literal_based.h"
#include "term.h"
//#include "trigger.h" /** \TODO see friend class remark below */
#include "util.h"

/** @brief A formula is either a basic action, an assignment, an achievement goal, an achievement goal with new focus, a test goal, the addition of a belief, or the deletion of a belief. */
enum class formulatypet { action, assign, achieve, achieve_new_focus, test, add, del };

/** @brief Represents a formula in the body of a plan */
class formulat : public literal_basedt<formulat>
{
  std::unique_ptr<vartermt> variable; /**< @brief If the formula is an assignment, this is used for storing the variable written. This is a custom extension of our AgentSpeak variant */
  formulatypet type; /**< @brief The type of the formula, @sa formulatypet */

  friend std::ostream& operator<<(std::ostream&, const formulat&);
  friend class triggert;  /** \TODO only constructor should be friend, but incomplete type troubles... */
  
public:
  /**
   * @brief Construct an assignment with the given variable and literal.
   */
  formulat(vartermt *_variable, literalt *_literal) 
    : literal_basedt(_literal),
    variable(_variable),
    type(formulatypet::assign)
  {
    ASSERT(variable->is_named(), "variable needs name");
  }

  /**
   * @brief Construct a formula with the given literal and type (except assignment).
   */
  formulat(literalt* _literal, formulatypet _type = formulatypet::action) 
    : literal_basedt(_literal),
    type(_type)
  {
    ASSERT(type != formulatypet::assign, "variable required for assignment");
  }

  vartermt& get_variable() const
  {
    ASSERT(type == formulatypet::assign, "only formula of type assignment has variable");
    return *variable;
  }

  formulatypet get_type() const
  {
    return type;
  }
};

typedef std::vector<std::unique_ptr<formulat> > bodyt;

#endif /* AGENTSPEAK_FORMULA_H */
