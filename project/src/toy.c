#include "toy.h"
volatile uint8_t state = IDLE;
//Initialize
void init_hardware(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    // Configure LED pins
    P1DIR |= (LED_RED | LED_GREEN);  // Output
    P1SEL |= LED_GREEN;              // P1.6 = TA0.1 output for PWM
    P1OUT &= ~(LED_RED | LED_GREEN);

    // Configure buttons P2.0–P2.3 with pull-ups and interrupts
    P2DIR &= ~(BTN0 | BTN1 | BTN2 | BTN3);
    P2REN |=  (BTN0 | BTN1 | BTN2 | BTN3);
    P2OUT |=  (BTN0 | BTN1 | BTN2 | BTN3);
    P2IES |=  (BTN0 | BTN1 | BTN2 | BTN3); // falling edge
    P2IFG  =  0; // clear flags
    P2IE  |=  (BTN0 | BTN1 | BTN2 | BTN3);

    // Timer0_A for LED PWM (P1.6)
    TA0CCR0 = 1000;               // PWM period
    TA0CCTL1 = OUTMOD_7;          // Reset/set mode
    TA0CCR1 = 200;                // Initial brightness (dim)
    TA0CTL = TASSEL__SMCLK | MC__UP | TACLR;

    // Timer1_A for speaker PWM (P2.6, optional — connect speaker to P2.6)
    P2DIR |= BIT6;
    P2SEL |= BIT6;
    TA1CCR0 = 0;   // Off by default
    TA1CCTL1 = OUTMOD_7;
    TA1CCR1 = 0;
    TA1CTL = TASSEL__SMCLK | MC__UP | TACLR;
}

//LED and Sound Functions
// Simple brightness control (change duty cycle)
void set_led_brightness(uint8_t bright)
{
    if (bright)
        TA0CCR1 = 800;   // bright
    else
        TA0CCR1 = 200;   // dim
}

// Flash patterns
void set_led_pattern_fast(void)
{
    P1OUT ^= LED_RED;
    __delay_cycles(100000);
}

void set_led_pattern_slow(void)
{
    P1OUT ^= LED_RED;
    __delay_cycles(300000);
}

// Idle state: both LEDs off
void do_idle(void)
{
    P1OUT &= ~(LED_RED);
    TA0CCR1 = 0;
}

// Random / fun mode
void do_random_effects(void)
{
    P1OUT ^= LED_RED;
    set_led_brightness((P1OUT & LED_RED) ? 1 : 0);
    TA1CCR0 = 500 + (rand() % 5000);
    TA1CCR1 = TA1CCR0 / 2;
    __delay_cycles(100000);
}

//Sound
void play_tone(unsigned int freq, unsigned int duration_ms)
{
    unsigned int period = 1000000 / freq;   
    TA1CCR0 = period;
    TA1CCR1 = period / 2;
    __delay_cycles(duration_ms * 1000);
    TA1CCR0 = 0;   // stop tone
    TA1CCR1 = 0;
}

void play_melody(void)
{
    play_tone(NOTE_C4, 200);
    play_tone(NOTE_E4, 200);
    play_tone(NOTE_G4, 200);
    play_tone(NOTE_C5, 400);
}

//State Machines

void update_state(void)
{
    switch (state) {
        case IDLE:        do_idle(); break;
        case PLAY_TUNE:   play_melody(); break;
        case FLASH_FAST:  set_led_pattern_fast(); break;
        case FLASH_SLOW:  set_led_pattern_slow(); break;
        case FREERUN:     do_random_effects(); break;
        default:           state = IDLE; break;
    }
}

//Buttons
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    if (P2IFG & BTN0) {
        asm_next_state();   // Advance state in assembly
    }
    if (P2IFG & BTN1) {
        state = (state == 0) ? NUM_STATES - 1 : state - 1; // back
    }
    if (P2IFG & BTN2) {
        play_melody(); // manual play
    }
    if (P2IFG & BTN3) {
        state = FREERUN;
    }
    P2IFG = 0; // clear flags
}
