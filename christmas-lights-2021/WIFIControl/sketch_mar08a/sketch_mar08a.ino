String payload;

void setup()
{
  Serial.begin(9600);
}


void loop()
{
  int data=50;
  while(Serial.available())
  {
    char c = Serial.read();
    //Serial.print(c);


    if (c == '\n') {
      Serial.println(payload);
      payload = "";
    } else {
      payload += c;
    }
  }
}
