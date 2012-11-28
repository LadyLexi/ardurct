/*
 * Sparkfun - Tests the macros (see TouchScreen_Macros.cpp for language)
 *
 * Copyright (c) 2010-2012 Laurent Wibaux <lm.wibaux@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
// Change the version to adapt to your screen
#define TOUSCRUINO_VERSION 1

#if (TOUSCRUINO_VERSION == 1)

#define TFT_CS 10
#define TFT_CD 9
#define TFT_RST 8

#include <SPI.h>
#include <ArduRCT_TouScruinoV1.h>
#include <SPI.h>

ArduRCT_TouScruinoV1 tft(TFT_CD, TFT_CS, TFT_RST);

#elif (TOUSCRUINO_VERSION == 2)

#define TFT_PORT 2 // PortB
#define TFT_CD     21
#define TFT_WR     22
#define TFT_RD     23
#define TFT_CS     0xFF
#define TFT_RST 0xFF
#define TFT_BL 5

#include <ArduRCT_TouScruinoV2.h>

ArduRCT_TouScruinoV2 tft(TFT_PORT, TFT_CD, TFT_WR, TFT_RD, TFT_CS, TFT_RST, TFT_BL);

#endif

#define LOGO_SCALE_MUL 7
#define LOGO_SCALE_DIV 5

#define LOGO_WIDTH 57
#define LOGO_HEIGHT 88

/**
 * Approximation of sparkfun logo, 57 x 88
 *    w 0                    write to eeprom slot 0
 *    pc F800                set color to red
 *    tf 0 42 44 42 0 88     fill bottom left triangle
 *    afe 27 35 30           fill right half circle
 *    pts 7                  set line thickness to 7, scalable
 *    annw 25 79 17          draw thick arc to join triangle and half circle
 *    afnw 17 42 17          fill middle left quarter circle
 *    rf 17 35 11 7          fill middle left rectangle
 *    cf 30 13 13            fill top circle
 *    pc FFFF                set color to white, to erase
 *    cf 20 32 9             fill circle at middle left
 *    cf 43 11 8             fill circle at top right
 *    pc F800                set color to red
 *    afnne 32 14 14         fill NNE arc at top right
 *    tf 49 18 49 14 51 17   fill triangle at top right
 *      0
 **/
uint8_t sparkfun[] = "w 0 "\
    "pc F800 "\
    "tf 0 42 44 42 0 88 "\
    "afe 27 35 30 "\
    "pts 7 "\
    "annw 25 79 17 "\
    "afnw 17 42 17 "\
    "rf 17 35 11 7 "\
    "cf 30 13 13 "\
    "pc FFFF "\
    "cf 20 32 9 "\
    "cf 43 11 8 "\
    "pc F800 "\
    "afnne 32 14 14 "\
    "tf 49 18 49 14 51 17"\
    "\0";
    
void setup() {    
    tft.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, NO_OVERLAY);
    tft.setBacklight(180);
    
    // center the drawing on the screen
    int16_t x = (tft.getWidth()-LOGO_WIDTH*LOGO_SCALE_MUL/LOGO_SCALE_DIV)/2;
    int16_t y = (tft.getHeight()-LOGO_HEIGHT*LOGO_SCALE_MUL/LOGO_SCALE_DIV)/2;
    
    // execute the macro without the initial write in slot 0
    tft.executeMacro(&sparkfun[3], x, y, LOGO_SCALE_MUL, LOGO_SCALE_DIV, true);

    // write the macro to eeprom slot 0
    //tft.executeMacro(sparkfun);

    //delay(3000);
    
    // erase the drawing
    //tft.fillRectangle(x-2, y-2, LOGO_WIDTH*LOGO_SCALE+4, LOGO_HEIGHT*LOGO_SCALE+4, WHITE);

    // execute the macro stored in slot 0. Any time we need the logo, in whatever scale, we can reuse "e 0"
    //tft.executeMacro((uint8_t *)"e 0", x, y, LOGO_SCALE, 1);
}

void loop() {
    delay(100);
}