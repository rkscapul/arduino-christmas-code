#include <Wire.h>

String msg = "";
int x = 0;
int pins[][3] = { {3, 5, 6}, {9, 10, 11} };

void setup() {
  Wire.begin(1);
  Wire.onReceive(receiveEvent);
  
  Serial.begin(9600);
}

void receiveEvent(int bytes) {
  String payload = "";

  char channel = Wire.read();

  int x = Wire.read();

  assignValues(channel, x);
}

void assignValues(char channel, int value) {
  int set_I = 0;

  // Send data to assigned pin //
  switch (channel) {
    case 'A':
      aWrite(3, value);
      break;
    case 'B':
      aWrite(5, value);
      break;
    case 'C':
      aWrite(6, value);
      break;
    case 'D':
      aWrite(9, value);
      break;
    case 'E':
      aWrite(10, value);
      break;
    case 'F':
      aWrite(11, value);
      break;
  }
}

void aWrite(int pin, int value) {
  Serial.print(pin);
  Serial.print(" ");
  Serial.println(value);

  analogWrite(pin, value);
}

void loop() {
  
}
