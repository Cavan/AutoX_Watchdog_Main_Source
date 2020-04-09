/* FILE:AutoXWatchdog_Main.ino
 * PROJECT: AutoX Watchdog
 * PROGRAMMER: Cavan Biggs
 * FIRST VERSION: January 27th 2020
 * DESCRIPTION: This file serves as the entry point for the program for setting up the hardware to start monitoring objects near by the ....
 *              user's property. The program will monitor PIR sensors, upon the event of a sensor being triggered a servo will be instructed to 
 *              move in the direction of the triggered sensor. Once in position the camera will then take a picture, and write it to an SD card, once that process ...
 *              completes the user is notified by a SMS text message of the detected movement.
 *          
 *        
 *        
 *            
 *        
*/



#include <Servo.h>
#include <SoftwareSerial.h>

//Arduino Mega 2560 settings TX #11 RX #12
SoftwareSerial mySerial(11,12);
Servo camServo;
Servo rearServo; // create servo object to position camera to the rear

int ledPin = 13;  // LED on Pin 13 of Arduino
int frontPirPin = 7; // Input for HC-S501
int leftPirPin = 8;
int rightPirPin = 5;
int rearPirPin = 4;

int frontPirValue; // Place to store read PIR Value
int leftPirValue;
int rightPirValue;
int rearPirValue;
//Servo trap
int camServoCount = 0;
int rearServoCount = 0;
//Servo switch
bool moveCamera;
//Define command
String const LOOKFRONT = "CMD<LOOK_FRONT>";
String const LOOKLEFT = "CMD<LOOK_LEFT>";
String const LOOKRIGHT = "CMD<LOOK_RIGHT>";
String const LOOKBEHIND = "CMD<LOOK_BEHIND>";
String const RESP_BAD_COMMAND = "Invalid command received, please use valid command";
String const RESP_FRONT_IMG_CAPTURED = "Captured front image";
String const RESP_LEFT_IMG_CAPTURED = "Captured left image";
String const RESP_RIGHT_IMG_CAPTURED = "Captured right image";
String const RESP_READ_IMG_CAPTURED = "Captured rear image";

//System response number 
String const USER_NUMBER = "AT+CMGS=\"+15196088364\"";


String incomingCMD = "";

//Variables for delaying sensors
unsigned long previousMillis = 0;
//interval constant 10 seconds
const long interval = 10000;

//Debounce setup
//Variables
int lastFrontPIR_State = LOW;
int lastLeftPIR_State = LOW;
int lastRightPIR_State = LOW;
int lastRearPIR_State = LOW;
int frontPIR_State;
int leftPIR_State;
int rightPIR_State;
int rearPIR_State;
//Time variables
unsigned long lastDebounceTime = 0; //the last time the output pin was toggled
unsigned long debounceDelay = 10000; //the debounce time.
//Pir sensor startup - sensors need 1 minute to startup
unsigned long systemStartupTime = 0; //How long has the system been powered on?
unsigned long startupDelay = 60000; //60000 milliseconds = 1 minute

void setup() {
  
  pinMode(ledPin, OUTPUT);
  pinMode(frontPirPin, INPUT);
  pinMode(leftPirPin, INPUT);
  pinMode(rightPirPin, INPUT);
  pinMode(rearPirPin, INPUT);

  digitalWrite(frontPirPin, LOW);
  digitalWrite(leftPirPin, LOW);
  digitalWrite(rightPirPin, LOW);
  digitalWrite(rearPirPin, LOW);
 
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
  //Servo setup
  camServo.attach(10);
  rearServo.attach(3);
  //Position the servos
  moveServos(90, 90);
  
  

//GSM setup 
//Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  Serial.print("Command Length: ");
  Serial.println(LOOKFRONT.length());
  //Begin serial communication with Arduino and SIM900
  mySerial.begin(9600);

  Serial.println("Initializing..."); 
  Serial.println("Setting up wireless connection to network provider");
  delay(1000);

  mySerial.println("AT"); //Handshaking with SIM900
  updateSerial();
  
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  //Replace this with a startup method
  updateSerial();
  
  //MMS Setup
  mmsConfiguration();



  //Camera setup
  camera_SD_setup();
  //Initialize Servo Switch
  moveCamera = true;
  //initialize sensor for 1 minute
  MotionSensorInitialization(); 
}

void loop() {
  frontPirValue = digitalRead(frontPirPin);
  leftPirValue = digitalRead(leftPirPin);
  rightPirValue = digitalRead(rightPirPin);
  rearPirValue = digitalRead(rearPirPin);



  if((millis() - lastDebounceTime) > debounceDelay)
  {
    //Set the servo switch back to true now that 10 seconds have passed
    moveCamera = true;
    Serial.print("Checking debounce time: ");
    Serial.println((millis() - lastDebounceTime));
    
  //Check if Front PIR state has changed
  //Check for motion detected
    if (frontPirValue == HIGH && moveCamera == true)
    {
        
          //Check if servos are already in position
          //Move servo to Front 90 degrees
          //moveServos(90, 90);
            Serial.println("Front motion sensor: FRONT MOTION DETECTED");
            
            //Send alert SMS
            processCommands(LOOKFRONT, "| ALERT:->Motion was detected at front of vehicle");
            //lastFrontPIR_State = frontPirValue;
            lastDebounceTime = millis();
            moveCamera = false;
       }
      else if (leftPirValue == HIGH && moveCamera == true)
        {
               //moveServos(180, 90);
              Serial.println("Left motion sensor: LEFT MOTION DETECTED");
               //Send alert SMS
              processCommands(LOOKLEFT, ",| ALERT:->Motion was detected at left side of vehicle");
              //lastLeftPIR_State = leftPirValue;
              lastDebounceTime = millis();
              moveCamera = false;
        }
      else if (rightPirValue == HIGH && moveCamera == true)
          {          
               //moveServos(0, 90);   
              Serial.println("Right motion sensor: RIGHT MOTION DETECTED");
               //Send alert SMS
              processCommands(LOOKRIGHT, "| ALERT:->Motion was detected at right side of vehicle");
              //lastRightPIR_State = rightPirValue;
              lastDebounceTime = millis();
              moveCamera = false;
          }
      else if (rearPirValue == HIGH && moveCamera == true){

              //moveServos(180, 180);
              Serial.println("Rear motion sensor: REAR MOTION DETECTED");
               //Send alert SMS
              processCommands(LOOKBEHIND, "| ALERT:->Motion was detected at the rear of the vehicle");
              //lastRearPIR_State = rearPirValue;
              lastDebounceTime = millis();
              moveCamera = false;
          }
  }

  //GSM module code
  updateSerial();
}

/*
*  FUNCTION     : MotionSensorInitialization
*
* DESCRIPTION   : Waits 1 minute to allow PIR sensors to initialize
*          
*
* PARAMETERS    : void
*
*
* RETURNS       : void
*
*/
//PIR Sensors have a 1 minute initialization period when system is powered on
void MotionSensorInitialization(){
    

  //unsigned long systemStartupTime = 0; //How long has the system been powered on?
  //unsigned long startupDelay = 60000; //60000 milliseconds = 1 minute


    while(1){
      if((millis() - systemStartupTime) > startupDelay)
      {
          //after 1 minute break out of loop
          Serial.println("PIR sensors are initialized");
          break;
      }

      systemStartupTime = millis()
     
  }
}




//GSM code
/*
*  FUNCTION     : updateSerial
*
* DESCRIPTION   : Facilitates communication between the arduino microcontroller, and the GSM unit ...
*                 through serial communication over the RX and TX lines.
*          
*
* PARAMETERS    : void
*
*
* RETURNS       : void
*
*/
void updateSerial()
{
  int cmdIndex = 0;
  String inputCMD = "";
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    char inputChar = mySerial.read();
    incomingCMD.concat(inputChar);
    //Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
    if(inputChar == '\n')
    {
      //Parse for command
      //Get index of command
      cmdIndex = incomingCMD.indexOf("CMD<");
      if(cmdIndex != -1)
      {  
          //Get the command as substring
          inputCMD = incomingCMD.substring(cmdIndex);
          Serial.print("Received: ");
          Serial.println(inputCMD);
          //Parse methods here
          Serial.print("Sent command length: ");
          inputCMD.trim();
          Serial.println(inputCMD.length());
          //Serial.print(inputCMD, HEX);
          processCommands(inputCMD, "");
      }
      //Clear the buffer
      incomingCMD = "";
      
    }
  }
  
}


/*
*  FUNCTION     : processCommands
*
* DESCRIPTION   : This method is responsible for taking the incoming commands and checking ...
*                 validility, and executing the appropriate action related to that command.
*          
*
* PARAMETERS    : String const command, String moreInfo
*
*
* RETURNS       : void
*
*/
//Process the incoming command
void processCommands(String const command, String moreInfo){

  //Look for the following values
  //#define LOOKFRONT "CMD<LOOK_FRONT>"
  //#define LOOKLEFT "CMD<LOOK_LEFT>"
  //#define LOOKRIGHT "CMD<LOOK_RIGHT>"
  //#define LOOKBEHIND "CMD<LOOK_BEHIND>"
      Serial.print("Executing:..... \n");
      //Serial.print("Time of execution: ");
      //mySerial.println("AT+CCLK?");

  if (command.equals(LOOKFRONT))
  {
    //Reattach Servos
    switchOnServos(camServo, rearServo);
    //Check if servos are already in position
          //Move servo to Front 90 degrees
         //Position the servos
          moveServos(90, 90);
    Serial.println(RESP_FRONT_IMG_CAPTURED);
    //Capture image
    myCAMSaveToSDFile();
    prepareResponse("Front Image Captured " + moreInfo);
  }
  else if(command.equals(LOOKLEFT)){
    //Reattach Servos
    switchOnServos(camServo, rearServo);
   //Move servo to Left 180 degrees
              //Position the servos
              moveServos(180, 90);
  Serial.println(RESP_LEFT_IMG_CAPTURED);
  //Capture image
    myCAMSaveToSDFile();
  prepareResponse("Left Image Captured " + moreInfo);
    
  }
  else if(command.equals(LOOKRIGHT)){
    //Reattach Servos
    switchOnServos(camServo, rearServo);
    //Move camera to the right and capture image
   //Move servo to the right 0 degrees
              //Position the servos
                moveServos(0, 90);
    Serial.println(RESP_RIGHT_IMG_CAPTURED);
    //Capture image
    myCAMSaveToSDFile();
    prepareResponse("Right Image Captured " + moreInfo);
    
  }
  else if(command.equals(LOOKBEHIND)){
    //Reattach Servos
    switchOnServos(camServo, rearServo);
    //Due to the servos being limited to 180 degrees we need to use 2 to capture the rear view
              //Position the servos
              moveServos(180, 180);
    Serial.println(RESP_READ_IMG_CAPTURED);
    //Capture image
    myCAMSaveToSDFile();
    prepareResponse("Rear Image Captured " + moreInfo);
  }
  else{
    Serial.println(RESP_BAD_COMMAND);
    prepareResponse(RESP_BAD_COMMAND);
  }

  
}

/*
*  FUNCTION     : prepareResponse
*
* DESCRIPTION   : This method sends back the response to the users mobile number ...
*                 after receiving an incoming command from the user.
*          
*
* PARAMETERS    : String responseMSG
*
*
* RETURNS       : void 
*
*/
void prepareResponse(String responseMSG){
//detach servos to stop interference 
switchOffServos(camServo, rearServo);
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  responseSMS();
  mySerial.println(USER_NUMBER);//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  responseSMS();
  mySerial.print(responseMSG); //text content
  responseSMS();
  mySerial.write(26);
 //Reattach servos when processing is done
 //switchOnServos(camServo, rearServo);
}

/*
*  FUNCTION     : responseSMS
*
* DESCRIPTION   : Sends the preparedResponse from the arduino to the sim900 unit which will ...
*                 then send the SMS over the cellular network to the users phone number.
*          
*
* PARAMETERS    : 
*
*
* RETURNS       :
*
*/
void responseSMS()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
