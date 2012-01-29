// ---------------------------------------------------------------------------
// Created by Florian Fida on 20/01/12
// Copyright 2012 - Under creative commons license 3.0:
//        Attribution-ShareAlike CC BY-SA
//        http://creativecommons.org/licenses/by-sa/3.0/
//
// This software is furnished "as is", without technical support, and with no
// warranty, express or implied, as to its usefulness for any purpose.
// ---------------------------------------------------------------------------
// fio_shiftOut1 functions are based on Shif1 protocol developed by Roman Black (http://www.romanblack.com/shift1.htm)
//
// TODO:
//  support chipkit (https://github.com/chipKIT32/chipKIT32-MAX/blob/master/hardware/pic32/cores/pic32/wiring_digital.c)

#ifndef FAST_IO_H
#define FAST_IO_H

#include <pins_arduino.h> // pleasing sanguino core
#include <inttypes.h>
#include <util/delay.h>

#ifndef __AVR__
#define FIO_FALLBACK
#endif

typedef uint8_t fio_bit;
#define SKIP 0x23

#ifndef FALLBACK
typedef volatile uint8_t * fio_register;
#else
// remove volatile to give optimizer a chance
typedef uint8_t fio_register;
#endif

/*!
 @function
 @abstract  Get the output register for specified pin.
 @discussion if fast digital IO is disabled this function returns NULL
 @param  pin[in] Number of a digital pin
 @result  Register
 */
fio_register fio_pinToOutputRegister(uint8_t pin, uint8_t initial_state = LOW);

/*!
 @function
 @abstract  Get the input register for specified pin.
 @discussion if fast digital IO is disabled this function returns NULL
 @param  pin[in] Number of a digital pin
 @result  Register
 */
fio_register fio_pinToInputRegister(uint8_t pin);

/*!
 @function
 @abstract Find the bit which belongs to specified pin
 @discussion if fast digitalWrite is disabled this function returns the pin
 @param pin[in] Number of a digital pin
 @result Bit
 */
fio_bit fio_pinToBit(uint8_t pin);


/*!
 @method
 @abstract direct digital write
 @discussion without any checks
 @discussion falls back to normal digitalWrite if fast io is disabled
 @param pinRegister[in] Register - ignored if fast digital write is disabled
 @param pinBit[in] Bit - Pin if fast digital write is disabled
 @param value[in] desired output
 */
// __attribute__ ((always_inline)) /* let the optimizer decide that for now */
void fio_digitalWrite(fio_register pinRegister, fio_bit pinBit,uint8_t value);

#ifndef FIO_FALLBACK
#define fio_digitalWrite_LOW(reg,bit) *reg &= ~bit
#define fio_digitalWrite_SWITCH(reg,bit) *reg ^= bit
#define fio_digitalWrite_HIGH(reg,bit) *reg |= bit
#else
// reg -> dummy, bit -> pin
#define fio_digitalWrite_HIGH(reg,bit) digitalWrite(bit,HIGH)
#define fio_digitalWrite_SWITCH(reg,bit) digitalWrite(bit, !digitalRead(bit))
#define fio_digitalWrite_LOW(reg,bit) digitalWrite(bit,LOW)
#endif

/*!
 @function
 @abstract direct digital read
 @discussion without any checks
 @discussion falls back to normal digitalRead if fast io is disabled
 @param pinRegister[in] Register - ignored if fast io is disabled
 @param pinBit[in] Bit - Pin if fast io is disabled
 @result Value read from pin
 */
int fio_digitalRead(fio_register pinRegister, fio_bit pinBit);

/*!
 @method
 @abstract faster shift out
 @discussion using fast digital write
 @discussion falls back to normal digitalWrite if fastio is disabled
 @param dataRegister[in] Register of data pin - ignored if fast digital write is disabled
 @param dataBit[in] Bit of data pin - Pin if fast digital write is disabled
 @param clockRegister[in] Register of data pin - ignored if fast digital write is disabled
 @param clockBit[in] Bit of data pin - Pin if fast digital write is disabled
 */
void fio_shiftOut(fio_register dataRegister, fio_bit dataBit, fio_register clockRegister, fio_bit clockBit, uint8_t value);

/*!
 @method
 @abstract faster shift out clear
 @discussion using fast digital write
 @discussion falls back to normal digitalWrite if fastio is disabled
 @param dataRegister[in] Register of data pin - ignored if fast digital write is disabled
 @param dataBit[in] Bit of data pin - Pin if fast digital write is disabled
 @param clockRegister[in] Register of data pin - ignored if fast digital write is disabled
 @param clockBit[in] Bit of data pin - Pin if fast digital write is disabled
 */
void fio_shiftOut(fio_register dataRegister, fio_bit dataBit, fio_register clockRegister, fio_bit clockBit);

/*!
 * @method
 * @abstract one wire shift out
 * @discussion protocol needs initialisation (fio_shiftOut1_init)
 * @param shift1Register[in] pins register
 * @param shift1Bit[in] pins bit
 * @param value[in] value to shift out, last byte is ignored and always shifted out LOW
 */
void fio_shiftOut1(fio_register shift1Register, fio_bit shift1Bit, uint8_t value);
/*!
 * @method
 * @abstract one wire shift out
 * @discussion protocol needs initialisation (fio_shiftOut1_init)
 * @param pin[in] digital pin
 * @param value[in] value to shift out, last byte is ignored and always shifted out LOW
 */
void fio_shiftOut1(uint8_t pin, uint8_t value);
/*!
 * @method
 * @abstract initializes one wire shift out protocol
 * @discussion Puts pin to HIGH state and delays until Capacitors are charged.
 * @param shift1Register[in] pins register
 * @param shift1Bit[in] pins bit
 */
void fio_shiftOut1_init(fio_register shift1Register, fio_bit shift1Bit);
/*!
 * @method
 * @abstract initializes one wire shift out protocol
 * @discussion Puts pin to HIGH state and delays until Capacitors are charged.
 * @param pin[in] digital pin
 */
void fio_shiftOut1_init(uint8_t pin);

#endif // FAST_IO_H
