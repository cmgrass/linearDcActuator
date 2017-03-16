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
        if (tempSetPoint >= 0 and tempSetPoint <= 255)
          {
            setPoint = tempSetPoint;
            setSpeed();
          }
      }
  }
  
void setSpeed()
  {
    
    // Assign H-bridge to drive
    digitalWrite(IN1, LOW);
    digitalWrite(IN1, HIGH);
    
    // Handle ramping
    if (lastSetPoint < setPoint)
      {
        // Ramp up
        for (int i = lastSetPoint; i <= setPoint; i++)
          {
            analogWrite(ENA, i);
            //Serial.println(i);
            delay(10);
          }
      }
    else
      {
        // Ramp down
        for (int i = lastSetPoint; i >= setPoint; i--)
          {
            analogWrite(ENA, i);
            //Serial.println(i);
            delay(10);
          }
      }
    Serial.println(setPoint);
  }
