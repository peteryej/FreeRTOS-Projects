#include <SPI.h>
#include "FXOS8700CQ.h"
#include <FreeRTOS_ARM.h>

FXOS8700CQ sensor;


const byte interruptPin = 51;  //interrupt pin
volatile int countNum = 0;   //counting number of magnetometer interrupts

QueueHandle_t Magdatax_Queue_Handle;
QueueHandle_t Magdatay_Queue_Handle;
QueueHandle_t Magdataz_Queue_Handle;

//initalize z threshold
  uint16_t zThs = 0;
  uint16_t yThs = 0;
  uint16_t xThs = 0;

void setup() {
  //intiate task return variable
  portBASE_TYPE s1;
  pinMode(interruptPin, INPUT_PULLUP);
  
 //  Initialize SerialUSB 
  SerialUSB.begin(9600);
  while(!SerialUSB);

  // Initialize SPI
  SPI.begin();

  // Initialize sensor
  sensor = FXOS8700CQ();
  sensor.init();

  sensor.checkWhoAmI();
  // TODO: Check the sensor's WhoAmI value
  if (sensor.whoAmIData != FXOS8700CQ_WHOAMI_VAL){
    SerialUSB.println ("SPI_WhoAmI_ERROR");
  }
  
  //calibrate magnetormeter interrupt threashold 
  ThresholdCalibration();
  SerialUSB.print ("x threshold is: ");
  SerialUSB.println (xThs);
  SerialUSB.print ("y threshold is: ");
  SerialUSB.println (yThs);
  SerialUSB.print ("z threshold is: ");
  SerialUSB.println (zThs);
  //enable magnetometer interrupt
  sensor.enableInt(xThs, yThs, zThs);
  
  //creat data queue
  Magdatax_Queue_Handle = xQueueCreate(100,sizeof(int16_t));
  Magdatay_Queue_Handle = xQueueCreate(100,sizeof(int16_t));
  Magdataz_Queue_Handle = xQueueCreate(100,sizeof(int16_t));

  //enable Arduino interrupt pin
  attachInterrupt(digitalPinToInterrupt(interruptPin), collectData, FALLING);
  
  s1 =  xTaskCreate(processData, NULL,300, NULL, 1, NULL);
  
  //start task scheduler
  vTaskStartScheduler();
//  while(1);
}

//collect magdata values 
void collectData(void){
 // detachInterrupt(digitalPinToInterrupt(interruptPin));
 // sensor.readMagData();
  uint8_t xInt =1;
  int x = xQueueSendFromISR(Magdatax_Queue_Handle, &(xInt), NULL);
  //int x = xQueueSend(Magdatax_Queue_Handle, &(sensor.magData.x), 5000);
  //int y = xQueueSend(Magdatax_Queue_Handle, &(sensor.magData.y), 5000);
  //int z = xQueueSend(Magdatax_Queue_Handle, &(sensor.magData.z), 5000);
//  if ((!x)||(!y)||(!z)) {
//    SerialUSB.println("Failed to send magdata from queue. Queue is full.");
//  }

  countNum ++;
// attachInterrupt(digitalPinToInterrupt(interruptPin), collectData, FALLING);
}

//output number of times interrupt happens 
static void processData(void* arg){
  while(1){
//  FXOS8700CQ::SRAWDATA MagData;
  sensor.readMagData();
//  int x = xQueueReceive(Magdatax_Queue_Handle, &(MagData.x), 5000);
//  int y = xQueueReceive(Magdatax_Queue_Handle, &(MagData.y), 5000);
  int z = xQueueReceive(Magdatax_Queue_Handle, &(sensor.magData.z), 5000);
//  if ((!x)||(!y)||(!z)) {
  if (!z) {
//    SerialUSB.println("Failed to receive mag data from queue. Queue is empty.");
  }
  else{
//    countNum ++;
//    SerialUSB.print("Mag X: "); 
//    SerialUSB.println(MagData.x);
//    SerialUSB.print("Mag Y: ");
//    SerialUSB.println(MagData.y);
    SerialUSB.print("Mag Z: " );
    SerialUSB.println(sensor.magData.z); 
    
    SerialUSB.print("Num of interrupts: ");  
    SerialUSB.println(countNum);
  }
  vTaskDelay(500);
  }
}

//calibrate the threshold of the magnetometer interrupt
void ThresholdCalibration(){
  SerialUSB.println("calibrating... ");
  int16_t magx[100];
  int16_t magy[100];
  int16_t magz[100];
  long sumx, sumy, sumz;
  long sumdiffx, sumdiffy, sumdiffz;
  long stdvx, stdvy, stdvz;
  long avex, avey, avez;
  sumx =0;
  sumy = 0;
  sumz=0;
  sumdiffx=0;
  sumdiffy=0;
  sumdiffz=0;
  stdvx =0;
  stdvy=0;
  stdvz=0;
  avex=0;
  avey=0;
  avez=0;
  for (int i=0;i<100;i++){
    sensor.readMagData();
    magx[i] = sensor.magData.x;
    magy[i] = sensor.magData.y;
    magz[i] = sensor.magData.z;
    sumx += (long)sensor.magData.x;
    sumy += (long)sensor.magData.y;
    sumz += (long)sensor.magData.z;
    SerialUSB.print("all X: "); 
    SerialUSB.println(magx[i]);
    delay(100);
    
  }
  avex = (sumx/100);
  avey = (sumy/100);
  avez = (sumz/100);
  SerialUSB.print("average X: "); 
  SerialUSB.println(avex);
  SerialUSB.print("average Y: ");
  SerialUSB.println(avey);
  SerialUSB.print("average Z: " );
  SerialUSB.println(avez);  
  for (int i=0;i<100;i++){
    sumdiffx += sq(magx[i]-avex);
    sumdiffy += sq(magy[i]-avey);
    sumdiffz += sq(magz[i]-avez);
  }
  SerialUSB.print("sumdiff X: "); 
  SerialUSB.println(sumdiffx);
  SerialUSB.print("sumdiff y: "); 
  SerialUSB.println(sumdiffy);
  SerialUSB.print("sumdiff z: "); 
  SerialUSB.println(sumdiffz);
  stdvx = sqrt(sumdiffx/100);
  stdvy = sqrt(sumdiffy/100);
  stdvz = sqrt(sumdiffz/100);
  SerialUSB.print("std dev X: "); 
  SerialUSB.println(stdvx);
  SerialUSB.print("std dev Y: ");
  SerialUSB.println(stdvy);
  SerialUSB.print("std dev Z: " );
  SerialUSB.println(stdvz);    
  zThs = abs(avez)+stdvz*5;
  yThs = abs(avey)+stdvy*5;
  xThs = abs(avex)+stdvx*5;
  SerialUSB.println("Done calibration.");
}

void loop() {
  // TO DO: Read Mag data. Get all parameters 
//  sensor.readMagData();
//  SerialUSB.print("magX: "); 
//  SerialUSB.println(sensor.magData.x);
//  SerialUSB.print("magY: ");
//  SerialUSB.println(sensor.magData.y);
//  SerialUSB.print("magZ: " );
//  SerialUSB.println(sensor.magData.z);
//  delay(500);
}
