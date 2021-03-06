
/* Copyright (C) 2011 Keith Wilson.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the names of the authors or their
 * institutions shall not be used in advertising or otherwise to promote the
 * sale, use or other dealings in this Software without prior written
 * authorization from the authors.
 */

/*! \file lpc23xx-uart.h
*/

#ifndef _LPC23XX_UART_H
#define _LPC23XX_UART_H

#include <stdint.h>
#include <stdbool.h>
#include "ringbuffer.h"

#define         UART_MAXBUFFER            250

/*
 * uart pins and register masks
 */

#define         UART_INTST_BIT            0
#define         UART_INTSOURCE_BITS       0b1110

#define         UART_RBRIE_BIT            0
#define         UART_THREIE_BIT           1
#define         UART_RXLSIE_BIT           2

#define         UART0_INTR_PRIORITY       12
#define         UART0_RBR_INT_ENABLE      ( U0IER = U0IER | (1<<UART_RBRIE_BIT ) )
#define         UART0_THRE_INT_ENABLE     ( U0IER = U0IER | (1<<UART_THREIE_BIT) )
#define         UART0_RXLS_INT_ENABLE     ( U0IER = U0IER | (1<<UART_RXLSIE_BIT) )

#define         UART0_RBR_INT_DISABLE     ( U0IER = U0IER & ( ~(1<<UART_RBRIE_BIT ) ) )
#define         UART0_THRE_INT_DISABLE    ( U0IER = U0IER & ( ~(1<<UART_THREIE_BIT) ) )
#define         UART0_RXLS_INT_DISABLE    ( U0IER = U0IER & ( ~(1<<UART_RXLSIE_BIT) ) )

#define         P0_RXD0_TXD0_MASK         (~(0xF0))
#define         P0_RXD0_TXD0_ENABLE       0x50

#define         SET_RXD0_TXD0             ( PINSEL0 = ( ( PINSEL0 & P0_RXD0_TXD0_MASK ) | P0_RXD0_TXD0_ENABLE ) )
#define         CLEAR_RXD0_TXD0           ( PINSEL0 &= P0_RXD0_TXD0_MASK )

#define         P0_RXD2_TXD2_MASK         (~(0xF00000))
#define         P0_RXD2_TXD2_ENABLE       0x500000

#define         SET_RXD2_TXD2             ( PINSEL0 = ( ( PINSEL0 & P0_RXD2_TXD2_MASK ) | P0_RXD2_TXD2_ENABLE ) )
#define         CLEAR_RXD2_TXD2           ( PINSEL0 &= P0_RXD2_TXD2_MASK )

#define         P0_RXD3_TXD3_MASK         (~(0xF))
#define         P0_RXD3_TXD3_ENABLE       0xA

#define         SET_RXD3_TXD3             ( PINSEL0 = ( ( PINSEL0 & P0_RXD3_TXD3_MASK ) | P0_RXD3_TXD3_ENABLE ) )
#define         CLEAR_RXD3_TXD3           ( PINSEL0 &= P0_RXD3_TXD3_MASK )

#define         P0_TXD1_MASK              (~(0xC000))
#define         P0_TXD1_ENABLE            0x40000000

#define         SET_TXD1                  ( PINSEL0 = ( ( PINSEL0 & P0_TXD1_MASK ) | P0_TXD1_ENABLE ) )
#define         CLEAR_TXD1                ( PINSEL0 &= P0_TXD1_MASK )

#define         P1_RXD1_MASK              (~(0x3))
#define         P1_RXD1_ENABLE            0x1

#define         SET_RXD1                  ( PINSEL1 = ( ( PINSEL1 & P1_RXD1_MASK ) | P1_RXD1_ENABLE ) )
#define         CLEAR_RXD1                ( PINSEL1 &= P1_RXD1_MASK )

#define         DLAB_MASK                 ( ~((0x1)<<7) )
#define         SET_DLAB0                 ( U0LCR = ( U0LCR  | (0x1<<7) ) )
#define         CLEAR_DLAB0               ( U0LCR = (U0LCR & DLAB_MASK) )
#define         SET_DLAB2                 ( U2LCR = (( U2LCR & DLAB_MASK)  | (1<<7)))
#define         CLEAR_DLAB2               ( U2LCR = (U2LCR & DLAB_MASK) )
#define         SET_DLAB3                 ( U3LCR = (( U3LCR & DLAB_MASK)  | (1<<7)))
#define         CLEAR_DLAB3               ( U3LCR = (U3LCR & DLAB_MASK) )

#define         UART0_8N1                 ( U0LCR = 0x3 )
#define         UART2_8N1                 ( U2LCR = 0x3 )
#define         UART3_8N1                 ( U3LCR = 0x3 )

#define         UART0_FCR_ONE_CHAR_EN     ( U0FCR = 0x1 | 0x6 )
#define         UART2_FCR_ONE_CHAR_EN     ( U0FCR = 0x1 | 0x6 )
#define         UART3_FCR_ONE_CHAR_EN     ( U0FCR = 0x1 | 0x6 )

#define         U0THR_THRE_MASK           ( (0x1 << 0x5) )
#define         U0THR_EMPTY               ( (U0LSR & U0THR_THRE_MASK ) >> 0x5 )

#define         U0THR_THRE_MASK           ( (0x1 << 0x5) )
#define         U0THR_EMPTY               ( (U0LSR & U0THR_THRE_MASK ) >> 0x5 )

#define         U0LSR_RDR_MASK            ( ( 0x1 ) )
#define         U0RDR_READY               ( (U0LSR & U0LSR_RDR_MASK ) )

enum UARTLSR { LSR_RDR=0, LSR_OE, LSR_PE, LSR_FE, LSR_BI, LSR_THRE, LSR_TEMT, LSR_RXFE};

typedef enum Baudtype {
    ZERO_H_B              = 0,
    TWELVE_H_B            = 1200,
    FOURTY_EIGHT_H_B      = 4800,
    NINETY_SIX_H_B        = 9600,
    NINETEEN_TWO_H_B      = 19200,
    THIRTY_EIGHT_FOUR_H_B = 38400,
    FIFTY_SEVEN_SIX_H_B   = 57600,
    ONE_FIFTEEN_TWO_H_B   = 115200
} Baud;

typedef void (uart_getstring_cb)(char* line);

typedef enum {UART0=0, UART1, UART2, UART3} uartport;

typedef enum {UART_RLS = 0b0011,
    UART_RDA = 0b0010,
    UART_CTI = 0b0110,
    UART_THRE= 0b0001
} uart_iir_intid;

typedef struct  {
    uint8_t           uart_rdr;
    uint8_t           uart_oe;
    uint8_t           uart_pe;
    uint8_t           uart_fe;
    uint8_t           uart_bi;
    uint8_t           uart_thre;
    uint8_t           uart_temt;
    uint8_t           uart_rxfe;
} uart_lsr_status;

typedef struct {
    Baud              baudrate;
    uint8_t           charlength;
    uint8_t           stopbits;
} uart_status;

extern uart_status    uart0_status;

extern Ringbuffer     uart0_rb_rx_g;
extern Ringbuffer     uart0_tx_rb_g;
extern bool           uart0_kick_thr_int_g;

void           uart_enable_interrupt(uartport u);

void           uart_disable_interrupt(uartport u);

Baud           uart0_query_baud(void) ;
uint8_t        uart0_query_charlength(void) ;
uint8_t        uart0_query_stopbits(void) ;

void           uart0_init_9600(void) ;
void           uart0_init_115200(void) ;

bool           uart0_init_rb() ;

uart_iir_intid get_uart0_iir_intid(uint32_t u0iir);
void           get_uart0_lsr_status(uart_lsr_status* lsrstatus) ;

void           uart0_interrupt_service(void) __attribute__ ((interrupt("IRQ"), optimize("00") ));

volatile bool           uart0_putchar_intr(char ch) ;
void           uart0_putchar(char ch) ;

bool           uart0_putstring_intr(const char *s) ;
void           uart0_putstring(const char *s) ;

char           uart0_getchar_intr(void);
char           uart0_getchar (void) ;

char*          uart0_getstring (void) ;
char*          uart0_getstring_intr (void) ;

void           uart0_set_getstring_cb(uart_getstring_cb cb);
#endif

