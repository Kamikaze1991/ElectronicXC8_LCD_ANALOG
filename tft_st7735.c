#include "tft_st7735.h"

const uint8_t
  Bcmd[] = {                        // Init commands for 7735B screens
    18,                             // 18 commands in list:
    ST77XX_SWRESET,   ST_CMD_DELAY, //  1: Software reset, no args, w/delay
      50,                           //     50 ms delay
    ST77XX_SLPOUT,    ST_CMD_DELAY, //  2: Out of sleep mode, no args, w/delay
      255,                          //     255 = max (500 ms) delay
    ST77XX_COLMOD,  1+ST_CMD_DELAY, //  3: Set color mode, 1 arg + delay:
      0x05,                         //     16-bit color
      10,                           //     10 ms delay
    ST7735_FRMCTR1, 3+ST_CMD_DELAY, //  4: Frame rate control, 3 args + delay:
      0x00,                         //     fastest refresh
      0x06,                         //     6 lines front porch
      0x03,                         //     3 lines back porch
      10,                           //     10 ms delay
    ST77XX_MADCTL,  1,              //  5: Mem access ctl (directions), 1 arg:
      0x08,                         //     Row/col addr, bottom-top refresh
    ST7735_DISSET5, 2,              //  6: Display settings #5, 2 args:
      0x15,                         //     1 clk cycle nonoverlap, 2 cycle gate
                                    //     rise, 3 cycle osc equalize
      0x02,                         //     Fix on VTL
    ST7735_INVCTR,  1,              //  7: Display inversion control, 1 arg:
      0x0,                          //     Line inversion
    ST7735_PWCTR1,  2+ST_CMD_DELAY, //  8: Power control, 2 args + delay:
      0x02,                         //     GVDD = 4.7V
      0x70,                         //     1.0uA
      10,                           //     10 ms delay
    ST7735_PWCTR2,  1,              //  9: Power control, 1 arg, no delay:
      0x05,                         //     VGH = 14.7V, VGL = -7.35V
    ST7735_PWCTR3,  2,              // 10: Power control, 2 args, no delay:
      0x01,                         //     Opamp current small
      0x02,                         //     Boost frequency
    ST7735_VMCTR1,  2+ST_CMD_DELAY, // 11: Power control, 2 args + delay:
      0x3C,                         //     VCOMH = 4V
      0x38,                         //     VCOML = -1.1V
      10,                           //     10 ms delay
    ST7735_PWCTR6,  2,              // 12: Power control, 2 args, no delay:
      0x11, 0x15,
    ST7735_GMCTRP1,16,              // 13: Gamma Adjustments (pos. polarity), 16 args + delay:
      0x09, 0x16, 0x09, 0x20,       //     (Not entirely necessary, but provides
      0x21, 0x1B, 0x13, 0x19,       //      accurate colors)
      0x17, 0x15, 0x1E, 0x2B,
      0x04, 0x05, 0x02, 0x0E,
    ST7735_GMCTRN1,16+ST_CMD_DELAY, // 14: Gamma Adjustments (neg. polarity), 16 args + delay:
      0x0B, 0x14, 0x08, 0x1E,       //     (Not entirely necessary, but provides
      0x22, 0x1D, 0x18, 0x1E,       //      accurate colors)
      0x1B, 0x1A, 0x24, 0x2B,
      0x06, 0x06, 0x02, 0x0F,
      10,                           //     10 ms delay
    ST77XX_CASET,   4,              // 15: Column addr set, 4 args, no delay:
      0x00, 0x02,                   //     XSTART = 2
      0x00, 0x81,                   //     XEND = 129
    ST77XX_RASET,   4,              // 16: Row addr set, 4 args, no delay:
      0x00, 0x02,                   //     XSTART = 1
      0x00, 0x81,                   //     XEND = 160
    ST77XX_NORON,     ST_CMD_DELAY, // 17: Normal display on, no args, w/delay
      10,                           //     10 ms delay
    ST77XX_DISPON,    ST_CMD_DELAY, // 18: Main screen turn on, no args, delay
      255 },                        //     255 = max (500 ms) delay                      //     255 = max (500 ms) delay

  Rcmd2[] = {                       // 7735R init, part 1 (red or green tab)
    15,                             // 15 commands in list:
    ST77XX_SWRESET,   ST_CMD_DELAY, //  1: Software reset, 0 args, w/delay
      150,                          //     150 ms delay
    ST77XX_SLPOUT ,   ST_CMD_DELAY, //  2: Out of sleep mode, 0 args, w/delay
      255,                          //     500 ms delay
    ST7735_FRMCTR1, 3,              //  3: Framerate ctrl - normal mode, 3 arg:
      0x01, 0x2C, 0x2D,             //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2, 3,              //  4: Framerate ctrl - idle mode, 3 args:
      0x01, 0x2C, 0x2D,             //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3, 6,              //  5: Framerate - partial mode, 6 args:
      0x01, 0x2C, 0x2D,             //     Dot inversion mode
      0x01, 0x2C, 0x2D,             //     Line inversion mode
    ST7735_INVCTR , 1,              //  6: Display inversion ctrl, 1 arg:
      0x07,                         //     No inversion
    ST7735_PWCTR1 , 3,              //  7: Power control, 3 args, no delay:
      0xA2,
      0x02,                         //     -4.6V
      0x84,                         //     AUTO mode
    ST7735_PWCTR2 , 1,              //  8: Power control, 1 arg, no delay:
      0xC5,                         //     VGH25=2.4C VGSEL=-10 VGH=3 * AVDD
    ST7735_PWCTR3 , 2,              //  9: Power control, 2 args, no delay:
      0x0A,                         //     Opamp current small
      0x00,                         //     Boost frequency
    ST7735_PWCTR4 , 2,              // 10: Power control, 2 args, no delay:
      0x8A,                         //     BCLK/2,
      0x2A,                         //     opamp current small & medium low
    ST7735_PWCTR5 , 2,              // 11: Power control, 2 args, no delay:
      0x8A, 0xEE,
    ST7735_VMCTR1 , 1,              // 12: Power control, 1 arg, no delay:
      0x0E,
    ST77XX_INVOFF , 0,              // 13: Don't invert display, no args
    ST77XX_MADCTL , 1,              // 14: Mem access ctl (directions), 1 arg:
      0xC8,                         //     row/col addr, bottom-top refresh
    ST77XX_COLMOD , 1,              // 15: set color mode, 1 arg, no delay:
      0x05 },                       //     16-bit color
        
        
  Rcmd1[] = {                // Init commands for 7789 screens
    8,                              //  9 commands in list:
    ST77XX_SWRESET,   ST_CMD_DELAY, //  1: Software reset, no args, w/delay
      50,                          //     ~150 ms delay
    ST77XX_SLPOUT ,   ST_CMD_DELAY, //  2: Out of sleep mode, no args, w/delay
      255,                          //      10 ms delay
    ST77XX_COLMOD , 1+ST_CMD_DELAY, //  3: Set color mode, 1 arg + delay:
      0x05,                         //     16-bit color
      10,                           //     10 ms delay
    ST77XX_MADCTL , 1,              //  4: Mem access ctrl (directions), 1 arg:
      0x08,                         //     Row/col addr, bottom-top refresh
    ST77XX_NORON  ,   ST_CMD_DELAY, //  8: Normal display on, no args, w/delay
      10,                           //     10 ms delay
    ST77XX_DISPON ,   ST_CMD_DELAY, //  9: Main screen turn on, no args, delay
      10,
    ST77XX_CASET  , 4,              //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x7F,                   //     XEND = 127
    ST77XX_RASET  , 4,              //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x7F },                 //     XEND = 127//    10 ms delay

  Rcmd2green[] = {                  // 7735R init, part 2 (green tab only)
    2,                              //  2 commands in list:
    ST77XX_CASET  , 4,              //  1: Column addr set, 4 args, no delay:
      0x00, 0x02,                   //     XSTART = 0
      0x00, 0x7F+0x02,              //     XEND = 127
    ST77XX_RASET  , 4,              //  2: Row addr set, 4 args, no delay:
      0x00, 0x01,                   //     XSTART = 0
      0x00, 0x9F+0x01 },            //     XEND = 159

  Rcmd2red[] = {                    // 7735R init, part 2 (red tab only)
    2,                              //  2 commands in list:
    ST77XX_CASET  , 4,              //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x7F,                   //     XEND = 127
    ST77XX_RASET  , 4,              //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x9F },                 //     XEND = 159

  Rcmd2green144[] = {               // 7735R init, part 2 (green 1.44 tab)
    2,                              //  2 commands in list:
    ST77XX_CASET  , 4,              //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x7F,                   //     XEND = 127
    ST77XX_RASET  , 4,              //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x7F },                 //     XEND = 127

  Rcmd2green160x80[] = {            // 7735R init, part 2 (mini 160x80)
    2,                              //  2 commands in list:
    ST77XX_CASET  , 4,              //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x7F,                   //     XEND = 79
    ST77XX_RASET  , 4,              //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x9F },                 //     XEND = 159

  Rcmd3[] = {                       // 7735R init, part 3 (red or green tab)
    4,                              //  4 commands in list:
    ST7735_GMCTRP1, 16      ,       //  1: Gamma setting, I think?
      0x02, 0x1c, 0x07, 0x12,
      0x37, 0x32, 0x29, 0x2d,
      0x29, 0x25, 0x2B, 0x39,
      0x00, 0x01, 0x03, 0x10,
    ST7735_GMCTRN1, 16      ,       //  2: More gamma?
      0x03, 0x1d, 0x07, 0x06,
      0x2E, 0x2C, 0x29, 0x2D,
      0x2E, 0x2E, 0x37, 0x3F,
      0x00, 0x00, 0x02, 0x10,
    ST77XX_NORON  ,   ST_CMD_DELAY, //  3: Normal display on, no args, w/delay
      10,                           //     10 ms delay
    ST77XX_DISPON ,   ST_CMD_DELAY, //  4: Main screen turn on, no args w/delay
      100 };                        //     100 ms delay

#ifndef FONT5X7_H
static const uint8_t font[256][5] = {
   0x00, 0x00, 0x00, 0x00, 0x00,
   0x3E, 0x5B, 0x4F, 0x5B, 0x3E,
   0x3E, 0x6B, 0x4F, 0x6B, 0x3E,
   0x1C, 0x3E, 0x7C, 0x3E, 0x1C,
   0x18, 0x3C, 0x7E, 0x3C, 0x18,
   0x1C, 0x57, 0x7D, 0x57, 0x1C,
   0x1C, 0x5E, 0x7F, 0x5E, 0x1C,
   0x00, 0x18, 0x3C, 0x18, 0x00,
   0xFF, 0xE7, 0xC3, 0xE7, 0xFF,
   0x00, 0x18, 0x24, 0x18, 0x00,
   0xFF, 0xE7, 0xDB, 0xE7, 0xFF,
   0x30, 0x48, 0x3A, 0x06, 0x0E,
   0x26, 0x29, 0x79, 0x29, 0x26,
   0x40, 0x7F, 0x05, 0x05, 0x07,
   0x40, 0x7F, 0x05, 0x25, 0x3F,
   0x5A, 0x3C, 0xE7, 0x3C, 0x5A,
   0x7F, 0x3E, 0x1C, 0x1C, 0x08,
   0x08, 0x1C, 0x1C, 0x3E, 0x7F,
   0x14, 0x22, 0x7F, 0x22, 0x14,
   0x5F, 0x5F, 0x00, 0x5F, 0x5F,
   0x06, 0x09, 0x7F, 0x01, 0x7F,
   0x00, 0x66, 0x89, 0x95, 0x6A,
   0x60, 0x60, 0x60, 0x60, 0x60,
   0x94, 0xA2, 0xFF, 0xA2, 0x94,
   0x08, 0x04, 0x7E, 0x04, 0x08,
   0x10, 0x20, 0x7E, 0x20, 0x10,
   0x08, 0x08, 0x2A, 0x1C, 0x08,
   0x08, 0x1C, 0x2A, 0x08, 0x08,
   0x1E, 0x10, 0x10, 0x10, 0x10,
   0x0C, 0x1E, 0x0C, 0x1E, 0x0C,
   0x30, 0x38, 0x3E, 0x38, 0x30,
   0x06, 0x0E, 0x3E, 0x0E, 0x06,
   0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x5F, 0x00, 0x00,
   0x00, 0x07, 0x00, 0x07, 0x00,
   0x14, 0x7F, 0x14, 0x7F, 0x14,
   0x24, 0x2A, 0x7F, 0x2A, 0x12,
   0x23, 0x13, 0x08, 0x64, 0x62,
   0x36, 0x49, 0x56, 0x20, 0x50,
   0x00, 0x08, 0x07, 0x03, 0x00,
   0x00, 0x1C, 0x22, 0x41, 0x00,
   0x00, 0x41, 0x22, 0x1C, 0x00,
   0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
   0x08, 0x08, 0x3E, 0x08, 0x08,
   0x00, 0x80, 0x70, 0x30, 0x00,
   0x08, 0x08, 0x08, 0x08, 0x08,
   0x00, 0x00, 0x60, 0x60, 0x00,
   0x20, 0x10, 0x08, 0x04, 0x02,
   0x3E, 0x51, 0x49, 0x45, 0x3E,
   0x00, 0x42, 0x7F, 0x40, 0x00,
   0x72, 0x49, 0x49, 0x49, 0x46,
   0x21, 0x41, 0x49, 0x4D, 0x33,
   0x18, 0x14, 0x12, 0x7F, 0x10,
   0x27, 0x45, 0x45, 0x45, 0x39,
   0x3C, 0x4A, 0x49, 0x49, 0x31,
   0x41, 0x21, 0x11, 0x09, 0x07,
   0x36, 0x49, 0x49, 0x49, 0x36,
   0x46, 0x49, 0x49, 0x29, 0x1E,
   0x00, 0x00, 0x14, 0x00, 0x00,
   0x00, 0x40, 0x34, 0x00, 0x00,
   0x00, 0x08, 0x14, 0x22, 0x41,
   0x14, 0x14, 0x14, 0x14, 0x14,
   0x00, 0x41, 0x22, 0x14, 0x08,
   0x02, 0x01, 0x59, 0x09, 0x06,
   0x3E, 0x41, 0x5D, 0x59, 0x4E,
   0x7C, 0x12, 0x11, 0x12, 0x7C,
   0x7F, 0x49, 0x49, 0x49, 0x36,
   0x3E, 0x41, 0x41, 0x41, 0x22,
   0x7F, 0x41, 0x41, 0x41, 0x3E,
   0x7F, 0x49, 0x49, 0x49, 0x41,
   0x7F, 0x09, 0x09, 0x09, 0x01,
   0x3E, 0x41, 0x41, 0x51, 0x73,
   0x7F, 0x08, 0x08, 0x08, 0x7F,
   0x00, 0x41, 0x7F, 0x41, 0x00,
   0x20, 0x40, 0x41, 0x3F, 0x01,
   0x7F, 0x08, 0x14, 0x22, 0x41,
   0x7F, 0x40, 0x40, 0x40, 0x40,
   0x7F, 0x02, 0x1C, 0x02, 0x7F,
   0x7F, 0x04, 0x08, 0x10, 0x7F,
   0x3E, 0x41, 0x41, 0x41, 0x3E,
   0x7F, 0x09, 0x09, 0x09, 0x06,
   0x3E, 0x41, 0x51, 0x21, 0x5E,
   0x7F, 0x09, 0x19, 0x29, 0x46,
   0x26, 0x49, 0x49, 0x49, 0x32,
   0x03, 0x01, 0x7F, 0x01, 0x03,
   0x3F, 0x40, 0x40, 0x40, 0x3F,
   0x1F, 0x20, 0x40, 0x20, 0x1F,
   0x3F, 0x40, 0x38, 0x40, 0x3F,
   0x63, 0x14, 0x08, 0x14, 0x63,
   0x03, 0x04, 0x78, 0x04, 0x03,
   0x61, 0x59, 0x49, 0x4D, 0x43,
   0x00, 0x7F, 0x41, 0x41, 0x41,
   0x02, 0x04, 0x08, 0x10, 0x20,
   0x00, 0x41, 0x41, 0x41, 0x7F,
   0x04, 0x02, 0x01, 0x02, 0x04,
   0x40, 0x40, 0x40, 0x40, 0x40,
   0x00, 0x03, 0x07, 0x08, 0x00,
   0x20, 0x54, 0x54, 0x78, 0x40,
   0x7F, 0x28, 0x44, 0x44, 0x38,
   0x38, 0x44, 0x44, 0x44, 0x28,
   0x38, 0x44, 0x44, 0x28, 0x7F,
   0x38, 0x54, 0x54, 0x54, 0x18,
   0x00, 0x08, 0x7E, 0x09, 0x02,
   0x18, 0xA4, 0xA4, 0x9C, 0x78,
   0x7F, 0x08, 0x04, 0x04, 0x78,
   0x00, 0x44, 0x7D, 0x40, 0x00,
   0x20, 0x40, 0x40, 0x3D, 0x00,
   0x7F, 0x10, 0x28, 0x44, 0x00,
   0x00, 0x41, 0x7F, 0x40, 0x00,
   0x7C, 0x04, 0x78, 0x04, 0x78,
   0x7C, 0x08, 0x04, 0x04, 0x78,
   0x38, 0x44, 0x44, 0x44, 0x38,
   0xFC, 0x18, 0x24, 0x24, 0x18,
   0x18, 0x24, 0x24, 0x18, 0xFC,
   0x7C, 0x08, 0x04, 0x04, 0x08,
   0x48, 0x54, 0x54, 0x54, 0x24,
   0x04, 0x04, 0x3F, 0x44, 0x24,
   0x3C, 0x40, 0x40, 0x20, 0x7C,
   0x1C, 0x20, 0x40, 0x20, 0x1C,
   0x3C, 0x40, 0x30, 0x40, 0x3C,
   0x44, 0x28, 0x10, 0x28, 0x44,
   0x4C, 0x90, 0x90, 0x90, 0x7C,
   0x44, 0x64, 0x54, 0x4C, 0x44,
   0x00, 0x08, 0x36, 0x41, 0x00,
   0x00, 0x00, 0x77, 0x00, 0x00,
   0x00, 0x41, 0x36, 0x08, 0x00,
   0x02, 0x01, 0x02, 0x04, 0x02,
   0x3C, 0x26, 0x23, 0x26, 0x3C,
   0x1E, 0xA1, 0xA1, 0x61, 0x12,
   0x3A, 0x40, 0x40, 0x20, 0x7A,
   0x38, 0x54, 0x54, 0x55, 0x59,
   0x21, 0x55, 0x55, 0x79, 0x41,
   0x22, 0x54, 0x54, 0x78, 0x42, // a-umlaut
   0x21, 0x55, 0x54, 0x78, 0x40,
   0x20, 0x54, 0x55, 0x79, 0x40,
   0x0C, 0x1E, 0x52, 0x72, 0x12,
   0x39, 0x55, 0x55, 0x55, 0x59,
   0x39, 0x54, 0x54, 0x54, 0x59,
   0x39, 0x55, 0x54, 0x54, 0x58,
   0x00, 0x00, 0x45, 0x7C, 0x41,
   0x00, 0x02, 0x45, 0x7D, 0x42,
   0x00, 0x01, 0x45, 0x7C, 0x40,
   0x7D, 0x12, 0x11, 0x12, 0x7D, // A-umlaut
   0xF0, 0x28, 0x25, 0x28, 0xF0,
   0x7C, 0x54, 0x55, 0x45, 0x00,
   0x20, 0x54, 0x54, 0x7C, 0x54,
   0x7C, 0x0A, 0x09, 0x7F, 0x49,
   0x32, 0x49, 0x49, 0x49, 0x32,
   0x3A, 0x44, 0x44, 0x44, 0x3A, // o-umlaut
   0x32, 0x4A, 0x48, 0x48, 0x30,
   0x3A, 0x41, 0x41, 0x21, 0x7A,
   0x3A, 0x42, 0x40, 0x20, 0x78,
   0x00, 0x9D, 0xA0, 0xA0, 0x7D,
   0x3D, 0x42, 0x42, 0x42, 0x3D, // O-umlaut
   0x3D, 0x40, 0x40, 0x40, 0x3D,
   0x3C, 0x24, 0xFF, 0x24, 0x24,
   0x48, 0x7E, 0x49, 0x43, 0x66,
   0x2B, 0x2F, 0xFC, 0x2F, 0x2B,
   0xFF, 0x09, 0x29, 0xF6, 0x20,
   0xC0, 0x88, 0x7E, 0x09, 0x03,
   0x20, 0x54, 0x54, 0x79, 0x41,
   0x00, 0x00, 0x44, 0x7D, 0x41,
   0x30, 0x48, 0x48, 0x4A, 0x32,
   0x38, 0x40, 0x40, 0x22, 0x7A,
   0x00, 0x7A, 0x0A, 0x0A, 0x72,
   0x7D, 0x0D, 0x19, 0x31, 0x7D,
   0x26, 0x29, 0x29, 0x2F, 0x28,
   0x26, 0x29, 0x29, 0x29, 0x26,
   0x30, 0x48, 0x4D, 0x40, 0x20,
   0x38, 0x08, 0x08, 0x08, 0x08,
   0x08, 0x08, 0x08, 0x08, 0x38,
   0x2F, 0x10, 0xC8, 0xAC, 0xBA,
   0x2F, 0x10, 0x28, 0x34, 0xFA,
   0x00, 0x00, 0x7B, 0x00, 0x00,
   0x08, 0x14, 0x2A, 0x14, 0x22,
   0x22, 0x14, 0x2A, 0x14, 0x08,
   0x55, 0x00, 0x55, 0x00, 0x55, // #176 (25% block) missing in old code
   0xAA, 0x55, 0xAA, 0x55, 0xAA, // 50% block
   0xFF, 0x55, 0xFF, 0x55, 0xFF, // 75% block
   0x00, 0x00, 0x00, 0xFF, 0x00,
   0x10, 0x10, 0x10, 0xFF, 0x00,
   0x14, 0x14, 0x14, 0xFF, 0x00,
   0x10, 0x10, 0xFF, 0x00, 0xFF,
   0x10, 0x10, 0xF0, 0x10, 0xF0,
   0x14, 0x14, 0x14, 0xFC, 0x00,
   0x14, 0x14, 0xF7, 0x00, 0xFF,
   0x00, 0x00, 0xFF, 0x00, 0xFF,
   0x14, 0x14, 0xF4, 0x04, 0xFC,
   0x14, 0x14, 0x17, 0x10, 0x1F,
   0x10, 0x10, 0x1F, 0x10, 0x1F,
   0x14, 0x14, 0x14, 0x1F, 0x00,
   0x10, 0x10, 0x10, 0xF0, 0x00,
   0x00, 0x00, 0x00, 0x1F, 0x10,
   0x10, 0x10, 0x10, 0x1F, 0x10,
   0x10, 0x10, 0x10, 0xF0, 0x10,
   0x00, 0x00, 0x00, 0xFF, 0x10,
   0x10, 0x10, 0x10, 0x10, 0x10,
   0x10, 0x10, 0x10, 0xFF, 0x10,
   0x00, 0x00, 0x00, 0xFF, 0x14,
   0x00, 0x00, 0xFF, 0x00, 0xFF,
   0x00, 0x00, 0x1F, 0x10, 0x17,
   0x00, 0x00, 0xFC, 0x04, 0xF4,
   0x14, 0x14, 0x17, 0x10, 0x17,
   0x14, 0x14, 0xF4, 0x04, 0xF4,
   0x00, 0x00, 0xFF, 0x00, 0xF7,
   0x14, 0x14, 0x14, 0x14, 0x14,
   0x14, 0x14, 0xF7, 0x00, 0xF7,
   0x14, 0x14, 0x14, 0x17, 0x14,
   0x10, 0x10, 0x1F, 0x10, 0x1F,
   0x14, 0x14, 0x14, 0xF4, 0x14,
   0x10, 0x10, 0xF0, 0x10, 0xF0,
   0x00, 0x00, 0x1F, 0x10, 0x1F,
   0x00, 0x00, 0x00, 0x1F, 0x14,
   0x00, 0x00, 0x00, 0xFC, 0x14,
   0x00, 0x00, 0xF0, 0x10, 0xF0,
   0x10, 0x10, 0xFF, 0x10, 0xFF,
   0x14, 0x14, 0x14, 0xFF, 0x14,
   0x10, 0x10, 0x10, 0x1F, 0x00,
   0x00, 0x00, 0x00, 0xF0, 0x10,
   0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
   0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
   0xFF, 0xFF, 0xFF, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xFF, 0xFF,
   0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
   0x38, 0x44, 0x44, 0x38, 0x44,
   0xFC, 0x4A, 0x4A, 0x4A, 0x34, // sharp-s or beta
   0x7E, 0x02, 0x02, 0x06, 0x06,
   0x02, 0x7E, 0x02, 0x7E, 0x02,
   0x63, 0x55, 0x49, 0x41, 0x63,
   0x38, 0x44, 0x44, 0x3C, 0x04,
   0x40, 0x7E, 0x20, 0x1E, 0x20,
   0x06, 0x02, 0x7E, 0x02, 0x02,
   0x99, 0xA5, 0xE7, 0xA5, 0x99,
   0x1C, 0x2A, 0x49, 0x2A, 0x1C,
   0x4C, 0x72, 0x01, 0x72, 0x4C,
   0x30, 0x4A, 0x4D, 0x4D, 0x30,
   0x30, 0x48, 0x78, 0x48, 0x30,
   0xBC, 0x62, 0x5A, 0x46, 0x3D,
   0x3E, 0x49, 0x49, 0x49, 0x00,
   0x7E, 0x01, 0x01, 0x01, 0x7E,
   0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
   0x44, 0x44, 0x5F, 0x44, 0x44,
   0x40, 0x51, 0x4A, 0x44, 0x40,
   0x40, 0x44, 0x4A, 0x51, 0x40,
   0x00, 0x00, 0xFF, 0x01, 0x03,
   0xE0, 0x80, 0xFF, 0x00, 0x00,
   0x08, 0x08, 0x6B, 0x6B, 0x08,
   0x36, 0x12, 0x36, 0x24, 0x36,
   0x06, 0x0F, 0x09, 0x0F, 0x06,
   0x00, 0x00, 0x18, 0x18, 0x00,
   0x00, 0x00, 0x10, 0x10, 0x00,
   0x30, 0x40, 0xFF, 0x01, 0x01,
   0x00, 0x1F, 0x01, 0x01, 0x1E,
   0x00, 0x19, 0x1D, 0x17, 0x12,
   0x00, 0x3C, 0x3C, 0x3C, 0x3C,
   0x00, 0x00, 0x00, 0x00, 0x00  // #255 NBSP
};
#endif

void WriteCommand(uint8_t cmd)
{
    TFT_DC_PIN = 0;
    SPI_Write(cmd);
    TFT_DC_PIN = 1;
}

void DisplayInit(const uint8_t *addr)
{
    uint8_t  numCommands, numArgs;
    uint16_t ms;
    
    TFT_CS_PIN = 0;
    numCommands = *addr++;
    while(numCommands--)
    {
        WriteCommand(*addr++);
        numArgs = *addr++;
        ms = numArgs & ST_CMD_DELAY;
        numArgs &= ~ST_CMD_DELAY;
        while(numArgs--)
        {
            SPI_Write(*addr++);
        }
        if(ms)
        {
            ms = *addr++;
            if(ms == 255) ms = 500;
            Delay_ms(ms);
        }
    }
    TFT_CS_PIN = 1;
}

void TFT_InitB(void)
{
    TFT_CS_DIR = 0;
    TFT_DC_DIR = 0;
    TFT_RESET_DIR = 0;
    TFT_RESET_PIN = 0;
    __delay_ms(100);
    TFT_RESET_PIN = 1;
    TFT_CS_PIN = 1;
    TFT_DC_PIN = 0;
    SPI_Init_Master(SPI_CLK_DIV_4, MODE_2, END_DATA);
    DisplayInit(Bcmd);
}


void TFT_ST7735_Init(uint8_t options) 
{
    TFT_CS_DIR = 0;
    TFT_DC_DIR = 0;
    TFT_RESET_DIR = 0;
    TFT_RESET_PIN = 0;
    __delay_ms(10);
    TFT_RESET_PIN = 1;
    TFT_CS_PIN = 1;
    TFT_DC_PIN = 0;
    SPI_Init_Master(SPI_CLK_DIV_4, MODE_2, END_DATA);
    DisplayInit(Rcmd1);
    _height   = ST7735_TFTHEIGHT_128;
    _width    = ST7735_TFTWIDTH_128;
    _colstart = 0;
    _rowstart = 0;
   
    
    
    if((options == INITR_BLACKTAB) || (options == INITR_MINI160x80)){
        TFT_CS_PIN = 0;
        WriteCommand(ST77XX_MADCTL);
        SPI_Write(0xC0);
        TFT_CS_PIN = 1;
    }
    if(options == INITR_HALLOWING){
        tabcolor = INITR_144GREENTAB;
        TFT_SetRotation(2);
    }
    else{
        tabcolor = options;
        TFT_SetRotation(0);
    }
}

void TFT_DrawPixel(uint8_t x, uint8_t y, uint16_t color)
{
    if((x < _width) && (y < _height))
    {
        TFT_CS_PIN = 0;
        SetAddrWindow(x, y, 1, 1);
        SPI_Write(color >> 8);
        SPI_Write(color & 0xFF);
        TFT_CS_PIN = 1;
    }
}

void SetAddrWindow(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    x += _xstart;
    y += _ystart;
    WriteCommand(ST77XX_CASET);
    SPI_Write(0);
    SPI_Write(x);
    SPI_Write(0);
    SPI_Write(x+w-1);
    WriteCommand(ST77XX_RASET);
    SPI_Write(0);
    SPI_Write(y);
    SPI_Write(0);
    SPI_Write(y+h-1);
    WriteCommand(ST77XX_RAMWR);
}

void TFT_SetRotation(uint8_t m)
{
    uint8_t madctl = 0;
    rotation = m & 3;
    
    switch(rotation)
    {
        case 0:
            if((tabcolor == INITR_BLACKTAB) || (tabcolor == INITR_MINI160x80)){
                madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST77XX_MADCTL_RGB;
            }else{
                madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST7735_MADCTL_BGR;
            }

            if(tabcolor == INITR_144GREENTAB){
                _height = ST7735_TFTHEIGHT_128;
                _width  = ST7735_TFTWIDTH_128;
            }else if (tabcolor == INITR_MINI160x80){
                _height = ST7735_TFTHEIGHT_160;
                _width  = ST7735_TFTWIDTH_80;
            }else{
                _height = ST7735_TFTHEIGHT_160;
                _width  = ST7735_TFTWIDTH_128;
            }
            _xstart = _colstart;
            _ystart = _rowstart;
            break;
            
        case 1:
            if((tabcolor == INITR_BLACKTAB) || (tabcolor == INITR_MINI160x80)){
                madctl = ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
            }else{
                madctl = ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST7735_MADCTL_BGR;
            }

            if(tabcolor == INITR_144GREENTAB){
                _width  = ST7735_TFTHEIGHT_128;
                _height = ST7735_TFTWIDTH_128;
            }else if (tabcolor == INITR_MINI160x80){
                _width  = ST7735_TFTHEIGHT_160;
                _height = ST7735_TFTWIDTH_80;
            }else {
                _width  = ST7735_TFTHEIGHT_160;
                _height = ST7735_TFTWIDTH_128;
            }
            _ystart = _colstart;
            _xstart = _rowstart;
            break;
            
        case 2:
            if((tabcolor == INITR_BLACKTAB) || (tabcolor == INITR_MINI160x80)){
                madctl = ST77XX_MADCTL_RGB;
            }else{
                madctl = ST7735_MADCTL_BGR;
            }

            if(tabcolor == INITR_144GREENTAB){
                _height = ST7735_TFTHEIGHT_128;
                _width  = ST7735_TFTWIDTH_128;
            }else if (tabcolor == INITR_MINI160x80){
                _height = ST7735_TFTHEIGHT_160;
                _width  = ST7735_TFTWIDTH_80;
            }else{
                _height = ST7735_TFTHEIGHT_160;
                _width  = ST7735_TFTWIDTH_128;
            }
            _xstart   = _colstart;
            _ystart   = _rowstart;
            break;
            
        case 3:
            if((tabcolor == INITR_BLACKTAB) || (tabcolor == INITR_MINI160x80)){
                madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
            }else{
                madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST7735_MADCTL_BGR;
            }

            if(tabcolor == INITR_144GREENTAB){
                _width  = ST7735_TFTHEIGHT_128;
                _height = ST7735_TFTWIDTH_128;
            }else if(tabcolor == INITR_MINI160x80){
                _width  = ST7735_TFTHEIGHT_160;
                _height = ST7735_TFTWIDTH_80;
            }else{
                _width  = ST7735_TFTHEIGHT_160;
                _height = ST7735_TFTWIDTH_128;
            }
            _ystart = _colstart;
            _xstart = _rowstart;
            break;
    }
    TFT_CS_PIN = 0;
    WriteCommand(ST77XX_MADCTL);
    SPI_Write(madctl);
    TFT_CS_PIN = 1;
}

void TFT_DrawHLine(uint8_t x, uint8_t y, uint8_t w, uint16_t color)
{
    if((x < _width) && (y < _height) && w)
    {
        uint8_t hi = color >> 8, lo = color;
        if((x >= _width) || (y >= _height))
            return;
        if((x + w - 1) >= _width)  
            w = _width  - x;
        TFT_CS_PIN = 0;
        SetAddrWindow(x, y, w, 1);
        while(w--){
            SPI_Write(hi);
            SPI_Write(lo);
        }
        TFT_CS_PIN = 1;
    }
}

void TFT_DrawVLine(uint8_t x, uint8_t y, uint8_t h, uint16_t color)
{
    if((x < _width) && (y < _height) && h)
    {  
        uint8_t hi = color >> 8, lo = color;
        if((y + h - 1) >= _height) 
            h = _height - y;
        TFT_CS_PIN = 0;
        SetAddrWindow(x, y, 1, h);
        while(h--){
            SPI_Write(hi);
            SPI_Write(lo);
        }
        TFT_CS_PIN = 1;
    }
}

void TFT_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color)
{
    if(w && h)
    {
        uint8_t hi = color >> 8, lo = color;
        if((x >= _width) || (y >= _height))
            return;
        if((x + w - 1) >= _width)  
            w = _width  - x;
        if((y + h - 1) >= _height) 
            h = _height - y;
        TFT_CS_PIN = 0;
        SetAddrWindow(x, y, w, h);
        uint16_t px = (uint16_t)w * h;
        while(px--){
            SPI_Write(hi);
            SPI_Write(lo);
        }
        TFT_CS_PIN = 1;
    }
}

void TFT_FillScreen(uint16_t color)
{
    TFT_FillRect(0, 0, _width, _height, color);
}

void TFT_InvertDisplay(bool i)
{
    TFT_CS_PIN = 0;
    WriteCommand(i ? ST77XX_INVON : ST77XX_INVOFF);
    TFT_CS_PIN = 1;
}

void TFT_PushColor(uint16_t color)
{
    uint8_t hi = color >> 8, lo = color;
    TFT_CS_PIN = 0;
    SPI_Write(hi);
    SPI_Write(lo);
    TFT_CS_PIN = 1;
}

void Delay_ms(unsigned int d)
{
    for(unsigned int i=0; i<d; i++)
    {
        __delay_ms(1);
    }
}

void WriteLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    bool steep = abs((int16_t)(y1 - y0)) > abs((int16_t)(x1 - x0));
    if(steep){
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }
    if(x0 > x1){
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }
    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs((int16_t)(y1 - y0));
    int16_t err = dx / 2;
    int16_t ystep;
    if(y0 < y1){
        ystep = 1;
    }else {
        ystep = -1;
    }
    for(; x0<=x1; x0++)
    {
        if(steep){
            display_drawPixel(y0, x0, color);
        }else{
            display_drawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0){
            y0 += ystep;
            err += dx;
        }
    }
}

void TFT_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    if(x0 == x1){
        if(y0 > y1) _swap_int16_t(y0, y1);
        display_drawVLine(x0, y0, y1 - y0 + 1, color);
    }else if(y0 == y1){
        if(x0 > x1) _swap_int16_t(x0, x1);
        display_drawHLine(x0, y0, x1 - x0 + 1, color);
    }else{
        WriteLine(x0, y0, x1, y1, color);
    }
}

void TFT_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    display_drawPixel(x0  , y0+r, color);
    display_drawPixel(x0  , y0-r, color);
    display_drawPixel(x0+r, y0  , color);
    display_drawPixel(x0-r, y0  , color);
    
    while(x<y){
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        display_drawPixel(x0 + x, y0 + y, color);
        display_drawPixel(x0 - x, y0 + y, color);
        display_drawPixel(x0 + x, y0 - y, color);
        display_drawPixel(x0 - x, y0 - y, color);
        display_drawPixel(x0 + y, y0 + x, color);
        display_drawPixel(x0 - y, y0 + x, color);
        display_drawPixel(x0 + y, y0 - x, color);
        display_drawPixel(x0 - y, y0 - x, color);
    }
}

void TFT_DrawCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        if(cornername & 0x4) {
            display_drawPixel(x0 + x, y0 + y, color);
            display_drawPixel(x0 + y, y0 + x, color);
        }
        if(cornername & 0x2) {
            display_drawPixel(x0 + x, y0 - y, color);
            display_drawPixel(x0 + y, y0 - x, color);
        }
        if(cornername & 0x8) {
            display_drawPixel(x0 - y, y0 + x, color);
            display_drawPixel(x0 - x, y0 + y, color);
        }
        if(cornername & 0x1) {
            display_drawPixel(x0 - y, y0 - x, color);
            display_drawPixel(x0 - x, y0 - y, color);
        }
    }
}

void TFT_FillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
    display_drawVLine(x0, y0-r, 2*r+1, color);
    TFT_FillCircleHelper(x0, y0, r, 3, 0, color);
}

void TFT_FillCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t corners, uint16_t delta, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    int16_t px    = x;
    int16_t py    = y;
    delta++;
    while(x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        if(x < (y + 1)){
            if(corners & 1) display_drawVLine(x0+x, y0-y, 2*y+delta, color);
            if(corners & 2) display_drawVLine(x0-x, y0-y, 2*y+delta, color);
        }
        if(y != py){
            if(corners & 1) display_drawVLine(x0+py, y0-px, 2*px+delta, color);
            if(corners & 2) display_drawVLine(x0-py, y0-px, 2*px+delta, color);
            py = y;
        }
        px = x;
    }
}

void TFT_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    display_drawHLine(x, y, w, color);
    display_drawHLine(x, y+h-1, w, color);
    display_drawVLine(x, y, h, color);
    display_drawVLine(x+w-1, y, h, color);
}

void TFT_DrawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color)
{
    int16_t max_radius = ((w < h) ? w : h) / 2;
    if(r > max_radius) r = max_radius;
    display_drawHLine(x+r, y, w-2*r, color);
    display_drawHLine(x+r, y+h-1, w-2*r, color);
    display_drawVLine(x, y+r, h-2*r, color);
    display_drawVLine(x+w-1, y+r, h-2*r, color);
    TFT_DrawCircleHelper(x+r, y+r, r, 1, color);
    TFT_DrawCircleHelper(x+w-r-1, y+r, r, 2, color);
    TFT_DrawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
    TFT_DrawCircleHelper(x+r, y+h-r-1, r, 8, color);
}

void TFT_FillRoundRect(uint16_t x, uint16_t y, uint16_t w,uint16_t h, uint16_t r, uint16_t color)
{
    int16_t max_radius = ((w < h) ? w : h) / 2;
    if(r > max_radius) r = max_radius;
    TFT_FillRect(x+r, y, w-2*r, h, color);
    TFT_FillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
    //display_fillCircleHelper(94, 18, 8, 1, 43, color);
    TFT_FillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}

void TFT_DrawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2, uint16_t color)
{
    TFT_DrawLine(x0, y0, x1, y1, color);
    TFT_DrawLine(x1, y1, x2, y2, color);
    TFT_DrawLine(x2, y2, x0, y0, color);
}

void TFT_FillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    int16_t a, b, y, last;
    if(y0 > y1){
        _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
    }
    if(y1 > y2){
        _swap_int16_t(y2, y1); _swap_int16_t(x2, x1);
    }
    if(y0 > y1){
        _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
    }
    if(y0 == y2){
        a = b = x0;
        if(x1 < a) a = x1;
        else if(x1 > b) b = x1;
        if(x2 < a) a = x2;
        else if(x2 > b) b = x2;
        display_drawHLine(a, y0, b-a+1, color);
        return;
    }
    int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1;
    int32_t
    sa = 0,
    sb = 0;
    
    if(y1 == y2) last = y1;
    else last = y1-1;

    for(y=y0; y<=last; y++) {
        a   = x0 + sa / dy01;
        b   = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if(a > b) _swap_int16_t(a,b);
        display_drawHLine(a, y, b-a+1, color);
    }
    
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for(; y<=y2; y++) {
        a   = x1 + sa / dy12;
        b   = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if(a > b) _swap_int16_t(a,b);
        display_drawHLine(a, y, b-a+1, color);
    }
}

void TFT_Print(uint8_t c)
{
    if(c == ' ' && cursor_x == 0 && wrap)
        return;
    for(uint8_t i = 0; i < 5; i++ ){
        uint8_t line = font[c][i];
        for(uint8_t j = 0; j < 8; j++, line >>= 1){
            if(line & 1) {
                if(textsize == 1)
                    display_drawPixel(cursor_x + i, cursor_y + j, textcolor);
                else
                    display_fillRect(cursor_x + i * textsize, cursor_y + j * textsize, textsize, textsize, textcolor);
            }
            else{
                if(textbgcolor != textcolor) {
                    if(textsize == 1)
                        display_drawPixel(cursor_x + i, cursor_y + j, textbgcolor);
                    else
                        display_fillRect(cursor_x + i * textsize, cursor_y + j * textsize, textsize, textsize, textbgcolor);
                }
            }
        }
    }
    if(textbgcolor != textcolor){
        if(textsize == 1) display_drawVLine(cursor_x + 5, cursor_y, 8, textbgcolor);
        else display_fillRect(cursor_x + 5 * textsize, cursor_y, textsize, 8 * textsize, textbgcolor);
    }
    cursor_x += textsize * 6;
    if(cursor_x > ((uint16_t)display_width + textsize * 6))
        cursor_x = display_width;
    if(wrap && (cursor_x + (textsize * 5)) > display_width)
    {
        cursor_x = 0;
        cursor_y += textsize * 8;
        if(cursor_y > ((uint16_t)display_height + textsize * 8))
            cursor_y = display_height;
    }
}

void TFT_CustomChar(const uint8_t *c)
{
    for(uint8_t i = 0; i < 5; i++ ){
        uint8_t line = c[i];
        for(uint8_t j = 0; j < 8; j++, line >>= 1){
            if(line & 1){
                if(textsize == 1)
                    display_drawPixel(cursor_x + i, cursor_y + j, textcolor);
                else
                    display_fillRect(cursor_x + i * textsize, cursor_y + j * textsize, textsize, textsize, textcolor);
            } 
            else{
                if(textbgcolor != textcolor) {
                    if(textsize == 1)
                        display_drawPixel(cursor_x + i, cursor_y + j, textbgcolor);
                    else
                        display_fillRect(cursor_x + i * textsize, cursor_y + j * textsize, textsize, textsize, textbgcolor);
                }
            }
        }
    }
    
    if(textbgcolor != textcolor){
        if(textsize == 1) display_drawVLine(cursor_x + 5, cursor_y, 8, textbgcolor);
        else display_fillRect(cursor_x + 5 * textsize, cursor_y, textsize, 8 * textsize, textbgcolor);
    }
    cursor_x += textsize * 6;
    
    if(cursor_x > ((uint16_t)display_width + textsize * 6))
        cursor_x = display_width;
    if(wrap && (cursor_x + (textsize * 5)) > display_width)
    {
        cursor_x = 0;
        cursor_y += textsize * 8;
        if(cursor_y > ((uint16_t)display_height + textsize * 8))
        cursor_y = display_height;
    }
}

void TFT_DrawChar(uint16_t x, uint16_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t size)
{
    uint16_t prev_x = cursor_x,
             prev_y = cursor_y,
             prev_color = textcolor,
             prev_bg = textbgcolor;
    uint8_t  prev_size = textsize;

    cursor_x = x;
    cursor_y = y;
    textsize = (size > 0) ? size : 1;
    textcolor = color;
    textbgcolor = bg;
    TFT_Print(c);
    cursor_x = prev_x;
    cursor_y  = prev_y;
    textcolor = prev_color;
    textbgcolor = prev_bg;
    textsize = prev_size;
}

void TFT_WriteString(uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t bg, uint8_t size)
{
    uint8_t x_prev = x;
    textsize = (size > 0) ? size : 1;
    textcolor = color;
    textbgcolor = bg;
    while(*str != '\0')
    {
        switch(textsize)
        {
            case 1:
                cursor_x = x_prev;
                cursor_y = y;
                TFT_Print(*str++);
                x_prev = x_prev + 6;
                break;
            case 2:
                cursor_x = x_prev;
                cursor_y = y;
                TFT_Print(*str++);
                x_prev = x_prev + 12;
                break;
                
            case 3:
                cursor_x = x_prev;
                cursor_y = y;
                TFT_Print(*str++);
                x_prev = x_prev + 18;
                break;
                
            case 4:
                cursor_x = x_prev;
                cursor_y = y;
                TFT_Print(*str++);
                x_prev = x_prev + 24;
                break;
        }
    }
}

uint16_t TFT_GetCursorX(void)
{
    return cursor_x;
}

uint16_t TFT_GetCursorY(void)
{
    return cursor_y;
}

void TFT_SetTextWrap(bool w)
{
    wrap = w;
}

uint8_t TFT_GetRotation(void)
{
    return rotation;
}

uint16_t TFT_GetWidth(void)
{
    return display_width;
}

uint16_t TFT_GetHeight(void)
{
    return display_height;
}

uint16_t TFT_Color565(uint8_t red, uint8_t green, uint8_t blue)
{
    return ((uint16_t)(red & 0xF8) << 8) | ((uint16_t)(green & 0xFC) << 3) | (blue >> 3);
}