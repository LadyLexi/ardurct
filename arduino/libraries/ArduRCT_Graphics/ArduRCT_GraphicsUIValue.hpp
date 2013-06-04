/*
 * GraphicsUIValue - an adjustable value
 *
 * Copyright (c) 2013 Laurent Wibaux <lm.wibaux@gmail.com>
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
 
#ifndef GRAPHICS_UI_VALUE_HPP
#define GRAPHICS_UI_VALUE_HPP 1

#include <inttypes.h>

class ArduRCT_GraphicsUIValue : public ArduRCT_GraphicsUIElement {
    
    public:
        ArduRCT_GraphicsUIValue(uint8_t id, int16_t value, int16_t min, int16_t max, bool (*actionHandler)(uint8_t elementId, int16_t value) = 0, int16_t step = 1, uint8_t radix = 10);

        void draw(ArduRCT_Graphics *graphics, int16_t uiX, int16_t uiY, uint16_t uiWidth);

        void autoSize(ArduRCT_Graphics *graphics);

        ArduRCT_GraphicsUIElement *setValue(int16_t value) ;

        ArduRCT_GraphicsUIElement *enter();

        bool release();

        bool escape();

        bool increase();

        bool decrease();

        void highlight();
        
        ArduRCT_GraphicsUIElement *touch(int16_t touchX, int16_t touchY);
        
    private:
        int16_t _max;
        int16_t _min;
        int16_t _step;
        char _display[6];
        uint8_t _radix;

        uint8_t _intToString(int16_t value);
}; 

#endif