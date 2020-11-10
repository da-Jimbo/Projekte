//(C)2010 by da-Jimbo
//made for Arduino Pro Mini

#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

int RELAISPIN = 2; //Relais an PIN2

#define LEDSPIN 6 //LEDs an PIN6
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(3, LEDSPIN, NEO_GRB + NEO_KHZ800);
int delayval = 500; //LED intervall

int STATUS = 0;

void setup() {
  Serial.begin(9600);

  pinMode(RELAISPIN, OUTPUT); // Initialisieren des Relaisausgangs

  Wire.begin(); // Initialisieren des I2C-Bus fuer den BH1750
  lightMeter.begin();

  pixels.begin(); // Initialisieren der LEDs
}
void Helligkeit() { //Funktion für die Helligkeitsmessung
  delay(140);
  float lux = lightMeter.readLightLevel();
  Serial.println(lux);
}
void TorOeffnen() { //Funktion um das Relais eine Sekunde ein und dann wieder aus zu schalten
  Serial.println("Schleifenende");
  digitalWrite(RELAISPIN, HIGH);  // Schaltet ein
  pixels.setPixelColor(2, pixels.Color(0, 255, 0)); // LED gruen leuchten lassen
  pixels.setBrightness(255);
  pixels.show(); 
  delay(1000);

  digitalWrite(RELAISPIN, LOW);  // Schaltet aus
  pixels.setPixelColor(2, pixels.Color(255, 0, 0)); //LED rot leuchten lassen
  pixels.setBrightness(15);
  pixels.show(); 
}
void Wartelicht() { //Leds Pulsieren lassen
  int TOTAL_LEDS = 3;
  int RED = 0;
  int GREEN = 0;
  int BLUE = 150;
  float MaximumBrightness = 255;
  float SpeedFactor = 0.008; 
  float StepDelay = 5; 

  for (int i = 0; i < 65535; i++) {
    float intensity = MaximumBrightness / 2.0 * (1.0 + sin(SpeedFactor * i));
    pixels.setBrightness(intensity);
    for (int ledNumber = 0; ledNumber < TOTAL_LEDS; ledNumber++) {
      pixels.setPixelColor(ledNumber, RED, GREEN, BLUE);
    }

    pixels.show();
    delay(StepDelay);
  }
}
void LEDaus(){ //Funktion um die LEDs aus zu schalten (wird fuer Zukuenftige Updates benoetigt)
  pixels.clear();
  pixels.show();
}

void loop() {
  if (STATUS == 0) {
    LEDaus(); //erstmal LEDs aus schalten
    do { //Schleife so lang bis der Helligkeitswert ueber 25 geht (kalibriert am Auto)
      delay(140); //Alle 140ms eine Messung um das Blinksignal sicher einzufangen
      float lux = lightMeter.readLightLevel();
      Serial.println(lux);
      if (lux > 25)break; 
    } while (true);    // Wiederhole bis break
    STATUS = 1;
  }
  if (STATUS = 1) {
    TorOeffnen(); //Relais schaltet wie in der obigen Funktion einmal fuer eine Sekunde ein dann aus
    STATUS = 2;
  }
  if (STATUS = 2) {
    Wartelicht(); //Wartelicht um nicht noch einmal den Toroeffner zu aktivieren (z.B. Wenn in der Garage helles licht an ist.)
  }
}
