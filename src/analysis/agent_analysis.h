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
 * @file agent_analysis.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_AGENT_ANALYSIS_H
#define AGENTSPEAK_AGENT_ANALYSIS_H

#include "agent.h"
#include "agent_config.h"
#include "believables.h"
#include "plan_usage.h"
#include "trigger_collector.h"
#include "trigger_graph.h"

/**
 * @brief Convenience class bundling all the various analyses available. See members for details.
 *
 */
class agent_analysist
{
public:
  believablest believables;
  plan_usaget plan_usage;
  trigger_collectort trigger_collector;
  trigger_grapht trigger_graph;

  agent_analysist(
    const agentt &_agent,
    const agent_configt &_config)
    : believables(_agent),
      plan_usage(_agent),
      trigger_collector(_agent, _config),
      trigger_graph(_agent)
  {
  }  
};

#endif /* AGENTSPEAK_AGENT_ANALYSIS_H */
