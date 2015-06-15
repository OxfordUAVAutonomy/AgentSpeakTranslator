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
 * @file used_variables.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_USED_VARIABLES_H
#define AGENTSPEAK_USED_VARIABLES_H

#include <set>

#include "agentspeak_syntax.h"

/**
 * @brief Simple analysis determining which variables are actually used in a plan. Unused variables can be replaced by unnammed variables in the original AgentSpeak code, and in turn, these variables can be omitted from the generated C code.
 * 
 * \TODO This is not in use yet.
 */
class used_variablest
{
private:
  const plant &plan; /**< @brief The plan to be analyzed. */

  std::set<std::string> used; /**< @brief The set of used variables. We use the variable name as the unique identifier of a variable within the given plan. */

  /** @brief Collect all used variables in the given plan. */
  void collect_used(void);
  
  /** @brief Collect all variables used in the given formula. */
  void collect_from_literal(const formulat&);

public:
  /** @brief Store given parameters and collect used variables for given plan. */
  used_variablest(
    const plant &_plan)
    : plan(_plan)
  {
    collect_used();
  }

  bool is_used(std::string);
};

#endif /* AGENTSPEAK_USED_VARIABLES_H */

