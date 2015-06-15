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
 * @file agentspeak_parser_driver.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_PARSER_DRIVER_H
#define AGENTSPEAK_PARSER_DRIVER_H

#include <istream>
#include <map>
#include <memory>
#include <ostream>
#include <string>

#include "agentspeak_parser.h"

#include "generic_parser_driver.h"
#include "agent_builder.h"
#include "agentspeak_syntax.h"

/**
 * @brief Driver for AgentSpeak parser, @sa generic_parser_drivert.
 */
class agentspeak_parser_drivert
  : public generic_parser_drivert<class agentspeak_parser_drivert>
{
public:
  agent_buildert agent_builder;

  agentspeak_parser_drivert(std::istream *_is, agentt &_agent)
    : generic_parser_drivert(_is),
    agent_builder(_agent)
  {
    init_scanner();
  }

  virtual ~agentspeak_parser_drivert()
  {
    destroy_scanner();
  }

  bool parse();

protected:
  virtual void init_scanner();	
  virtual void destroy_scanner();
};

int agentspeak_parser_parse(agentspeak_parser_drivert *driver);

#endif // AGENTSPEAK_PARSER_DRIVER_H

