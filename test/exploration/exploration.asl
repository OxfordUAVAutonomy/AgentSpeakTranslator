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

// initial goal
!init.

// initial beliefs
/* empty */

// some initialisation
+!init : true
  <- notifyUser("starting exploration");
     //!takeoff;
     Room = getCurrentRoom();
     +initialRoom(Room);
     +currentRoom(Room);
     !explore.

// if the current room is unexplored, explore it, then go to the next room
+!explore : currentRoom(Room) & not explored(Room)
  <- !exploreRoom;
     markExplored(Room);
     !flyToNextRoom.

// if the current room has been visited before, just go to the next room
+!explore : currentRoom(Room) & explored(Room)
  <- !flyToNextRoom.

// if there is a door we haven't used before, we use it now
+!flyToNextRoom : currentRoom(Room) & hasUnusedDoor(Room)
  <- Door = getNextDoor(Room);
     pushUsedDoorStack(Door);
     !flyThroughDoor(Door);
     !explore.

// if we have used all doors in this room, go back to where we came from
+!flyToNextRoom : currentRoom(Room) & not hasUnusedDoor(Room) & initialRoom(Initial) & not equalRooms(Room, Initial)
  <- Door = topUsedDoorStack();
     popUsedDoorStack();
     !flyThroughDoor(Door);
     !explore.

// if we are back in the initial room, we are done
+!flyToNextRoom : currentRoom(Room) & not hasUnusedDoor(Room) & initialRoom(Initial) & equalRooms(Room, Initial)
  <- //!land;
     notifyUser("exploration finished").

// move through door, update current room
+!flyThroughDoor(Door) : true
  <- /*Target = getPosition(Door);
     !goto(Target)*/
     goThrough(Door);
     markUsed(Door);
     Room = getCurrentRoom();
     +currentRoom(Room).

// some subroutine for exploring a room, finding doors in a room, e.g, by moving along the walls, etc.
+!exploreRoom : true
  <- /* TODO: move UAV around, detect doors, store their location */
     findDoors().

