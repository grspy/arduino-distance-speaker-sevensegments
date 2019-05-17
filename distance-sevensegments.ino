#include <NewPing.h>
#include <SevenSeg.h>
#include <TimerFreeTone.h>

#define TONE_PIN 2			// Pin you have a speaker/piezo connected to
#define LED_PIN 13			// Pin you have a LED connected to
#define TRIGGER_PIN  A0		// Pin tied to trigger pin on the ultrasonic sensor
#define ECHO_PIN     A1  	// Pin tied to echo pin on the ultrasonic sensor
#define MAX_DISTANCE 400 	// Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

SevenSeg disp(6, 7, 8, 9, 10, 11, 12); //Instantiate a seven segment controller object
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

const int numDigits = 3;
int digitPins[numDigits] = {3, 4, 5};

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results
  pinMode(13, OUTPUT);	// LED
  
  disp.setDigitPins(numDigits, digitPins);
  disp.setCommonCathode();
  disp.setTimer(1);
  disp.startTimer();
}

void loop() {
  delay(500); // 29ms should be the shortest delay between pings.

  Serial.print("Ping: ");
  Serial.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");

  int uS = sonar.ping_median(10); // Send 10 pings and get the median value of the results
  int distance = uS / US_ROUNDTRIP_CM;
  
  disp.write(distance); // Write the stance to the seven segment display
  
  if ((distance > 10) && (distance < 20))
  {
    digitalWrite(13, HIGH);
    TimerFreeTone(TONE_PIN, 311, 70);
    digitalWrite(13, LOW);
  }
  else if (distance < 10) // Closer distance: Intensify the tones
  {
    digitalWrite(13, HIGH);
    TimerFreeTone(TONE_PIN, 311, 70);
    digitalWrite(13, LOW);
    delay(50);
    digitalWrite(13, HIGH);
    TimerFreeTone(TONE_PIN, 311, 70);
    digitalWrite(13, LOW);
    delay(50);
    digitalWrite(13, HIGH);
    TimerFreeTone(TONE_PIN, 311, 70);
    digitalWrite(13, LOW);
  }
}

ISR(TIMER1_COMPA_vect) {
  disp.interruptAction();
}
