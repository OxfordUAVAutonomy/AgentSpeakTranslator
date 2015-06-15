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
 * @file used_variables.cpp
 *
 * @author Samuel Bucheli
 */

#include <set>

#include "used_variables.h"

#include "agentspeak_syntax.h"

void used_variablest::collect_used()
{
  for (std::size_t i = 0;
       i < plan.get_body_size();
       i++)
  {
    formulat &formula = plan.get_formula(i);
    switch (formula.get_type())
    {
      case formulatypet::achieve:
      case formulatypet::achieve_new_focus:
      case formulatypet::test:
      case formulatypet::add:
      case formulatypet::del:
      case formulatypet::action:
        collect_from_literal(formula);
        break;
      case formulatypet::assign:
        used.insert(formula.get_variable().get_name());
        collect_from_literal(formula);
        break;
      default:
        ASSERT(false, "formula " << formula << " has unknown type");
        break;
    }
  }
}

void used_variablest::collect_from_literal(const formulat &formula)
{
  for (std::size_t i = 0;
       i < formula.get_number_of_terms();
       i++)
  {
    const termt &term = formula.get_term(i);
    if (term.is_var())
    {
       const vartermt &var = dynamic_cast<const vartermt&>(term);
       used.insert(var.get_name());
    }
  }
}


bool used_variablest::is_used(std::string name)
{
  return (used.find(name) != used.end());
}
