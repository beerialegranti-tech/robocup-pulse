#include <Ultrasonic.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

// Left color sensor
#define LS0 14 
#define LS1 15 
#define LS2 16
#define LS3 17
#define LOUT 18

// Right color sensor
#define RS3 13
#define RS2 12
#define ROUT 11
#define RS0 30
#define RS1 31

#define ena 5
#define in1 6
#define in2 7
#define in3 8
#define in4 9
#define enb 10

#define LS 2
#define MS 3
#define RS 4

#define front_trigPin 40
#define front_echoPin 41

#define side_trigPin 13
#define side_echoPin 12

#define LED_PIN 8

Ultrasonic ultrasonic_front(front_trigPin, front_echoPin);
Ultrasonic ultrasonic_side(side_trigPin, side_echoPin);

const int object_detection_distance = 12; // cm
unsigned long object_evation_timer = 0;
const int object_evation_time = 200;

int LgreenFrequency = 0;
int RgreenFrequency = 0;

int speed=255;
int slowSpeed=190;

int leftSpeed = 155;
int rightSpeed = 155;

int lastFunction = 0;


const int ledPin = LED_BUILTIN;  
int isOn = false;  
unsigned long previousMillis = 0; 

const long intervalOn = 30;
const long intervalOff = 120;  

bool lookingUp = false;;

void setup()
{
 Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
  Serial.println("");
  delay(100);
 
  // put your setup code here, to run once:
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(ena, OUTPUT);

  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  pinMode(LS, INPUT);
  pinMode(MS, INPUT);
  pinMode(RS, INPUT);

  Serial.begin(9600);

}

void forward()
{

  // Serial.print("forward");
  if (lookingUp == true)
  {
    analogWrite(ena, 255);
    analogWrite(enb, 255);
  }
  else
  {
    leftSpeed = speed-50;
    rightSpeed = speed;
  }
  
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  
  lastFunction = 0;

 Serial.print("forward");
}

void spin()
{

  // Serial.print("forward");

  analogWrite(ena, 255);
  analogWrite(enb, 0);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  
  lastFunction = 3;

 Serial.print("SPIN");
}


void sharp_left()
{

  // Serial.print("sharp right");
 leftSpeed = speed - 130;
 rightSpeed = speed;


  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void sharp_right()
{

  // Serial.print("sharp right");
 leftSpeed = speed;
 rightSpeed = speed - 130;


  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void right()
{

  // Serial.print("right");
 leftSpeed = speed;
 rightSpeed = 0;

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

 lastFunction = 1;

 
}

void left()
{

  // Serial.print("left");

 leftSpeed = 0;
 rightSpeed = speed;

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

lastFunction = 2;


}

void stop()
{
  analogWrite(ena, 0);
  analogWrite(enb, 0);
}

void obstacle()
{
  digitalWrite(LED_PIN, HIGH);
  stop();
  delay(1000);

  int distance = 357;

  left();
  ;
  distance = ultrasonic_side.read();
  if (distance == 0)
  {
    distance = 357;
  }

  distance = 357;
  while (digitalRead(LS) == HIGH && digitalRead(MS) == HIGH && digitalRead(RS) == HIGH)
  {
    if (millis() < object_evation_timer + object_evation_time)
    {
      forward();
      digitalWrite(LED_PIN, HIGH);
      Serial.println(millis());
    }
    else
    {
      object_evation_timer = millis();
      Serial.print(" heheh ");
      Serial.println(object_evation_timer);

      distance = ultrasonic_side.read();
      if (distance == 0)
      {
        distance = 357;
      }

      Serial.println(distance);

      while (distance > object_detection_distance + 30)
      {
        digitalWrite(LED_PIN, LOW);
        sharp_right();
        distance = ultrasonic_side.read();
        if (distance == 0)
        {
          distance = 357;
        }
        Serial.println(distance);
      }
      delay(100); // to let it finish a bit more turning
    }
  }
  left();
}

void loop()
{

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);


  Serial.print(" Y: ");
  Serial.print(a.acceleration.y);
  int AccY=a.acceleration.y;
  if(AccY>2){
     Serial.println("  UP");
  lookingUp - true;
  }
  else{
     Serial.println("  DOWN");
  lookingUp - false;
  }

  if (digitalRead(LS) == HIGH && digitalRead(RS) == LOW)
  {
    if (digitalRead(MS) == LOW)
    {
      sharp_right();
    }
    else
    {
      right(); 
    }
  }

 if (digitalRead(LS) == HIGH && digitalRead(MS) == HIGH && digitalRead(RS) == HIGH)
    {
      forward();
    }

  if (digitalRead(LS) == LOW && digitalRead(RS) == HIGH)
  {
    if (digitalRead(MS) == LOW)
    {
     sharp_left();
    }
    else
    {
      left(); 
    }

  }

  if (digitalRead(LS) == LOW && digitalRead(MS) == LOW && digitalRead(RS) == LOW)
  {


   // stop();
    // 2. Read LGreeUn Photodiodes
    digitalWrite(LS2, HIGH);
    digitalWrite(LS3, HIGH);
    LgreenFrequency = pulseIn(LOUT, LOW, 30000);
    Serial.print("LG= ");
    Serial.print(LgreenFrequency);
    Serial.print("  ");

    // 2. Read RGreen Photodiodes
    digitalWrite(RS2, HIGH);
    digitalWrite(RS3, HIGH);
    RgreenFrequency = pulseIn(ROUT, LOW, 30000);
    Serial.print("RG= ");
    Serial.print(RgreenFrequency);
    Serial.println("  ");

    if (LgreenFrequency < 9 && RgreenFrequency > 9)
    {
     Serial.println("right");
     right();

    }
    else if (LgreenFrequency  > 9 && RgreenFrequency < 9)
    {
     Serial.println("left");
     left();

    }
     else if (LgreenFrequency  < 9 && RgreenFrequency < 9)
    {
     Serial.println("forward");
     forward();
     
    }

      else if (LgreenFrequency  > 10 && RgreenFrequency > 10)
    {
     Serial.println("spin");
     spin();
     delay(800);

    }
  }


    Serial.print(" ");
    Serial.print(digitalRead(LS));
    Serial.print(" ");
    Serial.print(digitalRead(MS));
    Serial.print(" ");
    Serial.print(digitalRead(RS));
    Serial.println(" ");
/*  
    Serial.print(leftSpeed);
    Serial.print(" ");
    Serial.println(rightSpeed); 
  */

//put temporary code here:
//---------------------------
forward();
//---------------------------

  unsigned long currentMillis = millis();
  if(isOn == false){
    if (currentMillis - previousMillis >= intervalOn) {
      previousMillis = currentMillis;
        isOn = true;
      analogWrite(ena, 0);
      analogWrite(enb, 0);
     
    }
  }
  else{
    if (currentMillis - previousMillis >= intervalOff) {
      previousMillis = currentMillis;
        isOn = false;

       analogWrite(ena, leftSpeed);
       analogWrite(enb, rightSpeed);

       
    }
  
  }

}
