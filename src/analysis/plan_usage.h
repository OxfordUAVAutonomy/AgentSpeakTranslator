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
 * @file plan_usage.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_PLAN_USAGE_H
#define AGENTSPEAK_PLAN_USAGE_H

#include <map>
#include <set>

#include "agentspeak_syntax.h"
#include "has_atom_set.h"

/**
 * @brief Simple analysis collecting information on which atoms are triggered internally in what way in order to avoid generating unnecessary functions in the C code.
 *
 */
class plan_usaget
{
private:
  const agentt &agent; /**< @brief The agent for which this analysis is conducted. */

  /** \TODO while an atom may be used as both achieve and achieve_new_focus, we probably don't want to allow them being used for, e.g., add and achieve, this can be easily added */
  std::map<const atomt*, std::set<formulatypet> > usage; /**< @brief Record in what way each atom is used */

  void collect_usage(void);

public:
  /**
   * @brief Store given parameters and collect usage of atoms.
   */
  plan_usaget(
    const agentt &_agent)
    : agent(_agent)
  {
    collect_usage();
  }
  
  /**
   * @brief Check whether an atom is used in the given way.
   */
  bool is_used_as(const atomt&, formulatypet type) const;
};

#endif /* AGENTSPEAK_PLAN_USAGE_H */

