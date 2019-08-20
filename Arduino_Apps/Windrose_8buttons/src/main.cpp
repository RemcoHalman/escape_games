#include <Arduino.h>

#define DEBUG

// CONSTANTS
// All Inputs sorted out for easyer debugging
const int north = 2;     //0
const int northEast = 3; //1
const int east = 4;      //2
const int southEast = 5; //3
const int south = 6;     //4
const int southWest = 7; //5
const int west = 8;      //6
const int northWest = 9; //7

// Possible "inputs" that the player can press
const int numInputs = 8;
// What pins connected (make the other wire a common ground)
int inputPins[numInputs] = {
    north,
    northEast,
    east,
    southEast,
    south,
    southWest,
    west,
    northWest,
};
// Define the number of steps in the sequence that the player must follow
const byte numSteps = 9;
// <Code nog invullen>
int steps[numSteps] = {5, 2, 4, 1, 7, 6, 2, 3, 0};

// This pin will be driven LOW to give a complete puls when puzzle is solved
const byte gamePin = A0;
const byte ledPin = 11;

// GLOBALS
// Assume the default state of each switch is HIGH.
bool lastInputState[] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

// What step of the sequence is the player currently on?
int currentStep = 0;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup()
{
  for (int i = 0; i < numInputs; i++)
  {
    pinMode(inputPins[i], INPUT_PULLUP);
  }

  // Set the lock pin as output and secure the lock
  pinMode(gamePin, OUTPUT);
  digitalWrite(gamePin, HIGH);
  // Set Lights on
  pinMode(ledPin, OUTPUT);
  // digitalWrite(ledPin, HIGH);

#ifdef DEBUG
  Serial.begin(9600);
  Serial.println(F("Serial communication started"));
#endif
}

void ledPulse(){
    for (int brightness = 0; brightness < 255; brightness++) {
      analogWrite(ledPin, brightness);
      delay(5);
    }
    // fade the LED on ledPin from brithstest to off:
    for (int brightness = 255; brightness >= 0; brightness--) {
      analogWrite(ledPin, brightness);
      delay(5);
    }
}

void onSolve()
{

#ifdef DEBUG
  Serial.println(F("Puzzle Solved!"));
#endif

  // Release the lock
  digitalWrite(gamePin, LOW);
  digitalWrite(ledPin, LOW);
  ledPulse();
}

void wrongSequence(){
  digitalWrite(ledPin, LOW);
  delay(150);
  digitalWrite(ledPin, HIGH);
  delay(150);
  digitalWrite(ledPin, LOW);
  delay(150);
  digitalWrite(ledPin, HIGH);
}

void loop()
{
  // Check that we've waited at least "debounceDelay" since last input
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    // Loop through all the inputs
    for (int i = 0; i < numInputs; i++)
    {
      int currentInputState = digitalRead(inputPins[i]);

      // If the input has changed, reset the debounce timer
      if (currentInputState != lastInputState[i])
      {
        lastDebounceTime = millis();
      }

      // If the input is currently being pressed (and wasn't before)
      if (currentInputState == LOW && lastInputState[i] == HIGH)
      {
        // Was this the correct input for this step of the sequence?
        if (steps[currentStep] == i)
        {
          currentStep += 1;

#ifdef DEBUG
          Serial.print(F("Correct input! Onto step #"));
          Serial.println(currentStep);
#endif
        }
        // Incorrect input!
        else
        {
          currentStep = 0;
          Serial.println(F("Incorrect input! Back to the beginning!"));
          // wrongSequence();
        }
      }

      // Update the stored value for this input
      lastInputState[i] = currentInputState;
    }
  }

  // Check whether the puzzle has been solved
  if (currentStep == numSteps)
  {
    onSolve();
  }
  else
  {
    analogWrite(ledPin, 255);
  }
}