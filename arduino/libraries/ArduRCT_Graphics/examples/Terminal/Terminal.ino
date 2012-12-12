/*
 * Terminal - Most simple terminal: all received chars are shown on screen
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

#include <SPI.h>

#define FONT_FACE FONT_PLAIN

//#include <ArduRCT_S6D04H0.h>
//ArduRCT_S6D04H0 tft(2, 21, 22, 23, 0xFF, 0xFF);

//#define FONT_SIZE FONT_MEDIUM

#include <ArduRCT_ST7735.h>
ArduRCT_ST7735 tft(10, 9, 8);

#define FONT_SIZE FONT_SMALL

void setup() {
    Serial.begin(57600);
    touscruino.begin(WHITE, BLUE, FONT_SIZE, FONT_FACE);
}

void loop() {
    if (Serial.available() > 0) touscruino.print(Serial.read()); 
}