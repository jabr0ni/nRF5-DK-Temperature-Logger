#ifndef _LED_HANDLER_H_
#define _LED_HANDLER_H_

#include "main.h" 

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

void initLED();
void toggleLED();

#endif /* _LED_HANDLER_H_ */