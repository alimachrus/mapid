#include <SoftwareSerial.h>    //Library for Ethernet communication, in this case using SIM900
SoftwareSerial mySerial(7, 8); //SIM900 initialization for completeness of the SoftwareSerial library
char databuffer[35];
double temp;
float lat = -6.9626995;
float lon = 107.5531258;
int d = 200;


void setup()
{
    Serial.begin(9600);
    mySerial.begin(9600); // the GPRS modul baud rate
}

void getBuffer() //Get weather status data
{
    int index;
    for (index = 0; index < 35; index++)
    {
        if (Serial.available())
        {
            databuffer[index] = Serial.read();
            if (databuffer[0] != 'c')
            {
                index = -1;
            }
        }
        else
        {
            index--;
        }
    }
}

int transCharToInt(char *_buffer, int _start, int _stop) //char to int?
{
    int _index;
    int result = 0;
    int num = _stop - _start + 1;
    int _temp[num];
    for (_index = _start; _index <= _stop; _index++)
    {
        _temp[_index - _start] = _buffer[_index] - '0';
        result = 10 * result + _temp[_index - _start];
    }
    return result;
}

int WindDirection() //Wind Direction
{
    return transCharToInt(databuffer, 1, 3);
}

float WindSpeedAverage() //air Speed (1 minute)
{
    temp = 0.44704 * transCharToInt(databuffer, 5, 7);
    return temp;
}

float WindSpeedMax() //Max air speed (5 minutes)
{
    temp = 0.44704 * transCharToInt(databuffer, 9, 11);
    return temp;
}

float Temperature() //Temperature ("C")
{
    temp = (transCharToInt(databuffer, 13, 15) - 32.00) * 5.00 / 9.00;
    return temp;
}

float RainfallOneHour() //Rainfall (1 hour)
{
    temp = transCharToInt(databuffer, 17, 19) * 25.40 * 0.01;
    return temp;
}

float RainfallOneDay() //Rainfall (24 hours)
{
    temp = transCharToInt(databuffer, 21, 23) * 25.40 * 0.01;
    return temp;
}

int Humidity() //Humidity
{
    return transCharToInt(databuffer, 25, 26);
}

float BarPressure() //Barometric Pressure
{
    temp = transCharToInt(databuffer, 28, 32);
    return temp / 10.00;
}

void loop()
{

    do
    {
        //get the data and test it is range
        getBuffer();
    } while (BarPressure() >= 1500 || BarPressure() <= 800 || Temperature() >= 40 || Temperature() <= 10 || Humidity() > 100 || Humidity() < 0);
        {
            //data is not in range
            delay(10);
            getBuffer();
        }
        
    Serial.print("Wind Direction: ");
    Serial.print(WindDirection());
    Serial.println("  ");
    Serial.print("Average Wind Speed (One Minute): ");
    Serial.print(WindSpeedAverage());
    Serial.println("m/s  ");
    Serial.print("Max Wind Speed (Five Minutes): ");
    Serial.print(WindSpeedMax());
    Serial.println("m/s");
    Serial.print("Rain Fall (One Hour): ");
    Serial.print(RainfallOneHour());
    Serial.println("mm  ");
    Serial.print("Rain Fall (24 Hour): ");
    Serial.print(RainfallOneDay());
    Serial.println("mm");
    Serial.print("Temperature: ");
    Serial.print(Temperature());
    Serial.println("C  ");
    Serial.print("Humidity: ");
    Serial.print(Humidity());
    Serial.println("%  ");
    Serial.print("Barometric Pressure: ");
    Serial.print(abs(BarPressure()));
    Serial.println("hPa");
    Serial.println("");


    sendingdata();
    delay(10);
}


void sendingdata()
{
    mySerial.println("AT");
    ShowSerialData();
    delay(500);

    mySerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
    ShowSerialData();
    delay(500);

    mySerial.println("AT+SAPBR=3,1,\"APN\",\"\"");
    ShowSerialData();
    delay(500);

    mySerial.println("AT+SAPBR=1,1");
    ShowSerialData();
    delay(500);

    mySerial.println("AT+SAPBR=2,1");
    ShowSerialData();
    delay(500);

    mySerial.println("AT+HTTPINIT");
    ShowSerialData();
    delay(500);

    mySerial.println("AT+HTTPPARA=\"CID\",1");
    ShowSerialData();
    delay(500);

    ShowSerialData();
    String str = "AT+HTTPPARA=\"URL\",\"api.mapid.io/api/update?key=d7e4d2eb368b918eb6b80b18d525f419&var1=" + String(lat) 
    + "&var2=" + String(lon)
    + "&var3=" + String(abs(Temperature())) 
    + "&var4=" + String(Humidity()) 
    + "&var5=" + String(abs(BarPressure())) 
    + "&var6=" + String(RainfallOneHour()) 
    + "&var7=" + String(RainfallOneDay()) 
    + "&var8=" + String(WindSpeedAverage()) 
    + "&var9=" + String(WindSpeedMax()) 
    + "&var10=" + String(WindDirection()) + "\"";
    mySerial.println(str);
    ShowSerialData();
    delay(500);

    mySerial.println("AT+HTTPSSL=1");
    ShowSerialData();
    delay(500);

    mySerial.println("AT+HTTPACTION=0");
    ShowSerialData();
    delay(500);
}

void ShowSerialData()
{
    while (mySerial.available() != 0) //displays data transmission activities on the Arduino serial monitor
        Serial.write(mySerial.read());
}
