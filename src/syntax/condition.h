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
 * @file condition.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_CONDITION_H
#define AGENTSPEAK_CONDITION_H

#include <memory>
#include <ostream>
#include <vector>

#include "literal_based.h"
#include "term.h"
#include "util.h"

/**
 * @brief Represents a single element in the context of a plan.
 */
class conditiont : public literal_basedt<conditiont>
{
  bool negated; /**< @brief Indicates whether the condition is prefixed with "not", i.e., negated */

  friend std::ostream& operator<<(std::ostream&, const conditiont&);

public:
  /**
   * @brief Construct a condition based on the given literal. 
   */
  conditiont(literalt* _literal, bool _negated=false)
    : literal_basedt(_literal),
    negated(_negated)
  {
  }

  bool is_negated()
  {
    return negated;
  }
};

/** @brief The context of a plan is a list of conditions */
typedef std::vector<std::unique_ptr<conditiont> > contextt;

#endif /* AGENTSPEAK_CONDITION_H */
