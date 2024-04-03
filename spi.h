/* #include "spi.h"
*
* Creada por: Ing. Abiezer Hernandez O.
* Fecha de creacion: 28/11/2019
* Electronica y Circuitos
*
*/


#include <xc.h>
#define _XTAL_FREQ 48000000

#define SPI_CLK_DIV_4  0x00
#define SPI_CLK_DIV_16 0x01
#define SPI_CLK_DIV_64 0x02
#define SPI_TMR2       0x03
#define SPI_SLAVE_PIN  0x04
#define SPI_SLAVE_IO   0x05

#define MODE_1 0x01
#define MODE_2 0x02
#define MODE_3 0x03
#define MODE_4 0x04

#define HALF_DATA 0
#define END_DATA  1

void SPI_Init_Master(char reloj, char modo, char smp);
void SPI_Init_Slave(char reloj, char modo);
void SPI_Write(unsigned char dato);
unsigned char SPI_Read(void);