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
 * @file plan_translator.cpp
 *
 * @author Samuel Bucheli
 */

#include <algorithm>
#include <ostream>
#include <vector>

#include "agentspeak_syntax.h"
#include "plan_translator.h"
#include "name_translation.h"
#include "util.h"

/** Plan translation proceeds as follows: */
void plan_translatort::emit_plan()
{
  /** - the plan's name consists of the trigger name and the plan's index */
  triggert &trigger = plan.get_trigger();
  const atomt &atom = trigger.get_atom();
  strm << "/*\n";
  strm << plan << "\n";
  strm << "*/\n";
  strm << "static bool " << get_plan_name(trigger) << "_plan" << index << "(";
  strm << get_param_list(atom);
  strm << ")" << "\n";
  strm << "{\n";
  /** - instantiate all trigger parameters, this makes translation of formulae easier, as they can access variables directly by their name, rather than having to figure out which parameter of the trigger they have to access */
  strm << "  /* instantiate trigger parameters */\n";
  for (std::size_t i = 0;
       i < trigger.get_number_of_terms();
       i++)
  {
    termt &term = trigger.get_term(i);
    ASSERT(term.is_var(), "term " << term << " in trigger " << trigger << " needs to be a variable");
    vartermt &var = dynamic_cast<vartermt&>(term);
    strm << "  " << atom.get_type(i) << " " << var << " = " << "param" << i << ";\n";
    mark_instantiated(var);
  }
  /** - for the plan context, a plan checks for its own applicability, if one of the tests fails, the plan returns false, this will lead the applicable plan selection method to try the next plan in order of textual appearance */
  strm << "  /* plan context, i.e., guards */\n";
  for (std::size_t i = 0;
       i < plan.get_context_size();
       i++)
  {
    conditiont &condition = plan.get_condition(i);
    emit_context(condition);
  } 
  /** - translate the plan body, this can be done formula by formula */
  strm << "  /* plan body */\n";
  for (std::size_t i = 0;
       i < plan.get_body_size();
       i++)
  {
    formulat &formula = plan.get_formula(i);
    emit_formula(formula, i == plan.get_body_size()-1);
  }
  /** - if the plan body was successfully executed, the plan returns true, thus indicating to the calling method (the applicable plan selection) that no further plans need to be tried */
  strm << "  return true;\n";
  strm << "}\n";
  strm << "\n";
}

bool plan_translatort::is_instantiated(vartermt &var)
{
    return (std::find(var_instantiated.begin(), 
                  var_instantiated.end(), 
                  var.get_name()) != var_instantiated.end());
}

void plan_translatort::mark_instantiated(vartermt &var)
{
  var_instantiated.push_back(var.get_name());
}


bool plan_translatort::term_ok(termt &term)
{
  if (!term.is_val() && !term.is_var()) return false;
  if (term.is_var())
  {
    vartermt &var = dynamic_cast<vartermt&>(term);
    if (!var.is_named()) return false;
    return is_instantiated(var);
  }
  return true;
}


void plan_translatort::emit_literal(literalt &literal)
{
  /** \TODO checks for instantiated variables or values! */
  const atomt &atom = literal.get_atom();
  strm << atom.get_name() << "(";
  if (literal.get_number_of_terms() > 0)
  {
    for (std::size_t i = 0;
         i < literal.get_number_of_terms()-1;
         i++)
    {
      termt &term = literal.get_term(i);
      strm << term << ", ";
    }
    termt &term = literal.get_term(literal.get_number_of_terms()-1);
    strm << term;
    }
  strm << ")";
}

void plan_translatort::emit_formula_action(formulat &formula)
{
  ASSERT(formula.get_type() == formulatypet::action, "formula " << formula << " needs to be a basic action");
  strm << "  ";
  emit_literal(formula.get_literal());
  strm << ";\n";
}

void plan_translatort::emit_formula_assign(formulat &formula)
{
  ASSERT(formula.get_type() == formulatypet::assign, "formula " << formula << " needs to be an assignment");
  /** \TODO should we allow already instantiated variables here? */
  vartermt &var = formula.get_variable();
  ASSERT(!is_instantiated(var), "variable " << var << " in formula " << formula << " is not instantiated");
  strm << "  " << formula.get_atom().get_return_type() << " " << formula.get_variable() << " = ";
  emit_literal(formula.get_literal());
  strm << ";\n";
}

void plan_translatort::emit_formula_achieve_type(formulat &formula, bool last)
{
  ASSERT(analysis.trigger_graph.is_triggering(formula.get_atom()), "formula " << formula << " has no corresponding plan with triggering event");
  const atomt &atom = formula.get_atom();
  if (!last)
    ASSERT(!analysis.trigger_graph.is_recursive(atom), "recursively triggering formula " << formula << " can only occur as last formula of plan");
  /** \TODO check the terms are values or instantiated variables */
  strm << "  ";
  /** \TODO If we allow new focus goals to appear in different places than as the last formula of a plan, the following needs to be amended to last || (formula.get_type() == formulatypet::achieve_new_focus) */
  if (last)
  {
    strm << get_trigger_function_name(formula, true);
  }
  else
  {
    strm << get_plan_name(formula);
  }
  strm << "(";
  if (formula.get_number_of_terms() > 0)
  {
    for (std::size_t i = 0;
         i < formula.get_number_of_terms()-1;
         i++)
    {
      termt &term = formula.get_term(i);
      strm << term << ", ";
    }
    strm << formula.get_term(formula.get_number_of_terms()-1);
  }
  strm << ");\n";

}

void plan_translatort::emit_formula_achieve(formulat &formula, bool last)
{
  ASSERT(formula.get_type() == formulatypet::achieve, "formula " << formula << " needs to be of achieve type");
  emit_formula_achieve_type(formula, last);
}

void plan_translatort::emit_formula_achieve_new_focus(formulat &formula, bool last)
{
  ASSERT(formula.get_type() == formulatypet::achieve_new_focus, "formula " << formula << " needs to be of achieve new focus type");
  /** \TODO should we relax this restriction? new focus achieve can occur at any place in the plan as long as no other triggering events occur there */
  ASSERT(last, "formula " << formula << " can only occur as last formula in plan");
  emit_formula_achieve_type(formula, last);
}

void plan_translatort::emit_formula_test(formulat &formula)
{
  ASSERT(formula.get_type() == formulatypet::test, "formula " << formula << " needs to be of test type");
  const atomt &atom = formula.get_atom();
  /** \TODO very simplistic implementation of test, AgentSpeak semantics are not specified properly here */
  strm << "  if (!" << atom.get_name() << "_set)\n  {\n    // TODO: this case needs to be specified\n  }\n";
  for (std::size_t i = 0;
       i < formula.get_number_of_terms();
       i++)
  {
    termt &term = formula.get_term(i);
    ASSERT(term.is_var(), "term " << term << " in formula " << formula << " needs to be a variable");
    vartermt &var = dynamic_cast<vartermt&>(term);
    strm << "  " << atom.get_type(i) << " " << var;
    strm << " = " << get_param_name(atom.get_name(), i);
  } 
  strm << ";\n";
}

/** \TODO Lots of duplicated code with plan_translatort::emit_formula_del(formulat&, bool) */
void plan_translatort::emit_formula_add(formulat &formula, bool last)
{
  ASSERT(formula.get_type() == formulatypet::add, "formula " << formula << " needs to be of type add");  
  const atomt &atom = formula.get_atom();
  /** \TODO this will have to move once the variants for triggering formulae are implemented */
  strm << "  " << atom.get_name() << "_set = true;\n";
  for (std::size_t i = 0;
       i < atom.get_arity();
       i++)
  {
    /** \TODO check terms are values */
    strm << "  " << get_param_name(atom.get_name(), i) << " = " << formula.get_term(i) << ";\n";
  }
  /** \TODO implement translation of recursively triggering add formula */
  if (analysis.trigger_graph.is_recursive(atom))
  {
    ASSERT(last, "formula " << formula << " can only occur as last formula of plan");
    ASSERT(false, "translation of recursively triggering formula " << formula << " is not implemented yet");
  }
  /** \TODO implement translation of triggering add formula */
  if (analysis.trigger_graph.is_triggering(atom))
  {
    ASSERT(false, "translation of triggering formula " << formula << " is not implemented yet");
  }
}

/** \TODO Lots of duplicated code with plan_translatort::emit_formula_add(formulat&, bool) */
void plan_translatort::emit_formula_del(formulat &formula, bool last)
{
  ASSERT(formula.get_type() == formulatypet::del, "formula " << formula << " needs to be of type del");
  const atomt &atom = formula.get_atom();
  /** \TODO this will have to move once the variants for triggering formulae are implemented */
  strm << "  " << atom.get_name() << "_set = false;\n";
  /** \TODO implement translation of recursively triggering del formula */
  if (analysis.trigger_graph.is_recursive(atom))
  {
    ASSERT(last, "formula " << formula << " can only occur as last formula of plan");
    ASSERT(false, "translation of recursively triggering formula " << formula << " is not implemented yet");
  } 
  /** \TODO implement translation of triggering add formula */
  if (analysis.trigger_graph.is_triggering(atom))
  {
    ASSERT(false, "translation of triggering formula " << formula << " is not implemented yet");
  }
}

/** \TODO maybe it's better to abandon the formulatype and make these polymorphic? */
void plan_translatort::emit_formula(formulat &formula, bool last)
{
  switch (formula.get_type())
  {
    case formulatypet::action:
      emit_formula_action(formula);
      break;
    case formulatypet::assign:
      emit_formula_assign(formula);
      break;
    case formulatypet::achieve:
      emit_formula_achieve(formula, last);
      break;
    case formulatypet::achieve_new_focus:
      emit_formula_achieve_new_focus(formula, last);
      break;
    case formulatypet::test:
      emit_formula_test(formula);
      break;
    case formulatypet::add:
      emit_formula_add(formula, last);
      break;
    case formulatypet::del:
      emit_formula_del(formula, last);
      break;
  }
}

void plan_translatort::emit_context_basic_function(conditiont &condition)
{
  /** \TODO This results in a deviation from the (arguably not very popular). MISRA C Rule 14.7: "A function shall have a single point of exit at the end of the function." */
  const atomt &atom = condition.get_atom();
  ASSERT(config.is_basic_function(atom), "atom " << atom << " needs to be a basic function");
  strm << "  if (";
  if (!condition.is_negated())
  {
    strm << "!";
  }
  strm << atom.get_name() << "(";
  if (condition.get_number_of_terms() > 0)
  {
    for (std::size_t i = 0;
         i < condition.get_number_of_terms()-1; 
         i++)
    {
      termt &term = condition.get_term(i);
      //assert(term_ok(term));
      strm << term << ",";
    }
    termt &term = condition.get_term(condition.get_number_of_terms()-1);
    //assert(term_ok(term));
    strm << term;
  }
  strm << "))\n  {\n    return false;\n  }\n";
}

void plan_translatort::emit_context_unification(conditiont &condition)
{
  /** \TODO This results in a deviation from the (arguably not very popular) MISRA C Rule 14.7: "A function shall have a single point of exit at the end of the function." */
  const atomt &atom = condition.get_atom();
  ASSERT(!config.is_basic_function(atom), "atom " << atom << "can not be a basic function");
  /** \TODO this only works for belief base elements without parameters => needs refactoring */
  strm << "  if (";
  if (!condition.is_negated())
  {
    strm << "!"; 
  }
  strm << atom.get_name() << "_set)\n  {\n    return false;\n  }\n";
  for (std::size_t i = 0;
       i < condition.get_number_of_terms();
       i++)
  {
    termt &term = condition.get_term(i);
    ASSERT(term.is_val() || term.is_var(), "term " << term << " can only be a variable or a value");
    if (term.is_val())
    {
      /** \TODO check values are equal to belief base */
      strm << "  // TODO: check values are equal to belief base\n";
        ASSERT(false, "checking " << term << " in " << atom << " is equal to value in belief base not implemented yet");
    }
    if (term.is_var())
    {
      vartermt &var = dynamic_cast<vartermt&>(term);
      if (is_instantiated(var))
      {
        /** \TODO check instantiated variables are equal to belief base */
        /** \TODO build in mechanism for more elaborate/custom equals-check on structs, etc. */
        strm << "  // TODO: check instantiated variables are equal to belief base\n";
        ASSERT(false, "checking " << var << " in " << atom << " is equal to value in belief base not implemented yet");
      }
      else
      {
        strm << "  " << atom.get_type(i) << " " << var;
        strm << " = " << get_param_name(atom.get_name(), i);
        strm << ";\n";
        mark_instantiated(var);
      }
    } 
  }
}

void plan_translatort::emit_context(conditiont &condition)
{
  const atomt &atom = condition.get_atom();
  if (config.is_basic_function(atom))
  {
    emit_context_basic_function(condition);
  }
  else
  {
    emit_context_unification(condition);
  }
}


