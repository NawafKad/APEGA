#include <Bluepad32.h>

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// Driver pins
const int PWMleft  = 14;   // ENA
const int IN1left  = 27;
const int IN2left  = 26;

const int PWMright = 32;   // ENB
const int IN1right = 33;   // actually IN3 on L298N side
const int IN2right = 25;   // actually IN4 on L298N side

// Brush pins
const int enb = 18;
const int in4 = 19;
const int in3 = 23;

// PWM settings
const int channelL = 0;
const int channelR = 1;
const int channelBrush = 2;
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

    stopMotors(true, true, true);

    if (!foundController) {
        Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
}

void setupPins() {
    pinMode(IN1left, OUTPUT);
    pinMode(IN2left, OUTPUT);
    pinMode(IN1right, OUTPUT);
    pinMode(IN2right, OUTPUT);
    pinMode(in4, OUTPUT);
    pinMode(in3, OUTPUT);

    ledcSetup(channelL, pwmFreq, pwmResolution);
    ledcAttachPin(PWMleft, channelL);

    ledcSetup(channelR, pwmFreq, pwmResolution);
    ledcAttachPin(PWMright, channelR);

    ledcSetup(channelBrush, pwmFreq, pwmResolution);
    ledcAttachPin(enb, channelBrush);

    // Stop motors initially
    stopMotors(true, true, true);
}

void stopMotors(bool L, bool R, bool brush) {
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

    if (brush) {
        digitalWrite(in4, LOW);
        digitalWrite(in3, LOW);
        ledcWrite(channelBrush, 0);
    }
}

void setMotorSpeedLeft(int axis) {
    int dz = 100;

    if (abs(axis) < dz) {
        stopMotors(true, false, false);
        return;
    }

    int speed = map(abs(axis), dz, 512, 0, 255);
    speed = constrain(speed, 0, 255);

    if (axis > 0) {
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
        stopMotors(false, true, false);
        return;
    }

    int speed = map(abs(axis), dz, 512, 0, 255);
    speed = constrain(speed, 0, 255);

    if (axis > 0) {
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

void setBrushSpeed(int axis) {
    int dz = 10;
    
    if (abs(axis) < dz) {
        stopMotors(false, false, false);
        return;
    }

    int speed = map(abs(axis), dz, 1023, 0, 255);
    speed = constrain(speed, 0, 255);

    if (axis < 0) {
        // Forward
        digitalWrite(in4, HIGH);
        digitalWrite(in3, LOW);
    } else {
        // Reverse
        digitalWrite(in4, LOW);
        digitalWrite(in3, HIGH);
    }

    ledcWrite(channelBrush, speed);
}

void processGamepad(ControllerPtr ctl) {
    int y = ctl->axisY();
    int ry = ctl->axisRY();
    int throttle = ctl->throttle();
    int brake = ctl->brake();
    int trigger = throttle - brake;

    Serial.print("Y: ");
    Serial.print(y);
    Serial.print("  RY: ");
    Serial.println(ry);
    Serial.print("  Trigger: ");
    Serial.println(trigger);

    setMotorSpeedLeft(y);
    setMotorSpeedRight(ry);
    setBrushSpeed(trigger);
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
