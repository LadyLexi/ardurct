/*
 * TouScruinoV1 - Screen and 5 ways switch management
 *
 * Copyright (c) 2012 Laurent Wibaux <lm.wibaux@gmail.com>
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

#include "ArduRCT_TouScruinoV1.hpp"
#include <ArduRCT_Graphics.h>
#include <ArduRCT_EventManager.h>

ArduRCT_TouScruinoV1::ArduRCT_TouScruinoV1(uint8_t cd, uint8_t cs, uint8_t reset, uint8_t backlightPin, ArduRCT_EventManager *eventManager,
            ArduRCT_Switch *up, ArduRCT_Switch *down, ArduRCT_Switch *left, ArduRCT_Switch *right, ArduRCT_Switch *center) {
    setupScreen(cd, cs, reset);
    _backlightPin = backlightPin;
    _eventManager = eventManager;
    _eventManager->registerSwitch(up);
    _eventManager->registerSwitch(down);
    _eventManager->registerSwitch(right);
    _eventManager->registerSwitch(left);
    _eventManager->registerSwitch(center);
}

void ArduRCT_TouScruinoV1::update() {
    _eventManager->update();
}

ArduRCT_RealTimeClock *ArduRCT_TouScruinoV1::getRTC() {
    return _eventManager->getRTC();
}

void ArduRCT_TouScruinoV1::registerEventHandler(ArduRCT_EventHandler *handler) {
    _eventManager->registerEventHandler(handler);
}

void ArduRCT_TouScruinoV1::registerSwitch(ArduRCT_Switch *aSwitch) {
    _eventManager.registerSwitch(aSwitch);
}

void ArduRCT_TouScruinoV1::registerAnalog(ArduRCT_Analog *anAnalog) {
    _eventManager.registerAnalog(anAnalog);
}

void ArduRCT_TouScruinoV1::registerEncoder(ArduRCT_Encoder *anEncoder) {
    _eventManager.registerEncoder(anEncoder);
}