/*
Copyright (C) 2015, Samuel Bucheli, Department of Computer Science, University
of Oxford.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// initial beliefs
takeOffTime(5000).
initialReachParameters(0.2, 5).
stayWithinParameters(0.5, 5).
stayTime(2000).

// initial goal
!waitForCommand.

// plans
/* wait for commands, if at ground, do nothing, wait for further commands */
+!waitForCommand : not takenOff
  <- !!waitForCommand. // new focus!
  
/* wait for commands, if in air, keep position, wait for further commands */
+!waitForCommand : takenOff & lastTarget(Target)
  <- ?myPosition(Pos);
     !makeMovement(Pos, Target);
     !!waitForCommand. // new focus!

/* wait for commands, if no last target set, use current position */
+!waitForCommand : takenOff
  <- notifyUser("no last target set, using current position");
     sendHover();
     ?myPosition(Pos);
     +lastTarget(Pos);
     !!waitForCommand.

/* receive take-off command, initiate take-off, update takenOff belief */
+!takeOff : not takenOff
  <- notifyUser("taking off");
     sendTakeOff();
     Time = getCurrentTimeMS();
     +takenOff;
     !completeTakeOff(Time).

/* received take-off command, check if enough time has passed, if not, send hover and wait */
+!completeTakeOff(StartTime) : takenOff & takeOffTime(WaitTime) & not longEnough(StartTime, WaitTime)
  <- sendHover();
     !completeTakeOff(StartTime).

/* received take-off command, check if enough time has passed, if yes, send hover, notify user, and done */
+!completeTakeOff(StartTime) : takenOff & takeOffTime(WaitTime) & longEnough(StartTime, WaitTime)
  <- sendHover();
     notifyUser("taken off").

/* receive goto command, set target */
+!goto(Target) : takenOff
  <- sendHover();
     +lastTarget(Target);
     !completeGoto.

/* received goto command, check if already there, if not, move towards target */
+!completeGoto : takenOff & myPosition(Pos) & lastTarget(Target) & initialReachParameters(XyzDist, YawDist) & not closeEnough(Pos, Target, XyzDist, YawDist)
  <- !makeMovement(Pos, Target);
     !completeGoto.

/* received goto command, check if already there, if yes, notify user, and try to keep position */
+!completeGoto : takenOff & myPosition(Pos) & lastTarget(Target) & initialReachParameters(XyzDist, YawDist) & closeEnough(Pos, Target, XyzDist, YawDist)
  <- notifyUser("target initially reached");
     Time = getCurrentTimeMS();
     !stayAtTarget(Time).

/* if getting too far away form target, try to get back there again */
+!stayAtTarget(StartTime) : takenOff & myPosition(Pos) & lastTarget(Target) & stayWithinParameters(XyzDist, YawDist) & not closeEnough(Pos, Target, XyzDist, YawDist)
  <- notifyUser("target lost");
     sendHover();
     !completeGoto.

/* if at target, and not enough time has elapsed, wait there */
+!stayAtTarget(StartTime) : takenOff & myPosition(Pos) & lastTarget(Target) & stayWithinParameters(XyzDist, YawDist) &  closeEnough(Pos, Target, XyzDist, YawDist) & stayTime(WaitTime) & not longEnough(StartTime, WaitTime)
  <- !makeMovement(Pos, Target);
     !stayAtTarget(StartTime).

/* if at target, and enough time has elapsed, you are done */
+!stayAtTarget(StartTime) : takenOff & myPosition(Pos) & lastTarget(Target) & stayWithinParameters(XyzDist, YawDist) &  closeEnough(Pos, Target, XyzDist, YawDist) & stayTime(WaitTime) & longEnough(StartTime, WaitTime)
  <- sendHover();
     notifyUser("goto completed").

/* receive land command, initiate landing, update takenOff belief */
+!land: takenOff
  <- notifyUser("landing");
     sendLand();
     -takenOff.

/* calculate control for movement and send to drone */
+!makeMovement(From, To) : takenOff
  <- Movement = calculateMovement(From, To);
     sendGoto(Movement).
