#include <Wire.h>

char channels[] = { '1', 'A', 'B', 'C', 'D', 'E', 'F' };
char channelsReservedForParent[] = { '1' };

int activeParentChannels[] = { 3, 4 };
int controllerCount = 2;
int activeParentChannelsCount = 2;

void setup() {
  Wire.begin();
  Serial.begin(9600);
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

void loop() {
    for (int c = 0; c < 7; c++) {
      for (int o = 0; o < 255; o += 5) {
        writeToChannel(0, channels[c], o);
        delay(10);
      }

      for (int o = 255; o >= 0; o -= 5) {
        writeToChannel(0, channels[c], o);
        delay(10);
      }
    }
}
