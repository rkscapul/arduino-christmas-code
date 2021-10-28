#include <Wire.h>

char channels[] = { '1', 'A', 'B', 'C', 'D', 'E', 'F' };
char channelsReservedForParent[] = { '1' };

int activeParentChannels[] = { 3, 5 };
int controllerCount = 2;
int activeParentChannelsCount = 2;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  resetLights();
}

// Main Functions //
int isChannelReservedForParent(char c) {
  for (int i = 0; i < 1; i++) {
    if (channelsReservedForParent[i] == c) {
      return 1;
    }
  }

  return 0;
}

void writeToChannel(int controller, char channel, int value) {  
  if (isChannelReservedForParent(channel) == 1) {
    int x = (activeParentChannelsCount / controllerCount); 
    int y = (x - 1) + controller;

    analogWrite(activeParentChannels[y], value);

    return;
  }

  Wire.beginTransmission(controller + 1);
  Wire.write(channel);
  Wire.write(value);
  Wire.endTransmission();
}

void resetLights() {
  for (int c = 0; c < 7; c++) {
    writeToChannel(0, channels[c], 255);
    writeToChannel(1, channels[(6 - c)], 255);

    delay(100);
  }

  for (int c = 0; c < 7; c++) {
    writeToChannel(0, channels[c], 0);
    writeToChannel(1, channels[(6 - c)], 0);

    delay(100);
  }
}

void runOnAllChannels(int controller, int value) {
  for (int c = 0; c < 7; c++) {
    writeToChannel(controller, channels[c], value);
  }
}

void fade_IO_A(int controller, int _delay) {
  for (int o = 0; o < 255; o += 5) {
    runOnAllChannels(controller, o);
    delay(_delay);
  }

  for (int o = 255; o >= 0; o -= 5) {
    runOnAllChannels(controller, o);
    delay(_delay);
  }
}

void fade_I(int controller, char channel, int _delay) {
  for (int o = 0; o < 255; o += 5) {
    writeToChannel(controller, channel, o);
    if (o <= 255) {
      delay(_delay);
    }
  }
}

void fade_O(int controller, char channel, int _delay) {
  for (int o = 255; o >= 0; o -= 5) {
    writeToChannel(controller, channel, o);
    if (o >= 0) {
      delay(_delay);
    }
  }
}

void fade_IO(int controller, char channel, int _delay) {
  fade_I(controller, channel, _delay);
  fade_O(controller, channel, _delay);
}

void fade_IO_LR(int controller) {
  for (int c = 0; c < 7; c++) {
    fade_IO(controller, channels[c], 10);
  }
}

void fade_IO_RL(int controller) {
  for (int c = 6; c >= 0; c--) {
    fade_IO(controller, channels[c], 10);
  }
}

void fade_Wipe_LR(int controller, int _delay) {
  for (int c = 0; c < 7; c++) {
    fade_I(controller, channels[c], _delay);
  }

  for (int c = 0; c < 7; c++) {
    fade_O(controller, channels[c], _delay);
  }
}

void _blink(int controller, char channel, int _delay, int repeat, bool endDelay) {
  Serial.println("blink");
  for (int x = 0; x < repeat; x++) {
    writeToChannel(controller, channel, 255);
    delay(_delay);
    writeToChannel(controller, channel, 0);

    if (endDelay) {
      delay(_delay);
    }
  }
}

void hardWipeIn_LR(int controller, int _delay) {
  for (int c = 0; c < 7; c++) {
    writeToChannel(controller, channels[c], 255);
    delay(_delay);
  }
}

void hardWipeIn_RL(int controller, int _delay) {
  for (int c = 6; c >= 0; c--) {
    writeToChannel(controller, channels[c], 255);
    delay(_delay);
  }
}

void hardWipeOut_LR(int controller, int _delay) {
  for (int c = 0; c < 7; c++) {
    writeToChannel(controller, channels[c], 0);
     delay(_delay);
  }
}

void hardWipeOut_RL(int controller, int _delay) {
  for (int c = 6; c >= 0; c--) {
    writeToChannel(controller, channels[c], 0);
    delay(_delay);
  }
}

void blink_LR_SF(int controller) {
  for (int d = 500; d >= 25; d -= 50) {
    for (int c = 0; c < 7; c++) {
      _blink(controller, channels[c], d, 1, false);
    }
  }

  for (int d = 0; d < 20; d++) {
    for (int c = 0; c < 7; c++) {
      _blink(controller, channels[c], 50, 1, false);
    }
  }
}

void blink_RL_SF(int controller) {
  for (int d = 500; d >= 25; d -= 50) {
    for (int c = 6; c >= 0; c--) {
      _blink(controller, channels[c], d, 1, false);
    }
  }

  for (int d = 0; d < 20; d++) {
    for (int c = 6; c >= 0; c--) {
      _blink(controller, channels[c], 50, 1, false);
    }
  }
}

void twinkle_LR(int controller) {
  for (int d = 0; d < 10; d++) {
    for (int c = 0; c < 7; c++) {
      _blink(controller, channels[c], 50, 6, true);
    }
  }
}

void twinkle_RL(int controller) {
  for (int d = 0; d < 5; d++) {
    for (int c = 6; c >= 0; c--) {
      _blink(controller, channels[c], 50, 6, true);
    }
  }
}


// Custom //
void fade_IO_LR_2A() {
  hardWipeIn_LR(1, 50);
  for (int d = 0; d < 3; d++) {
    fade_IO_LR(0);
  }

  hardWipeOut_LR(1, 50);
}

void fade_IO_LR_1A() {
  hardWipeIn_LR(0, 50);
  for (int d = 0; d < 3; d++) {
    fade_IO_LR(1);
  }

  hardWipeOut_LR(1, 50);
}

void fade_IO_1LR_2RL() {
  for (int d = 0; d < 3; d++) {
    for (int c = 0; c < 7; c++) {
      for (int o = 0; o < 255; o += 5) {
        writeToChannel(0, channels[c], o);
        writeToChannel(1, channels[(6 - c)], o);
        if (o <= 255) {
          delay(10);
        }
      }
  
      for (int o = 255; o >= 0; o -= 5) {
        writeToChannel(0, channels[c], o);
        writeToChannel(1, channels[(6 - c)], o);
        if (o <= 255) {
          delay(10);
        }
      }
    }
  }
}

void fade_IO_1RL_2LR() {
  for (int d = 0; d < 3; d++) {
    for (int c = 6; c >= 0; c--) {
      for (int o = 0; o < 255; o += 5) {
        writeToChannel(0, channels[c], o);
        writeToChannel(1, channels[(6 - c)], o);
        if (o <= 255) {
          delay(10);
        }
      }
  
      for (int o = 255; o >= 0; o -= 5) {
        writeToChannel(0, channels[c], o);
        writeToChannel(1, channels[(6 - c)], o);
        if (o <= 255) {
          delay(10);
        }
      }
    }
  }
}


// twinkle 1-- 2--
void twinkle_1LR_2RL() {
  for (int d = 0; d < 5; d++) {
    for (int c = 0; c < 7; c++) {
      for (int r = 0; r < 6; r++) {
        writeToChannel(0, channels[c], 255);
        writeToChannel(1, channels[(6 - c)], 255);
    
        delay(50);
    
        writeToChannel(0, channels[c], 0);
        writeToChannel(1, channels[(6 - c)], 0);
  
        delay(50);
      }
    }
  }
}

void twinkle_1RL_2LR() {
  for (int d = 0; d < 5; d++) {
    for (int c = 6; c >= 0; c--) {
      for (int r = 0; r < 6; r++) {
        writeToChannel(0, channels[c], 255);
        writeToChannel(1, channels[(6 - c)], 255);
    
        delay(50);
    
        writeToChannel(0, channels[c], 0);
        writeToChannel(1, channels[(6 - c)], 0);
  
        delay(50);
      }
    }
  }
}


// blink 1-- 2--
void blink_1LR_2RL() {
  for (int d = 0; d < 20; d++) {
    for (int c = 0; c < 7; c++) {
      writeToChannel(0, channels[c], 255);
      writeToChannel(1, channels[(6 - c)], 255);

      delay(50);

      writeToChannel(0, channels[c], 0);
      writeToChannel(1, channels[(6 - c)], 0);
    }
  }
}

void blink_1RL_2LR() {
  for (int d = 0; d < 20; d++) {
    for (int c = 6; c >= 0; c--) {
      writeToChannel(0, channels[c], 255);
      writeToChannel(1, channels[(6 - c)], 255);

      delay(50);

      writeToChannel(0, channels[c], 0);
      writeToChannel(1, channels[(6 - c)], 0);
    }
  }
}

void blink_1LR_2RL_SF() {
  for (int d = 500; d >= 50; d -= 25) {
    for (int c = 0; c < 7; c++) {
      writeToChannel(0, channels[c], 255);
      writeToChannel(1, channels[(6 - c)], 255);

      delay(d);

      writeToChannel(0, channels[c], 0);
      writeToChannel(1, channels[(6 - c)], 0);
    }
  }
}

void blink_1RL_2LR_SF() {
  for (int d = 500; d >= 50; d -= 25) {
    for (int c = 6; c >= 0; c--) {
      writeToChannel(0, channels[c], 255);
      writeToChannel(1, channels[(6 - c)], 255);

      delay(d);

      writeToChannel(0, channels[c], 0);
      writeToChannel(1, channels[(6 - c)], 0);
    }
  }
}


// twinkle transition - twinkle fill & hard wipe out
void twinkleFill_LR_wipeOut_LR() {
  for (int c = 0; c < 7; c++) {
    for (int r = 0; r < 6; r++) {
      writeToChannel(0, channels[c], 255);
      writeToChannel(1, channels[(6 - c)], 255);

      delay(50);

      writeToChannel(0, channels[c], 0);
      writeToChannel(1, channels[(6 - c)], 0);

      delay(50);
    }

    writeToChannel(0, channels[c], 255);
    writeToChannel(1, channels[(6 - c)], 255);
  }

  delay(5000);

  for (int c = 0; c < 7; c++) {
    writeToChannel(0, channels[c], 0);
    writeToChannel(1, channels[(6 - c)], 0);

    delay(500);
  }
}

void twinkleFill_RL_wipeOut_RL() {
  for (int c = 6; c >= 0; c--) {
    for (int r = 0; r < 6; r++) {
      writeToChannel(0, channels[c], 255);
      writeToChannel(1, channels[(6 - c)], 255);

      delay(50);

      writeToChannel(0, channels[c], 0);
      writeToChannel(1, channels[(6 - c)], 0);

      delay(50);
    }

    writeToChannel(0, channels[c], 255);
    writeToChannel(1, channels[(6 - c)], 255);
  }

  delay(5000);

  for (int c = 0; c < 7; c++) {
    writeToChannel(0, channels[c], 0);
    writeToChannel(1, channels[(6 - c)], 0);

    delay(500);
  }
}

void twinkleController(int controller) {
  for (int r = 0; r < 6; r++) {
    runOnAllChannels(controller, 255);
    delay(50);
    runOnAllChannels(controller, 0);
    if (r < 5) {
      delay(50);
    }
  }
}

void twinkleController_AASC() {
  for (int x = 0; x < 5; x++) {
    for (int c = 0; c < 2; c++) {
      twinkleController(c);
    }
  }
}

void twinkleController_AASC_wipeOut_RL() {
  for (int c = 0; c < 2; c++) {
    twinkleController(c);
    runOnAllChannels(c, 255);
  }

  delay(2000);

  for (int c = 6; c >= 0; c--) {
    writeToChannel(0, channels[c], 0);
    writeToChannel(1, channels[(6 - c)], 0);

    delay(250);
  }
}

void twinkleController_AASC_wipeOut_LR() {
  for (int c = 0; c < 2; c++) {
    twinkleController(c);
    runOnAllChannels(c, 255);
  }

  delay(2000);

  for (int c = 0; c < 7; c++) {
    writeToChannel(0, channels[c], 0);
    writeToChannel(1, channels[(6 - c)], 0);

    delay(250);
  }
}

void hardWipe_1RL_2LR_IO(int repeat) {
  for (int c = 0; c < repeat; c++) {
    for (int r = 6; r >= 0; r--) {
      writeToChannel(0, channels[r], 255);
      writeToChannel(1, channels[(6 - r)], 255);
  
      delay(50);
    }

    for (int r = 6; r >= 0; r--) {
      writeToChannel(0, channels[r], 0);
      writeToChannel(1, channels[(6 - r)], 0);
  
      delay(50);
    }
  }
}

void hardWipe_1LR_2RL_IO(int repeat) {
  for (int c = 0; c < repeat; c++) {
    for (int r = 0; r < 7; r++) {
      writeToChannel(0, channels[r], 255);
      writeToChannel(1, channels[(6 - r)], 255);
  
      delay(50);
    }

    for (int r = 0; r < 7; r++) {
      writeToChannel(0, channels[r], 0);
      writeToChannel(1, channels[(6 - r)], 0);
  
      delay(50);
    }
  }
}

void fill_1LRFromR_2RLFromL() {
  for (int r = 6; r >= 0; r--) {
    for (int c = 0; c < r + 1; c++) {
      writeToChannel(0, channels[c], 255);
      writeToChannel(1, channels[(6 - c)], 255);

      delay(50);

      if (c < r) {
        writeToChannel(0, channels[c], 0);
        writeToChannel(1, channels[(6 - c)], 0);
      }
    }
  }
}

void fill_1RLFromL_2LRFromR() {
  for (int r = 0; r < 7; r++) {
    for (int c = 6; c >= r; c--) {
      writeToChannel(0, channels[c], 255);
      writeToChannel(1, channels[(6 - c)], 255);

      delay(50);

      if (c > r) {
        writeToChannel(0, channels[c], 0);
        writeToChannel(1, channels[(6 - c)], 0);
      }
    }
  }
}

void random_AC() {
  int controller = 0;

  long randomChannel;

  for (int d = 0; d < 100; d++) {
    randomChannel = random(0, 7);
    Serial.print(controller);
    Serial.print(randomChannel);
    Serial.println(channels[randomChannel]);
    _blink(controller, channels[randomChannel], 100, 0, false);

    controller++;
    if (controller >= controllerCount) {
      controller = 0;
    }
  }
}

void loop() {

  fade_IO_1RL_2LR();
  blink_1RL_2LR_SF();
  blink_1RL_2LR();
  twinkle_1RL_2LR();

  twinkleFill_RL_wipeOut_RL();
  hardWipe_1RL_2LR_IO(5);
  fill_1RLFromL_2LRFromR();
  delay(3000);
  twinkleController_AASC();
  twinkleController_AASC_wipeOut_RL();

  fade_IO_1LR_2RL();
  blink_1LR_2RL_SF();
  blink_1LR_2RL();
  twinkle_1LR_2RL();

  twinkleFill_LR_wipeOut_LR();
  hardWipe_1LR_2RL_IO(5);
  fill_1LRFromR_2RLFromL();
  delay(3000);
  twinkleController_AASC();
  twinkleController_AASC_wipeOut_LR();

}
