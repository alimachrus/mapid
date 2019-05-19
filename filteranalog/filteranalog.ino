float DistanceMax;
float DistanceMin;

bool NotAccurate = true;

float AnalogValue;

void setup()
{
  Serial.begin(9600);
}

float  getDistance() {
AnalogValue = analogRead(A0);
  return AnalogValue;
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
 
  Serial.print("Value: ");
  Serial.println(getDistance(), 0);
  NotAccurate = true;
  delay(1000);
}
