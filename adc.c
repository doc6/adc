/*
 * adc.c
 *
 *  Created on: 21/08/2012
 *      Author: M. J. Cree
 *      Modified: D. O. Corlett
 *
 *  Modified 8/8/2013 to indicate 8-bit ADC read is broken.
 *
 *  Modified 6/09/16	to display the 8-bit approximation
 *  of the analogue input pin A0 on an LCD, by: D. O. Corlett
 */

#include <avr/io.h>
#include "adc.h"
#include "my_lcd.h"
#include <util/delay.h>

/*
 * adc_init()
 *
 * Power up the ADC hardware and select the ADC input pin.
 * pin must be set to a value listed in Table 23-4 of the ATmega manual
 */
void adc_init(int pin)
{
	pin &= 0xf;

	/* Make sure the ADC circuit is powered up */
	PRR &= ~(1<<PRADC);

	/* Turn off GPIO on the specified analogue input pin */
	if (pin <= 7) {
		/* Only the first eight inputs correspond to GPIO on Port C */
		DIDR0 = 1<<pin;
	}

	/* Set up ADC control registers ready for conversions */
        /* Enable ADC and set the lowest bit clock rate. */
	ADCSRA = (1<<ADEN) | 0x07;

	/* Unused so lets just set ADCSRB to 0 */
	ADCSRB = 0;

	/* Select ADC input source. */
	if (pin == 8) {
		/* Voltage ref is 1.1V for CPU temperature sensor */
		ADMUX = (1<<REFS1) | (1<<REFS0) | pin;
	}else{
		/* Voltage ref is AVcc (5V) for all other inputs */
		ADMUX = (1<<REFS0) | pin;
	}
}


/*
 * adc_read()
 *
 * Perform an analogue to digital conversion and return
 * the converted value.
 * Input source is specified by calling adc_init() first.
 */

int adc_read(void)
{
	int val = 0;

	ADCSRA |= 1<<ADSC; /* Insert code to trigger ADC conversion */

	// While conversion is still in progress:
	while ( ADCSRA & (1 << ADSC) )
	{
		/* do nothing */
	}

	/* Do 8-bit conversion --- student is to improve this to do a 10-bit conversion.
	 * Actually this code is broken and does not return an 8-bit conversion,
	 * therefore student is just to implement a working 10-bit ADC conversion.
	 */


	//val = ADCL;		// Do a 10-bit conversion by combining ADCL and ADCH making sure to read ADCL before ADCH. (this actually returns am 8 bit-conversion)
	//val |= ADCH;

	val = ADC;			// Do a 10-bit conversion

	return val;
}

int main()
{
	adc_init(0);		// Initalise pin 0 on portC for ADC read.
	my_lcd_init(4);		// Initalise 4 bit LCD on port D
	char con[32];		// Define display string

	while(1)
	{
		sprintf(con, "%s%i%c%s%X", "A0 = ", adc_read(), '\n',"A0 = 0x", adc_read());		// Create a string with the A0 ADC value.
		my_lcd_display(con);							// Display A0 value on LCD.
		_delay_ms(100);
	}

	return 0;
}
