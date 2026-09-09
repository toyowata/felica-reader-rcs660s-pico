/** Text LCD module "SB1602E" class library
 *
 *  @author  Tedd OKANO, Masato YAMANISHI & Toyomasa Watarai
 *  @version 2.1
 *  @date    07-April-2015
 *
 *  SB1602E is an I2C based low voltage text LCD panel (based Sitronix ST7032 chip)
 *  The module by StrawberryLinux
 *  http://strawberry-linux.com/catalog/items?code=27002 (Online shop page (Japanese))
 *  http://strawberry-linux.com/pub/ST7032i.pdf (datasheet of the chip)
 *
 *  This is a library to operate this module easy.
 *
 *  Released under the Apache 2 license License
 *
 *  revision history (class lib name was "TextLCD_SB1602E")
 *    revision 1.0  22-Jan-2010   a. 1st release
 *    revision 1.1  23-Jan-2010   a. class name has been changed from lcd_SB1602E to TextLCD_SB1602E
 *                                b. printf() added
 *                                c. copyright notice added
 *    revision 1.3  02-May-2014   a. puticon() added (for SB1602B) by Masato YAMANISHI san
 *    revision 2.0  20-Oct-2014   a. class name is changed and published as "SB1602E"
 *                                b. re-written for better usability
 *    revision 2.1  07-Apl-2015   a. add printf() with X and Y position
 *                                b. add setter for number of chars in a line (e.g. 8x2 LCD support)
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include <stdarg.h>
#include <stdio.h>

#include "SB1602E.h"

#define DEFAULT_CONTRAST 0x35

SB1602E::SB1602E( int I2C_sda, int I2C_scl, const char *init_massage ) : charsInLine(MaxCharsInALine) {
    i2c_init(i2c1, 100 * 1000); // 100kHz
    gpio_set_function(I2C_sda, GPIO_FUNC_I2C);
    gpio_set_function(I2C_scl, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_sda);
    gpio_pull_up(I2C_scl);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func((uint32_t)I2C_sda, (uint32_t)I2C_scl, (uint32_t)GPIO_FUNC_I2C));

    init( init_massage );
}

SB1602E::~SB1602E() {
    i2c_deinit(i2c1);
}


void SB1602E::init( const char *init_massage ) {
    const char init_seq0[]  = {
        Comm_FunctionSet_Normal,
        Comm_ReturnHome,             //    This may be required to reset the scroll function
        Comm_FunctionSet_Extended,
        Comm_InternalOscFrequency,
        Comm_ContrastSet            | ( DEFAULT_CONTRAST       & 0xF),
        Comm_PwrIconContrast        | ((DEFAULT_CONTRAST >> 4) & 0x3),
        Comm_FollowerCtrl           | 0x0A,
    };
    const char init_seq1[]  = {
        Comm_DisplayOnOff,
        Comm_ClearDisplay,
        Comm_EntryModeSet,
    };

    i2c_addr = (0x7C >> 1);

    sleep_ms(40);    //    interval after hardware reset

    for ( uint32_t i = 0; i < sizeof( init_seq0 ); i++ ) {
        lcd_command( init_seq0[ i ] );
        sleep_ms(1);
    }

    sleep_ms(300);

    for ( uint32_t i = 0; i < sizeof( init_seq1 ); i++ ) {
        lcd_command( init_seq1[ i ] );
        sleep_ms(2);
    }

    curs[ 0 ]    = 0;
    curs[ 1 ]    = 0;

    if ( init_massage )
    {
        puts( 0, init_massage );
        curs[ 0 ]    = 0;
    }
}

void SB1602E::printf( int line, const char *format, ... ) {
    char    s[ 32 ];
    va_list args;

    va_start( args, format );
    vsnprintf( s, 32, format, args );
    va_end( args );

    puts( line, s );
}

void SB1602E::printf( int x, int y, char *format, ... ) {
    char    s[ 32 ];
    va_list args;

    va_start( args, format );
    vsnprintf( s, 32, format, args );
    va_end( args );

    curs[ y ] = x;
    puts( y, s );
}

void SB1602E::putc( int line, char c ) {
    if ( (c == '\n') || (c == '\r') ) {
        clear_lest_of_line( line );
        curs[ line ]    = 0;
        return;
    }

    putcxy( c, curs[ line ]++, line );
}

void SB1602E::puts( int line, const char *s ) {
    while ( char c    = *s++ )
        putc( line, c );
}

void SB1602E::putcxy( char c, int x, int y ) {
    const char    Comm_SetDDRAMAddress        = 0x80;
    const char    DDRAMAddress_Ofst[]         = { 0x00, 0x40 };

    if ( (x >= charsInLine) || (y >= 2) )
        return;

    lcd_command( (Comm_SetDDRAMAddress | DDRAMAddress_Ofst[ y ]) + x );
    lcd_data( c );
}

void SB1602E::clear( void ) {
    lcd_command( Comm_ClearDisplay );
    sleep_ms(2);
    curs[ 0 ]    = 0;
    curs[ 1 ]    = 0;
}

void SB1602E::contrast( char contrast ) {
    lcd_command( Comm_FunctionSet_Extended );
    lcd_command( Comm_ContrastSet         |  (contrast     & 0x0f) );
    lcd_command( Comm_PwrIconContrast     | ((contrast>>4) & 0x03) );
    lcd_command( Comm_FunctionSet_Normal   );
}

void SB1602E::put_custom_char( char c_code, const char *cg, int x, int y ) {
    for ( int i = 0; i < 5; i++ ) {
        set_CGRAM( c_code, cg );
        putcxy( c_code, x, y );
    }
}

void SB1602E::set_CGRAM( char char_code, const char* cg ) {
    for ( int i = 0; i < 8; i++ ) {
        lcd_command( (Comm_SetCGRAM | (char_code << 3) | i) );
        lcd_data( *cg++ );
    }
}

void SB1602E::set_CGRAM( char char_code, char v ) {
    char    c[ 8 ];

    for ( int i = 0; i < 8; i++ )
        c[ i ]    = v;

    set_CGRAM( char_code, c );
}

void SB1602E::clear_lest_of_line( int line ) {
    for ( int i = curs[ line ]; i < charsInLine; i++ )
        putcxy( ' ', i, line );
}

int SB1602E::lcd_write( uint8_t first, uint8_t second ) {
    uint8_t cmd[2];

    cmd[ 0 ]    = first;
    cmd[ 1 ]    = second;

    i2c_write_blocking(i2c1, i2c_addr, cmd, 2, false);
    return 1;

}

int SB1602E::lcd_command( char command ) {
    return ( lcd_write( COMMAND, command ) );
}

int SB1602E::lcd_data( char data ) {
    return ( lcd_write( DATA, data ) );
}
