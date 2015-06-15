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
 * @file pretty_print_vector.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_PRETTY_PRINT_VECTOR_H
#define AGENTSPEAK_PRETTY_PRINT_VECTOR_H

#include <memory>
#include <ostream>
#include <string>
#include <vector>

/**
 * @brief Auxiliary class for pretty printing vectors.
 *
 * \TODO replace with a nicer solution, maybe http://louisdx.github.io/cxx-prettyprint/ ?
 */
template<typename T>
class pretty_print_vectort 
{
  const std::vector<T>& vector;
  const std::string separator;
  const bool afterEach;
  const std::string ifEmpty;

  friend std::ostream& operator<<(
    std::ostream &strm, 
    const pretty_print_vectort<T> &p)
  {
    if (p.vector.empty())
    {
      return strm << p.ifEmpty;
    }
    int last = p.vector.size();
    if (!p.afterEach) last -= 1;
    for (int i = 0; i<last; i++) strm << *(p.vector.at(i)) << p.separator;
    if (!p.afterEach) strm << *(p.vector.at(last));
    return strm;
  }

public:
  pretty_print_vectort(
    const std::vector<T >& _vector, 
    const char *_separator, 
    bool _afterEach, 
    const char *_ifEmpty)
    : vector(_vector),
    separator(_separator),
    afterEach(_afterEach),
    ifEmpty(_ifEmpty)
  {
  }
};

#endif /* AGENTSPEAK_PRETTY_PRINT_VECTOR_H */
