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
 * @file symbol_table.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_SYMBOL_TABLE_H
#define AGENTSPEAK_SYMBOL_TABLE_H

#include <assert.h>

#include <map>
#include <memory>

#include "agentspeak_syntax.h"
#include "util.h"

/**
 * @brief Generic simple implementation of symbol table as a map from keys (e.g. the unique string identifier of a symbol) to their implementation (e.g., an atom object).
 */
template <class T>
class symbol_tablet
{
public:
  typedef T& V; /**< @brief The value type for the symbol map. */
  typedef std::string K; /**< @brief The key type for the symbol map. */
  typedef typename std::map<K, std::unique_ptr<T>>::const_iterator symbols_iteratort; /**< @brief The symbol map iterator type. */

  /**
   * @brief Construct a new symbol table.
   */
  symbol_tablet()
  {
  }

  /**
   * @brief Check whether the given key is in the symbol table.
   */
  bool contains(
    const K &name) const
  {
    auto iter = symbols.find(name);
    return (iter != symbols.end());
  }

  /**
   * @brief Check whether the given value is in the symbol table. Assumes that the symbol has a name of key type by which it can be identified.
   */
  bool contains(
    const V symbol) const
  {
    return contains(symbol->get_name());
  }

  /**
   * @brief Get the symbol with the given key.
   */
  V get(
    const K& name)
  {
    auto iter = symbols.find(name);
    return *(iter->second);
  }

  /**
   * @brief Add the given symbol. Assumes that the symbol has a name of key type by which it can be identified.
   */
  void insert(
    const V symbol)
  {
    insert(symbol.get_name(), symbol);
  }

  void insert(
    const K& name,
    const V symbol)
  {
    symbols[name] = std::unique_ptr<T>(&symbol);
  }

  /**
   * @brief Iterator to the beginning of the symbol table.
   */
  symbols_iteratort begin() const
  {
    return symbols.begin();
  }

  /**
   * @brief Iterator to the end of the symbol table.
   */
  symbols_iteratort end() const
  {
    return symbols.end();
  }

protected:
  std::map<K, std::unique_ptr<T> > symbols; /**< @brief The symbol table. */

};

/**
 * @brief Symbol table for agents containing all atoms used by the agent.
 */
class atom_tablet 
  : public symbol_tablet<atomt>
{
public:
  /**
   * @brief If the symbol table already contains an atom with the given name and arity, retrieve it. Else, create a new atom, add it to the symbol table, and return the newly created atom.
   */
  atomt& get_or_create(
    const std::string& name, 
    const std::size_t arity)
  {
    if (this->contains(name))
    {
      atomt &atom = this->get(name);
      ASSERT(atom.get_arity() == arity, "atom " << name << " previously defined with arity " << atom.get_arity() << " conflicting with given arity " << arity);
      return atom;
    }
    else
    {
      atomt &atom = *(new atomt(name, arity));
      this->insert(name, atom);
      return atom;
    }
  }

  /**
   * @brief Simple auxiliary method printing the atom table, mostly for debugging.
   */
  void print(std::ostream &strm) const
  {
    for(auto it = symbols.begin(); it != symbols.end(); it++)
    {
      atomt &atom = *(it->second);
      atom.print_type_info(strm);
      strm << "\n";
    }
  }
};

#endif /* AGENTSPEAK_SYMBOL_TABLE_H */

