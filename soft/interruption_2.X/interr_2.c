/*
 * File:   interr_2.c
 * Author: Dariel Arian Acosta Campos e Jimy Li Yan
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

int flag;
int time;

void __interrupt() trem(void){
    if (INTF){
        INTCONbits.INTF = 0;
        Sirene = 1;                             //sirene ligado
        Fechando = 0;
        Abrindo = 0;
        if(SCA == 0){                           //se senor de cancela aberta ta acionado
            for(time = 20-time;time>0;time--){  //conta 20s para carro passa
                __delay_ms(1000);
            }
            while(SCF == 1){                    //se sensor cancela fechado ta desligado
                MCF = 1;                        //motor de cancela fechando aciona
                MCA = 0;
                Fechando = 1;                   //liga o led fechando
            }
            //__delay_ms(500);
        }
        if(SCF == 0){                           //se sensor de cancela fechado
            MCF = 0;
            MCA = 0;
            Fechando = 0;
        }
    flag = 1;
    time = 21;
    __delay_ms(40000);
    }
    return;
}

void main(void) {
    TRISB = 0b00011111;
    TRISD = 0b00000000;
    OPTION_REG = 0b01111111;                    //ativa resistor de pull-up
    MCA = 0;                                    //Motor abrindo desligado
    MCF = 0;                                    //Motor fechando desligado
    Sirene = 0;                                 //Sirene desligada
    Fechando = 0;                               //Led fechando desligado
    Abrindo = 0;                                //Led abrindo desligado
    INTCON = 0b10010000;                        //ativa os bits GIE e INTE, ou seja, a interrup??o externa
                                                //atrav?s do pino RB0
    while(1){
        flag = 0;
        Sirene = 0;
        if(SA == 0 && flag == 0){                //Se tem o carro
            MCA = 1;                            //motor de cancela abrindo
            MCF = 0;
            Abrindo = 1;                        //Led abrindo ligado
            Fechando = 0;
            //Sirene = 0;
        }
        if(SCA == 0 && flag == 0){              //se sensor de cancela aberta ta ligado 
            MCA = 0;                            //entao desliga o motor
            MCF = 0;
            Fechando = 0;
            Abrindo = 0;                        //e desliga o led abrindo
            for(time = 1;time<21;time++){
                __delay_ms(1000);                //delay de 20s
            }
            while(SCF == 1 && flag ==0){
                MCF = 1;                        //depois de 20s, aciona motor de cancela fechando
                MCA = 0;
                Fechando = 1;                   //liga o led fechando
            }
            //__delay_ms(100);
            //Sirene = 0;
        }
        if(SCF == 0 && flag == 0){               //se sensor de cancela fechada ta acionado
            MCF = 0;                            //desliga o motro de cancela fechando
            MCA = 0;
            Fechando = 0;                       //desliga o led fechando
            Abrindo = 0;
            //Sirene = 0;
        }
    }
    return;
}
