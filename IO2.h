/*
 * IO2.h
 * 
 * Copyright 2013 OLIMEX LTD/Stefan Mavrodiev <support@olimex.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 *
 * Updated for the new firmware of MOD-IO2 (4.3)
 * 2022/05/13 - Stanimir Petev
 */

#ifndef IO2_H
#define IO2_H

#include <inttypes.h>

#define DEFAULT_ADDRESS   0x21
#define IO2_BOARD_ID      0x23

#define IO2_SUCCESS        0

#define I2C_DATA_TOO_LONG  1
#define I2C_ADDRESS_NAK    2
#define I2C_DATA_NAK       3
#define I2C_ERROR          4
#define I2C_TIMEOUT        5

#define IO2_WRONG_ID      15
#define IO2_NOT_FOUND     16

#define IN    1
#define OUT   0
#define LO    0
#define HI    1
#define ON    1
#define OFF   0

#define RELAY1        0x01
#define RELAY2        0x02

#define GPIO0         0x01
#define GPIO1         0x02
#define GPIO2         0x04
#define GPIO3         0x08
#define GPIO4         0x10
#define GPIO5         0x20
#define GPIO6         0x40

#define AN0           0x10
#define AN1           0x11
#define AN2           0x12
#define AN3           0x13
#define AN5           0x15
#define AN6           0x16

#define SET_RELAY     0x40
#define SET_GPIO2_DAC 0x60
#define SET_ADDRESS   0xF0

#define SET_TRIS      0x01
#define SET_LAT       0x02
#define GET_PORT      0x03
#define SET_PU        0x04

#define GET_ID        0x20
#define GET_REV       0x21

#define PWM_OFF       0x50
#define PWM1          0x51
#define PWM2          0x52

#ifndef SDA
#define SDA 21
#endif

#ifndef SCL
#define SCL 22
#endif

class IO2 {
	protected:
		uint8_t address;
		uint8_t error;
		uint8_t relay_status;
		uint8_t tris_status;
		uint8_t lat_status;
		uint8_t pp_status;

		uint8_t regRead(uint8_t reg);
		uint16_t wordRead(uint8_t reg);
		void regWrite(uint8_t reg, uint8_t value);

	public:
		IO2(uint8_t addr = DEFAULT_ADDRESS, uint8_t sda = SDA, uint8_t scl = SCL);

        bool hasError();
        uint8_t getError();

		uint8_t getAddress();
		void setAddress(uint8_t newAddress);

		uint8_t detect();

		void setRelay(uint8_t relay, uint8_t state);
		void pinMode(uint8_t pin, uint8_t mode);
		void digitalWrite(uint8_t pin, uint8_t level);
		uint8_t digitalRead(uint8_t pin);
		uint16_t analogRead(uint8_t pin);
		void setPullpin(uint8_t pin, uint8_t state);
		uint8_t readID();
		uint8_t readRevision();
		void setGPIO2_DAC (uint8_t level);
		void pwmSet (uint8_t pin, uint8_t duty);
		void pwmOff (uint8_t pin);
};

#endif
