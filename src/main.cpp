/*
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

//Variavel que armazenará letras/números para o acionamento dos leds.
char caracter;

void setup(){
    Serial.begin(9600);
}

void loop(){
if(Serial.available()){
    
    caracter = Serial.read();
    Serial.print("Recebi os dados;");
    Serial.println(caracter);
         
          
// Condições para quando for precionada a respectiva letra, executa LIGA/DESLIGA LED.         
    if(caracter == 'a')
    {
        digitalWrite(13,HIGH);
    } else {
        digitalWrite(13,LOW);
    }

}
}
*/

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <SoftwareSerial.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);

SoftwareSerial bluetooth(0, 1);
String r = "";
String g = "";
String b = "";
String colors = "";
String w = "";
char buf[100];
// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.

void writeLEDs(int red, int green, int blue, int white){
  for(int i=0;i<30;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setBrightness(white);
    pixels.setPixelColor(i, red, green, blue); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  while(1){
    for(j=0; j<256; j++) {
      for(i=0; i< strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        if(bluetooth.available()){
          colors = bluetooth.readString();
          w = colors.substring(0,2);
          if(w != "00"){
            r = colors.substring(2,4);
            g = colors.substring(4,6);    
            b = colors.substring(6,8);
            w.toCharArray(buf,w.length() + 1);
            unsigned long white = strtoul(buf,NULL,16); 

            r.toCharArray(buf,r.length() + 1);
            unsigned long red = strtoul(buf,NULL,16); 

            g.toCharArray(buf,g.length() + 1);
            unsigned long green = strtoul(buf,NULL,16);

            b.toCharArray(buf,b.length() + 1);
            unsigned long blue = strtoul(buf,NULL,16);  

            writeLEDs(red, green, blue, white);

            return;
          }
            
        }
      }
      strip.show();
      delay(wait);
    }
  }
}

void oneColor(){ // Verifica se existe algum dado a ser lido da serial
  while(bluetooth.available()){ // verifica se existem bytes a serem lidos da porta serial virtual
    colors = bluetooth.readString();
    Serial.println(colors);
    w = colors.substring(0,2);
    r = colors.substring(2,4);
    g = colors.substring(4,6);
    b = colors.substring(6,8);
    
    if(w != "00"){
      w.toCharArray(buf,w.length() + 1);
      unsigned long white = strtoul(buf,NULL,16); 

      r.toCharArray(buf,r.length() + 1);
      unsigned long red = strtoul(buf,NULL,16); 

      g.toCharArray(buf,g.length() + 1);
      unsigned long green = strtoul(buf,NULL,16);

      b.toCharArray(buf,b.length() + 1);
      unsigned long blue = strtoul(buf,NULL,16);  

      writeLEDs(red, green, blue, white);
    } else {
      rainbowCycle(10);
    }

  }
}   



char caracter;

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  Serial.begin(115200);
  bluetooth.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Condições para quando for precionada a respectiva letra, executa LIGA/DESLIGA LED.         
  oneColor();
}
