/*
  TinyWireS.cpp - a wrapper class for Don Blake's usiTwiSlave routines.
  Provides TWI/I2C Slave functionality on ATtiny processers in Arduino environment.
  1/23/2011 BroHogan -  brohoganx10 at gmail dot com

  **** See TinyWireS.h for Credits and Usage information ****

  This library is free software; you can redistribute it and/or modify it under the
  terms of the GNU General Public License as published by the Free Software
  Foundation; either version 2.1 of the License, or any later version.
  This program is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
  PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

extern "C" {
  #include <inttypes.h>
  #include "usiTwiSlave.h"
  }

#include "TinyWireS84.h"

void (*USI_TWI_S::user_onRequest)(void);

// Constructors ////////////////////////////////////////////////////////////////

USI_TWI_S::USI_TWI_S(){
}


// Public Methods //////////////////////////////////////////////////////////////

void USI_TWI_S::begin(uint8_t slaveAddr){ // initialize I2C lib
  usiTwiSlaveInit(slaveAddr); 
  twi_attachSlaveTxEvent(onRequestService);
}

void USI_TWI_S::send(uint8_t data){  // send it back to master
  usiTwiTransmitByte(data);
}

uint8_t USI_TWI_S::available(){ // the bytes available that haven't been read yet
  return usiTwiDataInReceiveBuffer(); 
}
 
uint8_t USI_TWI_S::receive(){ // returns the bytes received one at a time
  return usiTwiReceiveByte(); 
}

// behind the scenes function that is called when data is requested
void USI_TWI_S::onRequestService(void)
{
  // don't bother if user hasn't registered a callback
  if(!user_onRequest){
    return;
  }
  // reset tx buffer iterator vars
  // !!! this will kill any pending pre-master sendTo() activity
  //txBufferIndex = 0;
  //txBufferLength = 0;
  // alert user program
  user_onRequest();
}

// sets function called on slave read
void USI_TWI_S::onRequest( void (*function)(void) )
{
  user_onRequest = function;
}

// Preinstantiate Objects //////////////////////////////////////////////////////

USI_TWI_S TinyWireS = USI_TWI_S();

