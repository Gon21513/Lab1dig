/* 
 * File:   display.c
 * Author: lpgp3
 *
 * Created on 19 de julio de 2023, 07:29 PM
 */

#include "display.h"
#include <xc.h>

uint8_t ADC;
uint8_t low;
uint8_t high;

uint8_t TABLA[16] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9
    0x77, // A
    0x7C, // B
    0x39, // C
    0x5E, // D
    0x79, // E
    0x71  // F
};


//read adc
void read_ADC(uint8_t dato){
    ADC = dato;
    //return ADRESH;
}

// Separa los 4 bits menos significativos en low y los 4 bits más significativos en high

void LH(uint8_t dato){
    low = dato & 0x0F;
    high = (dato >> 4) & 0x0F;
    
}

// Utiliza 'dato' como índice en la tabla de búsqueda TABLA y muestra el valor correspondiente en PORTD

void segmentos (uint8_t dato){
    PORTD = TABLA[dato];
}

