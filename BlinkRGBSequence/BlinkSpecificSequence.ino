/*
  BlinkSpecificSequence 
  Switch bewteen the RGB LEDs on PowerDue board for a period of time each repeatedly.

  Created by Peter Ye
  Date: 9/14/2017
*/

//include libraries
#include <FreeRTOS_ARM.h>

//declear global variables
#define LED_RGB_RED 6
#define LED_RGB_GREEN 7
#define LED_RGB_BLUE 8


SemaphoreHandle_t  sem;


// the setup function runs once when you press reset or power the board
void setup() {
  
  portBASE_TYPE s1, s2, s3, s4, s5, s6, s7;
  
  //Initialize digital pin 6, 7 and 8 as output.
  pinMode(LED_RGB_RED, OUTPUT);
  pinMode(LED_RGB_GREEN, OUTPUT);
  pinMode(LED_RGB_BLUE, OUTPUT);
  //Turn off all RGB LEDs on start-up
  digitalWrite(LED_RGB_RED, LOW);
  digitalWrite(LED_RGB_GREEN, LOW);
  digitalWrite(LED_RGB_BLUE, LOW);

  //begin serial
  SerialUSB.begin(9600);
  while(!SerialUSB) {}
  
  //Initialize semaphore
  sem = xSemaphoreCreateCounting(1,1);

  //create tasks
  s1 =  xTaskCreate(RedLEDblink_task, NULL, 512, NULL, 3, NULL);
  s2 = xTaskCreate(GreenLEDblink_task, NULL, 512, NULL, 3, NULL);  
  s3 =  xTaskCreate(BlueLEDblink_task, NULL, 512, NULL, 3, NULL);
  s4 = xTaskCreate(RedGreenLEDblink_task, NULL, 512, NULL, 3, NULL);
  s5 = xTaskCreate(RedBlueLEDblink_task, NULL, 512, NULL, 3, NULL);
  s6 = xTaskCreate(GreenBlueLEDblink_task, NULL, 512, NULL, 3, NULL);
  s7 = xTaskCreate(RedGreenBlueLEDblink_task, NULL, 512, NULL, 3, NULL);

  
  //start task scheduler
  vTaskStartScheduler();
  
}

//task thread of turning on red LED for 500ms and off for 500ms
static void RedLEDblink_task(void* arg){
  while (1) {
  //Wait until a semaphore is released
  if (xSemaphoreTake(sem, 9000)){
  
  //Turn RED LED on
  SerialUSB.println("Turning on the Red LED");
  digitalWrite(LED_RGB_RED, HIGH);

  //Sleep for 500 milliseconds
  vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);

  //Turn RED LED off
  SerialUSB.println("Turning off the Red LED");
  digitalWrite(LED_RGB_RED, LOW);
  
  //Sleep for 500 milliseconds
  vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);

  //Done with the thread
  SerialUSB.println("Thread done");

  //gives semaphore
  xSemaphoreGive(sem);  
  }else{
    SerialUSB.println("Failed to abtain semaphore");
  }
  vTaskDelay(1000);
  }
}

static void BlueLEDblink_task(void* arg){
  while (1) {
  //Wait until a semaphore is released
  if (xSemaphoreTake(sem, 9000)){
  //Turn Blue LED on
  SerialUSB.println("Turning on the blue LED");
  digitalWrite(LED_RGB_BLUE, HIGH);

  //Sleep for 500 milliseconds
  vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);

  //Turn Blue LED off
  SerialUSB.println("Turning off the blue LED");
  digitalWrite(LED_RGB_BLUE, LOW);
  
  //Sleep for 500 milliseconds
  vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);

  //Done with the thread
  SerialUSB.println("Thread done");
  
  //gives semaphore
  xSemaphoreGive(sem); 
  }else{
    SerialUSB.println("Failed to abtain semaphore");
  }  
  vTaskDelay(1000);
  }
}

static void GreenLEDblink_task(void* arg){
  while (1) {
  //Wait until a semaphore is released
  if (xSemaphoreTake(sem, 9000)){
  //Turn Green LED on
  SerialUSB.println("Turning on the green LED");
  digitalWrite(LED_RGB_GREEN, HIGH);

  //Sleep for 500 milliseconds
  vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);

  //Turn Green LED off
  SerialUSB.println("Turning off the Green LED");
  digitalWrite(LED_RGB_GREEN, LOW);
  
  //Sleep for 500 milliseconds
  vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);

  //Done with the thread
  SerialUSB.println("Thread done");

  //gives semaphore
  xSemaphoreGive(sem);
  }else{
    SerialUSB.println(F("Failed to abtain semaphore"));
  }  
  vTaskDelay(1000);
  }
  
}

static void RedGreenLEDblink_task(void* arg){
  while (1) {
  //Wait until a semaphore is released
  if (xSemaphoreTake(sem, 9000)){
  //Turn Green LED on
  SerialUSB.println("Turning on the red and green LED");
  digitalWrite(LED_RGB_GREEN, HIGH);
  digitalWrite(LED_RGB_RED, HIGH);
  
  //Sleep for 500 milliseconds
  vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);

  //Turn Green LED off
  SerialUSB.println("Turning off the Red and Green LED");
  digitalWrite(LED_RGB_GREEN, LOW);
  digitalWrite(LED_RGB_RED, LOW);
  
  //Sleep for 500 milliseconds
  vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);

  //Done with the thread
  SerialUSB.println("Thread done");

  //gives semaphore
  xSemaphoreGive(sem);
  }else{
    SerialUSB.println(F("Failed to abtain semaphore"));
  }  
  vTaskDelay(1000);
  }
  
}

static void RedBlueLEDblink_task(void* arg){
  while (1) {
  //Wait until a semaphore is released
//   xSemaphoreTake(sem, portMAX_DELAY);
  if (xSemaphoreTake(sem, 9000)){
  //Turn Green LED on
  SerialUSB.println("Turning on the red and green LED");
  digitalWrite(LED_RGB_BLUE, HIGH);
  digitalWrite(LED_RGB_RED, HIGH);
  
  //Sleep for 500 milliseconds
  vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);

  //Turn Green LED off
  SerialUSB.println("Turning off the Red and Green LED");
  digitalWrite(LED_RGB_BLUE, LOW);
  digitalWrite(LED_RGB_RED, LOW);
  
  //Sleep for 500 milliseconds
  vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);

  //Done with the thread
  SerialUSB.println("Thread done");

  //gives semaphore
  xSemaphoreGive(sem);
  }else{
    SerialUSB.println(F("Failed to abtain semaphore"));
  }  
  vTaskDelay(1000);
  }
  
}

static void GreenBlueLEDblink_task(void* arg){
  while (1) {
  //Wait until a semaphore is released
//   xSemaphoreTake(sem, portMAX_DELAY);
  if (xSemaphoreTake(sem, 9000)){
  //Turn Green LED on
  SerialUSB.println("Turning on the blue and green LED");
  digitalWrite(LED_RGB_BLUE, HIGH);
  digitalWrite(LED_RGB_GREEN, HIGH);
  
  //Sleep for 500 milliseconds
  vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);

  //Turn Green LED off
  SerialUSB.println("Turning off the Blue and Green LED");
  digitalWrite(LED_RGB_BLUE, LOW);
  digitalWrite(LED_RGB_GREEN, LOW);
  
  //Sleep for 500 milliseconds
  vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);

  //Done with the thread
  SerialUSB.println("Thread done");

  //gives semaphore
  xSemaphoreGive(sem);
  }else{
    SerialUSB.println(F("Failed to abtain semaphore"));
  }  
  vTaskDelay(1000);
  }
  
}

static void RedGreenBlueLEDblink_task(void* arg){
  while (1) {
  //Wait until a semaphore is released
  if (xSemaphoreTake(sem, 9000)){
  //Turn Green LED on
  SerialUSB.println("Turning on the red, blue and green LED");
  digitalWrite(LED_RGB_RED, HIGH);
  digitalWrite(LED_RGB_BLUE, HIGH);
  digitalWrite(LED_RGB_GREEN, HIGH);
  
  //Sleep for 500 milliseconds
  vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);

  //Turn Green LED off
  SerialUSB.println("Turning off the Red, Blue and Green LED");
  digitalWrite(LED_RGB_RED, LOW);
  digitalWrite(LED_RGB_BLUE, LOW);
  digitalWrite(LED_RGB_GREEN, LOW);
  
  //Sleep for 500 milliseconds
  vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);

  //Done with the thread
  SerialUSB.println("Thread done");

  //gives semaphore
  xSemaphoreGive(sem);
  }else{
    SerialUSB.println(F("Failed to abtain semaphore"));
  }  
  vTaskDelay(1000);
  }
  
}
void loop() {
  // put your main code here, to run repeatedly:

}
