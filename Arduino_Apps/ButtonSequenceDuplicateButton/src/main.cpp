#include <Arduino.h>

#define DEBUG

// CONSTANTS
// Possible "inputs" that the player can press
const int numInputs = 13;
// What pins connected (make the other wire a common ground)
int inputPins[numInputs] = {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, A1, A2};
// Define the number of steps in the sequence that the player must follow
const int stepsRequired = 8;

int resultE = (10) || (12);
int resultR = (0) || (9);
int resultR_1 = 0;
int resultR_2 = 9;
int resultE_1 = 10;
int resultE_2 = 12;
int steps[stepsRequired] = {8, resultE, resultR, 11, resultR, 5, resultE, 3};

// This pin will be driven LOW to give a complete puls when puzzle is solved
const byte gamePin = A0;
const byte ledPin = A4;

// GLOBALS
// Assume the default state of each switch is HIGH.
bool lastInputState[] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

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
  digitalWrite(ledPin, HIGH);

#ifdef DEBUG
  Serial.begin(9600);
  Serial.println(F("Serial communication started"));
#endif
}

void onSolve()
{

#ifdef DEBUG
  Serial.println(F("Puzzle Solved!"));
#endif

  // Release the lock
  digitalWrite(gamePin, LOW);
  digitalWrite(ledPin, LOW);
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
          currentStep++;

#ifdef DEBUG
          Serial.print(F("Correct input! Onto step #"));
          Serial.println(currentStep);
#endif
        }
        // Added or selection
        else if (steps[currentStep] == (0 || 9))
        {
          currentStep++;
        }

        else if (steps[currentStep] == (10 || 12))
        {
          currentStep++;
        }

        // Incorrect input!
        else
        {
          currentStep = 0;
          Serial.println(F("Incorrect input! Back to the beginning!"));
          digitalWrite(ledPin, LOW);
          delay(150);
          digitalWrite(ledPin, HIGH);
          delay(150);
          digitalWrite(ledPin, LOW);
          delay(150);
          digitalWrite(ledPin, HIGH);
        }
      }

      // Update the stored value for this input
      lastInputState[i] = currentInputState;
    }
  }

  // Check whether the puzzle has been solved
  if (currentStep == stepsRequired)
  {
    onSolve();
  }
  else
  {
    digitalWrite(ledPin, HIGH);
  }
}