#include <avr/io.h>

int main(void)
{
  // LED on PB0 as output
  DDRB |= (1 << PB0);

  // Timer0: CTC mode, prescaler = 64
  TCCR0 = (1 << WGM01) | (1 << CS01) | (1 << CS00);
  OCR0 = 124; // 1 ms compare match interval @ 8 MHz, prescaler 64

  uint16_t tickCount = 0;

  while (1)
  {
    // Poll the compare match flag (no interrupt used)
    if (TIFR & (1 << OCF0))
    {
      TIFR |= (1 << OCF0); // clear flag by writing 1 to it

      tickCount++;
      if (tickCount >= 500)
      {                      // 500 ms elapsed
        PORTB ^= (1 << PB0); // toggle LED
        tickCount = 0;
      }
    }
  }

  return 0;
}