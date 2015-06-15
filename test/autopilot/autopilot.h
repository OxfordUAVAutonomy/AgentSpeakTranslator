/* File autopilot.h generated from autopilot.asl using AgentSpeakTranslator */

#ifndef __AUTOPILOT_H
#define __AUTOPILOT_H
#include <stdbool.h>
#include "types.h"


/* "public" interface */
void init(void);
void next_step(void);
bool can_add_event(void);
void clear(void);
void updateBeliefs(void);

/* "public" trigger functions */
void add_initialReachParameters(double, double);
void add_stayTime(int);
void add_stayWithinParameters(double, double);
void achieve_goto(positiont);
void achieve_land(void);
void achieve_takeOff(void);

 /* setters for belief base */
void set_myPosition(positiont);
#endif /* __AUTOPILOT_H */
