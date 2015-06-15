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
 * @file plan.cpp
 *
 * @author Samuel Bucheli
 */

#include <memory>
#include <ostream>
#include <vector>

#include "condition.h"
#include "formula.h"
#include "plan.h"
#include "pretty_print_vector.h"
#include "trigger.h"

std::ostream& operator<<(std::ostream &strm, const plant &p) 
{
  return strm << *(p.trigger) 
              << "\n   : "
              << pretty_print_vectort<std::unique_ptr<conditiont>>(p.context, " & ", false, "true") 
              << "\n   <- "
              << pretty_print_vectort<std::unique_ptr<formulat>>(p.body, ";\n      ", false, "true\n");
              
}

