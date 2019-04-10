#include <SoftwareSerial.h>
#define DEBUG true
SoftwareSerial SIM808(7, 8);
String data[7], latitude, longitude, altitude, timegps, speedknot, c, state, com;
int d=500;

void setup() {
  Serial.begin(9600);
  SIM808.begin(9600);
  delay(5);
}

void loop() {
  SIM808.println("AT");
  delay(d);

  Serial.println(SIM808.readString());
  SIM808.println("AT+CGATT=1");
  delay(d);

  Serial.println(SIM808.readString());
  SIM808.println("AT+CIPSHUT");
  delay(d);

  Serial.println(SIM808.readString());
  SIM808.println("AT+CIPMUX=0");
  delay(d);

  Serial.println(SIM808.readString());
  SIM808.println("AT+CSTT=\"m2mdev\",\"wap\",\"wap123\"");
  delay(d);

  Serial.println(SIM808.readString());
  SIM808.println("AT+CSTT?");
  delay(d);

  Serial.println(SIM808.readString());
  SIM808.println("AT+CIICR");
  delay(d);

  Serial.println(SIM808.readString());
  SIM808.println("AT+CIFSR");
  delay(1000);

  Serial.println(SIM808.readString());
  SIM808.println(" AT+CGPSPWR=1");
  delay(d*3);

  SIM808.println("AT+CGNSPWR=1");
  delay(d);

  Serial.println(SIM808.readString());
  SIM808.println("AT+CGNSSEQ=RMC");
  delay(d);

  Serial.println(SIM808.readString());
  SIM808.println("AT+CGPSSTATUS?");
  delay(d);

  Serial.println(SIM808.readString());
  SIM808.println("AT+CGNSINF");
  delay(d);

  if (SIM808.available() > 0)
  {
    while (SIM808.available() > 0)
    {
      c = (SIM808.readString());
      delay(d*3);
      c.remove(100);
    }
  }

  state = c.substring(25, 26);
  timegps = c.substring(27, 41);
  latitude = c.substring(46, 55);
  longitude = c.substring(56, 66);
  altitude = c.substring(67, 74);
  speedknot = c.substring(75, 79);

  Serial.println("State      :" + state);
  Serial.println("Time       :" + timegps);
  Serial.println("Latitude   :" + latitude);
  Serial.println("Longitude  :" + longitude);
  Serial.println("Altitude   :" + altitude);
  Serial.println("Speed      :" + speedknot + " knot");
  Serial.println(" ");
  delay(d);

  Serial.println(SIM808.readString());
  SIM808.println("AT+CIPSEND");
  delay(d);

  SIM808.println("AT+CIPSTATUS");           //query current connection status
  delay(d);

  SIM808.println("AT+CIPMUX=0");            //start up multi-IP connection
  delay(d);

  Serial.println(SIM808.readString());
  SIM808.println("AT+CSTT=\"internet\"");   //start task and setting the APN,
  delay(d);

  Serial.println(SIM808.readString());
  SIM808.println("AT+CIICR");               //bring up wireless connection
  delay(d);

  Serial.println(SIM808.readString());
  SIM808.println("AT+CIFSR");               //get local IP adress
  delay(d);

  Serial.println(SIM808.readString());
  SIM808.println("AT+CIPSPRT=0");           //set prompt of '>' when modul sends data
  delay(d);

  Serial.println(SIM808.readString());
  SIM808.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(d);

  Serial.println(SIM808.readString());
  SIM808.println("AT+SAPBR=3,1,\"APN\",\"\"");//setting the APN,2nd parameter empty works for all networks
  delay(d);

  Serial.println(SIM808.readString());
  SIM808.println();
  SIM808.println("AT+SAPBR=1,1");
  delay(d*4);

  Serial.println(SIM808.readString());
  SIM808.println("AT+HTTPINIT"); //init the HTTP request
  delay(d*3);
  Serial.println(SIM808.readString());
  
  String str = "AT+HTTPPARA=\"URL\",\"https://api.mapid.io/api/update?key=242cc4f809146df922c21b3aadcafb2e&var1="
  +latitude+"&var2="
  +longitude+"&var3="+altitude
  +"\"";
  
  SIM808.println(str);// setting the httppara,
  //the second parameter is the website from where you want to access data
  delay(d);

  Serial.println(SIM808.readString());
  SIM808.println("AT+HTTPSSL=1");
  delay(d);

  Serial.println(SIM808.readString());
  SIM808.println();
  SIM808.println("AT+HTTPACTION=0");//submit the GET request
  delay(d);//the delay is important if the return datas are very large, the time required longer.

  Serial.println(SIM808.readString());
  SIM808.println("AT+HTTPREAD=0,20");// read the data from the website you access
  delay(d*2);

  Serial.println(SIM808.readString());
  SIM808.println("");
  delay(d);

  SIM808.println("AT+HTTPTERM");// terminate HTTP service
  Serial.println(SIM808.readString());
  SIM808.println("AT+CIPSHUT");             //close the connection
  delay(d);

  Serial.println(SIM808.readString());
  SIM808.println("AT+CIPCLOSE");
  delay(d);

  Serial.println(SIM808.readString());
  delay(d*10);

  SIM808.flush();
  SIM808.println("AT+CIPCLOSE");
  delay(d);

  Serial.println(SIM808.readString());
  Serial.println("GPS Initialising...");
  delay(d*10);
}
