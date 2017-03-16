// ECE491 Practice DC motor speed control
// 20170311A

// Motor 1 pin assignment
int ENA = 9;  // PWM
int IN1 = 3;  // H-Bridge input 1
int IN2 = 2;  // H-Bridge input 2

// Global control variable initialization
int setPoint = 0;
int lastSetPoint = 0;
int tempSetPoint = 0;

void setup()
  {
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, setPoint);
    Serial.begin(9600);
    Serial.println("void setup");
  }

void loop()
  {
    if (Serial.available() > 0)
      {
        lastSetPoint = setPoint;
        tempSetPoint = Serial.parseInt();
        if (tempSetPoint >= 0 and tempSetPoint <= 511)  // CCW
          {
            setPoint = tempSetPoint;
            setSpeed();
          }
      }
  }
  
void setSpeed()
  {
    // Initialize local variables
    int state = 0;
    int set = 0;
    int lastSet = 0;
    
    // Define state
    if (lastSetPoint == setPoint)
      {
        state = 0;  // Do nothing
      }
    else if (lastSetPoint <= 255 and setPoint <=255)
      {
        if (lastSetPoint < setPoint)
          {
            state = 1;  // Ramp up CCW
          }
        else
          {
            state = 2; // Ramp down CCW
          }
      }
    else if (lastSetPoint <= 255 and setPoint > 255)
      {
        state = 3;  // Ramp CCW to zero, Ramp CW up
      }
    else if (lastSetPoint > 255 and setPoint > 255)
      {
        if (lastSetPoint < setPoint)
          {
            state = 4;  // Ramp up CW
          }
        else
          {
            state = 5;  // Ramp down CW
          }
      }
    else if (lastSetPoint > 255 and setPoint <= 255)
      {
        state = 6;  // Ramp CW to 255, Ramp CCW down
      }

    Serial.println("--------------");
    Serial.println("Motor State :");
    Serial.println(state);
    Serial.println("");

    // Execute State Control
    switch (state)
      {
        case 0:  //
          // Do nothing
        break;
          
        case 1:  //
          setDirection(1);
          rampUp(lastSetPoint, setPoint);
        break;

        case 2:  //
          setDirection(1);
          rampDown(lastSetPoint, setPoint);
        break;
          
        case 3:  //
          setDirection(1);
          rampDown(lastSetPoint, 0);
          setDirection(2);
          set = setPoint - 256;
          rampUp(0, set);
        break;
        
        case 4:  //
          setDirection(2);
          set = setPoint - 256;
          lastSet = lastSetPoint - 256;
          rampUp(lastSet, set);
        break;
          
        case 5:  //
          setDirection(2);
          set = setPoint - 256;
          lastSet = lastSetPoint - 256;
          rampDown(lastSet, set);
        break;
        
        case 6:  //
          setDirection(2);
          lastSet = lastSetPoint - 256;
          rampDown(lastSet, 0);
          setDirection(1);
          rampUp(0, setPoint);
        break;
      }
    
    Serial.println("Set Point :");
    Serial.println(setPoint);
    Serial.println("--------------");

  }   
    
void rampUp(int low, int high)
  {
    for (int i = low; i <= high; i++)
      {
        analogWrite(ENA, i);
        delay(5);
      }
  }
  
void rampDown(int high, int low)
  {
    for (int i = high; i >= low; i--)
      {
        analogWrite(ENA, i);
        delay(5);
      }
  }
  
void setDirection(int direction)
  {
    switch (direction)
      {
        case 0:  // STOP
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, LOW);     
        break;
        
        case 1:  // CCW
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
        break;
        
        case 2:  // CW
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
        break;
      }
  }
