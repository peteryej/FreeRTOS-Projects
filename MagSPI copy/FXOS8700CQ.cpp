#include "FXOS8700CQ.h"
#include <math.h>

//******************************************************************************
// Public Function Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// FXOS8700CQ(): Initialize configuration parameters
//------------------------------------------------------------------------------
FXOS8700CQ::FXOS8700CQ() {
		magODR = MODR_100HZ; // Magnetometer data/sampling rate
		magOSR = MOSR_5;     // Choose magnetometer oversample rate

    pinMode(CS_PIN, OUTPUT);        // Select the GPIO Pin 4 as SPI Chip Select
    digitalWrite(CS_PIN, HIGH);     // Set Pin to high (Active Low)
    
}

//------------------------------------------------------------------------------
// writeReg(): Writes to a register
//------------------------------------------------------------------------------
void FXOS8700CQ::writeReg(byte reg, byte data) {
     // TO DO: 
    digitalWrite(CS_PIN, LOW);
    spi_write_cmd(reg, data);
    digitalWrite(CS_PIN, HIGH);
}

//------------------------------------------------------------------------------
// readReg(): Reads from a register
//------------------------------------------------------------------------------
uint8_t FXOS8700CQ::readReg(byte reg) {
    // TO DO:
   uint8_t rx_data; 
   digitalWrite(CS_PIN,LOW);
   rx_data = spi_read_cmd(reg);
   digitalWrite(CS_PIN, HIGH);
   return rx_data;
}

//------------------------------------------------------------------------------
// readmagdata(): Read the magnometer X, Y and Z axisdata
//------------------------------------------------------------------------------
void FXOS8700CQ::readMagData() {
		// TO DO
    magData.x =  ((uint16_t)readReg(FXOS8700CQ_M_OUT_X_MSB)<<8)|readReg(FXOS8700CQ_M_OUT_X_LSB); 
    magData.y =  ((uint16_t)readReg(FXOS8700CQ_M_OUT_Y_MSB)<<8)|readReg(FXOS8700CQ_M_OUT_Y_LSB);
    magData.z =  ((uint16_t)readReg(FXOS8700CQ_M_OUT_Z_MSB)<<8)|readReg(FXOS8700CQ_M_OUT_Z_LSB);   
}

//------------------------------------------------------------------------------
// standby(): Put the FXOS8700CQ into standby mode for writing to registers
//------------------------------------------------------------------------------
void FXOS8700CQ::standby() {
		// TO DO:
    uint8_t databyte = 0; 
    databyte = readReg(FXOS8700CQ_CTRL_REG1);
    databyte = databyte & 0xfe;     
    writeReg(FXOS8700CQ_CTRL_REG1, databyte); // set ODR and standby mode
}

//------------------------------------------------------------------------------
// active(): Put the FXOS8700CQ into active mode to output data
//------------------------------------------------------------------------------
void FXOS8700CQ::active() {
		// TO DO:
    uint8_t databyte; 
    databyte = readReg(FXOS8700CQ_CTRL_REG1);
    databyte = databyte | 1;    
    writeReg(FXOS8700CQ_CTRL_REG1, databyte); //set to active mode
}

//------------------------------------------------------------------------------
// init(): Initialize the magnetometer
//         This function will put the magnetometer in standby mode, modify the 
//         registers that put the device in mag-only mode, set the correct data
//         rate (ODR) and oversampling rate (OSR) for the magnetometer and put
//         it back in active mode
//------------------------------------------------------------------------------
void FXOS8700CQ::init() {
    // TO DO:
    uint8_t databyte=0;
    standby();
    databyte = databyte | (magODR << 3); 
    writeReg(FXOS8700CQ_CTRL_REG1, databyte); // set ODR 
    databyte = readReg(FXOS8700CQ_CTRL_REG1);
    uint8_t lower5 = (magOSR<<2)| 1;
    databyte = databyte | lower5; //set OSR and mag only mode
    writeReg(FXOS8700CQ_M_CTRL_REG1, databyte);
    writeReg(FXOS8700CQ_M_CTRL_REG3, 0x80);
    active();
}

//------------------------------------------------------------------------------
// checkWhoAmI(): Check the whoAmI register
//------------------------------------------------------------------------------
void FXOS8700CQ::checkWhoAmI(void) {
    // TO DO:
    whoAmIData = readReg(FXOS8700CQ_WHO_AM_I);
}

void FXOS8700CQ::disableInt(void){
 // standby();
//  writeReg(FXOS8700CQ_CTRL_REG4, 0); //disable interrupts
  writeReg(FXOS8700CQ_M_THS_CFG , 0x0); 
//  active();
}

void FXOS8700CQ::enableInt(uint16_t xThs, uint16_t yThs,uint16_t zThs){
 // readReg(FXOS8700CQ_TRANSIENT_CFG);
 //uint8_t transBit = 0x20;  
 //uint8_t sampleCount = 0x0a;  //10 samples, means 0.1 s 
// xThs =4000;
// yThs =4000;
// zThs =4000;
 uint8_t xMSBs, yMSBs, zMSBs,xLSBs, yLSBs,zLSBs; 
 xMSBs =0;
 yMSBs =0;
 zMSBs =0;
 xLSBs =0;
 yLSBs =0;
 zLSBs =0;
 xMSBs = (xThs >> 8);
 yMSBs = (yThs >> 8);
 zMSBs = (zThs >> 8);
  
 xLSBs = (xThs|xLSBs) ;
 yLSBs = (yThs|yLSBs);
 zLSBs = (zThs|zLSBs );

// standby();
 
// writeReg(FXOS8700CQ_CTRL_REG4, transBit); //enable transient interrupt
// writeReg(FXOS8700CQ_CTRL_REG5, transBit); //route to INT1 pin
 writeReg(FXOS8700CQ_M_THS_CFG , 0x7b); // config magnetometer transient interrupt setting

 writeReg(FXOS8700CQ_M_THS_X_MSB, xMSBs);
 writeReg(FXOS8700CQ_M_THS_X_LSB, xLSBs);
 writeReg(FXOS8700CQ_M_THS_Y_MSB, yMSBs);
 writeReg(FXOS8700CQ_M_THS_Y_LSB, yLSBs);
 writeReg(FXOS8700CQ_M_THS_Z_MSB, zMSBs);
 writeReg(FXOS8700CQ_M_THS_Z_LSB, zLSBs);

// writeReg(FXOS8700CQ_M_THS_COUNT, sampleCount); //set sample counts before next interrupt
// active();
}

//------------------------------------------------------------------------------
// read interrupt source
//------------------------------------------------------------------------------
void FXOS8700CQ::readIntSource(void){
   // intSource = readReg(FXOS8700CQ_M_INT_SRC); //Magnetometer interrupt source reg
    intSource = readReg(FXOS8700CQ_M_THS_SRC); //Magnetometer threshold interrupt source reg
    
}
//*****************************************************************************
