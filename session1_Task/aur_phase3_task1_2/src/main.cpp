#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    
    DDRB |= (1 << PB3);

    
    TCCR0 = (1 << WGM01) | (1 << WGM00) | (1 << COM01);

    
    TCCR0 |= (1 << CS01);

    // f_PWM = 8MHz / (8 * 256) = 3906.25 Hz

    OCR0 = 64; 
    while (1) {
        
        for (uint8_t duty = 64; duty < 255; duty++) {
            OCR0 = duty;
            _delay_ms(10);
        }

        
        for (uint8_t duty = 255; duty > 64; duty--) {
            OCR0 = duty;
            _delay_ms(10);
        }

        OCR0 = 64; 
        _delay_ms(1000); 
    }

    return 0;
}