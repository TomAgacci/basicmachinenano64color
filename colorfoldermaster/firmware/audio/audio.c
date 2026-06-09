#include <avr/io.h>
#include <avr/interrupt.h>
#include "audio.h"

volatile uint8_t audio_sample = 128;

void audio_init(void) {
    // OC2A as PWM audio out
    DDRB |= (1 << PB3); // pin 11 on many boards
    TCCR2A = (1<<COM2A1) | (1<<WGM21) | (1<<WGM20); // Fast PWM
    TCCR2B = (1<<CS20); // no prescale
    OCR2A = 128;
    TIMSK2 = (1<<TOIE2); // enable overflow ISR
}

ISR(TIMER2_OVF_vect) {
    OCR2A = audio_sample;
}
