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
 * @file atom.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_ATOM_H
#define AGENTSPEAK_ATOM_H

#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include <stddef.h>

#include "util.h"

class atom_buildert;

typedef std::string typet;

/**
 * @brief Atoms act the basic building blocks of the AgentSpeak syntax. Basically, they are predicate symbols, have an arity, and, for translation in our case, are typed. They may also have a return type in case they are used as basic functions.
 */
class atomt
{
  std::string name; /**< @brief The identifier of the atom (typically starting with a lower-case letter in the AgentSpeak/logic programming tradition) */
  typet return_type; /**< @brief If the atom represents a basic function, it has a return type */
  std::vector<typet> parameter_types; /**< @brief The list of types of the parameters, this also implicitly defines the arity of the atom */

  friend std::ostream& operator<<(std::ostream&, const atomt&);
  friend class atom_buildert;

  /**
   * @brief This constructor is used by the atom builder, so the parser can start out with first setting the name and then adding types.
   */
  atomt(const std::string _name)
    : name(_name)
  {
  }

  /**
   * @brief Make atoms un-copyable.
   */
  atomt(const atomt &other) = delete;

  /**
   * @brief Make atoms un-assignable.
   */
  atomt &operator=(const atomt &other) = delete;

  /**
   * @brief Used by the atom builder.
   */
  void set_return_type(const typet type)
  {
    return_type = type;
  }

  /**
   * @brief Used by the atom builder.
   */
  void push_front(const typet &type)
  {
    parameter_types.insert(parameter_types.begin(), type);
  }

public:
  /**
   * @brief Construct an atom with the given name and arity.
   */
  atomt(const std::string _name, const std::size_t _arity)
    : name(_name),
      parameter_types(_arity)
  {
  }

  std::string get_name() const
  {
    return name;
  }

  std::size_t get_arity() const
  {
    return parameter_types.size();
  }

  bool has_return_type() const
  {
    return (return_type != "");
  }

  typet get_return_type() const
  {
    ASSERT(has_return_type(), "no return type defined for atom " << get_name());
    return return_type;
  }

  typet get_type(std::size_t index) const
  {
    ASSERT(index < get_arity(), "index for type out of bounds for atom " << get_name());
    return parameter_types[index];
  }

  /**
   * @brief A convenience method for debugging.
   */
  void print_type_info(std::ostream& strm) const
  {
    int arity = get_arity();
    strm << name << "(";
    for (int i=0; i < arity-1; i++)
      strm << get_type(i) << ", ";
    if (arity>0)
      strm << get_type(arity-1);
    else
      strm << "void";
    strm << ")/" << arity;
    if (has_return_type())
      strm << " = " << get_return_type();
  }
};

#endif /* AGENTSPEAK_ATOM_H */
