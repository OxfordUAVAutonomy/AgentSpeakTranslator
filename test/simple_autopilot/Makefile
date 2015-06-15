CC=gcc
CFLAGS+=-std=c99 -Wall -pedantic
LFLAGS+=-lm
SOURCES=autopilot.c actions.c test.c
GENERATED=autopilot.c autopilot.h actions.h
OBJECTS=$(SOURCES:.c=.o)
TARGET=test

all: $(TARGET)

autopilot.c:	autopilot.asl autopilot.conf
		$(MAKE) -C ../../src/
		../../src/agentspeak -c autopilot.conf

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LFLAGS)

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f *.o $(GENERATED) $(TARGET)
