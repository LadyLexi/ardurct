/*
 * S6D04H0 - Implementation of the ScreenHAL abstract functions for the S6D04H0
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

#include "ArduRCT_S6D04H0.h"

#define S6D04H0_WIDTH  240
#define S6D04H0_HEIGHT 320

#define S6D04H0_DELAY    120

#define S6D04H0_CASET    0x2a
#define S6D04H0_PASET    0x2b
#define S6D04H0_RAMWR    0x2c
#define S6D04H0_RAMRD    0x2e
#define S6D04H0_MADCTL   0x36
#define S6D04H0_NOP      0x00

#define S6D04H0_R0       0x48
#define S6D04H0_R90      0x28
#define S6D04H0_R180     0x88
#define S6D04H0_R270     0xE8

// the following functions are defined as macros to speed up the execution
#define _clearRD() { *_rdPort &= _rdLow; *_rdPort &= _rdLow; }

#define _prepareWR() { _wrPortLowWR = (*_wrPort) & _wrLow; _wrPortHighWR = (*_wrPort) | _wrHigh; }

#define _writeCommand(cmd) { *_cdPort &= ~_cdBitMask; S6D04H0_DATA_OUT_PORT = cmd; *_wrPort &= _wrLow;  *_wrPort |= _wrHigh; *_cdPort |= _cdBitMask; }

#define _pulseWR() { *_wrPort = _wrPortLowWR;  *_wrPort = _wrPortHighWR; }

#define _writeData(data8b) { S6D04H0_DATA_OUT_PORT = data8b; *_wrPort = _wrPortLowWR;  *_wrPort = _wrPortHighWR; }

#define _write16bData(data16b) { S6D04H0_DATA_OUT_PORT = data16b >> 8; *_wrPort = _wrPortLowWR;  *_wrPort = _wrPortHighWR; S6D04H0_DATA_OUT_PORT = data16b; *_wrPort = _wrPortLowWR;  *_wrPort = _wrPortHighWR; }

const unsigned char PROGMEM ArduRCT_S6D04H0_initialization_code[] = {
    0xFE,           /* delay(S6D04H0_DELAY) */
    0, 0x11,        /* SLPOFF: SLeeP OFF */
    0xFE,           /* delay(S6D04H0_DELAY) */
    0, 0x13,        /* NORON: NORmal mode ON */
    0xFE,           /* delay(S6D04H0_DELAY) */
    5, 0xf3, 0x01, 0xff, 0x1f, 0x00, 0x03,
    17, 0xf2, 0x28, 0x60, 0x7f, 0x08, 0x08, 0x00, 0x00, 0x15, 0x48, 0x00, 0x07, 0x01, 0x00, 0x00, 0x94, 0x08, 0x08,
    20, 0xf4, 0x09, 0x00, 0x00, 0x00, 0x21, 0x47, 0x01, 0x02, 0x2A, 0x5D, 0x07, 0x2A, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    12, 0xf5, 0x00, 0x3c, 0x30, 0x00, 0x00, 0x12, 0x00, 0x00, 0x0d, 0x01, 0x00, 0x00,
    9, 0xf6, 0x01, 0x01, 0x07, 0x00, 0x02, 0x0c, 0x02, 0x08, 0x03,
    4, 0xf7, 0x01, 0x00, 0x10, 0x00,
    3, 0xf8, 0x99, 0x00, 0x00,
    1, 0xf9, 0x01,
    1, 0x26, 0x02,  /* GASET: Gamma Set */
    1, 0x35, 0x00,  /* TEON: Tearing on */
    1, 0x36, 0x48,  /* MADCTL: Memory ADdress ConTroL, B7,B6,B5=0 => no rotation, B3=1 => BGR filter */
    1, 0x3a, 0x55,  /* COLMOD: COLor MODe: 16 bits/pixel */
    4, 0x2a, 0x00, 0x00, 0x00, 0xef,    /* CASET: Column Address SET */
    4, 0x2b, 0x00, 0x00, 0x01, 0x3f,     /* PASET:Page Address SET */
    0, 0x29,        /* DISPON:    DISplay ON */
    0xFE,           /* delay(S6D04H0_DELAY) */
    0xFF            /* End initialization */
};


ArduRCT_S6D04H0::ArduRCT_S6D04H0() {    
    _widthImpl = S6D04H0_WIDTH;
    _heightImpl = S6D04H0_HEIGHT;
    setupScreen(S6D04H0_CD_PIN, S6D04H0_WR_PIN, S6D04H0_RD_PIN, S6D04H0_CS_PIN, S6D04H0_RESET_PIN, S6D04H0_SPI_ON_BUS);
    setupBacklight(S6D04H0_BACKLIGHT_PIN);
}
 
/* ---------------- Protected functions ------------------------ */

void ArduRCT_S6D04H0::initScreenImpl(void) {
    uint8_t buffer[32];
    uint16_t i = 0;
    
    // init the screen
    _prepareWR();
    while (1) {
        memcpy_P(buffer, &(ArduRCT_S6D04H0_initialization_code[i]), 32);
        if (buffer[0] == 0xFF) break;
        else if (buffer[0] == 0xFE) delay(S6D04H0_DELAY);
        else {
            _writeCommand(buffer[1]);
            for (uint8_t j=0; j<buffer[0]; j++) _writeData(buffer[2+j]);
            i += buffer[0]+1;
        }
        i++;
    }
}

// draw a single pixel
// we inline the function to go as fast as possible
void ArduRCT_S6D04H0::drawPixelImpl(uint16_t x, uint16_t y, uint16_t color) {
    uint8_t xh = x >> 8;
    uint8_t xl = x;
    uint8_t yh = y >> 8;
    uint8_t yl = y;
    _prepareWR();
    _writeCommand(S6D04H0_CASET);
    _writeData(xh);
    _writeData(xl);
    _writeData(xh);
    _writeData(xl);
    _writeCommand(S6D04H0_PASET);
    _writeData(yh);
    _writeData(yl);
    _writeData(yh);
    _writeData(yl);
    _writeCommand(S6D04H0_RAMWR);
    _write16bData(color);
}


void ArduRCT_S6D04H0::fillAreaImpl(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy, uint16_t color) {
    uint32_t nbPixels = hx - lx + 1;
    nbPixels *= (hy - ly + 1);
    _prepareWR();
    _setClippingRectangle(lx, ly, hx, hy);
    _writeCommand(S6D04H0_RAMWR);
    uint8_t colH = color >> 8;
    uint8_t colL = color;
    uint8_t lowWR = (*_wrPort) & _wrLow; 
    uint8_t highWR = (*_wrPort) | _wrHigh;
    uint8_t nbPixelsH = 1 + (nbPixels >> 16);
    uint8_t nbPixelsM = 1 + (nbPixels >> 8);
    uint8_t nbPixelsL = nbPixels;
    if (colH == colL) {
        asm (
          "out %1,%7\n\t"           // portB = colH
        "pixelL1:\n\t"
          "out %0,%2\n\t"           // WR low
          "out %0,%3\n\t"           // WR high
          "out %0,%2\n\t"           // WR low
          "out %0,%3\n\t"           // WR high
          "dec %6\n\t"              // nbPixelL --
          "brne pixelL1\n\t"        // if nbPixelL != 0 goto pixelL1
          "dec %5\n\t"              // nbPixelM --
          "brne pixelL1\n\t"        // if nbPixelM != 0 goto pixelL1
          "dec %4\n\t"              // nbPixelH --
          "brne pixelL1\n\t"        // if nbPixelH != 0 goto pixelL1
        ::
        "I" (_SFR_IO_ADDR(S6D04H0_WR_PORT)),  // %0
        "I" (_SFR_IO_ADDR(S6D04H0_DATA_OUT_PORT)),  // %1
        "r" (lowWR),                // %2
        "r" (highWR),               // %3
        "r" (nbPixelsH),            // %4
        "r" (nbPixelsM),            // %5
        "r" (nbPixelsL),            // %6
        "r" (colH)                  // %7
        );
    } else {
        asm (
        "pixelL2:\n\t"
          "out %1,%7\n\t"           // portB = colH
          "out %0,%2\n\t"           // WR low
          "out %0,%3\n\t"           // WR high
          "out %1,%8\n\t"           // portB = colL
          "out %0,%2\n\t"           // WR low
          "out %0,%3\n\t"           // WR high
          "dec %6\n\t"              // nbPixelL --
          "brne pixelL2\n\t"        // if nbPixelL != 0 goto pixelL2
          "dec %5\n\t"              // nbPixelM --
          "brne pixelL2\n\t"        // if nbPixelM != 0 goto pixelL2
          "dec %4\n\t"              // nbPixelH --
          "brne pixelL2\n\t"        // if nbPixelH != 0 goto pixelL2
        ::
        "I" (_SFR_IO_ADDR(S6D04H0_WR_PORT)),  // %0
        "I" (_SFR_IO_ADDR(S6D04H0_DATA_OUT_PORT)),  // %1
        "r" (lowWR),                // %2
        "r" (highWR),               // %3
        "r" (nbPixelsH),            // %4
        "r" (nbPixelsM),            // %5
        "r" (nbPixelsL),            // %6
        "r" (colH),                 // %7
        "r" (colL)                  // %8
        );
    }
}


uint16_t *ArduRCT_S6D04H0::retrieveBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    uint32_t nbPixels = width;
    nbPixels *= height;
    
    _prepareWR();
    _setClippingRectangle(x, y, x+width-1, y+height-1); 
    _writeCommand(S6D04H0_RAMRD);
    S6D04H0_DATA_DDR_PORT = 0x00;    
    // one dummy read
    _clearRD();
    bitmap[0] = S6D04H0_DATA_IN_PORT;
    *_rdPort |= _rdHigh; 
    // the data is written in R5G6B5, changed to B6G6R6 by MADCTL
    // it therefore reads back in B6G6R6 format, each color byte is [cccccc00]
    for (uint32_t i=0; i<nbPixels; i++) {
        // read B, keep 5
        _clearRD();
        bitmap[i] = S6D04H0_DATA_IN_PORT >> 3;
        *_rdPort |= _rdHigh; 
        // read G, keep 6
        _clearRD();
        bitmap[i] |= (S6D04H0_DATA_IN_PORT >> 2) << 5;
        *_rdPort |= _rdHigh; 
        // read R, keep 5
        _clearRD();
        bitmap[i] |= (S6D04H0_DATA_IN_PORT >> 3) << 11;
        *_rdPort |= _rdHigh; 
    }
    S6D04H0_DATA_DDR_PORT = 0xFF;
    return bitmap;
}

void ArduRCT_S6D04H0::pasteBitmapImpl(uint16_t *bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    // nbPixels is always < 8192 because of RAM constraint
    uint16_t nbPixels = width;
    nbPixels *= height;
    _prepareWR();
    _setClippingRectangle(x, y, x+width-1, y+height-1); 
    _writeCommand(S6D04H0_RAMWR);
    
    uint8_t lowWR = (*_wrPort) & _wrLow; 
    uint8_t highWR = (*_wrPort) | _wrHigh;
    uint8_t *bitmap8 = (uint8_t *)bitmap;
    uint8_t pixelL;
    uint8_t pixelH;
    while (nbPixels-- > 0) {
        pixelL = *bitmap8;
        bitmap8++;
        pixelH = *bitmap8;
        bitmap8++;
        S6D04H0_DATA_OUT_PORT = pixelH;
        S6D04H0_WR_PORT = lowWR;
        S6D04H0_WR_PORT = highWR;
        S6D04H0_DATA_OUT_PORT = pixelL;
        S6D04H0_WR_PORT = lowWR;
        S6D04H0_WR_PORT = highWR;
    }

    /*
    uint8_t *bitmap8 = (uint8_t *)bitmap;
    uint8_t lowWR = (*_wrPort) & _wrLow; 
    uint8_t highWR = (*_wrPort) | _wrHigh;
    uint8_t nbPixelsM = 1 + (nbPixels >> 8);
    uint8_t nbPixelsL = nbPixels;
    asm (
    "pixelL3:\n\t"
      "ld r18, X+\n\t"          // r18 = (X), X++
      "ld r19, X+\n\t"          // r19 = (X), X++
      "out %1,r19\n\t"          // portB = r19
      "out %0,%2\n\t"           // WR low
      "out %0,%3\n\t"           // WR high
      "out %1,r18\n\t"          // portB = r18
      "out %0,%2\n\t"           // WR low
      "out %0,%3\n\t"           // WR high
      "dec %5\n\t"              // nbPixelL --
      "brne pixelL3\n\t"        // if nbPixelL != 0 goto pixelL3
      "dec %4\n\t"              // nbPixelM --
      "brne pixelL3\n\t"        // if nbPixelM != 0 goto pixelL3
    ::
    "I" (_SFR_IO_ADDR(PORTC)),  // %0
    "I" (_SFR_IO_ADDR(PORTB)),  // %1
    "r" (lowWR),                // %2
    "r" (highWR),               // %3
    "r" (nbPixelsM),            // %4
    "r" (nbPixelsL),            // %5
    "x" (bitmap8)               // %6
    : "r18", "r19"
    );
    */
}


void ArduRCT_S6D04H0::setRotationImpl(uint8_t rotation) {
    _prepareWR();
    _writeCommand(S6D04H0_MADCTL);
    if (rotation == GRAPHICS_ROTATION_0) {
        _writeData(S6D04H0_R0);
    } else if (rotation == GRAPHICS_ROTATION_90) {
        _writeData(S6D04H0_R90);
    } else if (rotation == GRAPHICS_ROTATION_180) {
        _writeData(S6D04H0_R180);    
    } else if (rotation == GRAPHICS_ROTATION_270) {
        _writeData(S6D04H0_R270);
    }
}


void ArduRCT_S6D04H0::selectScreenImpl() {
    if (_screenSelected) return;
    if (_spiOnBus) {
        // if SPI is active, disable it but remember that we disabled it
        if (SPCR & _BV(SPE)) {
            SPCR &= ~_BV(SPE);
            _spiUsed = true;
        }
    }
    // set the direction to output
    S6D04H0_DATA_DDR_PORT = 0xFF;
    // select the screen
    if (_cs != 0xFF) digitalWrite(_cs, LOW);
    // put the screen in data mode
    *_cdPort |= _cdBitMask;
    _screenSelected = true;
}


void ArduRCT_S6D04H0::unselectScreenImpl() {
    // unselect the screen
    if (_cs != 0xFF) digitalWrite(_cs, HIGH);
    // release the SCK line, to switch off the led
    digitalWrite(SCK, LOW);
    // restore the SPI if it was active
    if (_spiOnBus & _spiUsed) {
        // we have to set SS as an output to enable SPI
        pinMode(SS, OUTPUT);
        // we always restore the master mode
        SPCR |= _BV(MSTR);
        SPCR |= _BV(SPE);
    }
    _screenSelected = false;
}


/* ---------------- Private functions -------------------------- */

// define the zone we are going to write to
void ArduRCT_S6D04H0::_setClippingRectangle(uint16_t lx, uint16_t ly, uint16_t hx, uint16_t hy) {
    _writeCommand(S6D04H0_CASET);
    _write16bData(lx);
    _write16bData(hx);
    _writeCommand(S6D04H0_PASET);
    _write16bData(ly);
    _write16bData(hy);
}
