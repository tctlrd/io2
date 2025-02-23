/*
 * IO2.cpp
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
 * 2024/07/15 - Peter Valkov
 */

#include "IO2.h"
#include <Wire.h>
#include <Arduino.h>

/*
 * Constructors
 */
IO2::IO2(uint8_t addr, uint8_t sda = SDA, uint8_t scl = SCL) {
  address = addr;
  error = IO2_SUCCESS;

  Wire.begin(sda, scl);
}

bool IO2::hasError() {
	return error != IO2_SUCCESS;
}

uint8_t IO2::getError() {
	uint8_t e = error;
	error = IO2_SUCCESS;
	return e;
}

uint8_t IO2::regRead(uint8_t reg) {
  if (hasError()) {
    // Previous error is not cleared
    return 0xFF;
  }
  Wire.beginTransmission(address);
  Wire.write(reg);
  error = Wire.endTransmission();
  if (error != IO2_SUCCESS) {
    return 0xFF;
  }
  uint8_t count = Wire.requestFrom((int)address, 1);
  if (count != 1) {
	error = I2C_ERROR;
	return 0xFF;
  }
  return Wire.read();
}

uint16_t IO2::wordRead(uint8_t reg) {
  if (hasError()) {
    // Previous error is not cleared
    return 0xFFFF;
  }
  uint16_t value;
  Wire.beginTransmission(address);
  Wire.write(reg);
  error = Wire.endTransmission();	
  if (error != IO2_SUCCESS) {
    return 0xFFFF;
  }
  uint8_t count = Wire.requestFrom((int)address, 2);
  if (count != 2) {
	error = I2C_ERROR;
    return 0xFFFF;
  }
  value = Wire.read();
  return (Wire.read() << 8) | value;
}

void IO2::regWrite(uint8_t reg, uint8_t value) {
  if (hasError()) {
    // Previous error is not cleared
    return;
  }
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(value);
  error = Wire.endTransmission();
}

uint8_t IO2::getAddress() {
  return address;
}

void IO2::setAddress(uint8_t newAddress) {
  uint8_t oldAddress = address;
  regWrite(SET_ADDRESS, newAddress);
  if (hasError()) {
     return;
  }

  address = newAddress;
  delay(100);

  if (readID() != IO2_BOARD_ID) {
    error = IO2_NOT_FOUND;
    address = oldAddress;
  }
}

uint8_t IO2::detect() {
  for (uint8_t a = 0x01; a < 0x80; a++) {
    address = a;
	
    uint8_t boardID = readID(); 
	if (getError() != IO2_SUCCESS) {
		continue;
	}
	
	if (boardID == IO2_BOARD_ID) {
		return address;
	}
  }
  error = IO2_NOT_FOUND;
  return 0xFF;
}

void IO2::setRelay(uint8_t relay, uint8_t state) {
  if(state == 1)
    relay_status |= relay;
  else if(state == 0)
    relay_status &= ~relay;

  regWrite(SET_RELAY, relay_status);
}

uint16_t IO2::analogRead(uint8_t pin) {
  return wordRead(pin);
}

void IO2::pinMode(uint8_t pin, uint8_t mode) {
  tris_status = regRead(SET_TRIS);
  if(mode == 1)
    tris_status |= pin;
  else if(mode == 0)
    tris_status &= ~pin;

  regWrite(SET_TRIS, tris_status);
}

void IO2::digitalWrite(uint8_t pin, uint8_t level) {
  lat_status = regRead(GET_PORT);
  if(level == 1) {
    lat_status |= pin;
  } else if(level == 0) {
    lat_status &= ~pin;
  }

  regWrite(SET_LAT, lat_status);
}

void IO2::setPullpin(uint8_t pin, uint8_t state) {
  if(state == 1) {
    pp_status |= pin;
  } else if(state == 0) {
    pp_status &= ~pin;
  }
  
  regWrite(SET_LAT, pp_status);
}

uint8_t IO2::digitalRead(uint8_t pin) {
  return ((regRead(GET_PORT) & pin) == 0 ? 0 : 1);
}

uint8_t IO2::readID() {
  uint8_t boardID = regRead(GET_ID);
  if (hasError()) {
    return 0xFF;
  }
  if (boardID != IO2_BOARD_ID) {
    error = IO2_NOT_FOUND;
  }
  return boardID;
}

uint8_t IO2::readRevision() {
  return regRead(GET_REV);
}

void IO2::setGPIO2_DAC(uint8_t level) {
  level &= 0x1F;
  regWrite(SET_GPIO2_DAC, level);
}

void IO2::pwmSet (uint8_t pwm, uint8_t duty) {
	regWrite(pwm, duty);
}

void IO2::pwmOff (uint8_t pwm) {
	regWrite(PWM_OFF, pwm & 0x0F);
}
