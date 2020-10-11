#include <Stepper.h>
const int stepsPerRevolution = 2048;
// Συνδεσμολογία: Pin4->IN1, Pin5->IN2, Pin6->IN3, Pin7->IN4
Stepper myStepper = Stepper(stepsPerRevolution, 4, 6, 5, 7); 
// προσοχή στη σειρά!!!

// χρησιμοποιούμε σταθερές για τους αριθμούς θυρών:
const int buttonPin1 = 2;     // η θύρα που συνδέεται το πρώτο κουμπί
const int buttonPin2 = 3;     // η θύρα που συνδέεται το πρώτο κουμπί

// χρησιμοποιούμε μεταβλητή που αποθηκεύει την κατάσταση του κουμπιού:
int buttonState1 = LOW;       // LOW: όχι πατημένο, HIGH: πατημένο
int buttonState2 = LOW;

// οι συνδέσεις των pins του arduino με τα pins της πλακέτας ελέγχου του μοτέρ (L298N H-Bridge)
const int enA = 10;
const int in1 = 9;
const int in2 = 8;

const int ledPin = 13;    // το ενσωματωμένο led
const int sensorPin = 11; // θύρα εισόδου

int sensorVal;

void setup()
{
  // Set the speed to 5 rpm:
  myStepper.setSpeed(5);
  
  // ορίζουμε ότι τα κουμπιά είναι σε θύρα εισόδου:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);

  // ορισμός όλων των pins που συνδέονται στον ελεγκτή μοτέρ ως εξόδου
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // pin εξόδου
  pinMode(ledPin, OUTPUT);

  // pin εισόδου με ενεργοποιημένη την εσωτερική pullup αντίσταση
  pinMode(sensorPin, INPUT_PULLUP);
}

void loop()
{
  // διαβάζουμε τον επαγωγικό αισθητήρα
  sensorVal = digitalRead(sensorPin);
  // ο επαγωγικός αισθητήρας δίνει LOW αν ανιχνεύσει μέταλλο, αλλιώς HIGH
  if (sensorVal == HIGH)
  {
    digitalWrite(ledPin, LOW);
  }
  else
  {
    digitalWrite(ledPin, HIGH);
  }
  
  // διαβάζουμε τη θύρα του κουμπιού και αποθηκεύουμε την κατάστασή του:
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);

  // αν δεν είναι πατημένο κανένα κουμπί:
  if (buttonState1 == LOW && buttonState2 == LOW)   
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enA, 0);    // ορισμός της ταχύτητας στο 0    
  }
  else if (buttonState1 == HIGH)  // αλλιώς αν είναι πατημένο το 1ο κουμπί:
  {
    // πήγαινε το φορείο προς τα δεξιά για 0.7 sec και μετά stop
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // ορισμός της ταχύτητας από το εύρος 0~255
    analogWrite(enA, 200);
    delay(700);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enA, 0);

    // κάνε μια πλήρη περιστροφή του stepper motor προς τη μια κατεύθυνση
    myStepper.step(stepsPerRevolution);
    delay(500);
  }
  else if (buttonState2 == HIGH)  // αλλιώς αν είναι πατημένο το 2ο κουμπί:
  {
    // πήγαινε το φορείο προς τα αριστερά για 0.7 sec και μετά stop
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // ορισμός της ταχύτητας από το εύρος 0~255
    analogWrite(enA, 200);
    delay(700);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enA, 0);

    // κάνε μια πλήρη περιστροφή του stepper motor προς την άλλη κατεύθυνση
    myStepper.step(-stepsPerRevolution);
    delay(500);
  }
  delay(50);
}
