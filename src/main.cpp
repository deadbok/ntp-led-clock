#include <Wire.h>
#include <TM1650.h>

#define SCL_PIN 12
#define SDA_PIN 13

TM1650 d;

void setup()
{
  Wire.begin(SDA_PIN, SCL_PIN); // Join the bus as master

  Serial.begin(115200);
  Serial.println("TM1650 Example Code");

  d.init();
}

void loop()
{
  Serial.println("0b00000001");
  Wire.beginTransmission(TM1650_DISPLAY_BASE);
  Wire.write(0b00000001);
  Wire.endTransmission();
  delay(5000);

  Serial.println("0b00000010");
  Wire.beginTransmission(TM1650_DISPLAY_BASE);
  Wire.write(0b00000010);
  Wire.endTransmission();
  delay(5000);
  
  Serial.println("0b00000100");
  Wire.beginTransmission(TM1650_DISPLAY_BASE);
  Wire.write(0b00000100);
  Wire.endTransmission();
  delay(5000);
  
  Serial.println("0b00001000");
  Wire.beginTransmission(TM1650_DISPLAY_BASE);
  Wire.write(0b00001000);
  Wire.endTransmission();
  delay(5000);

Serial.println("0b000100000");
  Wire.beginTransmission(TM1650_DISPLAY_BASE);
  Wire.write(0b00010000);
  Wire.endTransmission();
  delay(5000);

Serial.println("0b001000000");
  Wire.beginTransmission(TM1650_DISPLAY_BASE);
  Wire.write(0b00100000);
  Wire.endTransmission();
  delay(5000);

Serial.println("0b010000000");
  Wire.beginTransmission(TM1650_DISPLAY_BASE);
  Wire.write(0b01000000);
  Wire.endTransmission();
  delay(5000);

Serial.println("0b100000000");
  Wire.beginTransmission(TM1650_DISPLAY_BASE);
  Wire.write(0b10000000);
  Wire.endTransmission();
  delay(5000);

/*  Serial.println("Display off");
  d.displayOff();
  Serial.println("____"); 
  d.displayString("____");
  Serial.println("Minimum brightness");
  d.setBrightness(TM1650_MIN_BRIGHT);
  Serial.println("Display on");
  d.displayOn();
  delay(100);
  char line[] = "1234";

  Serial.println(line);
  d.displayString(line);
  Serial.println("Gradually to maximum brightness");
  d.setBrightnessGradually(TM1650_MAX_BRIGHT);
  delay(2000);
  Serial.println("Gradually to minimum brightness");
  d.setBrightnessGradually(TM1650_MIN_BRIGHT);
  Serial.println("Display off");
  d.displayOff();
  delay(1000);

  line[1] |= 128;
  Serial.println("Display on");
  d.displayOn();
  Serial.println("Gradually to maximum brightness");
  d.setBrightnessGradually(TM1650_MAX_BRIGHT);
  Serial.println(line);
  d.displayString(line);
  delay(2000);

  //  for (int i=0; i<8; i++) {
  //    d.setBrightness(i);
  //    delay(500);
  //  }

  Serial.println("abcd");
  d.displayString("abcd");
  delay(2000);

  Serial.println("789 ");
  d.displayString("789 ");
  delay(2000);

  Serial.println("Running: 1234567890abcdefghijklmnop");
  if (d.displayRunning("1234567890abcdefghijklmnop"))
  {
    while (d.displayRunningShift())
      delay(500);
  }
  delay(2000);


  for (int i = 0; i < 20; i++)
  {
    Serial.println("Display off");
    d.displayOff();
    delay(200);
    Serial.println("Display on");
    d.displayOn();
    delay(200);
  }

  for (int i = 0; i < 20; i++)
  {
    Serial.println("Set brightness 1");
    d.setBrightness(1);
    delay(200);
    Serial.println("Set brightness 7");
    d.setBrightness(7);
    delay(200);
  }

  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      Serial.printf("%d dot on\n", j);
      d.setDot(j, true);
      delay(200);
    }
    for (int j = 0; j < 4; j++)
    {
      Serial.printf("%d dot off\n", j);
      d.setDot(j, false);
      delay(200);
    }
  }*/
}
