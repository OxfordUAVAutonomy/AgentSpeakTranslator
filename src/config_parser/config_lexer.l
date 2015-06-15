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
 * @file config_lexer.l 
 *
 * @brief Flex lexer for configuration files.
 *
 * \TODO It would be nice if the agentspeak_lexer.l and this file could share some token definitions.
 */

%option reentrant
%option prefix="config_parser_"
%option bison-bridge
%option bison-locations
%option nounput
%option noyywrap
%option yylineno

%{
// TODO: find a better fix for this...
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include <istream>
#include <string>

#include "config_parser_driver.h"
#include "config_parser.h"

#define YY_EXTRA_TYPE config_parser_drivert*
#define YY_USER_ACTION yylloc->first_line = yylineno;

#define YY_INPUT(buf,result,max) (result = yyextra->is->readsome(buf,max))

#define yyterminate() return END
#define ERROR(M) yyextra->error(yylloc->first_line, yylloc->first_column, M)
%}

DIGIT ([0-9])
UC_LETTER ([A-Z])
LC_LETTER ([a-z])
LETTER ({LC_LETTER}|{UC_LETTER})
CHAR ({LETTER}|{DIGIT}|_)
INCLUDEFILE ([\"<][^  \t\n\">]*[\">])
ATOM (({LC_LETTER}|\.{CHAR})({CHAR}|\.{CHAR})*)
IDENT (({LETTER}|"_"|"$")({LETTER}|{DIGIT}|"_"|"$")*)
FILENAME (({CHAR}|\.)*)
INTEGER ({DIGIT}{DIGIT}*)

%x COMMENT SETTINGS INCLUDES BASICS TYPES SETTABLES TRIGGERABLES

  int save_state;

%%

<*>[ \t\r]+               {  }
<*>\n                     {  }
<*>"//".*                 ;
<*>"/*"                   { save_state = YY_START; BEGIN COMMENT; }

<COMMENT>"*/"          { BEGIN save_state; }
<COMMENT>.|\n          ;
<COMMENT><<EOF>>       { ERROR("Unclosed comment"); yyterminate(); }

<*>"," |
<*>";" |
<*>"(" |
<*>")" |
<*>"{"                 { return yytext[0]; }

<*>"}"                 { BEGIN INITIAL; return yytext[0]; }

<TRIGGERABLES>"+" |
<TRIGGERABLES>"-" |
<TRIGGERABLES>"!" |
<TRIGGERABLES>"?"      { return yytext[0]; }

"settings"             { BEGIN SETTINGS; return TSETTINGS; }
<SETTINGS>"input"      { return TINPUT; }
<SETTINGS>"output"     { return TOUTPUT; }
<SETTINGS>"header"     { return THEADER; }
<SETTINGS>"basics"     { return TBASICS; }
<SETTINGS>"max_events" { return TMAX_EVENTS; }
<SETTINGS>"="          { return yytext[0]; }
<SETTINGS>{INTEGER}    { yylval->intNumber = std::stoi(yytext); return TINTEGER; }
<SETTINGS>{FILENAME}   { yylval->identifier = new std::string(yytext, yyleng); return TFILENAME; }

"includes"             { BEGIN INCLUDES; return TINCLUDES; }
<INCLUDES>"#include"   { return TINCLUDE; }
<INCLUDES>{INCLUDEFILE} { yylval->identifier = new std::string(yytext, yyleng); 
                          return TINCLUDEFILE; }


"basics"               { BEGIN BASICS; return TBASICS; }
"types"		       { BEGIN TYPES; return TTYPES; }
"settables"            { BEGIN SETTABLES; return TSETTABLES; }
"triggerables"         { BEGIN TRIGGERABLES; return TTRIGGERABLES; }

<BASICS,TYPES>"void"   { yylval->identifier = new std::string(yytext, yyleng); return TVOID; }
<BASICS,TYPES,SETTABLES,TRIGGERABLES>{ATOM} { yylval->identifier = new std::string(yytext, yyleng); return TATOM; }
<BASICS,TYPES>{IDENT}  { yylval->identifier = new std::string(yytext, yyleng); return TIDENT; }

.                      { ERROR("Unknown token!"); }

%%

void config_parser_drivert::init_scanner()
{
  yylex_init(&scanner);
  yyset_extra(this, scanner);
}

void config_parser_drivert::destroy_scanner()
{
  yylex_destroy(scanner);
}
