#ifndef Robot_h
#define Robot_h

#include "Arduino.h"

// Class to attach a drivetrain. This is defined as two dc motors, attached to a L9110S driver board.
class Drive {
  public:
    // The constructor for a drive instance. Requires the pin numbers of the left motor port and right motor port. Note that sideA is used as the direction control, and sideB is used as the power control
    Drive(int leftA, int leftB, int rightA, int rightB);
    // Method to set up hardware for the drive system. Call this in setup()
    void begin();

    // Moves the robot forwards at given power. power must be a positive integer between 0 and 255. Controls both wheels
    void forward(int power);
    // Moves the robot backwards at given power. power must be a positive integer between 0 and 255. Controls both wheels
    void backward(int power);
    // Does a point turn to the left (anti-clockwise) at the given power. power must be a positive integer between 0 and 255
    void turnLeft(int power);
    // Does a point turn to the right (clockwise) at the given power. power must be a positive integer between 0 and 255
    void turnRight(int power);
    // Sort of self documenting. It stops the motors
    void stop();
    // Function to control the drive train as seperate motors. Still needs to be in one function in order to ramp the power up and down
    void motors(int powerLeftVec, int powerRightVec);

  private:
    // Varaibles to store the current power of the left and right wheels. This is so that sudden changes in current draw can be avoided. Even with a capacitor over the motor power supply, I think I should build some software protection in as well. Also, variables for storing the motor pins
    int _leftpower, _rightpower, _leftA, _leftB, _rightA, _rightB;

    bool _leftForwards, _rightForwards;

};

// If an ultrasonic sensor is being used, this is the class to be called
class Ultrasonic {
  public:
    // Constructor. Takes the pin numbers for trig and echo
    Ultrasonic(int trig, int echo);
    // Function to setup the sensor. Call this from setup()
    void begin();

    // Pulses the trig pin, and listens for the echo. Returns the result in centimetres
    float ping();
  private:
    // Variables for storing the trig and echo pins
    int _trig, _echo;
};

class Infrared {
  public:
    // Constructor. Takes the pin that the sensor is tied to
    Infrared(int pin);
    // Fucntion to set up the sensor, call this from setup()
    void begin();
    // Reads the output pin to check for an object. Returns a boolean as to whether it is detecting an onstruction within the given distance.
    bool obstructed();
  private:
    int _pin;
};

#endif
