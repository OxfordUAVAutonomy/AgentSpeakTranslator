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
 * @file plan_translator.h
 *
 * @author Samuel Bucheli
 */

#ifndef AGENTSPEAK_PLAN_TRANSLATOR_H
#define AGENTSPEAK_PLAN_TRANSLATOR_H

#include <ostream>
#include <vector>

#include "agent_analysis.h"
#include "agentspeak_syntax.h"

/**
 * @brief Takes care of all translation tasks related to one plan, this is called from trigger_translatort::emit_plans().
 */
class plan_translatort
{
  const plant &plan; /**< @brief The plan to be translated. */
  const agent_configt &config; /**< @brief The given agent configuration. */
  const agent_analysist &analysis; /**< @brief The given agent analysis. */
  const std::size_t index; /**< @brief Plans with the same triggering event are enumerated in textual order, index stores the plan's position in this list. */
  std::ostream &strm; /**< @brief The output stream for the generated C code. */

  typedef std::vector<std::string> var_instantiatedt;   /**< @brief A convenience abbreviation. */
  var_instantiatedt var_instantiated; /**< @brief For storing which variables have been instantiated so far. */

public:
  /**
   * @brief Store all given parameters.
   */
  plan_translatort(
    const plant &_plan,
    const agent_configt &_config,
    const agent_analysist &_analysis,
    const std::size_t _index,
    std::ostream &_strm)
    : plan(_plan),
    config(_config),
    analysis(_analysis),
    index(_index),
    strm(_strm)
  {
  }

  /**
   * @brief Emit the C code for the given plan.
   */
  void emit_plan();

private:
  /**
   * @brief Simple check for terms.
   *
   * \TODO Currently not in use.
   */
  bool term_ok(termt &term);

  /**
   * @brief Check whether the given variable occurs on the list of instantiated variables.
   */
  bool is_instantiated(vartermt &var);

  /** 
   * @brief Add given variable to list of instantiated variables. 
   */
  void mark_instantiated(vartermt &var);

  /**
   * @brief Emits a literal with its parameters, used in plan_translatort::emit_formula_action(formulat&) and plan_translatort::emit_formula_assign(formulat&).
   */  
  void emit_literal(literalt &literal);

  /**
   * @brief Emits the translation of a basic action, basically just a call to the corresponding function.
   */
  void emit_formula_action(formulat &formula);

  /**
   * @brief Emits the translation of an assignment, basically just a call to the corresponding function with an assignment of the return value.
   */
  void emit_formula_assign(formulat &formula);

  /**
   * @brief Emits the translation of a formula of achieve type. Used by plan_translatort::emit_formula_achieve(formulat&, bool) and plan_translatort::emit_formula_achieve_new_focus(formulat&, bool).
   *
   * @param formula The formula to be translated.
   * @param last Indicates whether this formula is the last formula of the plan.
   *
   * - if the formula is not the last formula of the plan, it is not allowed to be recursively triggering. In this case, the formula can be translated to a call of the corresponding applicable plan selection.
   * - if the formula is the last formula of the plan, it may be recursively triggering. In this case, the formula is translated to the corresponding trigger function, i.e., creating a corresponding event.
   */
  void emit_formula_achieve_type(formulat &formula, bool last);

  /**
   * @brief See plan_translatort::emit_formula_achieve_type(formulat&, bool).
   */
  void emit_formula_achieve(formulat &formula, bool last);

  /**
   * @brief See plan_translatort::emit_formula_achieve_type(formulat&, bool).
   */
  void emit_formula_achieve_new_focus(formulat &formula, bool last);

  /**
   * Emits the translation of a formula of test type. The translated code checks whether the corresponding element of the belief base is set, and if so, reads its parameters into the given variables. 
   */
  void emit_formula_test(formulat &formula);

  /**
   * Emits the translation of a formula for adding a belief. In the simplest variant, this means just storing the belief in the belief base.
   */
  void emit_formula_add(formulat &formula, bool last);

  /**
   * Emits the translation of a formula for deleting a belief. In the simplest variant, this means just setting the corresponding flag in the belief base to false.
   */
  void emit_formula_del(formulat &formula, bool last);

  /**
   * Emits the translation of the given formula depending on its type using
   * - plan_translatort::emit_formula_achieve(formulat&, bool),
   * - plan_translatort::emit_formula_achieve_new_focus(formulat&, bool),
   * - plan_translatort::emit_formula_action(formulat&),
   * - plan_translatort::emit_formula_add(formulat&, bool),
   * - plan_translatort::emit_formula_assign(formulat&),
   * - plan_translatort::emit_formula_del(formulat&, bool),
   * - plan_translatort::emit_formula_test(formulat&).
   */
  void emit_formula(formulat &formula, bool last);

  /**
   * @brief Emits a call to a basic function and checks whether it returns true. Used in plan_translatort::emit_context(conditiont&).
   */
  void emit_context_basic_function(conditiont &condition);

  /**
   * @brief Emits code to read the belief base (a very simplistic variant of unification). Used in plan_translatort::emit_context(conditiont&).
   */
  void emit_context_unification(conditiont &condition);

  /**
   * @brief Emits the code for checking whether the plan is applicable using either plan_translatort::emit_context_basic_function(conditiont&) or plan_translatort::emit_context_unification(conditiont&).
   */
  void emit_context(conditiont &condition);
};

#endif /* AGENTSPEAK_PLAN_TRANSLATOR_H */

