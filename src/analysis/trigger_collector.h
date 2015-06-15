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
 * @file trigger_collector.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_TRIGGER_COLLECTOR_H
#define AGENTSPEAK_TRIGGER_COLLECTOR_H

#include <map>
#include <vector>

#include "agentspeak_syntax.h"

/**
 * @brief A convenient abbreviation for use in trigger_collectort.
 */
typedef std::map<const triggert*, std::vector<const plant*>, trigger_comparatort> plans_mapt;
/**
 * @brief A convenient abbreviation for use in trigger_iteratort.
 */
typedef plans_mapt::const_iterator plans_map_iteratort;

/**
 * @brief A simple helper class facilitating iteration over the key set of the plans map.
 */
struct trigger_iteratort : public plans_map_iteratort
{
  trigger_iteratort() : plans_map_iteratort()
  {
  }

  trigger_iteratort(plans_map_iteratort _it) 
    : plans_map_iteratort(_it)
  {
  }

  const triggert* operator->()
  {
    return (const triggert* const)&(plans_map_iteratort::operator->()->first);
  }

  const triggert* operator*()
  {
    return plans_map_iteratort::operator*().first;
  }
};

/**
 * @brief Collect all triggers that occur as the triggering event of a plan (considering them equal with respect to the order defined by @ref trigger_comparatort) and also store their corresponding plans.
 *
 * \TODO The config file may contain additional triggering events that are not triggering (in the triggerables section). We somehow need to collect them as well, but maybe there's a better place for it or a better way to do so?
 */
class trigger_collectort
{
private:
  const agentt &agent; /**< @brief The agent for which this analysis is to be conducted. */
  const agent_configt &config; /**< @brief The configuration for this agent, as there may be triggerable events only occurring in the configuration. */
  
  plans_mapt plans; /**< @brief A map associating each trigger with its plans. */

  /**
   * @brief Collect all triggers and their associated plans.
   */
  void collect_triggers();

public:
  /**
   * @brief Store given parameters and collect triggers.
   */
  trigger_collectort(
    const agentt &_agent,
    const agent_configt &_config)
    : agent(_agent),
      config(_config)
  {
    collect_triggers();
  }

  /**
   * @brief Return all plans for the given trigger.
   */
  const std::vector<const plant*>& get_plans(const triggert &trigger) const
  {
    return plans.at(&trigger);
  }

  /**
   * @brief Iterator to the beginning of the set of all triggers (key set of plan map).
   */
  trigger_iteratort begin(void) const
  {
    return trigger_iteratort(plans.cbegin());
  }

  /**
   * @brief Iterator to the end of the set of all triggers (key set of plan map).
   */
  trigger_iteratort end(void) const
  {
    return trigger_iteratort(plans.cend());
  }
};

#endif /* AGENTSPEAK_PLAN_TRANSLATOR_H */
