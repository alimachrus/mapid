#include <SoftwareSerial.h>
SoftwareSerial mySerial(7, 8); 
float temp;
int tempPin = A0;
float latitude = -7.0599;
float longitude = 109.4259;
int d=500;

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  temp = analogRead(tempPin);
  temp = temp * 0.48828125;
  sendingdata();
}

void sendingdata()
{
  mySerial.println("AT"); 
  ShowSerialData();
  delay(d);

  mySerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  ShowSerialData();
  delay(d);

  mySerial.println("AT+SAPBR=3,1,\"APN\",\"\"");
  ShowSerialData();
  delay(d);

  mySerial.println("AT+SAPBR=1,1");
  ShowSerialData();
  delay(d);

  mySerial.println("AT+SAPBR=2,1");
  ShowSerialData();
  delay(d);

  mySerial.println("AT+HTTPINIT");
  ShowSerialData();
  delay(d);
  
  mySerial.println("AT+HTTPPARA=\"CID\",1");
  ShowSerialData();
  delay(d);
  ShowSerialData();
  
  String str = "AT+HTTPPARA=\"URL\",\"api.mapid.io/api/update?key=7750141e8efe27a0bd712f1aa7606ce5&var1="+String(latitude)
  +"&var2="+String(longitude)
  +"&var3="+String(temp)+"\"";
  
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
