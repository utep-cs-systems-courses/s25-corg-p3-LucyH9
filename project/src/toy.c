#include "toy.h"

// ----------------------------
// Define state here to fix linker
volatile uint8_t state = IDLE;

// ----------------------------
// LED blink timing
volatile unsigned int blink_counter = 0;
unsigned int blink_threshold = 50; // default

// ----------------------------
// Initialize hardware
void init_hardware(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog

    // LEDs
    P1DIR |= LED_RED | LED_GREEN;
    P1OUT &= ~(LED_RED | LED_GREEN);

    // Buttons
    init_buttons();

    // Timer for blinking
    init_timer();

    __bis_SR_register(GIE); // Enable global interrupts
}

// ----------------------------
// Initialize buttons with interrupts
void init_buttons(void)
{
    // BTN0: P1.3 (next pattern)
    P1DIR &= ~BTN0;
    P1REN |= BTN0;
    P1OUT |= BTN0;
    P1IES |= BTN0;    // High-to-low edge
    P1IFG &= ~BTN0;
    P1IE |= BTN0;

    // BTN1: P2.3 (previous pattern)
    P2DIR &= ~BTN1;
    P2REN |= BTN1;
    P2OUT |= BTN1;
    P2IES |= BTN1;
    P2IFG &= ~BTN1;
    P2IE |= BTN1;
}

// ----------------------------
// Initialize Timer_A0
void init_timer(void)
{
    TA0CCR0 = 32768 / 100;            // base interval ~10ms
    TA0CCTL0 = CCIE;                  // enable interrupt
    TA0CTL = TASSEL_1 | MC_1 | TACLR; // ACLK, up mode
}

// ----------------------------
// Timer ISR: handle LED blinking
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0_ISR(void)
{
    blink_counter++;

    // Set blink speed based on current state
    switch(state) {
        case FLASH_FAST:  blink_threshold = 25; break;
        case FLASH_SLOW:  blink_threshold = 75; break;
        case FREERUN:     blink_threshold = 50; break;
        default:          blink_threshold = 500; break; // IDLE
    }

    if (blink_counter >= blink_threshold) {
        blink_counter = 0;
        if(state != IDLE) {
            P1OUT ^= LED_RED;
            P1OUT ^= LED_GREEN;
        }
    }
}

// ----------------------------
// Button ISRs
#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR(void)
{
    if(P1IFG & BTN0) {  // Next pattern
        if(state >= FREERUN) state = IDLE;
        else state++;
        P1IFG &= ~BTN0;
    }
}

#pragma vector=PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
    if(P2IFG & BTN1) {  // Previous pattern
        if(state == IDLE) state = FREERUN;
        else state--;
        P2IFG &= ~BTN1;
    }
}

// ----------------------------
// update_state: empty because buttons control state
void update_state(void)
{
    // No automatic state update
}
