#include <xc.h>
#define _XTAL_FREQ 48000000

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "spi.h"


#define TFT_CS_PIN      LATDbits.LD0
#define TFT_RESET_PIN   LATDbits.LD1
#define TFT_DC_PIN      LATDbits.LD2

#define TFT_CS_DIR      TRISDbits.RD0
#define TFT_RESET_DIR   TRISDbits.RD1
#define TFT_DC_DIR      TRISDbits.RD2

#define ST7735_TFTWIDTH_128   128
#define ST7735_TFTWIDTH_80     80
#define ST7735_TFTHEIGHT_128  128
#define ST7735_TFTHEIGHT_160  160

#define ST_CMD_DELAY      0x80

#define ST77XX_NOP        0x00
#define ST77XX_SWRESET    0x01
#define ST77XX_RDDID      0x04
#define ST77XX_RDDST      0x09

#define ST77XX_SLPIN      0x10
#define ST77XX_SLPOUT     0x11
#define ST77XX_PTLON      0x12
#define ST77XX_NORON      0x13

#define ST77XX_INVOFF     0x20
#define ST77XX_INVON      0x21
#define ST77XX_DISPOFF    0x28
#define ST77XX_DISPON     0x29
#define ST77XX_CASET      0x2A
#define ST77XX_RASET      0x2B
#define ST77XX_RAMWR      0x2C
#define ST77XX_RAMRD      0x2E

#define ST77XX_PTLAR      0x30
#define ST77XX_COLMOD     0x3A
#define ST77XX_MADCTL     0x36

#define ST77XX_MADCTL_MY  0x80
#define ST77XX_MADCTL_MX  0x40
#define ST77XX_MADCTL_MV  0x20
#define ST77XX_MADCTL_ML  0x10
#define ST77XX_MADCTL_RGB 0x00

#define ST77XX_RDID1      0xDA
#define ST77XX_RDID2      0xDB
#define ST77XX_RDID3      0xDC
#define ST77XX_RDID4      0xDD

#define INITR_GREENTAB    0x00
#define INITR_REDTAB      0x01
#define INITR_BLACKTAB    0x02
#define INITR_18GREENTAB  INITR_GREENTAB
#define INITR_18REDTAB    INITR_REDTAB
#define INITR_18BLACKTAB  INITR_BLACKTAB
#define INITR_144GREENTAB 0x01
#define INITR_MINI160x80  0x04
#define INITR_HALLOWING   0x05

#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH  0x04

#define ST7735_FRMCTR1    0xB1
#define ST7735_FRMCTR2    0xB2
#define ST7735_FRMCTR3    0xB3
#define ST7735_INVCTR     0xB4
#define ST7735_DISSET5    0xB6

#define ST7735_PWCTR1     0xC0
#define ST7735_PWCTR2     0xC1
#define ST7735_PWCTR3     0xC2
#define ST7735_PWCTR4     0xC3
#define ST7735_PWCTR5     0xC4
#define ST7735_VMCTR1     0xC5

#define ST7735_PWCTR6     0xFC

#define ST7735_GMCTRP1    0xE0
#define ST7735_GMCTRN1    0xE1

#define ST7735_BLACK   0x0000
#define ST7735_BLUE    0x001F
#define ST7735_RED     0xF800
#define ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_TEAL    0x0555
#define ST7735_MAGENTA 0xF81F
#define ST7735_PURPLE  0x8010
#define ST7735_PINK    0xF49A
#define ST7735_YELLOW  0xFFE0
#define ST7735_ORANGE  0xFB40
#define ST7735_WHITE   0xFFFF
#define ST7735_DKGREN  0x0B40
#define ST7735_BROWN   0x82C5
#define ST7735_CRIMSON 0xA004

uint8_t _width;
uint8_t _height;
uint8_t _xstart = 0;
uint8_t _ystart = 0;
uint8_t _colstart = 0;
uint8_t _rowstart = 0;
uint8_t tabcolor;
uint8_t rotation  = 0;

void TFT_InitB(void);
void TFT_ST7735_Init(uint8_t options);
void TFT_DrawPixel(uint8_t x, uint8_t y, uint16_t color);
void TFT_DrawHLine(uint8_t x, uint8_t y, uint8_t w, uint16_t color);
void TFT_DrawVLine(uint8_t x, uint8_t y, uint8_t h, uint16_t color);
void TFT_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
void TFT_FillScreen(uint16_t color);
void TFT_SetRotation(uint8_t m);
void TFT_InvertDisplay(bool i);
void TFT_PushColor(uint16_t color);

void DisplayInit(const uint8_t *addr);
void WriteCommand(uint8_t cmd);
void SetAddrWindow(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void WriteLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void Delay_ms(unsigned int d);

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

int16_t cursor_x = 0, cursor_y = 0;
uint16_t textcolor = 0xFFFF, textbgcolor = 0xFFFF;
uint8_t textsize = 1;
bool wrap = true; 

#define display_width   _width
#define display_height  _height

#define display_drawPixel   TFT_DrawPixel
#define display_drawVLine   TFT_DrawVLine
#define display_drawHLine   TFT_DrawHLine
#define display_fillScreen  TFT_FillScreen
#define display_fillRect    TFT_FillRect
#define display_setRotation TFT_SetRotation
#define display_invert      TFT_InvertDisplay

void TFT_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void TFT_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void TFT_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void TFT_DrawCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint16_t color);
void TFT_FillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void TFT_FillCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint16_t delta, uint16_t color);
void TFT_DrawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void TFT_FillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void TFT_DrawRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint16_t color);
void TFT_FillRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint16_t color);

void TFT_SetTextWrap(bool w);
void TFT_Print(uint8_t c);
void TFT_CustomChar(const uint8_t *c);
void TFT_DrawChar(uint16_t x, uint16_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t size);
void TFT_WriteString(uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t bgcolor, uint8_t size);

uint8_t  TFT_GetRotation();
uint16_t TFT_GetCursorX(void);
uint16_t TFT_GetCursorY(void);
uint16_t TFT_GetWidth();
uint16_t TFT_GetHeight();
uint16_t TFT_Color565(uint8_t red, uint8_t green, uint8_t blue);