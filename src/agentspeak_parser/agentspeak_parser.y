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
 * @file agentspeak_parser.y
 *
 * @brief Bison parser for AgentSpeak programs. It uses the following grammar:
 *
 * @dontinclude agentspeak_parser.output 
 * @skipline Grammar 
 * @until Terminals
 */

%pure-parser
%name-prefix="agentspeak_parser_"
%defines "agentspeak_parser.h"
%locations
%defines
%error-verbose
%parse-param { agentspeak_parser_drivert *driver }
%lex-param { void *scanner }

%code requires{
#include "agentspeak_syntax.h"
#include "agentspeak_parser_driver.h"

class agentspeak_parser_drivert;
}

%union {
    formulat *formula;
    std::string *identifier;
    literalt *literal;
    double number;
    termt *term;
    termst *terms;
}

%token			END	0		"end of file"
%token <identifier> 	TVAR	 		"variable"
%token <identifier> 	TUVAR	 		"unnamed variable"
%token <identifier>	TATOM			"atom"
%token <identifier> 	TSTRING			"string literal"
%token <number>		TNUMBER			"number"
%token			TTRUE			"true"
%token                  TACHIEVENF              "!!"
%token			TLARROW			"<-"
%token			TNOT			"not"

%type <literal> literal
%type <terms> terms
%type <term> term
%type <formula> action assignment formula goal percept

%{
#include <stdio.h>

#include <iostream>
#include <sstream>

#include "agentspeak_parser_driver.h"

int agentspeak_parser_lex(YYSTYPE* lvalp, YYLTYPE* llocp, void* scanner);		

void agentspeak_parser_error(YYLTYPE* locp, agentspeak_parser_drivert *driver, const char* err)
{
  driver->error(locp->first_line, locp->first_column, err);
}

#define scanner driver->scanner
#define agent_builder driver->agent_builder
%}

%start agent

%%

agent : 
  beliefs initial_goal plans END 
  | beliefs plans END
        
beliefs : 
  /*blank*/
  | beliefs literal '.' { agent_builder.add_initial_belief($2); }

literal : 
  TATOM '(' terms ')' { $$ = new literalt(agent_builder.get_atom(*$1, $3->size()), $3); delete $1; delete $3; }
  | TATOM { $$ = new literalt(agent_builder.get_atom(*$1)); delete $1; }

terms : 
  /*blank*/ { $$ = new termst(); }
  | term { $$ = new termst(); $$->push_back(std::unique_ptr<termt>($1)); }
  | terms ',' term { $1->push_back(std::unique_ptr<termt>($3)); $$=$1; }

term : // TODO: provide builders for terms
  literal { $$ = $1; } 
  | TVAR { $$ = new vartermt(*$1); delete $1; }
  | TUVAR { $$ = new vartermt(*$1, true); delete $1; }
  | TNUMBER { $$ = new numbertermt($1); }
  | TSTRING { $$ = new stringtermt(*$1); delete $1; } // TODO: handle strings properly

initial_goal : 
  '!' literal '.' { agent_builder.set_initial_goal(new formulat($2, formulatypet::achieve)); }
  // TODO: This does not make much sense currently as we do not support the more elaborate form of test goals.
  //| '?' literal '.' { agent_builder.set_initial_goal(new formulat($2, formulatypet::test)); }

plans : 
  plan 
  | plans plan

plan : 
  trigger ':' context TLARROW body '.' { agent_builder.plan_finished(); }

trigger: 
  percept { agent_builder.add_plan(new triggert($1)); delete $1; }
  | '+' goal { agent_builder.add_plan(new triggert($2, triggeropt::add)); delete $2; }
  | '-' goal { agent_builder.add_plan(new triggert($2, triggeropt::del)); delete $2; }

goal: 
  '!' literal { $$ = new formulat($2, formulatypet::achieve); }
  | TACHIEVENF literal { $$ = new formulat($2, formulatypet::achieve_new_focus); }
  | '?' literal { $$ = new formulat($2, formulatypet::test); }

percept : 
  '+' literal { $$ = new formulat($2, formulatypet::add); }
  | '-' literal { $$ = new formulat($2, formulatypet::del); }

context : 
  TTRUE
  | conditions

conditions : 
  condition
  | conditions '&' condition

condition : 
  TNOT literal { agent_builder.add_condition(new conditiont($2, true)); }
  | TNOT '(' literal ')' { agent_builder.add_condition(new conditiont($3, true)); }
  | literal { agent_builder.add_condition(new conditiont($1)); }

body : 
  TTRUE
  | formulas

formulas : 
  formula { agent_builder.add_formula($1); }
  | formulas ';' formula { agent_builder.add_formula($3); }

formula : 
  action
  | assignment
  | goal
  | percept

action :
  literal { $$ = new formulat($1); }

assignment : 
  TVAR '=' literal { $$ = new formulat(new vartermt(*$1), $3); delete $1; }

