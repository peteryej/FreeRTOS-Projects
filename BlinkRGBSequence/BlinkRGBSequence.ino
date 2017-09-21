
/*
  BlinkRTOSSequence 
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


QueueHandle_t Color_Queue_Handle;


// the setup function runs once when you press reset or power the board
void setup() {
  
  portBASE_TYPE s1, s2;
  
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
  
  //creat color queue
  Color_Queue_Handle = xQueueCreate(10,sizeof(char));

  //create tasks
  s1 = xTaskCreate(Schedule_task, NULL, 512, NULL, 4, NULL);
  s2 = xTaskCreate(Read_task, NULL, 512, NULL, 2, NULL);
 

  // check for creation errors
  if (s1 != pdPASS || s2 != pdPASS) {
    SerialUSB.println(F("Creation problem"));
    while(1);
  }
  
  //start task scheduler
  vTaskStartScheduler();
  
}

void Read_task(void *p){
  //w for red, green and blue, y for red and green, p for red and blue, c for green and blue
  char color;
  while (1){
    while(SerialUSB.available()==0) {}//wait for input
    if (SerialUSB.available()>0){
      color = SerialUSB.read();
      SerialUSB.print("User input is:");
      SerialUSB.println(color);
    }
    if (!xQueueSend(Color_Queue_Handle, &color, 500)){
    SerialUSB.println("Failed to send to queue. Queue is full.");
    }
    vTaskDelay(500);
  }
}

//task of scheduling the on and off sequence of RGB LEDs
static void Schedule_task(void* arg){
  char receivedcolor;
  while (1) {
    if (!xQueueReceive(Color_Queue_Handle, &receivedcolor, 5000)){
      SerialUSB.println("Failed to recieve color from queue. Queue is empty.");
    }else{
      SerialUSB.print("Recieved color is: ");
      SerialUSB.println(receivedcolor);
    }
    
        
    switch (receivedcolor){
      case 'w': 
      {
        SerialUSB.println("Turning on the Red, Blue and Green LED");
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
        //receivedcolor = 0;
        break;
      }
      case 'y': 
      {
        SerialUSB.println("Turning on the Red and Green LED");
        digitalWrite(LED_RGB_GREEN, HIGH);
        digitalWrite(LED_RGB_RED, HIGH);
        
        //Sleep for 500 milliseconds
        vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);
      
        //Turn Green LED off
        SerialUSB.println("Turning off the Red and Green LED");
        digitalWrite(LED_RGB_GREEN, LOW);
        digitalWrite(LED_RGB_RED, LOW);
        break;
      }
      case 'c': 
      {
        SerialUSB.println("Turning on the blue and green LED");
        digitalWrite(LED_RGB_BLUE, HIGH);
        digitalWrite(LED_RGB_GREEN, HIGH);
        
        //Sleep for 500 milliseconds
        vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);
      
        //Turn Green LED off
        SerialUSB.println("Turning off the Blue and Green LED");
        digitalWrite(LED_RGB_BLUE, LOW);
        digitalWrite(LED_RGB_GREEN, LOW);
        break;
      }
      case 'p': 
      {
        SerialUSB.println("Turning on the red and green LED");
        digitalWrite(LED_RGB_BLUE, HIGH);
        digitalWrite(LED_RGB_RED, HIGH);
        
        //Sleep for 500 milliseconds
        vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);
      
        //Turn Green LED off
        SerialUSB.println("Turning off the Red and Green LED");
        digitalWrite(LED_RGB_BLUE, LOW);
        digitalWrite(LED_RGB_RED, LOW);
        break;
      }
      case 'r': 
      {
        SerialUSB.println("Turning on the red LED");
        digitalWrite(LED_RGB_RED, HIGH);
        vTaskDelay(500);
        SerialUSB.println("Turning off the red LED");
        digitalWrite(LED_RGB_RED, LOW);
        //receivedcolor = 0;
        break;
      } 
      case 'g': 
      {
        SerialUSB.println("Turning on the green LED");
        digitalWrite(LED_RGB_GREEN, HIGH);
      
        //Sleep for 500 milliseconds
        vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);
      
        //Turn Green LED off
        SerialUSB.println("Turning off the Green LED");
        digitalWrite(LED_RGB_GREEN, LOW);
        break;
      } 
      case 'b': 
      {
        SerialUSB.println("Turning on the blue LED");
        digitalWrite(LED_RGB_BLUE, HIGH);
      
        //Sleep for 500 milliseconds
        vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);
      
        //Turn Blue LED off
        SerialUSB.println("Turning off the blue LED");
        digitalWrite(LED_RGB_BLUE, LOW);
        break;
      }       
      default: 
        SerialUSB.println("Not valid input.");           
        break;  
    }
    vTaskDelay(500);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
