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
 * @file believables.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_BELIEVABLES_H
#define AGENTSPEAK_BELIEVABLES_H

#include <set>

#include "agentspeak_syntax.h"
#include "has_atom_set.h"

/**
 * @brief Simple analysis collecting information on which atoms can occur as elements of the belief base. We call such atoms believables.
 *
 */
class believablest : public has_atom_sett
{
private:
  const agentt &agent; /**< @brief The agent for which the analysis is to be conducted. */

  std::set<const atomt*> believable; /**< @brief The set of believables. */

  void collect_believables(); /**< @brief Finds all believables for the given agent. */

public:
  /**
   * @brief Store given parameters and collect believables.
   */
  believablest(
    const agentt &_agent)
    : agent(_agent)
  {
    collect_believables();
  }

  /**
   * @brief Check whether the given atom is believable.
   */
  bool is_believable(const atomt&) const;
};

#endif /* AGENTSPEAK_BELIEVABLES_H */

