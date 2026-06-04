#include <NewPing.h>

// Pin definitions
#define LEFT_S0 44
#define LEFT_S1 45
#define LEFT_S2 42
#define LEFT_S3 41
#define LEFT_sensorOut 43

// Pin definitions
#define RIGHT_S0 36
#define RIGHT_S1 37
#define RIGHT_S2 34
#define RIGHT_S3 35
#define RIGHT_sensorOut 33

#define FRONT_TRIGGER_PIN  11  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define FRONT_ECHO_PIN     12  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.


#define SIDE_TRIGGER_PIN  52  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define SIDE_ECHO_PIN     53  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing front_sonar(FRONT_TRIGGER_PIN, FRONT_ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

NewPing side_sonar(SIDE_TRIGGER_PIN, SIDE_ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

// pins
#define ena 5
#define in1 6
#define in2 7
#define in3 8
#define in4 9
#define enb 10

#define leftSensor 4
#define centerSensor 3
#define rightSensor 2


const int ledPin = LED_BUILTIN;  
int ledState = LOW;  
unsigned long previousMillis = 0; 
const long intervalOn = 10;
const long intervalOff = 140;  
 

int speed=255;
int slowSpeed=190;
int leftSpeed = 0;
int rightSpeed = 0;

void setup() {

  pinMode(LEFT_S0,OUTPUT);    //pin modes
  pinMode(LEFT_S1,OUTPUT);
  pinMode(LEFT_S2,OUTPUT);
  pinMode(LEFT_S3,OUTPUT);
  pinMode(LEFT_sensorOut ,INPUT);

  digitalWrite(LEFT_S0,HIGH);  //Putting S0/S1 on HIGH/HIGH levels means the output frequency scalling is at 100%  (recommended)
  digitalWrite(LEFT_S1,HIGH); //LOW/LOW is off HIGH/LOW is 20% and  LOW/HIGH is  2%
  digitalWrite(LEFT_S2,HIGH);
  digitalWrite(LEFT_S3,HIGH);
  pinMode(RIGHT_S0,OUTPUT);    //pin modes
  pinMode(RIGHT_S1,OUTPUT);
  pinMode(RIGHT_S2,OUTPUT);
  pinMode(RIGHT_S3,OUTPUT);
  pinMode(RIGHT_sensorOut ,INPUT);
  digitalWrite(RIGHT_S0,HIGH);  //Putting S0/S1 on HIGH/HIGH levels means the output frequency scalling is at 100%  (recommended)
  digitalWrite(RIGHT_S1,HIGH); //LOW/LOW is off HIGH/LOW is 20% and  LOW/HIGH is  2%
  digitalWrite(RIGHT_S2,HIGH);
  digitalWrite(RIGHT_S3,HIGH);
 
  // put your setup code here, to run once:
  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enb, OUTPUT);

  pinMode(leftSensor, INPUT);
  pinMode(centerSensor, INPUT);
  pinMode(rightSensor, INPUT);

  Serial.begin(9600);

}

void forward() {
    Serial.print(" forward ");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

 leftSpeed = speed;
 rightSpeed = speed;
}

void right() {
  
    Serial.print(" right ");
  
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  leftSpeed = speed;
 rightSpeed = 0;
}

void hardright() {

    Serial.print(" hardright ");
  
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

 leftSpeed = speed;
 rightSpeed = speed;
}

void left() {

    Serial.print(" left ");
  
   digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

 leftSpeed = 0;
 rightSpeed = speed;

}


void hardleft() {

    Serial.print(" hardleft ");
  
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

 leftSpeed = speed;
 rightSpeed = speed;
}
void stop() {

    Serial.print(" stop ");
  
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

    
 leftSpeed = 0;
 rightSpeed = 0;

}

void loop() {
  //forward();
  //return;
  delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  int front_distance = front_sonar.ping_cm();
  Serial.print("front_distance: ");
  Serial.print(front_distance); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.print("cm ");

  if (front_distance > 0 && front_distance < 15)
  {

    stop();
    delay(50);

    int side_distance = side_sonar.ping_cm();
    Serial.print("side_distance: ");
    Serial.print(side_distance); // Send ping, get distance in cm and print result (0 = outside set distance range)
    Serial.println("cm");

    while (side_distance == 0 || side_distance > 15) {
      //  while(true){
      delay(50);
      side_distance = side_sonar.ping_cm();
      hardright();
      Serial.print("side_distance: 111 ");
      Serial.print(side_distance); // Send ping, get distance in cm and print result (0 = outside set distance range)
      Serial.println("cm");
    }
    while (digitalRead(centerSensor) == HIGH) {
      while (side_distance > 0 && side_distance < 75 && digitalRead(centerSensor) == HIGH) {
        //  while(true){
        delay(50);
        side_distance = side_sonar.ping_cm();
        forward();
        Serial.print("side_distance: 222 ");
        Serial.print(side_distance); // Send ping, get distance in cm and print result (0 = outside set distance range)
        Serial.println("cm");
      }
      while (side_distance == 0 || side_distance > 75 && digitalRead(centerSensor) == HIGH) {
        //  while(true){
        delay(50);
        side_distance = side_sonar.ping_cm();
        hardright();
        Serial.print("side_distance: ");
        Serial.print(side_distance); // Send ping, get distance in cm and print result (0 = outside set distance range)
        Serial.println("cm");
      }

    }
    Serial.println("stop");
  }
  
  Serial.print(digitalRead(leftSensor));
  Serial.print("\t");
  Serial.print(digitalRead(centerSensor));
  Serial.print("\t");
  Serial.print(digitalRead(rightSensor));

  if (digitalRead(leftSensor) == LOW && digitalRead(centerSensor) == HIGH && digitalRead(rightSensor) == HIGH)
  {
    left();
  }
  else if (digitalRead(leftSensor) == HIGH && digitalRead(centerSensor) == HIGH && digitalRead(rightSensor) == LOW)
  {
    right();
  }
  else if (digitalRead(leftSensor) == HIGH && digitalRead(centerSensor) == LOW && digitalRead(rightSensor) == LOW)
  {
    hardright();

  }
  else if (digitalRead(leftSensor) == HIGH && digitalRead(centerSensor) == LOW && digitalRead(rightSensor) == HIGH)
  {
    forward();

  }
  else if (digitalRead(leftSensor) == LOW && digitalRead(centerSensor) == LOW && digitalRead(rightSensor) == HIGH)
  {
    hardright();

  }
    else if (digitalRead(leftSensor) == LOW && digitalRead(centerSensor) == LOW && digitalRead(rightSensor) == LOW )
  {
   stop();
 
   Serial.print(" right  green = ");
   int  right_data=pulseIn( RIGHT_sensorOut ,LOW);       //here  we wait until "out" go LOW, we start measuring the 
   Serial.print(right_data);

   Serial.print(" left  green = ");
   int  left_data=pulseIn( LEFT_sensorOut ,LOW);       //here  we wait until "out" go LOW, we start measuring the 
   Serial.println(left_data);
    if(right_data<8 && left_data>8 ){
      right();
    }
    else if(right_data>8 && left_data<8 ){
      left();
    }

  }

  else {
    // stop();
  }

 //  hardright();

  unsigned long currentMillis = millis();
  if(ledState == LOW){
    if (currentMillis - previousMillis >= intervalOn) {
      previousMillis = currentMillis;
      ledState = HIGH;
      digitalWrite(ledPin, ledState);

      analogWrite(ena, 0);
      analogWrite(enb, 0);

    }
  }
  else{
    if (currentMillis - previousMillis >= intervalOff) {
      previousMillis = currentMillis;
      ledState = LOW;
      digitalWrite(ledPin, ledState);
      
       analogWrite(enb, leftSpeed);
       analogWrite(ena, rightSpeed);
    }
  }
}
