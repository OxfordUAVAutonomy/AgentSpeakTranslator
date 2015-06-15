LEX=flex
YACC=bison
RM=rm -f

LFLAGS+=
YFLAGS+=--report=state -d

YACCGENS=agentspeak_parser.cpp agentspeak_parser.h agentspeak_parser.output 
LEXGENS=agentspeak_lexer.cpp

all:				agentspeak_parser.cpp agentspeak_lexer.cpp

agentspeak_parser.cpp:		agentspeak_parser.y
				$(YACC) $(YFLAGS) -o agentspeak_parser.cpp --defines=agentspeak_parser.h agentspeak_parser.y 

agentspeak_lexer.cpp:		agentspeak_lexer.l
				$(LEX) $(LFLAGS) -o agentspeak_lexer.cpp agentspeak_lexer.l

clean:				
				$(RM) $(YACCGENS) $(LEXGENS)

