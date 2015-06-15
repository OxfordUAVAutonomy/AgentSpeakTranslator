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

//! [initial beliefs]
// initial beliefs
tolerance(1).
//! [initial beliefs]

//! [initial goal]
// initial goal
!waitForCommand.
//! [initial goal]

// plans
//! [waitForCommand]
/* wait for commands, if at ground, do nothing, wait for further commands */
+!waitForCommand : not takenOff
  <- notifyUser("doing nothing");
     !!waitForCommand. // new focus!
  
/* wait for commands, if in air, send hover, wait for further commands */
+!waitForCommand : takenOff
  <- notifyUser("sending hover");
     sendHover();
     !!waitForCommand. // new focus!
//! [waitForCommand]

//! [takeOff]
/* received take-off command, initiate take-off, update takenOff belief */
+!takeOff : not takenOff
  <- notifyUser("taking off");
     sendTakeOff();
     +takenOff.
//! [takeOff]

//! [goto]
/* received goto command, check if already there, if not, move towards target */
+!goto(Target) : takenOff & myPosition(Pos) & tolerance(Tolerance) 
                   & not closeEnough(Pos, Target, Tolerance)
  <- notifyUser("going towards target");
     Movement = calculateMovement(Pos,Target);
     sendControl(Movement);
     !goto(Target).

/* received goto command, check if already there, if yes, done */
+!goto(Target) : takenOff & myPosition(Pos) & tolerance(Tolerance) 
                   & closeEnough(Pos, Target, Tolerance)
  <- notifyUser("target reached").
//! [goto]

//! [land]
/* received land command, initiate landing, update takenOff belief */
+!land: takenOff
  <- notifyUser("landing");
     sendLand();
     -takenOff.
//! [land]
