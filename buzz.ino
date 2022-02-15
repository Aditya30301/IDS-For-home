#include<SoftwareSerial.h>
SoftwareSerial mySerial(9, 10);
#define sensor 2 //The output of PIR sensor connected to pin 7
#define push_switch 6 // push button switch connected to pin 6
//#define buzzer A0 // buzzer connected at pin 8
int sensor_value=0; //variable to hold read sensor value
int sms_count=0;
String incomingData;   // for storing incoming serial data
String message = "";   // A String for storing the message
//int relay_pin = 13;    // Initialized a pin for relay module
int l=0;




void setup()
{
  delay(10000);
pinMode(sensor,INPUT); // configuring pin 7 as Input
pinMode(push_switch,INPUT); // configuring pin 6 as Input
pinMode(A0,OUTPUT); // configuring pin 8 as OUTPUT
pinMode(A1,OUTPUT); // configuring pin 8 as OUTPUT
//pinMode(relay_pin, OUTPUT);// Setting erlay pin as output pin
analogWrite(A0,HIGH);
delay(100);
analogWrite(A1,LOW);
digitalWrite(sensor,LOW);
sensor_value=0;
mySerial.begin(9600);
Serial.begin(9600);
mySerial.print("AT+CMGF=1\r");  
delay(100);
mySerial.print("AT+CNMI=2,2,0,0,0\r"); 
delay(100);
}






void loop()
{
  //Function for receiving sms
  receive_message();
  
  Serial.println(""+incomingData);
 // if received command is to turn off relay
  if(incomingData.indexOf("aditya30")>=0)
  {
     digitalWrite(A0, HIGH);
     delay(100);
    
    Serial.println("PASSWORD_CORRECT");
    Serial.println("SYSTEM_DISARM");
    digitalWrite(A0, LOW);
    Check_Reset();
    message = "System_Disarmed";
    // Send a sms back to confirm that the relay is turned on
    send_message(message);
    
      disarmed();
     
  }
  
  // if received command is to turn off relay
  if(incomingData.indexOf("resetadi30")>=0)
  {
    digitalWrite(A0, HIGH);
    delay(100);
    digitalWrite(A0, LOW);
    delay(100);
    digitalWrite(A0, HIGH);
    delay(100);
    digitalWrite(A0, LOW);
    
   Serial.println("PASSWORD_CORRECT");
   
   delay(1000);
   Serial.println("BUZZER OFF");
   delay(1000);
   Check_Reset();
    
  }
  else    
  Serial.println(" NO INPUT");    
  incomingData="";
Check_Burglar();// subroutine to check sensor status and activation of outputs
//Check_Reset(); // subroutine to check if alarm reset switch pressed or not
Serial.println("BUZZER"+String(A0));
}




void disarmed()
{
  while(l==0){
    delay(1000);
  Serial.println("SYSTEM DISARMED!!!");
  delay(1000);
 // Serial.println("RESTART TO ARM AGAIN!!!");
  receive_message();
    if(incomingData.indexOf("on")>=0)
    {
        digitalWrite(A0, HIGH);
    delay(100);
    digitalWrite(A0, LOW);
    delay(100);
    digitalWrite(A0, HIGH);
    delay(100);
    digitalWrite(A0, LOW);
    digitalWrite(A0, HIGH);
    delay(100);
    digitalWrite(A0, LOW);  
      l=1;
    }
    incomingData="";
 
  }
  l=0;
}





void Check_Burglar()
{
sensor_value=digitalRead(sensor); // Reading sensor value from pin 7
 Serial.println("SESNOR"+String(sensor_value));
if(sensor_value==1) // Checking if PIR sensor sends a HIGH signal to Arduino
{
  Serial.println("INTRUDER DETECTED!!!!");
  digitalWrite(A0,HIGH); // Activating the buzzer 
  while(sms_count<3) //Number of SMS Alerts to be sent limited at 3
    {  
      SendTextMessage(); // Function to send AT Commands to GSM module
    }

}
}









void Check_Reset()
{
  Serial.println("RESET_CHECK");
  int a=digitalRead(push_switch);
  Serial.println("RESET"+String(a));
  digitalWrite(A0,LOW); // turning OFF the buzzer 
  sms_count=0; // Reactivating the SMS Alert Facility
}






void SendTextMessage()
{
  mySerial.println("AT+CMGF=1");    //To send SMS in Text Mode
  delay(1000);
  mySerial.println("AT+CMGS=\"+919476026848\"\r"); // change to the phone number you using 
  delay(1000);
  mySerial.println("INTRUDER ALERT !");//the content of the message
  delay(200);
  mySerial.println((char)26);//the stopping character
  delay(1000);
  sms_count++;
}







void receive_message()
{
  
  if (mySerial.available() > 0)
  {
    incomingData = mySerial.readString(); // Get the data from the serial port.
    Serial.print(incomingData); 
    delay(10); 
  }
}




void send_message(String message)
{
  mySerial.println("AT+CMGF=1");    //Set the GSM Module in Text Mode
  delay(100);  
  mySerial.println("AT+CMGS=\"+919476026848\""); // Replace it with your mobile number
  delay(100);
  mySerial.println(message);   // The SMS text you want to send
  delay(100);
  mySerial.println((char)26);  // ASCII code of CTRL+Z
  delay(100);
  mySerial.println();
  delay(1000);  
}
