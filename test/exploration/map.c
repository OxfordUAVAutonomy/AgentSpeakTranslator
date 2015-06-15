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

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "map.h"
#include "types.h"

room_nodet *rooms_head;
room_nodet *current_room;
door_nodet *door_stack;

unsigned int door_id;
unsigned int room_id;

// add door between given rooms
door_nodet* add_door(room_nodet *from, room_nodet *to)
{
  // TODO: make sure you don't add already existing doors
  door_nodet* door = (door_nodet*) malloc(sizeof(door_nodet));
 
  door->used = false;
  door->id = door_id++;
  
  door->from = from;
  door->to = to;

  door->next_from = from->doors_head;
  from->doors_head = door;

  door->next_to = to->doors_head;
  to->doors_head = door;

  door->next_stack = NULL;
  
  return door;
}

// add room to rooms list and initialize doors list for room
room_nodet* add_room(void) 
{
  room_nodet* room = (room_nodet*) malloc(sizeof(room_nodet));
  room->explored = false;
  room->id = room_id++;
  room->doors_head = NULL;
  room->next = rooms_head;
  rooms_head = room;

  // for testing we assume that the first room to be added is the room exploration starts in
  if (current_room == NULL)
  {
    current_room = room;
  }

  return room;
}

void mark_doors_unused(void)
{
  room_nodet *room = rooms_head;
  while(room != NULL)
  {
    door_nodet *door = room->doors_head;
    while(door != NULL) 
    {
      door->used = false;
      door = get_next_door(door, room);
    }
    room = room->next;
  }
}

void empty_stack(void)
{
  while(door_stack != NULL)
  {
    pop();
  }
}

void clean_up_map(void)
{
  // empty door stack, we will (ab)use it for freeing door memory below
  empty_stack();
  mark_doors_unused();
  
  // for all rooms
  room_nodet *room = rooms_head;
  while(room != NULL)
  {
    // push all doors onto stack
    door_nodet *door = room->doors_head;
    while(door != NULL) 
    {
      if (!is_used(door))
      {
        push(door);
        mark_used(door);
      }
      door = get_next_door(door, room);
    }
    // free room
    room_nodet *to_free = room;
    room = room->next;
    free(to_free);
  }

  // free all doors
  while(door_stack != NULL)
  {
    door_nodet *to_free = door_stack;
    door_stack = door_stack->next_stack;
    free(to_free);
  }
}

room_nodet* get_current_room(void)
{
  return current_room;
}

unsigned int get_door_id(door_nodet *door)
{
  return door->id;
}

door_nodet* get_next_door(door_nodet *door, room_nodet *room)
{
  if (room == door->from)
  {
    return door->next_from;
  }
  else
  {
    return door->next_to;
  }
}

unsigned int get_room_id(room_nodet *room)
{
  return room->id;
}

void init_map(void)
{
  rooms_head = NULL;
  current_room = NULL;
  door_stack = NULL;
 
  door_id = 0;
  room_id = 0;
}

bool is_explored(room_nodet* room)
{
  return room->explored;
}

bool is_used(door_nodet* door)
{
  return door->used;
}

void mark_explored(room_nodet* room)
{
  room->explored = true;
}

void mark_used(door_nodet* door)
{
  door->used = true;
}

void pop(void)
{
  door_nodet* temp = door_stack;
  door_stack = door_stack->next_stack;
  temp->next_stack = NULL;
}

void push(door_nodet* door)
{
  door->next_stack = door_stack;
  door_stack = door;
}

void set_current_room(room_nodet* room)
{
  current_room = room;
}

door_nodet* top(void)
{
  return door_stack;
}
