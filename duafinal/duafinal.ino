#include <SoftwareSerial.h>
SoftwareSerial mySerial(7, 8);
float latitude = -4.141592;
float longitude = 105.733;
int d = 1500;

float DistanceMax;
float DistanceMin;

bool NotAccurate = true;

#define trig_pin     3   // pin TRIG to D1
#define echo_pin     2   // pin ECHO to D2

// defines variables
float duration;
float distance;
//float next = 250;
//float value;
//float ema = 0.2;

float AnalogWert;

void setup()
{
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
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

  do {

    getDistance();
    while (getDistance() >= 500 || getDistance() <=  20) {
      //data is not in range
      getDistance();
    }
    //Now the data is in range, we need to save the data values and compare it. Only once.
    //If it's bad, then try it all again.
    DistanceMax = (getDistance() + 30);
    DistanceMin = (getDistance() - 30);

    getDistance();

    while (getDistance() >= 500 || getDistance() <=  20) {
      //data is not in range
      getDistance();
    }

    if (getDistance() > DistanceMax || getDistance() < DistanceMin) {
      //our data is bad
      NotAccurate = true;
    }
    else {
      //our data is good
      NotAccurate = false;
    }

  } while (NotAccurate);

  //  next = getDistance();
  //  nilai= getValue();
  //  value = (ema * next) + ((1 - ema) * value);

  AnalogWert = analogRead(A0);
 
  Serial.print("Distance: ");
  Serial.print(getDistance(), 0);
  Serial.print(" cm");
  Serial.print("\t");
  Serial.println(AnalogWert);

  sendingdata();
  NotAccurate = true;

  delay(80000);
}

void sendingdata()
{
  mySerial.println("AT");
  //ShowSerialData();
  delay(d);

  mySerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  //ShowSerialData();
  delay(d);

  mySerial.println("AT+SAPBR=3,1,\"APN\",\"\"");
  //ShowSerialData();
  delay(d);

  mySerial.println("AT+SAPBR=1,1");
  //ShowSerialData();
  delay(d);

  mySerial.println("AT+SAPProBR=2,1");
  //ShowSerialData();
  delay(d);

  mySerial.println("AT+HTTPINIT");
  //ShowSerialData();
  delay(d);

  mySerial.println("AT+HTTPPARA=\"CID\",1");
  //ShowSerialData();
  delay(d);
  //ShowSerialData();

  String str = "AT+HTTPPARA=\"URL\",\"api.mapid.io/api/update?key=98eb19afeab734a3c2ed1db68f1e670c&var1=" + String(latitude,4)
               + "&var2=" + String(longitude,4) 
               + "&var3=" + String(getDistance())
               + "&var4=" + String(AnalogWert)
               + "\"";

  mySerial.println(str);
  //ShowSerialData();
  delay(d);

  mySerial.println("AT+HTTPSSL=1");
  //ShowSerialData();
  delay(d);

  mySerial.println("AT+HTTPACTION=0");
  //ShowSerialData();
  delay(d);
}

//void ShowSerialData()
//{
//  while (mySerial.available() != 0)
//    Serial.write(mySerial.read());
//}
