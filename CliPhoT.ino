#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int minimum = 999;
int n, maximum = 0;
unsigned long avg = 0;

void setup() {
  Serial.begin(9600);
  pinMode(15, INPUT);
  pinMode(5, INPUT_PULLUP);


  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();

  // Clear the buffer
  display.clearDisplay();


}


void loop(){ 


  unsigned long start;
  unsigned long end;

  bool flash = false;
  bool click = false;

  if (digitalRead(5) == LOW){
    delay(500);
    if (digitalRead(5) == LOW){
      minimum = 999;
      n = 0;
      maximum = 0;
      avg = 0;
      display.clearDisplay();
      display.setTextSize(2);             // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE);        // Draw white text
      display.setCursor(0,0);
      display.println(F("STATS"));
      display.println(F("RESET"));
      display.display();
      delay(1000);
    }
  }

  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);
  display.print(F("READY"));

  display.setTextSize(1);
  display.print(F(" CliPhoT"));
  display.cp437(true);
  display.write(15);
  display.cp437(false);
  display.setTextSize(2);
  display.println();

  display.setTextSize(1);
  display.print(F("Current Avg: "));
  
  n > 0 ? display.print(avg/n) : display.print(0);
  display.println(F("ms"));

  display.print(F("Min: "));
  n > 0 ? display.print(minimum) : display.print(0);
  display.print(F("ms "));


  display.print(F("Max: "));
  display.print(maximum);
  display.print(F("ms"));


  display.display();
  
  while(!click){
    if (digitalRead(5) == LOW){
      start = micros();
      click = true;
    }
  }

  while(!flash){
    if (!digitalRead(15)){
      end = micros();
      flash = true;
    }
  }

  unsigned long time = (end - start)/1000;
  avg += time;
  n++;

  if (time > maximum) maximum = time;
  else if (time < minimum) minimum = time;

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  display.print(time);
  display.println(F("ms"));

  display.setTextSize(2);  
  display.print(F("avg: "));
  display.print(avg/n);
  display.print(F("ms"));
  display.display();

  Serial.print(time);
  Serial.print("ms, ");
  Serial.print((end - start));
  Serial.print("μs");
  Serial.println();
  delay(500);
}


