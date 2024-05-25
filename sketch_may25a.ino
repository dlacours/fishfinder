#include <math.h>
using namespace std;
/*
  Ping))) Sensor

  This sketch reads a PING))) ultrasonic rangefinder and returns the distance
  to the closest object in range using a phased array with two sensors. To do this, it sends a pulse to the sensor to
  initiate a reading, then listens for a pulse to return on two separate sensors. The length of the
  returning pulse is proportional to the distance of the object from the sensor, and the time delay between sensors receiving the same pulse determines the angle to target.
  This used two HC-SR04 separated by (insert num of pins here) pins on a breadboard

  The circuit:
	- VCC_1 connection of the SENS_1))) attached to +5V
	- GND_1 connection of the SENS_1))) attached to ground
	- ECHO_1 connection of the SENS_1))) attached to digital pin 7
	- VCC_2 connection of the SENS_2))) attached to +5V
	- GND_2 connection of the SENS_2))) attached to ground
	- ECHO_2 connection of the SENS_2))) attached to digital pin 8
	- TRIG_2 connection of the SENS_2))) attached to digital pin 9

  created 3 Nov 2008
  by David A. Mellis
  modified 25 May 2024
  by Dylan LaCourse

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Ping
*/

// this constant won't change. It's the pin number of the sensor's output:
const int readPin_1 = 7;
const int readPin_2 = 8;
const int pingPin_2 = 9;

const double DISTANCE_BETWEEN_SENSORS = 7.15; // cm

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  pinMode(pingPin_2, OUTPUT);
  pinMode(readPin_1, INPUT);
  pinMode(readPin_2, INPUT);
}

void loop() {
  // establish variables for duration of the ping, and the distance result
  // in inches and centimeters:
  long duration_1, duration_2, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:

  digitalWrite(pingPin_2, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin_2, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin_2, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.

  duration_1 = pulseIn(readPin_1, HIGH);
  duration_2 = pulseIn(readPin_2, HIGH);

  // convert the time into a distance
  double d1 = (double)microsecondsToCentimeters(duration_1);
  double d2 = (double)microsecondsToCentimeters(duration_2);

  double numerator = pow(DISTANCE_BETWEEN_SENSORS, 2) - pow(d1, 2) - pow(d2, 2) ;
  double denominator = 2 * d1 * d2;
  double theta = acos(numerator/denominator);

  double x = d2 * cos(theta);
  double y = d2 * sin(theta);

  Serial.print("x coordinate: ");
  Serial.println(x);
  Serial.print("y coordinate: ");
  Serial.println(y);
  Serial.println();

  delay(100);
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: https://www.parallax.com/package/ping-ultrasonic-distance-sensor-downloads/
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}
