#include <SoftwareSerial.h>
#define flamePin 11
SoftwareSerial mySerial(9, 10);

int sensor=A1;
float temp_read,Temp_alert_val,Temp_shut_val;
int sms_count=0,Fire_Set;


void setup()
{
  pinMode(flamePin,INPUT);
  pinMode(sensor,INPUT);
  mySerial.begin(9600);   
  Serial.begin(9600);    
  delay(500);
  Serial.print("Calibrating");
  for(int i = 0; i <15; i++){
    if (i==4){
      Serial.print(".");
    }
    else  Serial.print(".");
      delay(500);
  }
  Serial.println("Done");
  delay(1000);
  Serial.println("Flame Sensor Active");
  delay(1500);
}

void loop()
{
CheckFire();
CheckShutDown();
}

void CheckFire()
{
int Flame = digitalRead(flamePin);
Serial.print("Fire Scan - ON");
Temp_alert_val=CheckTemp();
if(Temp_alert_val>45 && Flame == LOW)
{
 Serial.print("Flame is Detected");
 SetAlert(); // Function to send SMS Alerts
}
else {
  SetAlert();
}
}

float CheckTemp()
{
temp_read=analogRead(sensor); // reads the sensor output (Vout of LM35)
temp_read=temp_read*5;    // converts the sensor reading to temperature
temp_read=temp_read/10;  // adds the decimal point
return temp_read; // returns temperature value in degree celsius
}

void SetAlert()
{
while(sms_count<3) //Number of SMS Alerts to be sent
{  
SendTextMessage(); // Function to send AT Commands to GSM module
}
Fire_Set=1; 
Serial.println("Fire Alert! SMS Sent!");
}

void CheckShutDown()
{
if(Fire_Set==1)
{

Temp_shut_val=CheckTemp();
if(Temp_shut_val<28)
{
Serial.println("Fire Shut! SAFE NOW");
sms_count=0;
Fire_Set=0;
}}}

void SendTextMessage()
{
  mySerial.println("AT+CMGF=1");    //To send SMS in Text Mode
  delay(2000);
  mySerial.println("AT+CMGS=\"+91xxxxxxxxxx\"\r"); // change to the phone number you using 
  delay(2000);
  mySerial.println("Fire in NEW ROOM!");//the content of the message
  delay(200);
  mySerial.println((char)26);//the stopping character
  delay(5000);
  mySerial.println("AT+CMGS=\"+91xxxxxxxxx\"\r"); // change to the phone number you using 
  delay(2000);
  mySerial.println("Fire in NEW ROOM!");//the content of the message
  delay(200);
  mySerial.println((char)26);//the message stopping character
  delay(5000);
  sms_count++;
}
