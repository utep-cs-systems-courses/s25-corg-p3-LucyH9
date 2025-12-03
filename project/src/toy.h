#ifndef TOY_H
#define TOY_H

#include <msp430.h>
#include <stdint.h>

// LED & button pin definitions
#define LED_RED     BIT0   // P1.0
#define LED_GREEN   BIT6   // P1.6

#define BTN0 BIT0   // P2.0
#define BTN1 BIT1   // P2.1
#define BTN2 BIT2   // P2.2
#define BTN3 BIT3   // P2.3

#define NUM_STATES 5

// Tone frequencies (Hz)
#define NOTE_C4 261
#define NOTE_E4 329
#define NOTE_G4 392
#define NOTE_C5 523

// State machine states
typedef enum {
    IDLE = 0,
    PLAY_TUNE,
    FLASH_FAST,
    FLASH_SLOW,
    FREERUN
} toy_state_t;

// ❗ FIXED TYPO: should be uint8_t, not uint8t_t
extern volatile uint8_t state;

// Functions implemented in C
void init_hardware(void);
void update_state(void);
void play_melody(void);
void set_led_brightness(uint8_t bright);
void set_led_pattern_fast(void);
void do_idle(void);
void do_random_effects(void);

// Function implemented in assembly
// Assembly file MUST contain:   .global asm_next_state
// and a RET
extern void asm_next_state(void);

#endif
