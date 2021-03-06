#include "timing.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#if !defined(__AVR_ATmega328P__) && \
    !defined(__AVR_ATmega32__)
  #error "Timing does not support the selected MCU"
#endif

volatile time_t gMillis = 0;

/* Story time:
 *  Due to the frequency of the millis counting timer, 
 *  it has the potential to affect similar, higher priority
 *  interrupts, such as software serial.
 *
 *  Due to this, and through research, I have included the
 *  ISR attribute ISR_NOBLOCK. This makes sure that
 *  as soon as possible, interrupts are reenabled.
 *  Essentially, this timing interrupt isn't that important
 *  compared to real-time software serial. That attribute
 *  allows for the real important interrupts to have priority.
 *  
 *  Neat!
 */
#if defined(__AVR_ATmega328P__)
ISR(TIMER2_COMPA_vect, ISR_NOBLOCK)
#elif defined(__AVR_ATmega32__)
ISR(TIMER2_COMP_vect, ISR_NOBLOCK)
#endif
{
  gMillis++;
}

void timing_init()
{
  // Timer2 to normal mode with clk/64 (4us)
  // Interrupt on OCR2 match (250)
  // 4us * 250 = 1ms
  // Could be adjusted for the time an interrupt takes
#if defined(__AVR_ATmega328P__)
  OCR2A = 250;
  TIMSK2 |= _BV(OCIE2A); // enable interrupt for that timer
  TCCR2A = _BV(WGM21); // enable timer
  TCCR2B = _BV(CS22);
#elif defined(__AVR_ATmega32__)
  OCR2 = 250;
  TIMSK |= _BV(OCIE2); // enable interrupt for that timer
  TCCR2 = _BV(CS22) | _BV(WGM21); // enable timer
#endif
}
