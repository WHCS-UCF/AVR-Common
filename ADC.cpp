#include "ADC.h"

#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "pinout.h"

static volatile uint16_t gConversion = 0;

/*ISR(ADC_vect)
{
  gConversion = ADC;
}*/

EMPTY_INTERRUPT(ADC_vect);

void WHCSADC::init()
{
  PIN_MODE_INPUT(ADC); // all inputs (to engage pull ups)

  // select Vref=AVcc
  ADMUX |= (1<<REFS0);

  // set prescaller to 128 and enable ADC 
  ADCSRA |= _BV(ADPS2) | _BV(ADPS1) |
    _BV(ADPS0) | _BV(ADEN);// | _BV(ADIE);
}

uint16_t WHCSADC::read(uint8_t pin)
{
  // select ADC channel with safety mask
  ADMUX = (ADMUX & 0xF0) | (pin & 0x0F);

  // single conversion mode
  ADCSRA |= _BV(ADSC);

  // wait for ADC conversion
  while (ADCSRA & _BV(ADSC));

  return ADC;

  /*loop_until_bit_is_set(UCSRA, UDRE);
  loop_until_bit_is_set(UCSRA, TXC);

  set_sleep_mode(SLEEP_MODE_ADC); // ADC low noise
  sleep_enable();

  while (ADCSRA & _BV(ADSC)) {
    sleep_cpu();
  }

  sleep_disable();
  set_sleep_mode(0); // ADC low noise

  return ADC;*/
}
