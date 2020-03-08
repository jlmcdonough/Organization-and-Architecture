const int sensorPin = A0;


const int greenLEDPin = 9;
const int blueLEDPin = 10;
const int redLEDPin = 11;

const float upperBound = 4; //range

static int counter = 0;
static float rollingTemp[5];
static float baselineTemp = 0.0;
static int sensorVal;
static float voltage;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);
  baselineTemp = initialTemp();
}

void loop() {
  // put your main code here, to run repeatedly:
  float sumTemp = 0;
  float avgTemp = 0;
  int redValue = 0;
  int blueValue = 255;
  float temperature = getTemp();

  float * currTemp = shiftTemp(rollingTemp, temperature);  //gets array pointer currTemp which references the shifted temp array
  
  for (int l = 0; l<5; l++)   //gets sum of rolling temp
  {
    sumTemp += currTemp[l];
  }
  avgTemp = sumTemp/5;
  float change = avgTemp - baselineTemp; 
  
  if(avgTemp<baselineTemp)  //if falls below baseline (blowing cold air on it?) it will stay blue 
  {
    redValue = 0;
    blueValue = 255;
    analogWrite(redLEDPin, redValue);
    analogWrite(blueLEDPin, blueValue);
  }
  else if(avgTemp>baselineTemp+upperBound) //when greater than alloted upperBound, it will remain red
  {
    redValue = 255;
    blueValue = 0;
    analogWrite(redLEDPin, redValue);
    analogWrite(blueLEDPin, blueValue);
  }
  else   
  {
    redValue = 0 + (change * (255/upperBound));   //255 total digits so 255/maxTemp is how many digits per degree change
    blueValue = 255 - (change * (255/upperBound)); 
    analogWrite(redLEDPin, redValue);
    analogWrite(blueLEDPin, blueValue);
  }

  Serial.print("Sensor Value: ");
  Serial.print(sensorVal);
  Serial.print(" , Normalized Sensor Value: ");
  int nSV = ((10.24*avgTemp)+512)/5;  //reverted formula to get temperature from sensor value since average sensor value is not kept, but rather average temperature
  Serial.print(nSV);
  Serial.print(" , Volts: ");
  Serial.print(voltage);
  Serial.print(" , Degrees C: ");
  Serial.print(temperature);
  Serial.print(" , Degrees C Over Baseline: ");
  Serial.print(change);
  Serial.print(" , Red Value: ");
  Serial.print(redValue);
  Serial.print(" , Blue Value: ");
  Serial.print(blueValue);
  Serial.println(" ");
  
  for (int m = 0; m<5; m++)   //updates rollingTemp array with the latest temperature reading
  {
    rollingTemp[m] = currTemp[m];
  }
}

float * shiftTemp(float arr[], float temp)  //shifts old temps down one and replaces oldest one with current temp
{
  static float shiftedArray[5];
  for (int j = 0; j<4; j++)
  {
    shiftedArray[j] = arr[j+1];
  }
    shiftedArray[4] = temp;
  return shiftedArray;
}

float initialTemp()    //gets the initial baseline temperature
{
  float baseline[20];
  int k = 0;
  float baselineSum,baselineAverage = 0;
  while(k < 20) 
  {
    int sensorVal = analogRead(sensorPin);
    float voltage = (sensorVal/1024.0)*5.0;
    float temperature = (voltage - .5) *100;
    baseline[k] = temperature;
    k++;
  }
  for (int n =0; n<20; n++)
  {
    baselineSum += baseline[n];
  }
  baselineAverage = baselineSum/20;
  return baselineAverage;
}

float getTemp()   //reads current temperature
{
  sensorVal = analogRead(sensorPin);
  voltage = (sensorVal/1024.0)*5.0;
  float temperature = (voltage - .5) *100;
  return temperature;
}
