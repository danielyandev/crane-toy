#include <IRremote.h>

// RGB
int RGBLED_RedPin = 9;
int RGBLED_GreenPin = 10;
int RGBLED_BluePin = 11;

// RGB changer
// 0: black/off, 1: red, 2: green, 3: blue
int LED_COLOR_BLACK = 0;
int LED_COLOR_RED = 1;
int LED_COLOR_GREEN = 2;
int LED_COLOR_BLUE = 3;

// off by default
int CURRENT_LED_COLOR = LED_COLOR_BLACK;


// numbers come from IR remote
int COMMAND_UP = 25245;
int COMMAND_DOWN = 765;
int COMMAND_LEFT = 57375;
int COMMAND_RIGHT = 36975;
int COMMAND_COLOR = 8925;

int delayTime = 200;
int lastPressed = 0;

int RECV_PIN = A2;
IRrecv irrecv(RECV_PIN);
decode_results results;


// This keeps track of the last code RX'd
uint16_t lastCode = 0;

// left/right pins
int MOTOR_A[2] = {4, 5};

// up/down pins
int MOTOR_B[2] = {6, 7};

void setup() {
  for (int i = 0; i < 2; i++) {
    pinMode(MOTOR_A[i], OUTPUT);
    pinMode(MOTOR_B[i], OUTPUT);

    // stpped by default
    digitalWrite(MOTOR_A[i], LOW);
    digitalWrite(MOTOR_B[i], LOW);
  }

  // Serial.begin(9600);

  // Start the receiver
  irrecv.enableIRIn();

  pinMode(RGBLED_RedPin, OUTPUT);
  pinMode(RGBLED_GreenPin, OUTPUT);
  pinMode(RGBLED_BluePin, OUTPUT);
}

void loop() {

  if (irrecv.decode(&results)) {
    /* read the RX'd IR into a 16-bit variable: */
    uint16_t resultCode = (results.value & 0xFFFF);
    if (resultCode == 0xFFFF) {
      resultCode = lastCode;
    }
    else {
      lastCode = resultCode;
    }
    
    // Serial.println(resultCode);

    if (resultCode == COMMAND_LEFT ) {
      moveLeft();
    } else if (resultCode == COMMAND_RIGHT) {
      moveRight();
    }
    
    if (resultCode == COMMAND_UP) {
      moveUp();
    } else if (resultCode == COMMAND_DOWN) {
      moveDown();
    }

    if (resultCode == COMMAND_COLOR) {
      changeCurrentColor();
    }

    // Receive the next value
    irrecv.resume();
  } else {

    if (millis() - lastPressed > delayTime) {
      lastPressed = millis();
      // Serial.println("Iddle");
      stop(MOTOR_A);
      stop(MOTOR_B);
    }
  }

  // for serial tests
  if (Serial.available()) {
    char command = Serial.read();

    // control with WASD
    if (command == 'a') {
      moveLeft();
    } else if (command == 'd') {
      moveRight();
    }
    
    if (command == 'w') {
      moveUp();
    } else if (command == 's') {
      moveDown();
    }
  }

  checkColor();

  delay(delayTime);
}

void moveLeft() {
  // Serial.println("Moving left");
  digitalWrite(MOTOR_A[0], LOW);
  digitalWrite(MOTOR_A[1], HIGH);
}

void moveRight() {
  // Serial.println("Moving right");
  digitalWrite(MOTOR_A[0], HIGH);
  digitalWrite(MOTOR_A[1], LOW);
}

void moveUp() {
  // Serial.println("Moving up");
  digitalWrite(MOTOR_B[0], LOW);
  digitalWrite(MOTOR_B[1], HIGH);
}

void moveDown() {
  // Serial.println("Moving down");
  digitalWrite(MOTOR_B[0], HIGH);
  digitalWrite(MOTOR_B[1], LOW);
}

void stop(int motor[2]) {
  digitalWrite(motor[0], LOW);
  digitalWrite(motor[1], LOW);
}

// invert colors for common anode
void setColor(int red, int green, int blue) {
  analogWrite(RGBLED_RedPin, 255 - red);
  analogWrite(RGBLED_GreenPin, 255 - green);
  analogWrite(RGBLED_BluePin, 255 - blue);
}

void checkColor() {
  if (CURRENT_LED_COLOR == LED_COLOR_RED) {
    setColor(255, 0, 0);
  }
  else if (CURRENT_LED_COLOR == LED_COLOR_GREEN) {
    setColor(0, 255, 0);
  }
  // blue doesn't work properly, set another
  else if (CURRENT_LED_COLOR == LED_COLOR_BLUE) {
    setColor(100, 255, 0);
  }
  else if (CURRENT_LED_COLOR == LED_COLOR_BLACK) {
    setColor(0, 0, 0);
  }
}

void changeCurrentColor() {
  if (CURRENT_LED_COLOR >= LED_COLOR_BLUE) {
    CURRENT_LED_COLOR = 0;
  } else {
    CURRENT_LED_COLOR++;    
  }

  // Serial.print("Current color: ");
  // Serial.println(CURRENT_LED_COLOR);
}