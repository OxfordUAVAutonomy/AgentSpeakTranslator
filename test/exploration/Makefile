CC=gcc
CFLAGS+=-std=c99 -Wall -pedantic
LFLAGS+=-lm
SOURCES=actions.c exploration.c map.c test.c
GENERATED=actions.h exploration.c exploration.h
OBJECTS=$(SOURCES:.c=.o)
TARFILE=../exploration.tar.gz
TARGET=exploration

all: 		$(TARGET)

test:		$(TARGET)
		./exploration

mem_test:	$(TARGET)
		valgrind --leak-check=yes ./exploration

exploration.c:	exploration.asl exploration.conf
		$(MAKE) -C ../../src/
		../../src/agentspeak -c exploration.conf

$(TARGET): 	$(OBJECTS)
		$(CC) $(LFLAGS) -o $@ $^

.c.o:
		$(CC) -c $(CFLAGS) -o $@ $<

tar:		clean exploration.c
		tar -cvzf $(TARFILE) --exclude='*~' *

.PHONY:		clean

clean:
		rm -f *.o $(GENERATED) $(TARGET) $(TARFILE)
