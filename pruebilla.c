#pragma config PLLDIV = 2, CPUDIV = OSC1_PLL2, USBDIV = 2
#pragma config FOSC = HSPLL_HS, FCMEN = OFF, IESO = OFF
#pragma config PWRT = OFF, BOR = OFF, BORV = 3, VREGEN = OFF
#pragma config WDT = OFF
#pragma config WDTPS = 32768
#pragma config CCP2MX = ON, PBADEN = OFF, LPT1OSC = OFF, MCLRE = ON
#pragma config STVREN = ON, LVP = OFF, ICPRT = OFF, XINST = OFF
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF
#pragma config CPB = OFF, CPD = OFF
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF
#pragma config WRTC = OFF, WRTB = OFF, WRTD = OFF
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF
#pragma config EBTRB = OFF

#define _XTAL_FREQ 48000000
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

#include "spi.h"                                // Libreria del protocolo SPI
#include "tft_st7735.h"                         // Libreria de la pantalla TFT ST7735

void main()
{
    
    ADCON1=0X03;        //establecer grupo de pines analogicos pag. 266
    ADCON0=0x00;        //00000000 pag 265  elegir el pin analogico exacto
    ADCON2=0x8f;        //pag 267 establece configuracion de velicidad clock
    ADCON0bits.ADON=1;  //encender el conversor analogico digital
    
    
    TFT_ST7735_Init(INITR_144GREENTAB);
    TFT_SetRotation(0);                         // Inicializa la pantalla vertical
    TFT_FillScreen(ST7735_BLUE);               // Limpia la pantalla TFT
    
    
    while(1)
    {
        
        ADCON0bits.GODONE=1;
        float res=0;
        char cadena[16];
        while(ADCON0bits.GODONE==1){
            int bit_low=ADRESL;
            int bit_hight=ADRESH;
            
            int calc=(ADRESH<<8)+ADRESL;
            res=(float)calc;
            //res=ADRES;
            __delay_ms(10);
        }
        cadena[0]=' ';
        cadena[1]=' ';
        cadena[2]=' ';
        cadena[3]=' ';
        cadena[4]=' ';
        cadena[5]=' ';
        cadena[6]=' ';
        sprintf(cadena,"%0.2f",res);
        TFT_WriteString(0, 10, "<medida resistencia>" , ST7735_GREEN, ST7735_BLACK, 1);
 
        TFT_WriteString(1, 40, cadena, ST7735_GREEN, ST7735_BLACK, 1);
        
        TFT_FillRect(80,40,60,60,ST7735_BLUE);

        
        
        
        
        
       
        
        
        
        

       
    }
}