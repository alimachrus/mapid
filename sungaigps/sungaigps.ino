#include <SoftwareSerial.h>
SoftwareSerial mySerial(7, 8);
String data[7], latitude, longitude, altitud, timegps, speedknot, c, state, com;
int d = 1700;

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
float Powerwert;

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
  Powerwert = AnalogWert * (27.24 / 1023); //in case of higher voltage, change the code: powerwert = AnalogWert *(x/1023); x= your new power supply maximum voltage

  Serial.print("Distance: ");
  Serial.print(getDistance(), 0);
  Serial.print(" cm");
  Serial.print("\t");
  Serial.println(AnalogWert);

  sendingdata();
  NotAccurate = true;

  delay(100000-40000);
}

void sendingdata()
{
  mySerial.println("AT");
  delay(d);

  Serial.println(mySerial.readString());
  mySerial.println("AT+CGATT=1");
  delay(d);

  Serial.println(mySerial.readString());
  mySerial.println("AT+CIPSHUT");
  delay(d);

  Serial.println(mySerial.readString());
  mySerial.println("AT+CIPMUX=0");
  delay(d);

  Serial.println(mySerial.readString());
  mySerial.println("AT+CSTT=\"m2mdev\",\"wap\",\"wap123\"");
  delay(d);

  Serial.println(mySerial.readString());
  mySerial.println("AT+CSTT?");
  delay(d);

  Serial.println(mySerial.readString());
  mySerial.println("AT+CIICR");
  delay(d);

  Serial.println(mySerial.readString());
  mySerial.println("AT+CIFSR");
  delay(1000);

  Serial.println(mySerial.readString());
  mySerial.println(" AT+CGPSPWR=1");
  delay(d*3);

  mySerial.println("AT+CGNSPWR=1");
  delay(d);

  Serial.println(mySerial.readString());
  mySerial.println("AT+CGNSSEQ=RMC");
  delay(d);

  Serial.println(mySerial.readString());
  mySerial.println("AT+CGPSSTATUS?");
  delay(d);

  Serial.println(mySerial.readString());
  mySerial.println("AT+CGNSINF");
  delay(d);

  if (mySerial.available() > 0)
  {
    while (mySerial.available() > 0)
    {
      c = (mySerial.readString());
      delay(d*3);
      c.remove(100);
    }
  }

  state = c.substring(25, 26);
  timegps = c.substring(27, 41);
  latitude = c.substring(46, 55);
  longitude = c.substring(56, 66);
  altitud = c.substring(67, 74);
  speedknot = c.substring(75, 79);

  Serial.println("State      :" + state);
  Serial.println("Time       :" + timegps);
  Serial.println("Latitude   :" + latitude);
  Serial.println("Longitude  :" + longitude);
  Serial.println("altitud   :" + altitud);
  Serial.println("Speed      :" + speedknot + " knot");
  Serial.println(" ");
  delay(d);

  Serial.println(mySerial.readString());
  mySerial.println("AT+CIPSEND");
  delay(d);

  mySerial.println("AT+CIPSTATUS");           //query current connection status
  delay(d);

  mySerial.println("AT+CIPMUX=0");            //start up multi-IP connection
  delay(d);

  Serial.println(mySerial.readString());
  mySerial.println("AT+CSTT=\"internet\"");   //start task and setting the APN,
  delay(d);

  Serial.println(mySerial.readString());
  mySerial.println("AT+CIICR");               //bring up wireless connection
  delay(d);

  Serial.println(mySerial.readString());
  mySerial.println("AT+CIFSR");               //get local IP adress
  delay(d);

  Serial.println(mySerial.readString());
  mySerial.println("AT+CIPSPRT=0");           //set prompt of '>' when modul sends data
  delay(d);

  Serial.println(mySerial.readString());
  mySerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(d);

  Serial.println(mySerial.readString());
  mySerial.println("AT+SAPBR=3,1,\"APN\",\"\"");//setting the APN,2nd parameter empty works for all networks
  delay(d);

  Serial.println(mySerial.readString());
  mySerial.println();
  mySerial.println("AT+SAPBR=1,1");
  delay(d*4);

  Serial.println(mySerial.readString());
  mySerial.println("AT+HTTPINIT"); //init the HTTP request
  delay(d*3);
  Serial.println(mySerial.readString());
  
  String str = "AT+HTTPPARA=\"URL\",\"api.mapid.io/api/update?key=f387a8be7d434caa5a1b721ae918c24a&var1=" + latitude
               + "&var2=" + longitude
               + "&var3=" + String(getDistance(),2)
               + "&var4=" + String(AnalogWert)
               + "\"";
  
  mySerial.println(str);// setting the httppara,
  //the second parameter is the website from where you want to access data
  delay(d);

  Serial.println(mySerial.readString());
  mySerial.println("AT+HTTPSSL=1");
  delay(d);

  Serial.println(mySerial.readString());
  mySerial.println();
  mySerial.println("AT+HTTPACTION=0");//submit the GET request
  delay(d);//the delay is important if the return datas are very large, the time required longer.

  Serial.println(mySerial.readString());
  mySerial.println("AT+HTTPREAD=0,20");// read the data from the website you access
  delay(d*2);

  Serial.println(mySerial.readString());
  mySerial.println("");
  delay(d);

  mySerial.println("AT+HTTPTERM");// terminate HTTP service
  Serial.println(mySerial.readString());
  mySerial.println("AT+CIPSHUT");             //close the connection
  delay(d);

  Serial.println(mySerial.readString());
  mySerial.println("AT+CIPCLOSE");
  delay(d);

  Serial.println(mySerial.readString());
  delay(d*10);

  mySerial.flush();
  mySerial.println("AT+CIPCLOSE");
  delay(d);

  Serial.println(mySerial.readString());
  Serial.println("GPS Initialising...");
  delay(d*10);
}
