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
 * @file agentspeak_lexer.l 
 *
 * @brief Flex lexer used by the AgentSpeak parser @ref agentspeak_parser.y. Token definitions should equal the ones used by Jason.
 */

%option reentrant
%option prefix="agentspeak_parser_"
%option bison-bridge
%option bison-locations
%option nounput
%option noyywrap
%option yylineno

%{
/** @TODO find a better fix for this... (flex generates functions it doesn't use, but there's no configuration option to get rid of them...) */
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include <istream>
#include <string>

#include "agentspeak_parser_driver.h"
#include "agentspeak_parser.h"

#define YY_EXTRA_TYPE agentspeak_parser_drivert*
#define YY_USER_ACTION yylloc->first_line = yylineno;

#define YY_INPUT(buf,result,max) (result = yyextra->is->readsome(buf,max))

#define yyterminate() return END
#define ERROR(M) yyextra->error(yylloc->first_line, yylloc->first_column, M)
#define NO_SUPPORT ERROR("Token currently not supported")
%}

DIGIT ([0-9])
UC_LETTER ([A-Z])
LC_LETTER ([a-z])
LETTER ({LC_LETTER}|{UC_LETTER})
CHAR ({LETTER}|{DIGIT}|_)
VAR ({UC_LETTER}{CHAR}*)
UVAR (_{CHAR}*)
ATOM (({LC_LETTER}|\.{CHAR})({CHAR}|\.{CHAR})*)
INTEGER ({DIGIT}{DIGIT}*)
FLOATA  ({DIGIT}*\.{DIGIT}+([eE][-+]?{DIGIT}+)?) 
FLOATB  ({DIGIT}+[eE][-+]?{DIGIT}+)
NUMBER ({INTEGER}|{FLOATA}|{FLOATB})
STRINGCHAR ([^\"\\\n\r])
ESCAPECHAR (\\([ntbrf\\\'\"]|[0-7][0-7]?|[0-3][0-7][0-7]))
STRING  (\"({STRINGCHAR}|{ESCAPECHAR})*\")

%x COMMENT

%%

[ \t\r]+              { }
\n                    { }
"//".*                ;
"/*"                  { BEGIN COMMENT; }
<COMMENT>"*/"         { BEGIN INITIAL; }
<COMMENT>.|\n         ;
<COMMENT><<EOF>>      { ERROR("Unclosed comment"); yyterminate(); }
true                  { return TTRUE; }
false                 { NO_SUPPORT; }
not                   { return TNOT; }
~                     { NO_SUPPORT; }
div                   { NO_SUPPORT; }
mode                  { NO_SUPPORT; }
begin                 { NO_SUPPORT; }
end                   { NO_SUPPORT; }
@                     { NO_SUPPORT; }
if                    { NO_SUPPORT; }
else                  { NO_SUPPORT; }
for                   { NO_SUPPORT; }
while                 { NO_SUPPORT; }
"!!"                  { return TACHIEVENF; }
"<-"                  { return TLARROW; }
"+" |
"-" |
"=" |
"!" |
"?" |
"," |
"." |
";" |
":" |
"(" |
")" |
"&"                   { return *yytext; }
"[" |
"]"                   { NO_SUPPORT; } 
{VAR}                 { yylval->identifier = new std::string(yytext, yyleng); return TVAR; }
{UVAR}                { yylval->identifier = new std::string(yytext, yyleng); return TUVAR; }
{ATOM}                { yylval->identifier = new std::string(yytext, yyleng); return TATOM; }
{NUMBER}              { yylval->number = std::stod(yytext); return TNUMBER; }
{STRING}              { yylval->identifier = new std::string(yytext, yyleng); return TSTRING; }  // TODO: handle strings properly
.                     { ERROR("Unknown token!"); }

%%

void agentspeak_parser_drivert::init_scanner()
{
  yylex_init(&scanner);
  yyset_extra(this, scanner);
}

void agentspeak_parser_drivert::destroy_scanner()
{
  yylex_destroy(scanner);
}
