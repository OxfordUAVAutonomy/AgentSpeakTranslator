/* File actions.h generated from autopilot.asl using AgentSpeakTranslator */

#ifndef __ACTIONS_H
#define __ACTIONS_H
#include <stdbool.h>
#include "types.h"


/* basic functions */
control_commandt calculateMovement(positiont, positiont);
bool closeEnough(positiont, positiont, double, double);
int getCurrentTimeMS(void);
bool longEnough(int, int);
void notifyUser(messaget);
void sendGoto(control_commandt);
void sendHover(void);
void sendLand(void);
void sendTakeOff(void);
void updateBeliefs(void);
#endif /* __ACTIONS_H */
