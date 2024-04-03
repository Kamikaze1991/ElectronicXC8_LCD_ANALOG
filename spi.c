/* #include "spi.h"
*
* Creada por: Ing. Abiezer Hernandez O.
* Fecha de creacion: 28/11/2019
* Electronica y Circuitos
*
*/

#include "spi.h"

void SPI_Init_Master(char reloj, char modo, char smp)
{
    //ADCON1bits.PCFG=0x05;
    TRISBbits.RB0 = 1;      // SDI
    TRISBbits.RB1 = 0;      // SCK
    TRISCbits.RC7 = 0;      // SDO
    SSPCON1bits.SSPM = reloj;

    if(modo == MODE_1){
        SSPCON1bits.CKP = 1;
        SSPSTATbits.CKE = 1;
    }
    else if(modo == MODE_2){
        SSPCON1bits.CKP = 0;
        SSPSTATbits.CKE = 1;
    }
    else if(modo == MODE_3){
        SSPCON1bits.CKP = 1;
        SSPSTATbits.CKE = 0;
    }
    else if(modo == MODE_4){
        SSPCON1bits.CKP = 0;
        SSPSTATbits.CKE = 0;
    }
    SSPSTATbits.SMP = smp;
    SSPCON1bits.SSPEN = 1; 
}

void SPI_Init_Slave(char reloj, char modo)
{
    TRISBbits.RB0 = 1;      // SDI
    TRISBbits.RB1 = 1;      // SCK
    TRISAbits.RA5 = 1;      // SS
    TRISCbits.RC7 = 0;      // SDO
    SSPCON1bits.SSPM = reloj;

    if(modo == MODE_1){
        SSPCON1bits.CKP = 1;
        SSPSTATbits.CKE = 1;
    }
    else if(modo == MODE_2){
        SSPCON1bits.CKP = 0;
        SSPSTATbits.CKE = 1;
    }
    else if(modo == MODE_3){
        SSPCON1bits.CKP = 1;
        SSPSTATbits.CKE = 0;
    }
    else if(modo == MODE_4){
        SSPCON1bits.CKP = 0;
        SSPSTATbits.CKE = 0;
    }
    SSPCON1bits.SSPEN = 1;  
}

void SPI_Write(unsigned char dato)
{
    SSPBUF = dato;
    while(SSPSTATbits.BF == 0);
    while(PIR1bits.SSPIF == 0);
    PIR1bits.SSPIF = 0;
}

unsigned char SPI_Read(void)
{
	while(SSPSTATbits.BF == 0);
    return SSPBUF;
}