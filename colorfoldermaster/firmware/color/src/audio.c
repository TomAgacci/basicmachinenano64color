#include <avr/io.h>
#include <avr/interrupt.h>
#include "audio.h"

volatile uint8_t audio_sample = 128;

void audio_init(void) {
    DDRB |= (1 << PB3); // OC2A PWM audio out
    TCCR2A = (1<<COM2A1) | (1<<WGM21) | (1<<WGM20);
    TCCR2B = (1<<CS20);
    OCR2A = 128;
    TIMSK2 = (1<<TOIE2);
}

ISR(TIMER2_OVF_vect) {
    OCR2A = audio_sample;
}
