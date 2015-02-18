//#include <TimerOne.h>

boolean firstRun = true; // Used for one-run-only stuffs;
//First pin being used for floppies, and the last pin. Used for looping over all pins.
const byte STEP_CONTROL_PIN = 2;
const byte DIRECTION_PIN = 3;

#define RESOLUTION 80 //Microsecond resolution for notes

const byte MAX_POSITION = 158;

byte currentPosition = 0;

unsigned int currentDirection = LOW;

unsigned int currentState = LOW;

unsigned int currentTick = 0;

const unsigned int currentPeriod = 40;


void setup() {
  pinMode(13, OUTPUT);// Pin 13 has an LED connected on most Arduino boards
  pinMode(STEP_CONTROL_PIN, OUTPUT); // Step control 1
  pinMode(DIRECTION_PIN, OUTPUT); // Direction 1

//  Timer1.initialize(RESOLUTION); // Set up a timer at the defined resolution
//  Timer1.attachInterrupt(tick); // Attach the tick function

  Serial.begin(9600);
}


void pressWithFinger () {

  goForward();
  blinkLED();  
    
  goBackward();
  blinkLED();  
}


void goForward () {
  digitalWrite(DIRECTION_PIN, LOW);
  doGo(1);  
}

void goBackward () {
  digitalWrite(DIRECTION_PIN, HIGH);
  doGo(-1);    
}

void doGo (const byte delta) {
  
  for (; currentPosition >= 0 && currentPosition <= MAX_POSITION; currentPosition += delta) {
    digitalWrite(STEP_CONTROL_PIN, currentState);
    Serial.println(currentPosition);
    currentState = ~currentState;
  }
  currentPosition -= delta;
//  while (true) {
//    if ((delta < 0) && (currentPosition <= 0) {
////      currentPosition = 0;
//      return;
//    }
//    
//    if (delta > 0 && currentPosition >= MAX_POSITION) {
////      currentPosition = MAX_POSITION;
//      return;
//    }
//   
//    digitalWrite(STEP_CONTROL_PIN, currentState);
//    currentState = ~currentState;
//
//    currentPosition += delta;    
//    
//    delay(1);
//   digitalWrite(STEP_CONTROL_PIN, HIGH);
//   delay(5);    
//   digitalWrite(STEP_CONTROL_PIN, LOW);    
//  }
}


void loop() {  
  //The first loop, reset all the drives, and wait 2 seconds...
  if (firstRun)
  {
    blinkLED();
    delay(1000);
    blinkLED();
    
    reset();

    delay(2000);

    blinkLED();
    
    firstRun = false;    
  }
  
  pressWithFinger();
}

void tick () {
  if (firstRun) {
    return;
  }
  if (currentPeriod > 0) {
    currentTick++;

    if (currentTick >= currentPeriod) {
      togglePins();
//      Serial.print("position: ");
//      Serial.print(currentPosition);
//      Serial.print("\tdirection: ");
//      Serial.print(currentDirection);
//      Serial.print("\tstate:");
//      Serial.println(currentState);
      currentTick = 0;
    }
  }
}


void togglePins () {
  //Switch directions if end has been reached

  if (currentPosition >= MAX_POSITION) {
    currentDirection = HIGH;
    digitalWrite(DIRECTION_PIN, HIGH);

  }
  
  if (currentPosition <= 0) {
    currentDirection = LOW;
    digitalWrite(DIRECTION_PIN, LOW);

  }

  //Update currentPosition
  if (currentDirection == HIGH) {
    currentPosition--;
  }
  else {
    currentPosition++;
  }

  //Pulse the control pin
  digitalWrite(STEP_CONTROL_PIN, currentState);
  currentState = ~currentState;
}


// utility functions
//For a given controller pin, runs the read-head all the way back to 0
void reset()
{
  digitalWrite(DIRECTION_PIN, HIGH); // Go in reverse

  for (byte s = 0; s < MAX_POSITION; s += 2 ) { //Half max because we're stepping directly (no toggle)
    digitalWrite(STEP_CONTROL_PIN, HIGH);
    delay(5);    
    digitalWrite(STEP_CONTROL_PIN, LOW);

  }

  currentPosition = 0; // We're reset.
  digitalWrite(DIRECTION_PIN, LOW);
  currentDirection = LOW;
  currentPosition = 0;
  //currentPosition = 0; // Ready to go forward.
}

void blinkLED() {
  digitalWrite(13, HIGH); // set the LED on
  delay(250); // wait for a second
  digitalWrite(13, LOW);
}


