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
 * @file has_atom_set.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_HAS_ATOM_SET_H
#define AGENTSPEAK_HAS_ATOM_SET_H

#include <set>

#include "atom.h"
#include "symbol_table.h"
#include "util.h"

/**
 * @brief Auxiliary class for analysis classes, @sa believablest @sa plan_usaget.
 *
 */
class has_atom_sett
{
protected:
  void add_to_set(const atomt &atom, std::set<const atomt*> &set)
  {
    set.insert(&atom);
  }
 
  bool is_in_set(const atomt &atom, const std::set<const atomt*> &set) const
  {
    return (set.find(&atom) != set.end());
  }
};

/**
 * @brief Auxiliary class for config class, @sa agent_configt.
 *
 */
class has_atom_set_and_tablet : public has_atom_sett
{
protected:
  atom_tablet &atom_table;

  atomt& get_atom(const std::string &atom_name)
  {
    ASSERT(atom_table.contains(atom_name), "no types for atom " << atom_name << " defined");
    atomt &atom = atom_table.get(atom_name);
    return atom;
  }

  void add_atom_to_set(const std::string &atom_name, std::set<const atomt*> &set)
  {
    const atomt &atom = get_atom(atom_name);
    add_to_set(atom, set);
  }

public:
  has_atom_set_and_tablet(
    atom_tablet &_atom_table)
    : atom_table(_atom_table)
  {
  }
};

#endif /* AGENTSPEAK_HAS_ATOM_SET_H */
