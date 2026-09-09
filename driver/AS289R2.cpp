/* AS289R2 thermal control component for mbed OS
 * Copyright (c) 2016-2025, Toyomasa Watarai
 * SPDX-License-Identifier: Apache-2.0
*/

#include "AS289R2.h"
#include <cstring>
#include <cstdio>
#include <stdarg.h>

AS289R2::AS289R2(int txd, int rxd, uart_inst_t* ch, uint32_t baud)
{
    uart_ch = ch;
    uart_init(uart_ch, baud);
    gpio_set_function(txd, UART_FUNCSEL_NUM(uart_ch, txd));
    gpio_set_function(rxd, UART_FUNCSEL_NUM(uart_ch, rxd));
    gpio_pull_up(txd);
    gpio_pull_up(rxd);
    initialize();
}

AS289R2::~AS289R2()
{
    uart_deinit(uart_ch);
}

void AS289R2::initialize(void)
{
    uart_write_blocking(uart_ch, (const uint8_t *)"\x1B\x40", 2);
}

void AS289R2::putLineFeed(uint32_t lines)
{
    for(uint32_t i = 0; i < lines; i++) {
        uart_write_blocking(uart_ch, (const uint8_t *)"\r", 1);
    }
}

void AS289R2::clearBuffer(void)
{
    uart_write_blocking(uart_ch, (const uint8_t *)"\x19", 1);
}

void AS289R2::setDoubleSizeHeight(void)
{
    uart_write_blocking(uart_ch, (const uint8_t *)"\x1B\x4E\x31", 3);
}

void AS289R2::clearDoubleSizeHeight(void)
{
    uart_write_blocking(uart_ch, (const uint8_t *)"\x1B\x4E\x30", 3);
}

void AS289R2::setDoubleSizeWidth(void)
{
    uart_write_blocking(uart_ch, (const uint8_t *)"\x1B\x57\x31", 3);
}

void AS289R2::clearDoubleSizeWidth(void)
{
    uart_write_blocking(uart_ch, (const uint8_t *)"\x1B\x57\x30", 3);
}

void AS289R2::setLargeFont(void)
{
    uart_write_blocking(uart_ch, (const uint8_t *)"\x1B\x4C\x31", 3);
}

void AS289R2::clearLargeFont()
{
    uart_write_blocking(uart_ch, (const uint8_t *)"\x1B\x4C\x30", 3);
}

void AS289R2::setANKFont(uint32_t font)
{
    uint8_t buf[3];
    buf[0] = 0x1B;
    buf[1] = 0x68;
    buf[2] = (char)font;
    uart_write_blocking(uart_ch, buf, 3);
}

void AS289R2::setKanjiFont(uint32_t font)
{
    uint8_t buf[3];
    buf[0] = 0x12;
    buf[1] = 0x53;
    buf[2] = (char)font;
    uart_write_blocking(uart_ch, buf, 3);
}

void AS289R2::printQRCode(uint32_t err, const uint8_t* param)
{
    uint32_t len = strlen((const char*)param);
    uint8_t buf[4] = {0x1D, 0x78};
    buf[2] = err;
    buf[3] = len;
    uart_write_blocking(uart_ch, buf, 4);
    uart_write_blocking(uart_ch, param, len);
}

void AS289R2::printBarCode(uint32_t code, const uint8_t* param)
{
    uint32_t len = strlen((const char*)param);
    uint8_t buf[4] = {0x1D, 0x6B};
    buf[2] = code;
    buf[3] = (code & 0x0F);
    uart_write_blocking(uart_ch, buf, 4);
    uart_write_blocking(uart_ch, param, len);
}

void AS289R2::printBitmapImage(uint32_t mode, uint16_t lines, const uint8_t * image)
{
    uint8_t buf[3] = {0x1C, 0x2A};
    buf[2] = mode;
    uart_write_blocking(uart_ch, buf, 3);

    buf[0] = (lines >> 8) & 0xFF;
    buf[1] = (lines >> 0) & 0xFF;
    uart_write_blocking(uart_ch, buf, 2);

    if (mode == 0x61) {
        return;
    }

    uart_write_blocking(uart_ch, image, (48 * lines));
}

void AS289R2::setLineSpaceing(uint32_t space)
{
    uint8_t buf[3];
    buf[0] = 0x1B;
    buf[1] = 0x33;
    buf[2] = (char)space;
    uart_write_blocking(uart_ch, buf, 3);
}

void AS289R2::defaultLineSpaceing()
{
    uart_write_blocking(uart_ch, (const uint8_t *)"\x1B\x33\x04", 3);
}

void AS289R2::setPrintDirection(uint32_t direction)
{
    uint8_t buf[3];
    buf[0] = 0x1B;
    buf[1] = 0x49;
    buf[2] = (char)direction;
    uart_write_blocking(uart_ch, buf, 3);
}

void AS289R2::putPaperFeed(uint32_t space)
{
    uint8_t buf[3];
    buf[0] = 0x1B;
    buf[1] = 0x4A;
    buf[2] = (char)space;
    uart_write_blocking(uart_ch, buf, 3);
}

void AS289R2::setInterCharacterSpace(uint32_t space)
{
    uint8_t buf[3];
    buf[0] = 0x1B;
    buf[1] = 0x20;
    buf[2] = (char)space;
    uart_write_blocking(uart_ch, buf, 3);
}

void AS289R2::defaultInterCharacterSpace()
{
    uart_write_blocking(uart_ch, (const uint8_t *)"\x1B\x20\x01", 3);
}

void AS289R2::putPrintPosition(uint32_t position)
{
    uint8_t buf[3];
    buf[0] = 0x1B;
    buf[1] = 0x6C;
    buf[2] = (char)position;
    uart_write_blocking(uart_ch, buf, 3);
}

void AS289R2::setScript(script_mode script)
{
    uint8_t buf[3];
    buf[0] = 0x1B;
    buf[1] = 0x73;
    buf[2] = (char)script;
    uart_write_blocking(uart_ch, buf, 3);
}

void AS289R2::clearScript()
{
    uart_write_blocking(uart_ch, (const uint8_t *)"\x1B\x73\x30", 3);
}

void AS289R2::setQuadrupleSize()
{
    uart_write_blocking(uart_ch, (const uint8_t *)"\x1C\x57\x31", 3);
}

void AS289R2::clearQuadrupleSize()
{
    uart_write_blocking(uart_ch, (const uint8_t *)"\x1C\x57\x30", 3);
}

void AS289R2::setEnlargement(uint32_t width, uint32_t height)
{
    uint8_t buf[4];
    buf[0] = 0x1C;
    buf[1] = 0x65;
    buf[2] = (char)width;
    buf[3] = (char)height;
    uart_write_blocking(uart_ch, buf, 4);
}

void AS289R2::clearEnlargement()
{
    uart_write_blocking(uart_ch, (const uint8_t *)"\x1C\x65\x31\x31", 4);
}

void AS289R2::setBarCodeHeight(uint32_t height)
{
    uint8_t buf[3];
    buf[0] = 0x1D;
    buf[1] = 0x68;
    buf[2] = (char)height;
    uart_write_blocking(uart_ch, buf, 3);
}

void AS289R2::defaultBarCodeHeight()
{
    uart_write_blocking(uart_ch, (const uint8_t *)"\x1D\x68\x50", 3);
}

void AS289R2::setBarCodeBarSize(uint32_t narrowbar, uint32_t widebar)
{
    uint8_t buf[4];
    buf[0] = 0x1D;
    buf[1] = 0x77;
    buf[2] = (char)narrowbar;
    buf[3] = (char)widebar;
    uart_write_blocking(uart_ch, buf, 4);
}

void AS289R2::defaultBarCodeBarSize()
{
    uart_write_blocking(uart_ch, (const uint8_t *)"\x1D\x77\x02\x05", 4);

}

int AS289R2::printf(const char *format, ...) {
    char buf[200];

    va_list arg;
    va_start(arg, format);
    sprintf(buf, "%s\x00", va_arg(arg, void*));
    //::printf("> %s", buf);
    uart_write_blocking(uart_ch, (const uint8_t *)buf, strlen(buf));
    va_end(arg);
    return 0;
}
