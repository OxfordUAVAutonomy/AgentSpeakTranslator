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

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "autopilot.h"
#include "actions.h"
#include "types.h"

// very simple simulation of environment
static positiont currentPosition = { 0, 0, 0, 0 };

static positiont getDifference(positiont origin, positiont target)
{
  positiont diff = {
    target.x - origin.x,
    target.y - origin.y,
    target.z - origin.z,
    target.yaw - origin.yaw,
  };
  return diff;
}

static double getNorm(positiont pos)
{
  return sqrt( (pos.x * pos.x) + (pos.y * pos.y) + (pos.z * pos.z) + (pos.yaw * pos.yaw) );
}

// calculates one unit-vector length of movement towards target
control_commandt calculateMovement(positiont position, positiont target)
{
  positiont diff = getDifference(position, target);
  double norm = getNorm(diff);
  // TODO: danger of division by zero...
  control_commandt cmd = { diff.x/norm, diff.y/norm, diff.z/norm, diff.yaw/norm };
  return cmd;
}

//! [closeEnough]
bool closeEnough(positiont position, positiont target, double tolerance)
{
  positiont diff = getDifference(position, target);
  double distance = getNorm(diff);
  return (distance < tolerance);
}
//! [closeEnough]

void notifyUser(const char* message)
{
  printf("notification: %s\n", message);
}

void sendTakeOff(void)
{
  printf("-> take off sent...\n");
}

void sendHover(void)
{
  printf("-> hover sent...\n");
}

void sendControl(control_commandt command)
{
  // TODO: abusing fields for mock implementation!!!
  currentPosition.x += command.yaw;
  currentPosition.y += command.roll;
  currentPosition.z += command.pitch;
  printf("-> control (%f, %f, %f) sent...\n", command.yaw, command.roll, command.pitch);
  printf("-> current position is (%f, %f, %f)...\n", 
    currentPosition.x, currentPosition.y, currentPosition.z);
}

void sendLand(void)
{
  printf("-> land sent...\n");
}

//! [updateBeliefs]
void updateBeliefs(void)
{
  set_myPosition(currentPosition);
}
//! [updateBeliefs]
