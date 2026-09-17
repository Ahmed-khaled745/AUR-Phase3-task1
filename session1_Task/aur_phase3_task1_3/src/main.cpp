#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t ledState = 0;

ISR(INT0_vect)
{
  ledState ^= 1;
  if (ledState)
    PORTB |= (1 << PB0);
  else
    PORTB &= ~(1 << PB0);
}

int main(void)
{
  DDRB |= (1 << PB0);
  PORTB &= ~(1 << PB0);

  DDRD &= ~(1 << PD2);
  PORTD |= (1 << PD2);

  MCUCR |= (1 << ISC01);
  MCUCR &= ~(1 << ISC00);

  GICR |= (1 << INT0);

  sei();

  while (1)
  {
  }

  return 0;
}