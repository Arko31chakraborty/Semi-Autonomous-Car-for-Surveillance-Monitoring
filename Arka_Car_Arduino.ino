#include <Servo.h>

int L1 = 7, L2 = 6;       // Left motor pins
int R1 = 5, R2 = 4;       // Right motor pins
int ServoPin = 3;         // Servo motor pin
int trig = A0, echo = A1; // Ultrasonic sensor pins

Servo myServo;

char command = 'S';       // Default to Stop
bool isMoving = false;

long readDistanceCM() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH, 30000); // timeout in case of no echo
  long distance = duration * 0.034 / 2;
  return distance;
}

void stopCar() {
  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
  digitalWrite(R1, LOW);
  digitalWrite(R2, LOW);
  isMoving = false;
}

void moveForward() {
  digitalWrite(L1, HIGH);
  digitalWrite(L2, LOW);
  digitalWrite(R1, HIGH);
  digitalWrite(R2, LOW);
  isMoving = true;
}

void moveBackward() {
  digitalWrite(L1, LOW);
  digitalWrite(L2, HIGH);
  digitalWrite(R1, LOW);
  digitalWrite(R2, HIGH);
  isMoving = true;
}

void turnLeft() {
  digitalWrite(L1, LOW);
  digitalWrite(L2, HIGH);
  digitalWrite(R1, HIGH);
  digitalWrite(R2, LOW);
  isMoving = true;
}

void turnRight() {
  digitalWrite(L1, HIGH);
  digitalWrite(L2, LOW);
  digitalWrite(R1, LOW);
  digitalWrite(R2, HIGH);
  isMoving = true;
}

void sweepServo() {
  for (int pos = 0; pos <= 180; pos += 90) {
    myServo.write(pos);
    delay(500);
  }
}

void setup() {
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  
  myServo.attach(ServoPin);
  myServo.write(90); // Default to center

  Serial.begin(115200);
  stopCar();
}

void loop() {
  if (Serial.available()) {
    command = Serial.read();
//    command = toupper(command); // Make it case-insensitive

    switch (command) {
      case 'F':
        moveForward();
        break;
      case 'B':
        moveBackward();
        break;
      case 'L':
        turnLeft();
        break;
      case 'R':
        turnRight();
        break;
      case 'S':
        stopCar();
        break;
    }
  }

  // Check distance only if car is moving forward
  if (isMoving && command == 'F') {
    long distance = readDistanceCM();
//    Serial.print("Distance: ");
//    Serial.print(distance);
//    Serial.println(" cm");

    if (distance > 0 && distance < 10) {
      stopCar();
//      Serial.println("Obstacle detected! Car stopped.");
      sweepServo();
    }
  }
}
