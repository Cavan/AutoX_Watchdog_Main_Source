/* FILE: servoController.ino
 * PROJECT: AutoX Watchdog
 * PROGRAMMER: Cavan Biggs
 * FIRST VERSION: March 1st 2020
 * DESCRIPTION: The source contained in this file is responsible for controlling the servos that position the camera ...
 *              there was a need to gain further control over the servos due to their eratic behavior such as twitiching, and moving ...
 *              after an image was taken. This was causing the images to be blury and distorted because of the unwanted movement.
 *          
 *        
 *        
 *            
 *        
*/


/*
*  FUNCTION     : moveServos
*
* DESCRIPTION   : Controls what direction the servos will move to.
*          
*
* PARAMETERS    : int cameraAngle, int rearAngle
*
*
* RETURNS       : void
*
*/
void moveServos(int cameraAngle, int rearAngle){
  camServo.write(cameraAngle);
  rearServo.write(rearAngle);
}

/*
*  FUNCTION     : switchOffServos
*
* DESCRIPTION   : Turns servos off to stop interference from causing them to suddenly move ...
*                 while a picture is being taken.
*          
*
* PARAMETERS    : Servo cam, Servo rear
*
*
* RETURNS       : void
*
*/
void switchOffServos(Servo cam, Servo rear){
  cam.detach();
  rear.detach();
}

/*
*  FUNCTION     : switchOnServos 
*
* DESCRIPTION   : Turns the servos back on once an image is captured to ...
*                 ensure they can move to their next target.
*
* PARAMETERS    : Servo cam, Servo rear
*
*
* RETURNS       : void
*
*/
void switchOnServos(Servo cam, Servo rear){
  cam.attach(10);
  rear.attach(3);
}
