#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// Comment out this line if you want faster results/no serial
//#define SERIAL_VALUES

/*****************************************************************************/
// Tree config

int treeFrequencyBoundaries[] = {0, 7, 15, 22, 30, 35, 40, 50}; // to hold starts of each frequency



/*****************************************************************************/
// CONSTS
// DON'T MODIFY UNLESS YOU SCREWED SOMETHING UP

const int analogPin = 0; // read from multiplexer using analog input 0
const int strobePin = 2; // strobe is attached to digital pin 2
const int resetPin = 3; // reset is attached to digital pin 3

const int buttonPin1 = 12;
const int buttonPin2 = 13;
const int ledDataPin = 30;
const int numPixels = 50;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, ledDataPin, NEO_GRB + NEO_KHZ800);

/*****************************************************************************/
// NOT CONSTS
// YOU CAN CHANGE THESE, BUT IT WON'T HELP ANYTHING, THEY'LL CHANGE LATER ANYWAY
double maxBass = 0;
// int redPins[8];
// int greenPins[8];
// int bluePins[8];
int bright = 0;
int spectrumValue[7]; // to hold a2d values

int lastMode = 0;   // start here



int getButtonPress(int button)
{
    return digitalRead(button);
}

void clearLeds()
{
  for (unsigned int i = 0; i < strip.numPixels(); i++)
  {
      strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();
}

void getValues()
{
    digitalWrite(resetPin, HIGH);
    digitalWrite(resetPin, LOW);
    for (int i = 0; i < 7; i++)
    {
        digitalWrite(strobePin, LOW);
        delayMicroseconds(30); // to allow the output to settle
        spectrumValue[i] = analogRead(analogPin);
        digitalWrite(strobePin, HIGH);
    }
}

void printSerialValues() {
#ifdef SERIAL_VALUES
    for (int i = 0; i < 7; i++) {
        // comment out/remove the serial stuff to go faster
        // - its just here for show
        if (spectrumValue[i] < 10)
        {
            Serial.print(" ");
            Serial.print(spectrumValue[i]);
        }
        else if (spectrumValue[i] < 100 )
        {
            Serial.print(" ");
            Serial.print(spectrumValue[i]);
        }
        else
        {
            Serial.print(" ");
            Serial.print(spectrumValue[i]);
        }
    }
    Serial.println();
#endif
}

void ledGrEqBrightness()
{
    for (int i = 0; i < 7; i++)
    {
        int val = spectrumValue[i] / 4;

        // if it's low, just turn it off
        if      (val <  48) { bright = 0;   }
        else if (val <  96) { bright = 48;  }
        else if (val < 128) { bright = 64;  }
        else if (val < 192) { bright = 128; }
        else if (val < 224) { bright = 192; }
        else { bright = 256; }

        for (int j = treeFrequencyBoundaries[i]; j < treeFrequencyBoundaries[i+1] - 1; j++)
        {
          strip.setPixelColor(j, bright, 0, 0);
        }
    }
    strip.show();
}

void ledGrEqBass()
{
  	// Get bass value
  	double val = spectrumValue[0] + spectrumValue[1];

    if (val > maxBass)
    {
      maxBass = val;
    }

  	double percent = val / maxBass;
  	int bass = percent * 100.0;

#ifdef SERIAL_VALUES
    Serial.print("Val=");
    Serial.print(val);
    Serial.print("   Percent=");
    Serial.print(percent);
    Serial.print("   Bass=");
    Serial.print(bass);
    Serial.print("         ");
#endif

  	if (bass > 20) {
      strip.setPixelColor(0, 0, 128, 0);
    }
  	if (bass > 30) {
      strip.setPixelColor(1, 0, 128, 0);
    }
  	if (bass > 40) {
      strip.setPixelColor(2, 0, 128, 0);
    }
  	if (bass > 50) {
      strip.setPixelColor(3, 128, 128, 0);
    }
  	if (bass > 60) {
      strip.setPixelColor(4, 128, 128, 0);
    }
  	if (bass > 70) {
      strip.setPixelColor(5, 128, 128, 0);
    }
    if (bass > 80) {
      strip.setPixelColor(6, 128, 0, 0);
    }
    if (bass > 90) {
      strip.setPixelColor(7, 128, 0, 0);
    }
    strip.show();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


void setup()
{
    Serial.begin(9600);

    pinMode(analogPin, INPUT);
    pinMode(strobePin, OUTPUT);
    pinMode(resetPin, OUTPUT);
    analogReference(DEFAULT);

    digitalWrite(resetPin, LOW);
    digitalWrite(strobePin, HIGH);

    strip.begin();
    strip.show();

}

void loop()
{
    clearLeds();
    getValues();
    printSerialValues();

    // if (getButtonPress(buttonPin1) == HIGH)
    // {
    //     lastMode = 0;
    // }
    // if (getButtonPress(buttonPin2) == HIGH)
    // {
    //     lastMode = 1;
    // }

    // if (lastMode)
    // {
    //   ledGrEqBass();
    // }
    // else
    // {
      ledGrEqBrightness();
    // }
    delay(50);
}
