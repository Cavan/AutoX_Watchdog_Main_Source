//This function is responsible for positioning the camera


void moveServos(int cameraAngle, int rearAngle){
  camServo.write(cameraAngle);
  rearServo.write(rearAngle);
}

void switchOffServos(Servo cam, Servo rear){
  cam.detach();
  rear.detach();
}

void switchOnServos(Servo cam, Servo rear){
  cam.attach(10);
  rear.attach(3);
}
