/*
 * File:   interr_2.c
 * Author: Jimmy e Dariel
 *
 * Created on 2021?4?14?, ??4:58
 */


#include <xc.h>
#include <pic16f877a.h>             // biblioteca do chip
#include <stdio.h>                  // biblioteca do C

#define _XTAL_FREQ 4000000          // define a frequencia do clock, 4M

#define MCA PORTBbits.RB5
#define MCF PORTBbits.RB6
#define Sirene PORTBbits.RB7
//#define ST PORTBbits.RB0
#define SA PORTBbits.RB1
#define SCF PORTBbits.RB2
#define SCA PORTBbits.RB3
#define Fechando PORTDbits.RD7
#define Abrindo PORTDbits.RD6

int flag = 0;

void __interrupt() trem(void){
    if (INTF){
        flag = 1;
        Sirene = 1;
        if(SA == 0){            //se senor do carro ta acionado
            __delay_ms(20000);
            
        }
        if(SCA == 0){           //se sensor de cancela fechado nao esta ligado
            MCF = 1;            //motor de cancela fecha
            MCA = 0;
            Fechando = 1;       //led fechando ligado
            Abrindo = 0;
        }
        if(SCF == 0){            //se sensor de cancela fechado
            MCF = 0;
            MCA = 0;
            Fechando = 0;
        }
    }
    return;
}

void main(void) {
    TRISB = 0b00011111;
    TRISD = 0b00000000;
    OPTION_REG = 0b01111111;        //ativa resistor de pull-up
    MCA = 0;                        //Motor abrindo desligado
    MCF = 0;                        //Motor fechando desligado
    Sirene = 0;                     //Sirene desligada
    Fechando = 0;                   //Led fechando desligado
    Abrindo = 0;                    //Led abrindo desligado
    INTCON = 0b10010000; //ativa os bits GIE e INTE, ou seja, a interrup??o externa
                        //atrav?s do pino RB0
    while(1){
        if(SA == 0 && flag == 0){                //Se tem o carro
            MCA = 1;                //motor de cancela abrindo
            MCF = 0;
            Abrindo = 1;            //Led abrindo ligado
        }
        if(SCA == 0 && flag == 0){           //se sensor de cancela aberta ta ligado 
            MCA = 0;            //entao desliga o motor
            MCF = 0;
            Abrindo = 0;        //e desliga o led abrindo
            __delay_ms(20000);      //delay de 20s
            MCF = 1;                //depois de 20s, aciona motor de cancela fechando
            MCA = 0;
            Fechando = 1;           //liga o led fechando
        }
        if(SCF == 0 && flag == 0){               //se sensor de cancela fechada ta acionado
            MCF = 0;                //desliga o motro de cancela fechando
            MCA = 0;
            Fechando = 0;           //desliga o led fechando
        }
    }
    return;
}
