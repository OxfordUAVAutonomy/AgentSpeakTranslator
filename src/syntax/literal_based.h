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
 * @file literal_based.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_LITERAL_BASED_H
#define AGENTSPEAK_LITERAL_BASED_H

#include <memory>
#include <ostream>

#include "term.h"
#include "util.h"

/**
 * @brief Common (abstract) superclass for conditiont, formulat, and triggert, which are all based on literals.
 * 
 * @tparam T for self type (used for curiously recurring template pattern)
 *
 * \TODO Make this class actually abstract (maybe using http://stackoverflow.com/questions/4640985/making-a-class-abstract-without-any-pure-virtual-methods ?)
 */
template <typename T>
class literal_basedt 
{
protected:
  std::unique_ptr<literalt> literal; /**< @brief The literal the inheriting class is based on */

  template <typename S> friend class literal_basedt; /**< @brief So we can use literal_basedt::literal_basedt(literal_basedt<S>*). */
  template <typename S> friend std::ostream& operator<<(std::ostream&, const literal_basedt<S>&);

public:
  /**
   * @brief Construct with given literal.
   */
  literal_basedt(literalt *_literal)
    : literal(_literal)
  {
  }

  /**
   * @brief Construct with given atom, used by triggert::triggert(atomt&, triggeropt, triggertypet) for @ref trigger_collectort as triggerables defined in the configuration file are given without terms.
   *
   * \TODO See also discussion regarding the related constructor literalt::literalt(const atomt&, termst*).
   */
  literal_basedt(atomt &_atom)
    : literal(new literalt(_atom))
  {
  }

  /**
   * @brief Construct from given other literal based, this is used by triggert::triggert(formulat*, triggeropt).
   */
  template <typename S>
  literal_basedt(literal_basedt<S> *_literal_based)
    : literal(std::move(_literal_based->literal))
  {
  }

  /** 
   * @brief Cast down to inheriting type (e.g., for output)
   *
   * @return A (const) reference to the object itself but cast to the inheriting type.
   */
  const T& asSpecializedType() const
  {
    return static_cast<const T&>(*this);
  }

  const atomt& get_atom() const
  {
    return literal->get_atom();
  }

  literalt& get_literal() const
  {
    return *literal;
  }

  std::size_t get_number_of_terms() const
  {
    return literal->get_number_of_terms();
  }

  termt& get_term(std::size_t index) const
  {
    ASSERT(index < get_number_of_terms(), "term index out of bounds in " << asSpecializedType());
    return literal->get_term(index);
  }
};

#endif /* AGENTSPEAK_LITERAL_BASED_H */
