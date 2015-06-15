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
 * @file config_parser.y 
 *
 * @brief Parser for config files. It uses the following grammar:
 *
 * @dontinclude config_parser.output 
 * @skipline Grammar 
 * @until Terminals
 */

%pure-parser
%name-prefix="config_parser_"
%defines "config_parser.h"
%locations
%defines
%error-verbose
%parse-param { config_parser_drivert *driver }
%lex-param { void *scanner }

%code requires{
#include "agentspeak_syntax.h"
#include "config_parser_driver.h"

class config_parser_drivert;
}
%union {
    std::string* identifier;
    int intNumber;
}


%token 			END	0		"end of file"
%token <identifier>	TVOID			"void"

%token                  TSETTINGS               "settings"
%token                  TINPUT                  "input"
%token                  TOUTPUT                 "output"
%token                  THEADER                 "header"
%token                  TMAX_EVENTS             "max_events"
%token <intNumber>      TINTEGER                "integer"
%token <identifier>     TFILENAME               "filename"

%token                  TINCLUDES               "includes"
%token                  TINCLUDE                "#include"
%token <identifier>	TINCLUDEFILE		"include file"

%token                  TBASICS                 "basics"
%token                  TTYPES                  "types"
%token                  TSETTABLES              "settables"
%token                  TTRIGGERABLES           "triggerables"

%token <identifier>	TATOM			"atom"
%token <identifier>	TIDENT			"identifier"

%type <identifier> identifier type_identifier return_type

%{
#include <stdio.h>

#include <iostream>
#include <sstream>

#include "config_parser_driver.h"
#include "trigger.h"

int config_parser_lex(YYSTYPE* lvalp, YYLTYPE* llocp, void* scanner);		

void config_parser_error(YYLTYPE* locp, config_parser_drivert *driver, const char* err)
{
  driver->error(locp->first_line, locp->first_column, err);
}

#define scanner driver->scanner
#define atom_builder driver->atom_builder
#define config driver->config
%}

%start configuration

%%

configuration:
  settings includes basics_declaration types_declaration settables_declaration triggerables_declaration END

settings:
  TSETTINGS '{' settings_list '}' ';'

includes:
  /* empty */
  | TINCLUDES '{' includes_list '}' ';'

types_declaration:
  /* empty */
  | TTYPES '{' type_declarations '}' ';'

basics_declaration:
  /* empty */
  | TBASICS '{' basic_declarations '}' ';'

settables_declaration:
  /* empty */
  | TSETTABLES '{' settable_declarations '}' ';'

triggerables_declaration:
  /* empty */
  | TTRIGGERABLES '{' triggerable_declarations '}' ';'

settings_list:
  setting ';'
  | setting ';' settings_list

includes_list:
  include
  | include includes_list

type_declarations:
  type_declaration ';'
  | type_declaration ';' type_declarations

basic_declarations:
  basic_declaration ';'
  | basic_declaration ';' basic_declarations

settable_declarations:
  settable_declaration ';'
  | settable_declaration ';' settable_declarations

triggerable_declarations:
  triggerable_declaration ';'
  | triggerable_declaration ';' triggerable_declarations

setting:
  TINPUT '=' TFILENAME { config.set_input_name(*$3); delete $3; }
  | TOUTPUT '=' TFILENAME { config.set_output_c_name(*$3); delete $3; }
  | THEADER '=' TFILENAME { config.set_output_h_name(*$3); delete $3; }
  | TBASICS '=' TFILENAME { config.set_basics_h_name(*$3); delete $3; }
  | TMAX_EVENTS '=' TINTEGER { config.set_max_events($3); }


include:
  TINCLUDE TINCLUDEFILE { config.add_include(*$2); delete $2; }

type_declaration:
  type_declaration_cases { atom_builder.end_declaration(); }

basic_declaration:
  basic_declaration_cases { config.make_basic_function(atom_builder.end_declaration()); }

settable_declaration:
  TATOM { config.make_settable(*$1); delete $1; }

triggerable_declaration:
  '+' TATOM { config.make_triggerable(*$2, triggeropt::add, triggertypet::percept); delete $2; }
  | '-' TATOM { config.make_triggerable(*$2, triggeropt::del, triggertypet::percept); delete $2; }
  | '+''!' TATOM { config.make_triggerable(*$3, triggeropt::add, triggertypet::achieve); delete $3; }


type_declaration_cases:
  symbol_name parameter_list
  | symbol_name

basic_declaration_cases:
  return_type symbol_name parameter_list { atom_builder.add_return_type(*$1); delete $1;}

parameter_list:
  '(' parameter_types ')'
  | '(' TVOID ')'
  | '(' ')'

return_type:
  TVOID
  | type_identifier

symbol_name:
  TATOM { atom_builder.add_symbol(*$1); delete $1; }

parameter_types:
  type_identifier { atom_builder.add_type_parameter(*$1); delete $1; }
  | type_identifier ',' parameter_types { atom_builder.add_type_parameter(*$1); delete $1; }

type_identifier:
  identifier

identifier:
  TATOM
  | TIDENT


