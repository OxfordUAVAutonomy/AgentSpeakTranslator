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
#include <stdio.h>
#include <stdlib.h>

#include "map.h"
#include "types.h"

bool explored(roomt room)
{
  return room->explored;
}

bool equalRooms(roomt first, roomt second)
{
  return first == second;
}

void findDoors()
{
  roomt room = get_current_room();
  printf("exploring room %i\n", get_room_id(room));
}

roomt getCurrentRoom(void)
{
  return get_current_room();
}

doort getNextDoor(roomt room)
{
  doort door = room->doors_head;
  while (door != NULL && door->used)
  {
    /*if (room == door->from)
    {
      door = door->next_from;
    }
    else
    {
      door = door->next_to;
    }*/
    door = get_next_door(door, room);
  }
  return door;
}

void goThrough(doort door)
{
  assert(get_current_room() == door->from || get_current_room() == door->to);
  printf("going through door %i\n", get_door_id(door));
  if (get_current_room() == door->from)
  {
    set_current_room(door->to); 
  }
  else
  {
    set_current_room(door->from);
  }
}

bool hasUnusedDoor(roomt room)
{
  return (getNextDoor(room) != NULL);
}

void markExplored(roomt room)
{
  mark_explored(room);
}

void markUsed(doort door)
{
  mark_used(door);
}

void notifyUser(messaget message)
{
  printf("%s\n", message);
}

void popUsedDoorStack(void)
{
  pop();
}

void pushUsedDoorStack(doort door)
{
  push(door);
}

doort topUsedDoorStack(void)
{
  return top();
}

void updateBeliefs()
{
  /* empty */
}

