#include <SoftwareSerial.h>
SoftwareSerial mySerial(7, 8);
float latitude = -7.0599;
float longitude = 109.4259;
int d = 500;

int coba = 99;

#define trig_pin     3   // pin TRIG to D1
#define echo_pin     2   // pin ECHO to D2

// defines variables
float duration;
float distance;
float next;
float value;
float ema = 0.2;

float AnalogWert;
float Powerwert;

void setup()
{
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT_PULLUP);
  Serial.begin(9600);
  mySerial.begin(9600);
}

float  getDistance() {
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  duration = pulseIn(echo_pin, HIGH);
  //  distance = (duration / 2) / 29.1;
  distance = (duration / 2) / 28.1;
  return distance;
}

void loop() {
  AnalogWert = analogRead(A0);
  Powerwert = AnalogWert * (12.0 / 1023)*2.21; //in case of higher voltage, change the code: powerwert = AnalogWert *(x/1023); x= your new power supply maximum voltage
  next = getDistance();
//  value = (ema * next) + ((1 - ema) * value);
//  Serial.print("Distance: ");
//  Serial.print(next,0);
//  Serial.print(" cm");  
//  Serial.print("\t");
//  Serial.println(Powerwert);
  sendingdata();
  delay(100);
}

void sendingdata()
{
  mySerial.println("AT");
  ShowSerialData();
  delay(d);

  mySerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  ShowSerialData();
  delay(d);

  mySerial.println("AT+SAPBR=3,1,\"APN\",\"Telkomsel\"");
  ShowSerialData();
  delay(d);

  mySerial.println("AT+SAPBR=1,1");
  ShowSerialData();
  delay(d);

  mySerial.println("AT+SAPProBR=2,1");
  ShowSerialData();
  delay(d);

  mySerial.println("AT+HTTPINIT");
  ShowSerialData();
  delay(d);

  mySerial.println("AT+HTTPPARA=\"CID\",1");
  ShowSerialData();
  delay(d);
  ShowSerialData();

  String str = "AT+HTTPPARA=\"URL\",\"api.mapid.io/api/update?key=298645c02bfe2d56cf7ba58d99c9ba23&var1=" + String(latitude)
               + "&var2=" + String(longitude) 
               + "&var3=" + String(next)
               + "&var4=" + String(AnalogWert)
               + "\"";

  mySerial.println(str);
  ShowSerialData();
  delay(d);

  mySerial.println("AT+HTTPSSL=1");
  ShowSerialData();
  delay(d);

  mySerial.println("AT+HTTPACTION=0");
  ShowSerialData();
  delay(d);
}

void ShowSerialData()
{
  while (mySerial.available() != 0)
    Serial.write(mySerial.read());
}
