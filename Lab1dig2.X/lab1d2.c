/* 
 * File:   prelab10.c
 * Author: lpgp3
 *
 * Created on 30 de abril de 2023, 01:47 PM
 */
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#define _XTAL_FREQ 8000000

#include<xc.h>
#include<pic.h>
#include<stdint.h>
#include "boton.h"
#include "ADC.h"
#include "display.h"

//-------------------------------variables------------------------------------|
uint8_t counter;
uint8_t  ADC;

uint8_t low;
uint8_t high;

uint8_t bandera = 0;
uint8_t display[3];



//|------------------------------prototipos------------------------------------|

void setup(void);
void __interrupt() isr(void);


//----------------Interrupcion--------------------
void __interrupt() isr(void) {
if (RBIF) {
        // Si RB0 está presionado  y RB1 no está presionado 
        if (RB0 == 0 && RB1 == 1) {
            counter++; // Incrementar el contador
        }
        // Si RB0 no está presionado  y RB1 está presionado 
        if (RB0 == 1 && RB1 == 0) {
            counter--; // Decrementar el contador
        }
        

        // Limpiar la bandera de interrupción de PORTB 
        RBIF = 0;
    }

     //__delay_ms(10);
     
    if (PIR1bits.ADIF){
        read_ADC(ADRESH);
       // ADC = read_ADC();// mover el valor del adc al puerto D
        //PORTD = ADC;
    }
    PIR1bits.ADIF = 0;
    
    //interrupcion del tmr0 
    if (INTCONbits.T0IF){ //CHEQUEAR INTERRUPCION DEL TMR0
        //TMR0 = 216;
        //T0IF = 0;//LIMPIAR BANDARA
        PORTE = 0;
    
    // Chequear el valor de la variable bandera y actualizar el display correspondiente
//
//        if (bandera == 0){//chequear la nbadera
//            //PORTD = display[2];// usar el prot c
//            PORTE = 1;// habilita el diaplay
//            bandera = 1;//cambia el valor de la bandra
//            
        //}
        if (bandera == 0){ //chequea si la bandera es 1
            //PORTD = display[1];// muestra el digito en potrc
            segmentos(high);
            PORTE = 2;// habilita display de decenas
            bandera = 1;//cambia la bandera
        }
        else if (bandera == 1){ //chequea  si la bandera es 2
            //PORTD = display[0];//
            segmentos(low);
            PORTE = 4;//habiliya display de unidades
            bandera = 0;// cambia valor de la bandera
        }
        TMR0 = 216;
        INTCONbits.T0IF = 0; ///reiniciar el tmr0
            
    }
    //interrupcion del tmr0
    return;
}

//-------------main----------------------

void    main(void){
    setup();
    while(1){
        
        PORTC = counter; //poner el valor del contador en el puerto A
        LH(ADC);
        
        
        //alarma
        
        if (ADC < counter){
            PORTBbits.RB2 = 0;
        }
        else {
            PORTBbits.RB2 = 1;
        }
              
        //verifica la conversion adc
        if (ADCON0bits.GO == 0){
            ADCON0bits.GO = 1;
            __delay_ms(50);
        }


        
    }
}




//|------------------------------Setup----------------------

void setup(void){
    
//-------------configuracion de puertos----------------
    TRISA  = 0;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    


    
    ioc_init(0);
    ioc_init(1);
    ioc_init(2);


//------------interrupciones-----------------
    INTCONbits.RBIE = 1; //habilitar interrupciones en portb
    INTCONbits.PEIE = 1;//habilitar interrupciones perifericas 
    INTCONbits.GIE = 1; //habilitar interrupciones globales
    INTCONbits.RBIF = 0; //limpirar bander de interrupcion de portb

    //del tmr0
    INTCONbits.T0IF = 0; //interrupcion del tmr0
    INTCONbits.T0IE = 1;///interrupcion del tmr0
    
    //del adc
    PIE1bits.ADIE = 1; // habilitar interrupciones de ADC

    
    
   //Se inician los puertos 
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    
    
// --------------- Oscilador --------------- 
    OSCCONbits.IRCF = 0b111; // 8 MHz
    OSCCONbits.SCS = 1; // Seleccionar oscilador interno

//----------------ADC ------------------
    //ADC
    adc_init(0);
    
//------------------ tmr0-------------------------
    OPTION_REGbits.T0CS = 0; //Usar Timer0 con Fosc/4
    OPTION_REGbits.PSA = 0; //Prescaler con el Timer0
    OPTION_REGbits.PS2 = 1; //Prescaler de 256
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;  
    TMR0 = 216; ///VALOR INICIAL DEL TMR0
    //Se inicia el contador en 0
    counter =   0;

}

       


