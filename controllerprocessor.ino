#include <Bluepad32.h>

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// Driver pins
const int PWMleft  = 25;   // ENA
const int IN1left  = 26;
const int IN2left  = 27;

const int PWMright = 33;   // ENB
const int IN1right = 16;   // actually IN3 on L298N side
const int IN2right = 17;   // actually IN4 on L298N side

// PWM settings
const int channelL = 0;
const int channelR = 1;
const int pwmFreq = 5000;
const int pwmResolution = 8;

// Function prototypes
void stopMotors(bool L, bool R);
void setupPins();
void setMotorSpeedLeft(int axis);
void setMotorSpeedRight(int axis);
void processGamepad(ControllerPtr ctl);

void onConnectedController(ControllerPtr ctl) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
            ControllerProperties properties = ctl->getProperties();
            Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n",
                          ctl->getModelName().c_str(),
                          properties.vendor_id,
                          properties.product_id);
            myControllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }
    if (!foundEmptySlot) {
        Serial.println("CALLBACK: Controller connected, but could not find empty slot");
    }
}

void onDisconnectedController(ControllerPtr ctl) {
    bool foundController = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
            myControllers[i] = nullptr;
            foundController = true;
            break;
        }
    }

    stopMotors(true, true);

    if (!foundController) {
        Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
}

void setupPins() {
    pinMode(IN1left, OUTPUT);
    pinMode(IN2left, OUTPUT);
    pinMode(IN1right, OUTPUT);
    pinMode(IN2right, OUTPUT);

    ledcSetup(channelL, pwmFreq, pwmResolution);
    ledcAttachPin(PWMleft, channelL);

    ledcSetup(channelR, pwmFreq, pwmResolution);
    ledcAttachPin(PWMright, channelR);

    // Stop motors initially
    stopMotors(true, true);
}

void stopMotors(bool L, bool R) {
    if (L) {
        digitalWrite(IN1left, LOW);
        digitalWrite(IN2left, LOW);
        ledcWrite(channelL, 0);
    }

    if (R) {
        digitalWrite(IN1right, LOW);
        digitalWrite(IN2right, LOW);
        ledcWrite(channelR, 0);
    }
}

void setMotorSpeedLeft(int axis) {
    int dz = 100;

    if (abs(axis) < dz) {
        stopMotors(true, false);
        return;
    }

    int speed = map(abs(axis), dz, 512, 0, 255);
    speed = constrain(speed, 0, 255);

    if (axis < 0) {
        // Forward
        digitalWrite(IN1left, HIGH);
        digitalWrite(IN2left, LOW);
    } else {
        // Reverse
        digitalWrite(IN1left, LOW);
        digitalWrite(IN2left, HIGH);
    }

    ledcWrite(channelL, speed);
}

void setMotorSpeedRight(int axis) {
    int dz = 100;

    if (abs(axis) < dz) {
        stopMotors(false, true);
        return;
    }

    int speed = map(abs(axis), dz, 512, 0, 255);
    speed = constrain(speed, 0, 255);

    if (axis < 0) {
        // Forward
        digitalWrite(IN1right, HIGH);
        digitalWrite(IN2right, LOW);
    } else {
        // Reverse
        digitalWrite(IN1right, LOW);
        digitalWrite(IN2right, HIGH);
    }

    ledcWrite(channelR, speed);
}

void processGamepad(ControllerPtr ctl) {
    int y = ctl->axisY();
    int ry = ctl->axisRY();

    Serial.print("Y: ");
    Serial.print(y);
    Serial.print("  RY: ");
    Serial.println(ry);

    setMotorSpeedLeft(y);
    setMotorSpeedRight(ry);
}

void setup() {
    Serial.begin(115200);

    setupPins();

    BP32.setup(&onConnectedController, &onDisconnectedController);
    //BP32.forgetBluetoothKeys();

    Serial.println("Ready. Pair controller.");
}

void loop() {
    BP32.update();

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        ControllerPtr ctl = myControllers[i];
        if (ctl && ctl->isConnected() && ctl->hasData()) {
            processGamepad(ctl);
        }
    }

    delay(10);
}
