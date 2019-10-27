#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <stdint.h>

typedef enum {
    LED_STATE_OFF = 0,
    LED_STATE_ON

} LedState_e;

typedef enum {
    LED_PROCESS_DISABLED = 0,
    LED_PROCESS_BLINKING
}

typedef struct {
    LedState_e  State;                          /* Current state */

    uint16_t    LastTick;                       /* Timebase delta storage */

    void        (* EnableCmd)(uint8_t cmd);     /* Hardware abstraction layer. Function pointer to enable/disable led */


} Led;

void Led_SwitchOn(Led* led);
void Led_SwitchOff(Led* led);
void Led_Toggle(Led* led);
void Led_Write(Led* led, LedState_e state);


extern uint16_t Led_GetTick();                  /* API function to timebase system tick value */

#endif