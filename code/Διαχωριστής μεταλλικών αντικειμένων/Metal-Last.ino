#include <Stepper.h>
const int stepsPerRevolution = 2048;
Stepper myStepper = Stepper (stepsPerRevolution,4,6,5,7);

const int buttonPin = 2;    // το κουμπί έναρξης

const int enA = 10;     // οι συνδέσεις του motor driver
const int in1 = 9;
const int in2 = 8;

const int sensorPin = 11;   // ο αισθητήρας μετάλλων

const int switch1Pin = 12;  // οι 2 μικροδιακόπτες
const int switch2Pin = 13;


void setup()
{
  myStepper.setSpeed(5);
  
  pinMode(buttonPin, INPUT);
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  
  pinMode(sensorPin, INPUT_PULLUP);
  pinMode(switch1Pin, INPUT_PULLUP);
  pinMode(switch2Pin, INPUT_PULLUP);

    // επιστροφή του φορείου στην αρχική θέση
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enA, 200);

    // περίμενε μέχρι να φτάσει...
    while ( digitalRead(switch1Pin) == LOW )
      ;

    // stop
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enA, 0);  
}

void loop()
{
  // έλεγχος του κουμπιού έναρξης
  int buttonState = digitalRead(buttonPin);

  // αν πατήθηκε...
  if (buttonState == HIGH)
  {
    // διάβασμα του αισθητήρα μετάλλων και αποθήκευση της τιμής του
    int sensorVal = digitalRead(sensorPin);

    delay(1000);

    // πήγαινε το φορείο μέχρι την άλλη άκρη
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, 200);

    // περίμενε μέχρι να φτάσει...
    while ( digitalRead(switch2Pin) == LOW )
      ;

    // stop
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enA, 0);
    
    if (sensorVal == HIGH)  // αν βρέθηκε μέταλλο
    {
      // ανατροππή δεξιά και επαναφορά
      myStepper.step(300);
      delay(500);
      myStepper.step(-300);
    }
    else
    {
      // ανατροππή αριστερά και επαναφορά
      myStepper.step(-300);
      delay(500);
      myStepper.step(300);
    }

    // επιστροφή του φορείου στην αρχική θέση
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enA, 200);

    // περίμενε μέχρι να φτάσει...
    while ( digitalRead(switch1Pin) == LOW )
      ;

    // stop
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enA, 0);
  }

  delay(100);
}
