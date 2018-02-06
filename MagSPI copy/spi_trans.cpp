// Reading and Writing the SPI bus

//------------------------------------------------------------------------------
// Headers
//------------------------------------------------------------------------------
#include "spi_trans.h"

//******************************************************************************
// Function Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// spi_write_cmd(): Transmit to a SPI device
//------------------------------------------------------------------------------
void spi_write_cmd(uint8_t address, uint8_t tx_data) {
    // TO DO:
  SPI.transfer((char)(address|0x80));   //send LSByte address first 
  SPI.transfer((char)((address)&0x80));      //send MSB address
  SPI.transfer(tx_data); //Send register location
}

//------------------------------------------------------------------------------
// spi_read_cmd(): Read from a SPI device
//------------------------------------------------------------------------------
uint8_t spi_read_cmd(uint8_t address) {
  uint8_t rx_data;
    // TO DO:
  SPI.transfer((char)(address&0x7f));   //send LSByte address first 
  SPI.transfer((char)((address)&0x80));      //send MSB address  
  rx_data = SPI.transfer(0x00); //get data byte      
  return rx_data;
}

//*****************************************************************************
