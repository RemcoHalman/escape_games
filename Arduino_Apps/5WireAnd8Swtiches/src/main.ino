/**
* Connect the Wires Puzzle
*
* This puzzle requires the player to use a number of wires to
* join correct pairs of terminals together.
* Once all terminals have been correctly connected, the puzzle is solved.
*
* Demonstrates:
* - Use of inbuilt pull-up resistors
* - pinMode, digitalWrite, digitalRead
*/

#include "Arduino.h"

// DEFINES
// Flag to toggle debugging output
#define DEBUG

// CONSTANTS

// The total number of possible sockets from which connections can be made
const byte numSockets = 10;

// Wire pin setup
const int socket1 = 12;
const int socket2 = 11;
const int socket3 = 10;
const int socket4 = 9;
const int socket5 = A2;
const int red = 8;    // Cable color
const int blue = 7;   // Cable color
const int green = 4;  // Cable color
const int yellow = 2; // Cable color
const int black = A3; // Cable color
// Switches setup
const int switches_output = A4;
const int switches_state = A5;

int toggleSwitches = 0;

const int state_puzzel = A0;

// The pins of the LEDs corresponding to each of those connectors
// Show state, Red not Solved, Green Solved
const int ledRed = 3;
const int ledBlue = 5;
const int ledGreen = 6;

// The array of pins to which each socket is connected
const byte signalPins[numSockets] = {
    socket1, socket2, socket3, socket4, socket5,
    //    0,       1,       2,       3,       4,
    red, blue, green, yellow, black
    //5,    6,     7,      8,     9
};

// The total number of connections required to be made
const byte numConnections = 5;

// The connections that need to be made between pins, referenced by their index in the signalPins array

// secquence 1
const byte connections[numConnections][2] = {{0, 5}, {1, 7}, {2, 6}, {3, 8}, {4, 9}};

// GLOBAL VARIABLES
// Track state of which output pins are correctly connected
bool lastState[numConnections] = {false, false, false, false, false};
// Track state of overall puzzle
enum PuzzleState
{
  Initialising,
  Running,
  Solved
};
PuzzleState puzzleState = Initialising;

void setup()
{

  // Initialise the pins. When wires are disconnected, the input pins would be
  // floating and digitalRead would be unpredictable, so we'll initialise them
  // as INPUT_PULLUP and use the wires to connect them to ground.
  for (int i = 0; i < numSockets; i++)
  {

    // The default state of all pins will be INPUT_PULLUP,
    // though they will be reassigned throughout the duration of the program in order to test connection between pins
    pinMode(signalPins[i], INPUT_PULLUP);

    // We also initialise all the LED pins as ouput
    // Here to set RGB LED to show correct state
    // pinmode
    pinMode(ledRed, OUTPUT);
    pinMode(ledBlue, OUTPUT);
    pinMode(ledGreen, OUTPUT);
    pinMode(state_puzzel, OUTPUT);

    pinMode(switches_output, OUTPUT);
    pinMode(switches_state, INPUT_PULLUP);

    digitalWrite(state_puzzel, LOW);
    LedRed();
  }

// Serial connection used only for monitoring / debugging
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println(F("Serial communication started"));
  Serial.println(F("Correct pins are:"));
  //
  Serial.print("pin ");
  Serial.print(socket1);
  Serial.print(" and pin ");
  Serial.println(red);
  //
  Serial.print("pin ");
  Serial.print(socket2);
  Serial.print(" and pin ");
  Serial.println(green);
  //
  Serial.print("pin ");
  Serial.print(socket3);
  Serial.print(" and pin ");
  Serial.println(blue);
  //
  Serial.print("pin ");
  Serial.print(socket4);
  Serial.print(" and pin ");
  Serial.println(yellow);
#endif

  puzzleState = Running;
}

void loop()
{

  // Assume that all wires are correct
  bool AllWiresCorrect = true;

  // Assume that the puzzle state has not changed since last reading
  bool stateChanged = false;

  // Check each connection in turn
  for (int i = 0; i < numConnections; i++)
  {

    // Get the pin numbers that should be connected by this wire
    byte pin1 = signalPins[connections[i][0]];
    byte pin2 = signalPins[connections[i][1]];

    // Test whether the appropriate pins are correctly connected
    bool currentState = isConnected(pin1, pin2);

    // Has the connection state changed since last reading?
    if (currentState != lastState[i])
    {

      // Set the flag to show the state of the puzzle has changed
      stateChanged = true;

      // Update the stored connection state
      lastState[i] = currentState;
    }

    // If any connection is incorrect, puzzle is not solved
    if (currentState == false)
    {
      AllWiresCorrect = false;
    }
  }

  // If a connection has been made/broken since last time we checked
  if (stateChanged)
  {
#ifdef DEBUG
    // Dump to serial the current state of all connections
    for (uint8_t i = 0; i < numConnections; i++)
    {
      Serial.print(F("Pin#"));
      Serial.print(signalPins[connections[i][0]]);
      Serial.print(F(" - Pin#"));
      Serial.print(signalPins[connections[i][1]]);
      Serial.print(F(" : "));
      Serial.println(lastState[i] ? "CONNECTED" : "NOT CONNECTED");
    }
    Serial.println(F("---"));
#endif
  }

  if (switches_state == HIGH)
  {
    toggleSwitches = 1;
  }
  else if (switches_state == LOW)
  {
    toggleSwitches = 0;
  }

  // If the state of the puzzle has changed
  if (AllWiresCorrect && puzzleState == Running && toggleSwitches == 0)
  {
    onSolve();
  }

  // If the state of the puzzle has changed
  else if (!AllWiresCorrect && puzzleState == Solved && toggleSwitches == 1)
  {
    onUnsolve();
  }
};

/**
 * Called when the puzzle is solved
 */
void onSolve()
{
#ifdef DEBUG
  Serial.println(F("The puzzle has been solved!"));
#endif
  digitalWrite(state_puzzel, HIGH);
  LedGreen();
  puzzleState = Solved;
};

/**
 * Called when the (previously solved) puzzle becomes unsolved
 */
void onUnsolve()
{
#ifdef DEBUG
  Serial.println(F("The puzzle is no longer solved!"));
#endif
  digitalWrite(state_puzzel, LOW);
  LedRed();
  puzzleState = Running;
}

// 255 is off, 0 is on
void LedGreen()
{
  analogWrite(ledRed, 255);
  analogWrite(ledBlue, 255);
  analogWrite(ledGreen, 120);
}

void LedRed()
{
  analogWrite(ledRed, 120);
  analogWrite(ledBlue, 255);
  analogWrite(ledGreen, 255);
}

/**
 * Tests whether an output pin is connected to a given INPUT_PULLUP pin
 */
bool isConnected(byte OutputPin, byte InputPin)
{

  // To test whether the pins are connected, set the first as output and the second as input
  pinMode(OutputPin, OUTPUT);
  pinMode(InputPin, INPUT_PULLUP);

  // Set the output pin LOW
  digitalWrite(OutputPin, LOW);

  // If connected, the LOW signal should be detected on the input pin
  // (Remember, we're using LOW not HIGH, because an INPUT_PULLUP will read HIGH by default)
  bool isConnected = !digitalRead(InputPin);

  // Set the output pin back to its default state
  pinMode(OutputPin, INPUT_PULLUP);

  return isConnected;
}
