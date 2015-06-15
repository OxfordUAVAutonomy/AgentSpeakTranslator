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
 * @file term.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_TERM_H
#define AGENTSPEAK_TERM_H

#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "atom.h"
#include "pretty_print_vector.h"
#include "util.h"

/**
 * @brief Common base class for all terms.
 *
 * \TODO Rule of three! Needs copy constructor and copy assigment operator.
 */
class termt
{
public:
  /**
   * @brief Virtual destructor.
   */
  virtual ~termt()
  {
  }

  /**
   * @brief Actual values (strings, numbers) can override this.
   */
  virtual bool is_val() const
  {
    return false;
  }

  /**
   * @brief Actual variables can override this.
   */
  virtual bool is_var() const
  {
    return false;
  }

protected:
  virtual void print(std::ostream& strm) const = 0; /**< @brief Overwritten by inheriting methods for output, @sa operator<<(std::ostream&, const termt&).  */
  friend std::ostream& operator<<(std::ostream&, const termt&);
};

/**
 * @brief Variable term, represents a named or unnamed variable.
 */
class vartermt : public termt
{
  std::string name; /**< @brief The name of the variable */
  bool unnamed; /**< @brief Indicates whether the variable is unnamed (in AgentSpeak, unnamed variables can have a "name" as long as they begin with an underscore, it seems) */

protected:
  virtual void print(std::ostream &strm) const {
    strm << name;
  }

public:
  /**
   * @brief Construct a variable with the given name.
   */
  vartermt(const std::string& _name, bool _unnamed=false)
    : name(_name),
    unnamed(_unnamed)
  {
  }

  ~vartermt()
  {
  }

  bool is_named() const
  {
    return !unnamed;
  }

  std::string get_name() const
  {
    return name;
  }

  bool is_var() const
  {
    return true;
  }
};

typedef std::vector<std::unique_ptr<termt> > termst;

/**
 * @brief Represents a (double) floating point number, the standard number type in AgentSpeak.
 */
class numbertermt : public termt 
{
  double value; /**< @brief The value of the term */

protected:
  virtual void print(std::ostream &strm) const {
    strm << value;
  }


public:
  numbertermt(double _value)
    : value(_value)
  {
  }

  ~numbertermt()
  {
  }
 
  bool is_val()
  {
    return true;
  }
};

/**
 * @brief Represents a string
 */
class stringtermt : public termt 
{
  std::string value; /**< @brief The value of the term */

protected:
  virtual void print(std::ostream &strm) const {
    strm << value;
  }


public:
  stringtermt(const std::string& _value)
    : value(_value)
  {
  }
  
  ~stringtermt()
  {
  }

  bool is_val()
  {
    return true;
  }
};

/**
 * @brief Represents a literal, @sa literal_basedt
 */
class literalt : public termt
{
  const atomt &atom; /**< @brief The atom, i.e., predicate symbol, of the literal */
  termst terms; /**< @brief The list of terms of the literal */

protected:
  virtual void print(std::ostream &strm) const {
    strm << atom
         << "("
         << pretty_print_vectort<std::unique_ptr<termt>>(terms, ", ", false, "")
         << ")";
  }

public:
  /**
   * @brief Build a literal from the given atom, this is used by triggert::triggert(atomt&, triggeropt, triggertypet) as the triggerables @ref trigger_collectort reads from the configuration are listed without terms..
   */
  literalt(const atomt &_atom, termst* _terms = new termst())
    : atom(_atom),
    terms(std::move(*_terms))
  {
    /** \TODO Fill terms with (dummy) variables if _terms is empty? */
  }

  ~literalt()
  {
  }

  const atomt& get_atom() const
  {
    return atom;
  }

  std::size_t get_number_of_terms() const
  {
    return terms.size();
  }

  termt& get_term(std::size_t index) const
  {
    ASSERT(index < get_number_of_terms(), "term index out of bounds");
    return *(terms[index]);
  }
};

typedef std::vector<std::unique_ptr<literalt> > beliefst; /**< @brief (Initial) beliefs are a list of literals */

#endif /* AGENTSPEAK_TERM_H */
