
//motor A pins
int enA = 14;
int in1 = 27;
int in2 = 26;

//motor B pins
int enB = 32;
int in3 = 25;
int in4 = 33;

// Motor C
int enC = 23;
int in5 = 22;
int in6 = 21;

// Motor D
int enD = 5;
int in7 = 19;
int in8 = 18;

void setup() {
  // setup code, runs once
  
  //Set motor control pins to output only.
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT); 

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT); 

  pinMode(enC, OUTPUT);
  pinMode(in5, OUTPUT);
  pinMode(in6, OUTPUT); 

  pinMode(enD, OUTPUT);
  pinMode(in7, OUTPUT);
  pinMode(in8, OUTPUT); 

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

  //turn off motors but enable them
  
  // Motor A
  digitalWrite(enA, HIGH);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  // Motor B
  digitalWrite(enB, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  // motor C
  digitalWrite(enC, HIGH);
  digitalWrite(in5, LOW);
  digitalWrite(in6, LOW);

  // Motor D
  digitalWrite(enD, HIGH);
  digitalWrite(in7, LOW);
  digitalWrite(in8, LOW);
}

void loop() {
  // Runs repeatedly
}
