/* 
 * File:   boton.c
 * Author: lpgp3
 *
 * Created on 18 de julio de 2023, 03:49 PM
 */

#include "boton.h"
#include <xc.h>

//void ioc_init(char pin) {
//    INTCONbits.RBIE = 1; //habilitar interrupciones en portb
//
//    OPTION_REGbits.nRBPU = 0; //habilitarr pullups
//
//    // Configurar el pin como entrada
//    TRISB |= (1 << pin);
//    
//    // Configurar la interrupción por cambio de estado
//    IOCB |= (1 << pin);
//    
//    WPUB |= (1<< pin);
//
//    INTCONbits.RBIF = 0;
//}

void ioc_init(char pin) {
    ANSEL   =   0;
    ANSELH  =   0;
    INTCONbits.RBIE = 1; // Habilitar interrupciones en el puerto B

    OPTION_REGbits.nRBPU = 0; // Habilitar pull-ups

    // Configurar el pin como entrada
    switch (pin) {
        case 0:
            TRISBbits.TRISB0 = 1;
            break;
        case 1:
            TRISBbits.TRISB1 = 1;
            break;
        // Agregar más casos según sea necesario para otros pines
        default:
            break;
    }
    
    // Configurar la interrupción por cambio de estado
    switch (pin) {
        case 0:
            IOCBbits.IOCB0 = 1;
            break;
        case 1:
            IOCBbits.IOCB1 = 1;
            break;
        // Agregar más casos según sea necesario para otros pines
        default:
            break;
    }
    
    // Habilitar la resistencia de pull-up en el pin
    switch (pin) {
        case 0:
            WPUBbits.WPUB0 = 1;
            break;
        case 1:
            WPUBbits.WPUB1 = 1;
            break;
        // Agregar más casos según sea necesario para otros pines
        default:
            break;
    }
    
    // Limpiar la bandera de interrupción del puerto B
    INTCONbits.RBIF = 0;
}
