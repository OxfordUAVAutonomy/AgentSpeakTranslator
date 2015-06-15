CXX=g++
RM=rm -f

DEBUGCXXFLAGS=-g -DAGENTSPEAK_DEBUG
DEBUGLDFLAGS=-rdynamic

CXXFLAGS+=-W -Wall -Wextra -std=c++11 $(DEBUGCXXFLAGS)
LDFLAGS+=$(DEBUGLDFLAGS)

DOXYFILE=Doxyfile

ANALYSISDIR=analysis
ASPARSERDIR=agentspeak_parser
CFPARSERDIR=config_parser
DOXYGENDIR=../doc
SYNTAXDIR=syntax
TRANSLATDIR=translator
UTILDIR=util

INC=. $(ANALYSISDIR) $(ASPARSERDIR) $(CFPARSERDIR) $(SYNTAXDIR) $(TRANSLATDIR) $(UTILDIR)
INC_PARAMS=$(foreach d, $(INC), -I$d)

ANALYSISOBJS=$(ANALYSISDIR)/believables.o $(ANALYSISDIR)/plan_usage.o $(ANALYSISDIR)/trigger_collector.o $(ANALYSISDIR)/trigger_graph.o $(ANALYSISDIR)/used_variables.o

ASPARSEROBJS=$(ASPARSERDIR)/agentspeak_lexer.o $(ASPARSERDIR)/agentspeak_parser.o $(ASPARSERDIR)/agentspeak_parser_driver.o

CFPARSEROBJS=$(CFPARSERDIR)/config_lexer.o $(CFPARSERDIR)/config_parser.o $(CFPARSERDIR)/config_parser_driver.o

SYNTAXOBJS=$(SYNTAXDIR)/agent.o $(SYNTAXDIR)/atom.o $(SYNTAXDIR)/condition.o $(SYNTAXDIR)/formula.o $(SYNTAXDIR)/literal_based.o $(SYNTAXDIR)/plan.o $(SYNTAXDIR)/term.o $(SYNTAXDIR)/trigger.o

TRANSLATOBJS=$(TRANSLATDIR)/agent_translator.o $(TRANSLATDIR)/name_translation.o $(TRANSLATDIR)/plan_translator.o $(TRANSLATDIR)/trigger_translator.o

OBJS=agentspeak_main.o $(ANALYSISOBJS) $(SYNTAXOBJS) $(TRANSLATOBJS)
 
BIN=agentspeak

all:				parser config_parser $(TRANSLATDIR)/event_handling_with_queue.inc $(TRANSLATDIR)/event_handling_without_queue.inc $(BIN)

$(TRANSLATDIR)/event_handling_with_queue.inc:	$(TRANSLATDIR)/event_handling_with_queue.c
						$(TRANSLATDIR)/make_event_handling.sh $< $@

$(TRANSLATDIR)/event_handling_without_queue.inc:	$(TRANSLATDIR)/event_handling_without_queue.c
							$(TRANSLATDIR)/make_event_handling.sh $< $@


.PHONY: 			parser

parser:
				$(MAKE) -C $(ASPARSERDIR)

.PHONY: 			config_parser

config_parser:
				$(MAKE) -C $(CFPARSERDIR)

%.o:				%.cpp
				$(CXX) $(CXXFLAGS) $(INC_PARAMS) -c -o $@ $<

$(BIN):				$(ASPARSEROBJS) $(CFPARSEROBJS) $(OBJS)
				$(CXX) $(LDFLAGS) -o $@ $^

.PHONY:				documentation

documentation:			$(DOXYFILE)
				doxygen $(DOXYFILE)

.PHONY:				clean

clean:
				$(RM) $(BIN) $(ASPARSEROBJS) $(CFPARSEROBJS) $(OBJS)

autopilot:			$(BIN)
				./$(BIN) -c ../test/autopilot/autopilot.conf

test:				$(BIN)
				./$(BIN) -c ../test/simple_autopilot/autopilot.conf

.PHONY:				scratch

scratch:			extraclean all

.PHONY:				extraclean

extraclean:			clean
				$(RM) $(TRANSLATDIR)/event_handling.inc
				$(MAKE) clean -C $(ASPARSERDIR)
				$(MAKE) clean -C $(CFPARSERDIR)
				$(RM) -r $(DOXYGENDIR)

