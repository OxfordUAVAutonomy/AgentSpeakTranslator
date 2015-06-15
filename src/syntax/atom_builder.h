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
 * @file atom_builder.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_ATOM_BUILDER_H
#define AGENTSPEAK_ATOM_BUILDER_H

#include <memory>
#include <stack>
#include <string>

#include <stddef.h>

#include "atom.h"
#include "symbol_table.h"
#include "util.h"

/**
 * @brief An auxiliary class for building atoms, used by the parser.
 */
class atom_buildert
{
  atom_tablet &atom_table; /**< @brief The atom table to be populated. */

  atomt *atom; /**< @brief The atom to be built currently. */

public:
  /**
   * @brief Store given atom table.
   */
  atom_buildert(atom_tablet &_atom_table)
    : atom_table(_atom_table)
  {
  }
  
  /**
   * @brief Add a return type to the atom being currently built.
   */
  void add_return_type(const typet &type)
  {
    ASSERT(atom != NULL, "no atom to be currently built set");
    atom->set_return_type(type);
  }

  /**
   * @brief Begin the creation of an atom.
   */
  void add_symbol(const std::string &name)
  {
    atom = new atomt(name);
  }

  /**
   * @brief Add a parameter type to the atom being currently built.
   */
  void add_type_parameter(const typet &type)
  {
    ASSERT(atom != NULL, "no atom to be currently built set");
    atom->push_front(type);
  }

  /**
   * @brief Finish the creation of the atom being currently built by inserting it into the atom table and return the name of the atom just built.
   */
  std::string end_declaration()
  {
    atom_table.insert(atom->get_name(), *atom);
    std::string atom_name = atom->get_name();
    atom = NULL;
    return atom_name;
  }
};

#endif /* AGENTSPEAK_ATOM_BUILDER_H */


