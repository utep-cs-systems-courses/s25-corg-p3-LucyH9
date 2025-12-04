#ifndef TOY_H 
#define TOY_H

#include <msp430.h>
#include <stdint.h>

// LEDs
#define LED_RED   BIT0    // P1.0
#define LED_GREEN BIT6    // P1.6 (PWM capable)

// Buttons
#define BTN0 BIT3  // P1.3
#define BTN1 BIT3  // P2.3 (adjust if needed)

// States
#define IDLE       0
#define PLAY_TUNE  1
#define FLASH_FAST 2
#define FLASH_SLOW 3
#define FREERUN    4
#define NUM_STATES 5

// State variable
extern volatile uint8_t state;

// Function prototypes
void init_hardware(void);
void init_buttons(void);
void init_timer(void);
void update_state(void);

#endif

