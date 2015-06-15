/*
This file is part of simple_autopilot.

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

#include <stdbool.h>
// TODO: this seems to be necessary to make getline work...
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include "autopilot.h"

#define NUM_TARGETS 3
#define SPEED_UP 10
 
positiont targets[] = 
{
  {5, 0, 0, 0},
  {0, 5, 0, 0},
  {0, 0, 5, 0}
};

bool low_tolerance = false; 

int main(void)
{
  printf("Simple Autopilot Test\n");
  printf("*********************\n\n");
  printf("input any char sequence consisting of\n");
  printf("c: switch \"close enough\" tolerance betwen 1.0 and 2.0\n");
  printf("g: send goto command to autopilot\n");
  printf("l: send land command to autopilot\n");
  printf("n: execute next step\n");
  printf("t: send takeoff command to autopilot\n");
  printf("s: execute next %i steps\n\n", SPEED_UP);
  printf("Use (q) to quit\n\n");

  printf("-> initialising drone\n");
  init();
  int currentTarget = 0;
  bool on = true;
  while (on)
  {
    printf("> ");
    char *commands = NULL;
    size_t n = 0;
    size_t length;
    if ( (length = getline(&commands, &n, stdin)) == -1)
    {
      printf("no commands entered\n");
      free(commands);
      continue;
    }
    for (int i = 0; i < length-1; i++)
    {
      char c = commands[i];
      switch (c)
      {
        case 'c':
          add_tolerance(low_tolerance ? 1.0 : 2.0);
          low_tolerance = !low_tolerance;
          break;
        case 'g': 
          achieve_goto(targets[currentTarget]);
          currentTarget = (currentTarget + 1) % NUM_TARGETS;
          break;
        case 'l':  
          achieve_land(); 
          break;
        case 'n': 
          next_step(); 
          break;
        case 't': 
          achieve_takeOff(); 
          break;
        case 'q': 
          on = false; 
          break;
        case 's':
          for (int i = 0; i < SPEED_UP; i++) next_step();
          break;
        default:
          printf("unknown command: %c\n", c);
          break;
      }
    }
    free(commands);
  }
  return EXIT_SUCCESS;
}
