/**
 * Brief: Simple tool to check 12V Pb-Acid battery
 * (C) 2014 Sergey Shcherbakov <shchers@gmail.com>
 */
#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_RED_H	PB3
#define LED_GREEN	PB1
#define LED_YELLOW	PB2
#define LED_RED_L	PB0
#define VBAT_IN		ADC2

#define VR1	(11000UL)
#define VR2 (5100UL)
#define VK	((VR2+VR1)/VR2)
// Reference value x100
#define VREF (500UL)
#define VCONST	(VREF*VK)

// Levels of battery. As a reference used http://autogrep.ru/review/212.html
// NOTE: All values is a x100 to avoid float-point operations
#define CHARGE_00	(1050)
#define CHARGE_10	(1131)
#define CHARGE_20	(1158)
#define CHARGE_30	(1175)
#define CHARGE_40	(1190)
#define CHARGE_99	(1260)
#define GEN_FAULT	(1450)

// Helpers
#define LED_OFF(led)	PORTB |= _BV(led)
#define LED_ON(led)		PORTB &= ~_BV(led)

// ADC interrupt hander
ISR(ADC_vect)
{
	uint16_t vbat = (ADCW * VCONST) / 1024;

    LED_OFF(LED_RED_H);
    LED_OFF(LED_GREEN);
    LED_OFF(LED_YELLOW);
    LED_OFF(LED_RED_L);

	if (vbat > GEN_FAULT) {
		LED_ON(LED_RED_H);
	} else if (vbat <= GEN_FAULT && vbat > CHARGE_99) {
        LED_ON(LED_RED_H);
        LED_ON(LED_GREEN);
	} else if (vbat <= CHARGE_99 && vbat > CHARGE_40) {
        LED_ON(LED_GREEN);
    } else if (vbat <= CHARGE_40 && vbat > CHARGE_30) {
        LED_ON(LED_GREEN);
        LED_ON(LED_YELLOW);
    } else if (vbat <= CHARGE_30 && vbat > CHARGE_20) {
        LED_ON(LED_YELLOW);
    } else if (vbat <= CHARGE_20 && vbat > CHARGE_10) {
        LED_ON(LED_YELLOW);
        LED_ON(LED_RED_L);
    } else {
        LED_ON(LED_RED_L);
	}
}

int main(void)
{
	// Simple init
	DDRB = _BV(LED_RED_H) | _BV(LED_GREEN) | _BV(LED_YELLOW) | _BV(LED_RED_L);
	// Turn-off leds
	PORTB = 0xff;
	// Select ADC2 input
	ADMUX = 0x02;
	ADCSRA = _BV(ADEN) | _BV(ADATE) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
	// Free run
	ADCSRB = 0;

	// Global interrupt enable
	sei();

	// Start ADC
	ADCSRA |= _BV(ADSC);

	// Infinity loop
	while(1) {}
	return 0;
}
