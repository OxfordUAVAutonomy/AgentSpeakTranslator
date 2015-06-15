LEX=flex
YACC=bison
RM=rm -f

LFLAGS+=
YFLAGS+=--report=state -d

YACCGENS=config_parser.cpp config_parser.h config_parser.output
LEXGENS=config_lexer.cpp

all:			config_parser.cpp config_lexer.cpp

config_parser.cpp:	config_parser.y
			$(YACC) $(YFLAGS) -o config_parser.cpp --defines=config_parser.h config_parser.y 

config_lexer.cpp:	config_lexer.l
			$(LEX) $(LFLAGS) -o config_lexer.cpp config_lexer.l

clean:				
			$(RM) $(YACCGENS) $(LEXGENS)

