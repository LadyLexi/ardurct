/*
 * MacroTests - Tests the macros (see Macros.cpp for manual)
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
 
#include <TouchScreen.h>

TouchScreen touchscreen;

uint8_t seven[] = "ld 0 0 22 0 ld 0 9 arnww 38";
uint8_t buffer[256];
uint8_t bufferPtr = 0;

void setup() {
    Serial.begin(57600);
    
    touchscreen.begin(BLACK, WHITE, FONT_MEDIUM, FONT_BOLD, OVERLAY);
    touchscreen.setBacklight(180);

    touchscreen.executeMacro(seven, 20, 20);
    touchscreen.executeMacro(seven, 10, 100, 2);
    
    delay(3000);
    
    touchscreen.setBacklight(0);
    touchscreen.fillScreen(WHITE);
    touchscreen.setBacklight(180);
}

void loop() {
    while (Serial.available()) buffer[bufferPtr++] = Serial.read();
    if (bufferPtr == 0) return;
    
    // wait for end of sentence
    if (buffer[bufferPtr-1] == '.') {
        // mark end of macro
        buffer[bufferPtr-1] = 0;
        bufferPtr = 0;
        // execute the macro
        touchscreen.executeMacro(buffer, 10, 10);
    }
    delay(10);
}


