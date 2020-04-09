//Currently not being used
//The code in this file will retrieve the stored image and send it to the user.

//Set MMS configuration for the Rogers network
void mmsConfiguration(){
  //mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  mySerial.println("AT+CMMSINIT"); // Initialize MMS mode
  mySerial.println("AT+CMMSCURL=\"mms.gprs.rogers.com\""); //Set MMS centre
  mySerial.println("AT+CMMSCID=1"); //Set bearer context id
  mySerial.println("AT+CMMSPROTO=\"10.128.1.69\",80"); //Set the MMS Proxy and Port
  mySerial.println("AT+CMMSSENDCFG=6,3,0,0,2,4"); //Parameters for MMS PDU
  mySerial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\""); //Set bearer parameter
  mySerial.println("AT+SAPBR=3,1,\"APN\",\"CMWAP\"");
  mySerial.println("AT+SAPBR=1,1"); //Active bearer context
  mySerial.println("AT+SAPBR=2,1"); 
  
}

//Set Mode to MMS
void setMMS_Mode(){
  
}


//Send MMS to registered user
void sendMMS(){
  
}
