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

void main(void) {
    TRISB = 0;
    PORTBbits.RB1 = 1;
    return;
}
