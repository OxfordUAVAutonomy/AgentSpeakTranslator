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
 * @file tarjan_scc.h
 *
 * @author Samuel Bucheli
 */


#ifndef AGENTSPEAK_TARJAN_SCC_H
#define AGENTSPEAK_TARJAN_SCC_H

#include <deque>
#include <map>
#include <set>

/**
 * @brief A simple implementation of Tarjan's algorithm for strongly connected components
 */
template <typename T>
class tarjan_scct
{
private:
  const std::map<T, std::set<T> > &graph; /**< @brief We represent using adjacency lists. */

  unsigned int current_index; /**< @brief To store the current index for DFS. */
  std::map<T, std::size_t> index; /**< @brief Indices assigned to nodes. */
  std::map<T, std::size_t> lowlink; /**< @brief Low-links assigned to nodes. */
  std::deque<T> stack; /**< @brief Stack for DFS. */

  std::set<std::set<T> > sccs;  /**< @brief The set of resulting strongly connected components. */
  
  /**
   * @brief Recursively find strongly connected components starting from given node.
   */
  void scc(T v)
  {
    index[v] = current_index;
    lowlink[v] = current_index;
    current_index++;
    stack.push_back(v);
 
    for (auto nodes = graph.at(v).cbegin();
         nodes != graph.at(v).cend();
         ++nodes)
    {
      T w = *nodes;
      if (index.find(w) == index.end())
      {
        scc(w);
        lowlink[v] = std::min(lowlink[v], lowlink[w]);
      }
      else if (std::find(stack.begin(), stack.end(), w) != stack.end())
      {
        lowlink[v] = std::min(lowlink[v], index[w]);
      }
    }

    // root node found => new scc
    if (lowlink[v] == index[v])
    {
      std::set<T> scc;
      bool done = false;
      do
      {
        T w = stack.back();
        stack.pop_back();
        scc.insert(w);
        done = (v == w);
      } while (!done);
      sccs.insert(scc);
    }
  }

public:
  tarjan_scct(
    const std::map<T, std::set<T> > &_graph)
    : graph(_graph),
      current_index(0)
  {
  }

  /**
   * @brief Find strongly connected components for all nodes and return result.
   */
  std::set<std::set<T> > get_scc()
  {
    /** \TODO Maybe populate sccs upon creation of this object? */
    for (auto nodes = graph.cbegin();
         nodes != graph.cend();
         ++nodes)
    {
      if (index.find(nodes->first) == index.end())
      {
        scc(nodes->first);
      }
    }
    return sccs;
  }

};

#endif /* AGENTSPEAK_TARJAN_SCC_H */

