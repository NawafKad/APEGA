

//motor A pins
int enA = 14;
int in1 = 27;
int in2 = 26;

void setup() {
  // setup code, runs once
  
  //Set motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT); 

  /*

    MOTORS
  
    The motors are dc, so in order to set the direction to reverse you flip the current.
    Forward would be

    in1 > HIGH
    in2 > LOW

    Reverse would be the flip

    in1 > LOW
    in2 > HIGH

    Off would be

    in1 > LOW
    in2 > LOW

    Dont attempt, never attempted but im not sure what would happen.

    in1 > HIGH
    in2 > HIGH

    Another note is that you MUST set the pin en pin to High. The en port may be
    something to do with resistance but I am not sure, what matters is that low
    is off and high is on.
  
  */

  //turn off motors 
  
  // Motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

void loop() {
  // Runs repeatedly
