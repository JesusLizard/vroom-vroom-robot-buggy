#include "Arduino.h"
#include "Robot.h"

/* DRIVE SYSTEM */

// Drive constructor
Drive::Drive(int leftA, int leftB, int rightA, int rightB){
  _leftA = leftA;
  _leftB = leftB;
  _rightA = rightA;
  _rightB = rightB;
  _leftForwards = false;
  _rightForwards = false;
}

// Set up hardware for drive system, call this in setup()
void Drive::begin(){
  pinMode(_leftA, OUTPUT);
  pinMode(_leftB, OUTPUT);
  pinMode(_rightA, OUTPUT);
  pinMode(_rightB, OUTPUT);
}

// The below functions drive the robot in the suggested direction. They are passed power, an integer between 0 and 255.
void Drive::forward(int power){
  motors(power, power);
}

void Drive::backward(int power){
  motors((-1 * power), (-1 * power));
}

// The functions below peform a point turn, at the given power (Integer between 0 and 255). Arc turns can be initiated by calling left() and right() individually.
void Drive::turnLeft(int power){
  motors((-1 * power), power);
}

void Drive::turnRight(int power){
  motors(power, (-1 * power));
}

// this one stops the motors. Come as a surprise to anyone?
void Drive::stop(){
  // Write both terminals on both motors to high
  digitalWrite(_leftA, HIGH);
  digitalWrite(_leftB, HIGH);

  digitalWrite(_rightA, HIGH);
  digitalWrite(_rightB, HIGH);

  // Store that neither motor is moving
  _leftpower = 0;
  _rightpower = 0;
}

// Function to control the motors seperately. However, motors need to be controlled by one function to facilitate rampinig the powers up and down. A negative power turns the motor backwards
void Drive::motors(int powerLeftVec, int powerRightVec){
  // Get the absolute power values
  int powerLeft = abs(powerLeftVec);
  int powerRight = abs(powerRightVec);

  // If there is a sudden change in direction, turn off the motor
  if((powerLeftVec > 0) != _leftForwards){
    digitalWrite(_leftA, LOW);
    digitalWrite(_leftB, LOW);
    _leftpower = 0;
  }

  if((powerRightVec > 0) != _rightForwards){
    digitalWrite(_rightA, LOW);
    digitalWrite(_rightB, LOW);
    _rightpower = 0;
  }

  // Get a boolean as to whether each motor is forwards or backwards. true is forwards, false is backwards
  _leftForwards = (powerLeftVec > 0);
  _rightForwards = (powerRightVec > 0);

  // When left is forwards, _leftA is HIGH, while when it going backwards, _leftA is LOW
  digitalWrite(_leftA, (_leftForwards ? HIGH : LOW));
  digitalWrite(_rightA, (_rightForwards ? HIGH : LOW));

  // Ramp the powers to the desired rates, so as not to cause sudden changes and fry shit
  while (_leftpower != powerLeft || _rightpower != powerRight){
    // Adjust the power of the motor, if it is not already at the desired power
    if(_leftpower != powerLeft){
      _leftpower += (_leftpower > powerLeft ? -1 : 1);
      analogWrite(_leftB, (_leftForwards ? 255 - _leftpower : _leftpower));
    }
    if(_rightpower != powerRight){
      _rightpower += (_rightpower > powerRight? -1 : 1);
      analogWrite(_rightB, (_rightForwards ? 255 - _rightpower : _rightpower));
    }
    delay(1);
  }
  analogWrite(_leftB, (_leftForwards ? 255 - _leftpower : _leftpower));
  analogWrite(_rightB, (_rightForwards ? 255 - _rightpower : _rightpower));
}

/* ULTRASONIC SENSOR */

// Ultrasonic sensor constructor
Ultrasonic::Ultrasonic(int trig, int echo){
  _trig = trig;
  _echo = echo;
}

// Begin method for the ultrasonic sensor. This is to be called from within the setup() function so as to properly initialise the sensor pins
void Ultrasonic::begin(){
  pinMode(_trig, OUTPUT);
  pinMode(_echo, INPUT);
}

// Pings the ultrasonic sensor. Returns the distance to an object in centimetres as a floatiing point number
float Ultrasonic::ping(){
  // Send a pulse from the sensor
  digitalWrite(_trig, LOW);
  delayMicroseconds(5);
  digitalWrite(_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trig, LOW);

  // Get the time (in Microseconds) that it takes for the pulse to return to the sensor
  int duration = pulseIn(_echo, HIGH);

  // Return the distance travelled in centimeters
  return (duration / 2) * 0.03422;
}

/* INFRARED SENSOR */

// Infrared sensor constructor. Call one of these per infrared sensor that is on the robot
Infrared::Infrared(int pin){
  _pin = pin;
}

// Initialise the infrared sensor. This sets up the hardware side of things
void Infrared::begin(){
  pinMode(_pin, INPUT);
}

// Function to detect if the infrared sensor has an object in it's way
bool Infrared::obstructed(){
  return digitalRead(_pin);
}
