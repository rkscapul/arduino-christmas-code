const int latchPin = 3; //latch pin of the 74HC595
const int clockPin = 4; //clock pin of the 74HC595
const int dataPin = 24;   //data pin of the 74HC595

long randomSelection; 

void setup() {
  Serial.begin(9600);
  
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
}

void loop() {
   effect_1();
}

void updateShiftRegister(byte leds, int _delay)
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, LSBFIRST, leds);
   digitalWrite(latchPin, HIGH);

   delay(_delay);
}

void effect_1() {
  byte PINS[] = {246, 96};

  for (int x = 0; x < 2; x++) {
    updateShiftRegister(PINS[x], 500);
  }
}

void effect_2() {
  int PINS[] = {254, 253, 251, 247, 239, 223};

  for (int a = 0; a < 10; a++) {
    for (int x = 1; x < 6; x++) {
      updateShiftRegister(PINS[x], 100);
    }

    for (int x = 4; x >= 0; x--) {
      updateShiftRegister(PINS[x], 100);
    }
  }
}

void effect_3() {
  int PINS[] = {254, 253, 251, 247, 239, 223};

  for (int x = 0; x < 100; x++) {
    long selection = random(0, 6);
    if (randomSelection == selection) {
      randomSelection = random(0, 6);
    } else {
      randomSelection = selection;
    }
    
    updateShiftRegister(PINS[randomSelection], 100);
  }
}

void effect_4() {
  int PINS[] = {254, 253, 251, 247, 239};
  for (int x = 0; x < 5; x++) {
    for (int y = 0; y < 5; y++) {
      updateShiftRegister(PINS[y], 100);
      updateShiftRegister(223, 100);
    }
  }

  for (int x = 0; x < 5; x++) {
    for (int y = 4; y >= 0; y--) {
      updateShiftRegister(PINS[y], 100);
      updateShiftRegister(223, 100);
    }
  }
}

void effect_5() {
  int PINS[] = {222, 221, 219, 215, 207};

  for (int a = 500; a >= 25; a -= 25) {
    for (int x = 0; x < 5; x++) {
      updateShiftRegister(PINS[x], a);
    }
  }
}

void effect_6() {
  //int PINS[] = {1, 2, 4, 8, 16, 32};
  int PINS[] = {222, 220, 216, 208, 192};

  for (int x = 0; x < 5; x++) {
    updateShiftRegister(PINS[x], 100);
  }
}

void effect_7() {
  int PINS[] = {225, 226, 228, 232, 240};

//  for (int a = 0; a < 10; a++) {
//    for (int x = 0; x < 5; x++) {
//      updateShiftRegister(PINS[x], 100);
//    }
//  }

  for (int a = 0; a < 10; a++) {
    for (int x = 4; x >= 0; x--) {
      updateShiftRegister(PINS[x], 100);
    }
  }
}

void effect_8() {
  int EXIT_PINS[] = {220, 216, 208, 192};
  for (int x = 3; x >= 0; x--) {
    updateShiftRegister(EXIT_PINS[x], 100);
  }
}

void effect_9() {
  for (int a = 0; a < 10; a++) {
    updateShiftRegister(223, 100);
    updateShiftRegister(192, 100);
    updateShiftRegister(224, 100);
    updateShiftRegister(255, 100);
  }

  for (int a = 0; a < 10; a++) {
    updateShiftRegister(224, 100);
    updateShiftRegister(192, 100);
    updateShiftRegister(223, 100);
    updateShiftRegister(255, 100);
  }

  updateShiftRegister(223, 100);
}


void effect_10() {
  int PINS[] = {222, 221, 219, 215, 207};

  for (int a = 0; a < 1; a++) {
    for (int x = 0; x < 5; x++) {
      updateShiftRegister(PINS[x], 1000);
    }
  }
}

void effect_11() {
  int PINS[] = { 207, 199, 195, 193, 192 }; 

  for (int x = 0; x < 5; x++) {
    int d = 1000;
    updateShiftRegister(PINS[x], 100);
  }
}

void effect_12() {
  int PINS[] = { 208, 216, 220, 222 };
  
  for (int x = 0; x < 4; x++) {
    int d = 1000;
    updateShiftRegister(PINS[x], 100);
  }

  
}

void effect_13() {
  int PINS[] = { 225, 226, 228, 232, 240 };

  for (int a = 0; a < 10; a++) {
    for (int x = 0; x < 5; x++) {
      updateShiftRegister(PINS[x], 100);
    }
  }
}

void effect_14() {
  int PINS[] = { 222, 220, 216, 208, 192 };

  for (int x = 0; x < 5; x++) {
    int d = 1000;
    updateShiftRegister(PINS[x], 100);
  }

  
}

void effect_15() {
  int PINS[] = { 193, 195, 199, 207 };

  for (int x = 0; x < 4; x++) {
    int d = 1000;
    updateShiftRegister(PINS[x], 100);
  }
}

void effect_16() {
  int PINS[] = {222, 221, 219, 215, 207};
  updateShiftRegister(223, 5000);

  for (int a = 0; a < 20; a++) {
    for (int x = 0; x < 5; x++) {
      updateShiftRegister(PINS[x], 100);
    }
  }
}

void effect_17() {
  int PINS[] = {222, 221, 219, 215, 207};
  updateShiftRegister(223, 5000);

  for (int a = 0; a < 20; a++) {
    for (int x = 4; x >= 0; x--) {
      updateShiftRegister(PINS[x], 100);
    }
  }
}
