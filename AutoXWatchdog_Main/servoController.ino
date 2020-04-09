/* FILE:
 * PROJECT: 
 * PROGRAMMER: Cavan Biggs
 * FIRST VERSION: 
 * DESCRIPTION: 
 *          
 *        
 *        
 *            
 *        
*/


/*
*  FUNCTION    : 
*
* DESCRIPTION   : 
*          
*
* PARAMETERS    : 
*
*
* RETURNS     :
*
*/
void moveServos(int cameraAngle, int rearAngle){
  camServo.write(cameraAngle);
  rearServo.write(rearAngle);
}

/*
*  FUNCTION    : 
*
* DESCRIPTION   : 
*          
*
* PARAMETERS    : 
*
*
* RETURNS     :
*
*/
void switchOffServos(Servo cam, Servo rear){
  cam.detach();
  rear.detach();
}

/*
*  FUNCTION    : 
*
* DESCRIPTION   : 
*          
*
* PARAMETERS    : 
*
*
* RETURNS     :
*
*/
void switchOnServos(Servo cam, Servo rear){
  cam.attach(10);
  rear.attach(3);
}
