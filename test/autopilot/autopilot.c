/* File autopilot.c generated from autopilot.asl using AgentSpeakTranslator */

#include <stdbool.h>
#include "types.h"
#include "autopilot.h"
#include "actions.h"

/* belief base */
static bool initialReachParameters_set = false;
static double initialReachParameters_param0;
static double initialReachParameters_param1;

static bool lastTarget_set = false;
static positiont lastTarget_param0;

static bool myPosition_set = false;
static positiont myPosition_param0;

static bool stayTime_set = false;
static int stayTime_param0;

static bool stayWithinParameters_set = false;
static double stayWithinParameters_param0;
static double stayWithinParameters_param1;

static bool takeOffTime_set = false;
static int takeOffTime_param0;

static bool takenOff_set = false;

void set_myPosition(positiont param0)
{
  myPosition_set = true;
  myPosition_param0 = param0;
}

/* triggers */
typedef enum
{
  NONE,
  ADD_PERCEPT_INITIALREACHPARAMETERS,
  ADD_PERCEPT_STAYTIME,
  ADD_PERCEPT_STAYWITHINPARAMETERS,
  ADD_ACHIEVE_COMPLETETAKEOFF,
  ADD_ACHIEVE_COMPLETEGOTO,
  ADD_ACHIEVE_GOTO,
  ADD_ACHIEVE_LAND,
  ADD_ACHIEVE_MAKEMOVEMENT,
  ADD_ACHIEVE_STAYATTARGET,
  ADD_ACHIEVE_TAKEOFF,
  ADD_ACHIEVE_WAITFORCOMMAND
} triggert;

/* events */
typedef struct
{
  triggert trigger;
  double initialReachParameters_param0;
  double initialReachParameters_param1;
  int stayTime_param0;
  double stayWithinParameters_param0;
  double stayWithinParameters_param1;
  int completeTakeOff_param0;
  positiont goto_param0;
  positiont makeMovement_param0;
  positiont makeMovement_param1;
  int stayAtTarget_param0;
} eventt;

/* returns true if the event is triggering */
static bool is_triggering(eventt event)
{
  bool triggering = false;
  switch (event.trigger)
  {
    case ADD_ACHIEVE_COMPLETETAKEOFF:
      triggering = true;
      break;
    case ADD_ACHIEVE_COMPLETEGOTO:
      triggering = true;
      break;
    case ADD_ACHIEVE_GOTO:
      triggering = true;
      break;
    case ADD_ACHIEVE_LAND:
      triggering = true;
      break;
    case ADD_ACHIEVE_MAKEMOVEMENT:
      triggering = true;
      break;
    case ADD_ACHIEVE_STAYATTARGET:
      triggering = true;
      break;
    case ADD_ACHIEVE_TAKEOFF:
      triggering = true;
      break;
    case ADD_ACHIEVE_WAITFORCOMMAND:
      triggering = true;
      break;
    default:  // nothing to do
      break;
  }
  return triggering;
}

/* changes belief base according to triggering event */
static void handle_percept(eventt event)
{
  switch (event.trigger)
  {
    case ADD_PERCEPT_INITIALREACHPARAMETERS:
      initialReachParameters_set = true;
      initialReachParameters_param0 = event.initialReachParameters_param0;
      initialReachParameters_param1 = event.initialReachParameters_param1;
      break;
    case ADD_PERCEPT_STAYTIME:
      stayTime_set = true;
      stayTime_param0 = event.stayTime_param0;
      break;
    case ADD_PERCEPT_STAYWITHINPARAMETERS:
      stayWithinParameters_set = true;
      stayWithinParameters_param0 = event.stayWithinParameters_param0;
      stayWithinParameters_param1 = event.stayWithinParameters_param1;
      break;
    default:  // nothing to do
      break;
  }
}

/* in the event-queue-less version, we have potentially two events:
 * 1) an event that needs to be completed, this one will always be given
 *    preference, if set
 * 2) a deferred event, this one will be used, if no event to complete
 *    is set
 */
static bool rtc_event_set;
static eventt rtc_event;
static bool deferred_event_set;
static eventt deferred_event;

/* initialise the events, there's not much to do here... */
static void init_events(void)
{
  rtc_event_set = false;
  deferred_event_set = false;
}

/* indicate whether another event can be generated */
bool can_add_event(void)
{
  return !rtc_event_set;
}

/* add deferred event */
static bool add_event_new_focus(eventt event)
{
  if (!deferred_event_set)
  {
    deferred_event = event;
    deferred_event_set = true;
    return true;
  }
  // TODO: This should not happen
  return false;
}

/* add event to complete */
static bool add_event_to_complete(eventt event)
{
  if (!rtc_event_set)
  {
    rtc_event = event;
    rtc_event_set = true;
    return true;
  }
  // TODO: This should not happen
  return false;  
}

/* without an event queue, we handle external events immediately, as they are supposed
   to be only added when can_add_event() returns true, i.e., no event_to_complete is set */
static bool add_external_event(eventt event)
{
  return add_event_to_complete(event);
}

/* get next event: event_to_complete is given preference over deferred_event */
static eventt get_next_event(void)
{
  eventt event = { NONE };
  if (rtc_event_set) 
  {
    event = rtc_event;
    rtc_event_set = false;
  }
  else if (deferred_event_set)
  {
    event = deferred_event;
    deferred_event_set = false;
  }
  return event;
}

/* internal trigger functions */
static void internal_achieve_completeTakeOff(int param0)
{
  eventt event;
  event.trigger = ADD_ACHIEVE_COMPLETETAKEOFF;
  event.completeTakeOff_param0 = param0;
  add_event_to_complete(event);
}

static void internal_achieve_completeGoto(void)
{
  eventt event;
  event.trigger = ADD_ACHIEVE_COMPLETEGOTO;
  add_event_to_complete(event);
}

static void internal_achieve_stayAtTarget(int param0)
{
  eventt event;
  event.trigger = ADD_ACHIEVE_STAYATTARGET;
  event.stayAtTarget_param0 = param0;
  add_event_to_complete(event);
}

static void internal_achieve_new_focus_waitForCommand(void)
{
  eventt event;
  event.trigger = ADD_ACHIEVE_WAITFORCOMMAND;
  add_event_new_focus(event);
}

/* "public" trigger functions */
void add_initialReachParameters(double param0, double param1)
{
  eventt event;
  event.trigger = ADD_PERCEPT_INITIALREACHPARAMETERS;
  event.initialReachParameters_param0 = param0;
  event.initialReachParameters_param1 = param1;
  add_external_event(event);
}

void add_stayTime(int param0)
{
  eventt event;
  event.trigger = ADD_PERCEPT_STAYTIME;
  event.stayTime_param0 = param0;
  add_external_event(event);
}

void add_stayWithinParameters(double param0, double param1)
{
  eventt event;
  event.trigger = ADD_PERCEPT_STAYWITHINPARAMETERS;
  event.stayWithinParameters_param0 = param0;
  event.stayWithinParameters_param1 = param1;
  add_external_event(event);
}

void achieve_goto(positiont param0)
{
  eventt event;
  event.trigger = ADD_ACHIEVE_GOTO;
  event.goto_param0 = param0;
  add_external_event(event);
}

void achieve_land(void)
{
  eventt event;
  event.trigger = ADD_ACHIEVE_LAND;
  add_external_event(event);
}

void achieve_takeOff(void)
{
  eventt event;
  event.trigger = ADD_ACHIEVE_TAKEOFF;
  add_external_event(event);
}

/* applicable plan selection declarations */
static void add_achieve_completeTakeOff(int);

static void add_achieve_completeGoto(void);

static void add_achieve_goto(positiont);

static void add_achieve_land(void);

static void add_achieve_makeMovement(positiont, positiont);

static void add_achieve_stayAtTarget(int);

static void add_achieve_takeOff(void);

static void add_achieve_waitForCommand(void);

/* plans */
/*
+!completeTakeOff(StartTime)
   : takenOff() & takeOffTime(WaitTime) & not longEnough(StartTime, WaitTime)
   <- sendHover();
      !completeTakeOff(StartTime)
*/
static bool add_achieve_completeTakeOff_plan0(int param0)
{
  /* instantiate trigger parameters */
  int StartTime = param0;
  /* plan context, i.e., guards */
  if (!takenOff_set)
  {
    return false;
  }
  if (!takeOffTime_set)
  {
    return false;
  }
  int WaitTime = takeOffTime_param0;
  if (longEnough(StartTime,WaitTime))
  {
    return false;
  }
  /* plan body */
  sendHover();
  internal_achieve_completeTakeOff(StartTime);
  return true;
}

/*
+!completeTakeOff(StartTime)
   : takenOff() & takeOffTime(WaitTime) & longEnough(StartTime, WaitTime)
   <- sendHover();
      notifyUser("taken off")
*/
static bool add_achieve_completeTakeOff_plan1(int param0)
{
  /* instantiate trigger parameters */
  int StartTime = param0;
  /* plan context, i.e., guards */
  if (!takenOff_set)
  {
    return false;
  }
  if (!takeOffTime_set)
  {
    return false;
  }
  int WaitTime = takeOffTime_param0;
  if (!longEnough(StartTime,WaitTime))
  {
    return false;
  }
  /* plan body */
  sendHover();
  notifyUser("taken off");
  return true;
}

/*
+!completeGoto()
   : takenOff() & myPosition(Pos) & lastTarget(Target) & initialReachParameters(XyzDist, YawDist) & not closeEnough(Pos, Target, XyzDist, YawDist)
   <- !makeMovement(Pos, Target);
      !completeGoto()
*/
static bool add_achieve_completeGoto_plan0(void)
{
  /* instantiate trigger parameters */
  /* plan context, i.e., guards */
  if (!takenOff_set)
  {
    return false;
  }
  if (!myPosition_set)
  {
    return false;
  }
  positiont Pos = myPosition_param0;
  if (!lastTarget_set)
  {
    return false;
  }
  positiont Target = lastTarget_param0;
  if (!initialReachParameters_set)
  {
    return false;
  }
  double XyzDist = initialReachParameters_param0;
  double YawDist = initialReachParameters_param1;
  if (closeEnough(Pos,Target,XyzDist,YawDist))
  {
    return false;
  }
  /* plan body */
  add_achieve_makeMovement(Pos, Target);
  internal_achieve_completeGoto();
  return true;
}

/*
+!completeGoto()
   : takenOff() & myPosition(Pos) & lastTarget(Target) & initialReachParameters(XyzDist, YawDist) & closeEnough(Pos, Target, XyzDist, YawDist)
   <- notifyUser("target initially reached");
      Time = getCurrentTimeMS();
      !stayAtTarget(Time)
*/
static bool add_achieve_completeGoto_plan1(void)
{
  /* instantiate trigger parameters */
  /* plan context, i.e., guards */
  if (!takenOff_set)
  {
    return false;
  }
  if (!myPosition_set)
  {
    return false;
  }
  positiont Pos = myPosition_param0;
  if (!lastTarget_set)
  {
    return false;
  }
  positiont Target = lastTarget_param0;
  if (!initialReachParameters_set)
  {
    return false;
  }
  double XyzDist = initialReachParameters_param0;
  double YawDist = initialReachParameters_param1;
  if (!closeEnough(Pos,Target,XyzDist,YawDist))
  {
    return false;
  }
  /* plan body */
  notifyUser("target initially reached");
  int Time = getCurrentTimeMS();
  internal_achieve_stayAtTarget(Time);
  return true;
}

/*
+!goto(Target)
   : takenOff()
   <- sendHover();
      +lastTarget(Target);
      !completeGoto()
*/
static bool add_achieve_goto_plan0(positiont param0)
{
  /* instantiate trigger parameters */
  positiont Target = param0;
  /* plan context, i.e., guards */
  if (!takenOff_set)
  {
    return false;
  }
  /* plan body */
  sendHover();
  lastTarget_set = true;
  lastTarget_param0 = Target;
  internal_achieve_completeGoto();
  return true;
}

/*
+!land()
   : takenOff()
   <- notifyUser("landing");
      sendLand();
      -takenOff()
*/
static bool add_achieve_land_plan0(void)
{
  /* instantiate trigger parameters */
  /* plan context, i.e., guards */
  if (!takenOff_set)
  {
    return false;
  }
  /* plan body */
  notifyUser("landing");
  sendLand();
  takenOff_set = false;
  return true;
}

/*
+!makeMovement(From, To)
   : takenOff()
   <- Movement = calculateMovement(From, To);
      sendGoto(Movement)
*/
static bool add_achieve_makeMovement_plan0(positiont param0, positiont param1)
{
  /* instantiate trigger parameters */
  positiont From = param0;
  positiont To = param1;
  /* plan context, i.e., guards */
  if (!takenOff_set)
  {
    return false;
  }
  /* plan body */
  control_commandt Movement = calculateMovement(From, To);
  sendGoto(Movement);
  return true;
}

/*
+!stayAtTarget(StartTime)
   : takenOff() & myPosition(Pos) & lastTarget(Target) & stayWithinParameters(XyzDist, YawDist) & not closeEnough(Pos, Target, XyzDist, YawDist)
   <- notifyUser("target lost");
      sendHover();
      !completeGoto()
*/
static bool add_achieve_stayAtTarget_plan0(int param0)
{
  /* instantiate trigger parameters */
  int StartTime = param0;
  /* plan context, i.e., guards */
  if (!takenOff_set)
  {
    return false;
  }
  if (!myPosition_set)
  {
    return false;
  }
  positiont Pos = myPosition_param0;
  if (!lastTarget_set)
  {
    return false;
  }
  positiont Target = lastTarget_param0;
  if (!stayWithinParameters_set)
  {
    return false;
  }
  double XyzDist = stayWithinParameters_param0;
  double YawDist = stayWithinParameters_param1;
  if (closeEnough(Pos,Target,XyzDist,YawDist))
  {
    return false;
  }
  /* plan body */
  notifyUser("target lost");
  sendHover();
  internal_achieve_completeGoto();
  return true;
}

/*
+!stayAtTarget(StartTime)
   : takenOff() & myPosition(Pos) & lastTarget(Target) & stayWithinParameters(XyzDist, YawDist) & closeEnough(Pos, Target, XyzDist, YawDist) & stayTime(WaitTime) & not longEnough(StartTime, WaitTime)
   <- !makeMovement(Pos, Target);
      !stayAtTarget(StartTime)
*/
static bool add_achieve_stayAtTarget_plan1(int param0)
{
  /* instantiate trigger parameters */
  int StartTime = param0;
  /* plan context, i.e., guards */
  if (!takenOff_set)
  {
    return false;
  }
  if (!myPosition_set)
  {
    return false;
  }
  positiont Pos = myPosition_param0;
  if (!lastTarget_set)
  {
    return false;
  }
  positiont Target = lastTarget_param0;
  if (!stayWithinParameters_set)
  {
    return false;
  }
  double XyzDist = stayWithinParameters_param0;
  double YawDist = stayWithinParameters_param1;
  if (!closeEnough(Pos,Target,XyzDist,YawDist))
  {
    return false;
  }
  if (!stayTime_set)
  {
    return false;
  }
  int WaitTime = stayTime_param0;
  if (longEnough(StartTime,WaitTime))
  {
    return false;
  }
  /* plan body */
  add_achieve_makeMovement(Pos, Target);
  internal_achieve_stayAtTarget(StartTime);
  return true;
}

/*
+!stayAtTarget(StartTime)
   : takenOff() & myPosition(Pos) & lastTarget(Target) & stayWithinParameters(XyzDist, YawDist) & closeEnough(Pos, Target, XyzDist, YawDist) & stayTime(WaitTime) & longEnough(StartTime, WaitTime)
   <- sendHover();
      notifyUser("goto completed")
*/
static bool add_achieve_stayAtTarget_plan2(int param0)
{
  /* instantiate trigger parameters */
  int StartTime = param0;
  /* plan context, i.e., guards */
  if (!takenOff_set)
  {
    return false;
  }
  if (!myPosition_set)
  {
    return false;
  }
  positiont Pos = myPosition_param0;
  if (!lastTarget_set)
  {
    return false;
  }
  positiont Target = lastTarget_param0;
  if (!stayWithinParameters_set)
  {
    return false;
  }
  double XyzDist = stayWithinParameters_param0;
  double YawDist = stayWithinParameters_param1;
  if (!closeEnough(Pos,Target,XyzDist,YawDist))
  {
    return false;
  }
  if (!stayTime_set)
  {
    return false;
  }
  int WaitTime = stayTime_param0;
  if (!longEnough(StartTime,WaitTime))
  {
    return false;
  }
  /* plan body */
  sendHover();
  notifyUser("goto completed");
  return true;
}

/*
+!takeOff()
   : not takenOff()
   <- notifyUser("taking off");
      sendTakeOff();
      Time = getCurrentTimeMS();
      +takenOff();
      !completeTakeOff(Time)
*/
static bool add_achieve_takeOff_plan0(void)
{
  /* instantiate trigger parameters */
  /* plan context, i.e., guards */
  if (takenOff_set)
  {
    return false;
  }
  /* plan body */
  notifyUser("taking off");
  sendTakeOff();
  int Time = getCurrentTimeMS();
  takenOff_set = true;
  internal_achieve_completeTakeOff(Time);
  return true;
}

/*
+!waitForCommand()
   : not takenOff()
   <- !!waitForCommand()
*/
static bool add_achieve_waitForCommand_plan0(void)
{
  /* instantiate trigger parameters */
  /* plan context, i.e., guards */
  if (takenOff_set)
  {
    return false;
  }
  /* plan body */
  internal_achieve_new_focus_waitForCommand();
  return true;
}

/*
+!waitForCommand()
   : takenOff() & lastTarget(Target)
   <- ?myPosition(Pos);
      !makeMovement(Pos, Target);
      !!waitForCommand()
*/
static bool add_achieve_waitForCommand_plan1(void)
{
  /* instantiate trigger parameters */
  /* plan context, i.e., guards */
  if (!takenOff_set)
  {
    return false;
  }
  if (!lastTarget_set)
  {
    return false;
  }
  positiont Target = lastTarget_param0;
  /* plan body */
  if (!myPosition_set)
  {
    // TODO: this case needs to be specified
  }
  positiont Pos = myPosition_param0;
  add_achieve_makeMovement(Pos, Target);
  internal_achieve_new_focus_waitForCommand();
  return true;
}

/*
+!waitForCommand()
   : takenOff()
   <- notifyUser("no last target set, using current position");
      sendHover();
      ?myPosition(Pos);
      +lastTarget(Pos);
      !!waitForCommand()
*/
static bool add_achieve_waitForCommand_plan2(void)
{
  /* instantiate trigger parameters */
  /* plan context, i.e., guards */
  if (!takenOff_set)
  {
    return false;
  }
  /* plan body */
  notifyUser("no last target set, using current position");
  sendHover();
  if (!myPosition_set)
  {
    // TODO: this case needs to be specified
  }
  positiont Pos = myPosition_param0;
  lastTarget_set = true;
  lastTarget_param0 = Pos;
  internal_achieve_new_focus_waitForCommand();
  return true;
}

/* applicable plan selection */
static void add_achieve_completeTakeOff(int param0)
{
  if (add_achieve_completeTakeOff_plan0(param0))
  {
    return;
  }
  if (add_achieve_completeTakeOff_plan1(param0))
  {
    return;
  }
  // TODO: Handle the case where no plan is applicable
  return;
}

static void add_achieve_completeGoto(void)
{
  if (add_achieve_completeGoto_plan0())
  {
    return;
  }
  if (add_achieve_completeGoto_plan1())
  {
    return;
  }
  // TODO: Handle the case where no plan is applicable
  return;
}

static void add_achieve_goto(positiont param0)
{
  if (add_achieve_goto_plan0(param0))
  {
    return;
  }
  // TODO: Handle the case where no plan is applicable
  return;
}

static void add_achieve_land(void)
{
  if (add_achieve_land_plan0())
  {
    return;
  }
  // TODO: Handle the case where no plan is applicable
  return;
}

static void add_achieve_makeMovement(positiont param0, positiont param1)
{
  if (add_achieve_makeMovement_plan0(param0, param1))
  {
    return;
  }
  // TODO: Handle the case where no plan is applicable
  return;
}

static void add_achieve_stayAtTarget(int param0)
{
  if (add_achieve_stayAtTarget_plan0(param0))
  {
    return;
  }
  if (add_achieve_stayAtTarget_plan1(param0))
  {
    return;
  }
  if (add_achieve_stayAtTarget_plan2(param0))
  {
    return;
  }
  // TODO: Handle the case where no plan is applicable
  return;
}

static void add_achieve_takeOff(void)
{
  if (add_achieve_takeOff_plan0())
  {
    return;
  }
  // TODO: Handle the case where no plan is applicable
  return;
}

static void add_achieve_waitForCommand(void)
{
  if (add_achieve_waitForCommand_plan0())
  {
    return;
  }
  if (add_achieve_waitForCommand_plan1())
  {
    return;
  }
  if (add_achieve_waitForCommand_plan2())
  {
    return;
  }
  // TODO: Handle the case where no plan is applicable
  return;
}

/* init */
void init(void)
{
  // initialise event queue
  init_events();
  // set initial beliefs
  takeOffTime_set = true;
  takeOffTime_param0 = 5000;
  initialReachParameters_set = true;
  initialReachParameters_param0 = 0.2;
  initialReachParameters_param1 = 5;
  stayWithinParameters_set = true;
  stayWithinParameters_param0 = 0.5;
  stayWithinParameters_param1 = 5;
  stayTime_set = true;
  stayTime_param0 = 2000;
  // initial goal: !waitForCommand()
  add_achieve_waitForCommand();
}

/* clear */
void clear(void)
{
  // clear event queue
  init_events();
  // restart with initial goal: !waitForCommand()
  add_achieve_waitForCommand();
}

/* do one step */
void next_step(void)
{
  updateBeliefs();
  eventt event = get_next_event();
  handle_percept(event);
  if (!is_triggering(event))
  {
    event = get_next_event();
    handle_percept(event);
  }
  switch (event.trigger)
  {
    case ADD_ACHIEVE_COMPLETETAKEOFF:
      add_achieve_completeTakeOff(event.completeTakeOff_param0);
      break;
    case ADD_ACHIEVE_COMPLETEGOTO:
      add_achieve_completeGoto();
      break;
    case ADD_ACHIEVE_GOTO:
      add_achieve_goto(event.goto_param0);
      break;
    case ADD_ACHIEVE_LAND:
      add_achieve_land();
      break;
    case ADD_ACHIEVE_MAKEMOVEMENT:
      add_achieve_makeMovement(event.makeMovement_param0, event.makeMovement_param1);
      break;
    case ADD_ACHIEVE_STAYATTARGET:
      add_achieve_stayAtTarget(event.stayAtTarget_param0);
      break;
    case ADD_ACHIEVE_TAKEOFF:
      add_achieve_takeOff();
      break;
    case ADD_ACHIEVE_WAITFORCOMMAND:
      add_achieve_waitForCommand();
      break;
    default:  // nothing to do
      break;
  }
}
