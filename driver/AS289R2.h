/* AS289R2 thermal control component for mbed OS
 * Copyright (c) 2016-2025, Toyomasa Watarai
 * SPDX-License-Identifier: Apache-2.0
*/

#ifndef AS289R2_H
#define AS289R2_H

#include "hardware/uart.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

/*
 * A printer interface for driving AS-289R2 thermal printer shield 
 * of NADA Electronics, Ltd.
*/

class AS289R2
{
public:

    /**
     * @enum Kanji_font_size
     * Value of Japanese Kanji font size
     */
    enum Kanji_font_size {
        //! 24x24 dot font
        KANJI_24x24 = 0x30,
        //! 16x16 dot font
        KANJI_16x16,
        //! Defalut font size
        KANJI_DEFAULT = KANJI_24x24
    };

    /**
     * @enum ANK_font_size
     * Value of ANK font size
     */
    enum ANK_font_size {
        //! 8x16 dot font
        ANK_8x16 = 0x30,
        //! 12x24 dot font
        ANK_12x24,
        //! 16x16 dot font
        ANK_16x16,
        //! 24x24 dot fot
        ANK_24x24,
        //! Defalut font size
        ANK_DEFAULT = ANK_12x24
    };

    /**
     * @enum QRcode_error_level
     * Value of CQ code error correction level
     */
    enum QRcode_error_level {
        //! Error correction level L (7%)
        QR_ERR_LVL_L = 0x4C,
        //! Error correction level M (15%)
        QR_ERR_LVL_M = 0x4D,
        //! Error correction level Q (25%)
        QR_ERR_LVL_Q = 0x51,
        //! Error correction level H (30%)
        QR_ERR_LVL_H = 0x48
    };

    /**
     * @enum barcode_mode
     * Value of barcode mode
     */
    enum barcode_mode {
        //!  UPC-A : 11-digit, d1-d11, C/D
        BCODE_UPC_A = 0x30,
        //! JAN13 : 12-digit, d1-d12, C/D
        BCODE_JAN13 = 0x32,
        //! JAN8 : 7-digit, d1-d7, C/D
        BCODE_JAN8,
        //! CODE39 : variable, d1-d20, C/D
        BCODE_CODE39,
        //! ITF : variable, d1-d20
        BCODE_ITF,
        //! CODABAR (NW7) : variable, d1-d20
        BCODE_CODABAR
    };
    
    /**
     * @enum script_mode
     * Value of script mode
     */
    enum script_mode {
        //! Cancel script mode
        SCRIPT_CANCEL = 0,
        //! Super script
        SCRIPT_SUPER,
        //! Sub script
        SCRIPT_SUB
    };

    /** Create a AS289R2 instance
     *  which is connected to specified Serial pin with specified baud rate
     *
     * @param tx Serial TX pin
     * @param rx Serial RX pin (dummy)
     * @param baud (option) serial baud rate (default: 9600bps)
     */
    AS289R2(int txd, int rxd, uart_inst_t* ch, uint32_t baud = 9600);

    /** Destructor of AS289R2
     */
    virtual ~AS289R2();

    /** Initializa AS289R2
     *
     *  Issues initialize command for AS-289R2
     *
     */
    void initialize(void);

    /** Send line feed code
     *  which is connected to specified Serial pin with specified baud rate
     *
     * @param lines Number of line feed
     */
    void putLineFeed(uint32_t lines);

    /** Clear image buffer of the AS-289R2
     *
     */
    void clearBuffer(void);

    /** Set double height size font
     *
     */
    void setDoubleSizeHeight(void);

    /** Set normal height size font
     *
     */
    void clearDoubleSizeHeight(void);

    /** Set double width size font
     *
     */
    void setDoubleSizeWidth(void);

    /** Set normal width size font
     *
     */
    void clearDoubleSizeWidth(void);

    /** Set large size font (48x96)
     *
     */
    void setLargeFont(void);

    /** Set normal size font
     *
     */
    void clearLargeFont(void);

    /** Set ANK font
     *
     * @param font ANK font e.g. AS289R2::ANK_8x16
     */
    void setANKFont(uint32_t font);

    /** Set Kanji font size
     *
     * @param font Kanji font e.g. AS289R2::KANJI_16x16
     */
    void setKanjiFont(uint32_t font);

    /** Print QR code
     *
     * @param err QR code error correction level e.g. AS289R2::QR_ERR_LVL_M
     * @param buf Data to be printed
     */
    void printQRCode(uint32_t err, const uint8_t* buf);

    /** Print Bar code
     *
     * @param code Type of Bar code e.g. AS289R2::JAN13
     * @param buf Data to be printed
     */
    void printBarCode(uint32_t code, const uint8_t* param);

    /** Print bitmap image
     *
     * @param cmd Type of operation mode, 0x61: print image buffer, 0x62: register image buffer, 0x63: register -> print, 0x64: print -> register, 0x65: line print
     * @param lines Number of print line
     * @param image Data to be printed
     */
    void printBitmapImage(uint32_t cmd, uint16_t lines, const uint8_t * image);

    /** Set Line Spaceing
     *
     * @param space line spacing
     */
    void setLineSpaceing(uint32_t space);

    /** Set as default Line Spaceing
     *
     */
    void defaultLineSpaceing(void);

    /** Set Print Direction
     *
     * @param direction Print direction, 0: lister, 1: texter
     */
    void setPrintDirection(uint32_t direction);

    /** Send feed code
     *
     * @param space Paper feed
     */
    void putPaperFeed(uint32_t space);

    /** Set Inter Character Space
     *
     * @param space inter-character space
     */
    void setInterCharacterSpace(uint32_t space);

    /** Set as default Inter Character Space
     *
     */
    void defaultInterCharacterSpace(void);

    /** Send Print Position
     *
     * @param position Print position
     */
    void putPrintPosition(uint32_t position);

    /** Set Script
     *
     * @param script mode e.g. AS289R2::SCRIPT_SUPER
     */
    void setScript(script_mode script);

    /** Clear Script
     *
     */
    void clearScript(void);

    /** Set Quadruple size
     *
     */
    void setQuadrupleSize(void);

    /** Clear Quadruple size
     *
     */
    void clearQuadrupleSize(void);

    /** Set Enlargement size
     *
     * @param width enlargement
     * @param height enlargement
     */
    void setEnlargement(uint32_t width, uint32_t height);

    /** Clear Enlargement size
     *
     */
    void clearEnlargement(void);

    /** Set BarCode Height size
     *
     * @param height Bar height
     */
    void setBarCodeHeight(uint32_t height);

    /** Set as default BarCode Height size
     *
     */
    void defaultBarCodeHeight(void);

    /** Set BarCode Bar size
     *
     * @param narrowbar narrow bars size
     * @param widebar wide bars size
     */
    void setBarCodeBarSize(uint32_t narrowbar, uint32_t widebar);

    /** Set as default BarCode Bar size
     *
     */
    void defaultBarCodeBarSize(void);

    /** Print string
     *
     */
     int printf(const char *format, ...);

protected:
    // stub/constructor for derived classes
    AS289R2() : uart_ch(nullptr) {}

private:
    uart_inst_t* uart_ch;

};

#endif
