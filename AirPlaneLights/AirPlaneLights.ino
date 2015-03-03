/**
Modified Version

 Erics DIY Aircraft Navigation Lights Project
My Youtube Channel  : http://www.youtube.com/user/Shadow5549
Website http://mkme.org

V1 Set up basic lighting
Changed Pins to support nano
Fixed flash sequence
 
 * Features:
 * Landing light controlled via an RC channel (can use a Y-harness on flaps or landing gear channel)
 ##* 2 fading anti-collision beacons
 * Double flash strobe  +single Flash Tail Strobe
 +Flashing Beacon 

Adapted from : 
 * @author Harold Asbridge
 * @version 0.3
 * @date 2014-06-12

*/

/**
  modified by fokkerschrauber (Austria)
  Modification:  Airbus Pattern
  Tail Strobe light added, flashes syncronized to "Wing Strobe Lights" but only with the first of the double flashes.
  And beacon is Flashing syncron together but not to Strobe.
  
  If You want other aicraft patterns i.e. Fokker 100/70 then no flashing light is syncronized
  Boeing Beacons are not syncronized, and Strobes are not double flashing
  
  Bombardier Dash 8 's Beacons are not flashing but fading
*/

// Strobe settings
const int STB_PIN_LIGHT = 9; // Pin number for strobe light output
const int STB_PIN_LIGHTB = 8; // Pin number for strobe light output
const int STB_PIN_LIGHT_T = 7;  // Tail Strobe
const int STB_BLINK_INTERVAL = 1000000; // Blink interval for strobe light in microseconds

// Anti-collision beacon settings
const int ACB1_PIN_LIGHT = 6; // Pin number for anti-collision beacon 1
const int ACB2_PIN_LIGHT = 5; // Pin number for anti-collision beacon 2
const int ACB3_PIN_LIGHT = 4; // Pin number for anti-collision beacon 3
const int ACB_BLINK_INTERVAL = 1400000;
const int ACB_FADE_MIN = 0; // Minimum fade level for beacon (0-255)
const int ACB_FADE_MAX = 255; // Maximum fade level for beacon (0-255)
const int ACB_FADE_INTERVAL = 4000; // Fade step interval, in microseconds (lower numbers = faster fade)


unsigned long lastFadeTime = 0;
unsigned long lastACBStrobeTime = 0;
unsigned long lastStrobeTime = 0;
int currentFade = ACB_FADE_MIN;
int fadeDirection = 1;



// Called on power on or reset
void setup()
{
  // Declare output pins
  pinMode(STB_PIN_LIGHT, OUTPUT);
  pinMode(STB_PIN_LIGHTB, OUTPUT);
  pinMode(STB_PIN_LIGHT_T, OUTPUT);
  pinMode(ACB1_PIN_LIGHT, OUTPUT);
  pinMode(ACB2_PIN_LIGHT, OUTPUT);
  pinMode(ACB3_PIN_LIGHT, OUTPUT);
}

// Called continuouly
void loop()
{   

  unsigned long currentTime = micros();
  unsigned long currentACBTime = micros();
  
 // Check if it's time to fade the anti-collision lights
  if ((currentACBTime - lastACBStrobeTime) > ACB_BLINK_INTERVAL) {
    //doACBStrobe();
    doFade();
    lastACBStrobeTime = currentACBTime;
  }

  // Check if it's time to blink the strobes
  if ((currentTime - lastStrobeTime) > STB_BLINK_INTERVAL) {
    doStrobe();
    lastStrobeTime = currentTime; 
  }
  //doFade();
}

// Strobe double-blink
void doStrobe()
{
  digitalWrite(STB_PIN_LIGHT, HIGH);
  digitalWrite(STB_PIN_LIGHTB, HIGH);
  digitalWrite(STB_PIN_LIGHT_T, HIGH);
  delay(1000);
  digitalWrite(STB_PIN_LIGHT, LOW);
  digitalWrite(STB_PIN_LIGHTB, LOW);
  digitalWrite(STB_PIN_LIGHT_T, LOW);
  delay(500);
  digitalWrite(STB_PIN_LIGHT, HIGH);
  digitalWrite(STB_PIN_LIGHTB, HIGH);
  digitalWrite(STB_PIN_LIGHT_T, HIGH);
  delay(30);
  digitalWrite(STB_PIN_LIGHT, LOW);
  digitalWrite(STB_PIN_LIGHTB, LOW);
  digitalWrite(STB_PIN_LIGHT_T, HIGH);
}

void doACBStrobe()
{
  digitalWrite(ACB1_PIN_LIGHT, HIGH);
  digitalWrite(ACB2_PIN_LIGHT, HIGH);
  digitalWrite(ACB3_PIN_LIGHT, HIGH);
  delay(100);
  digitalWrite(ACB1_PIN_LIGHT, LOW);
  digitalWrite(ACB2_PIN_LIGHT, LOW);
  digitalWrite(ACB3_PIN_LIGHT, LOW);
}

// Fade anti-collision LEDs
void doFade()
{
  currentFade += fadeDirection;
  if (currentFade == ACB_FADE_MAX || currentFade == ACB_FADE_MIN) {
    // If we hit the fade limit, flash the high beacon, and flip the fade direction
    if (fadeDirection == 1) {
      digitalWrite(ACB1_PIN_LIGHT, 255);
    } else {
      digitalWrite(ACB2_PIN_LIGHT, 255);
    }
    delay(50); 
    fadeDirection *= -1; 
  }

  digitalWrite(ACB1_PIN_LIGHT, currentFade);
  digitalWrite(ACB2_PIN_LIGHT, currentFade);
  digitalWrite(ACB3_PIN_LIGHT, ACB_FADE_MAX - currentFade + ACB_FADE_MIN);
//  digitalWrite(ACB3_PIN_LIGHT, ACB_FADE_MAX - currentFade + ACB_FADE_MIN);
}
