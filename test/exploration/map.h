/*
This file is part of exploration.

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

#ifndef EXPLORATION_MAP_H
#define EXPLORATION_MAP_H

struct _room_nodet;

typedef struct _door_nodet {
  // flag door as used
  bool used;

  // just to make debugging easier
  unsigned int id;

  // rooms connected by this door
  // Note: Will be treated as undirected in code, i.e., from and to are not implying direction.
  struct _room_nodet* from;
  struct _room_nodet* to;

  // next door in linked list of doors of "from" room
  struct _door_nodet* next_from;

  // next door in linked list of doors of "to" room
  struct _door_nodet* next_to;

  // previous element of used door stack
  struct _door_nodet* next_stack;
} door_nodet;

typedef struct _room_nodet {
  // flag room as explored
  bool explored;

  // just to make debugging easier
  unsigned int id;

  // linked list of doors associated with room
  struct _door_nodet* doors_head;

  // linked list of all rooms -- mainly used for convenience in clean_up()
  struct _room_nodet* next;
} room_nodet;

door_nodet* add_door(room_nodet*, room_nodet*);
room_nodet* add_room(void);
void clean_up_map(void);
room_nodet* get_current_room(void);
unsigned int get_door_id(door_nodet*);
door_nodet* get_next_door(door_nodet*, room_nodet*);
unsigned int get_room_id(room_nodet*);
void init_map(void);
bool is_explored(room_nodet*);
bool is_used(door_nodet*);
void mark_explored(room_nodet*);
void mark_used(door_nodet*);
void pop(void);
void push(door_nodet*);
void set_current_room(room_nodet*);
door_nodet* top(void);

#endif /* EXPLORATION_MAP_H */
