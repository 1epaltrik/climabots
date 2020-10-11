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

const int switch1Pin = 12;  // οι 2 μικροδιακόπτες
const int switch2Pin = 13;

void setup()
{
  // ορίζουμε ότι τα κουμπιά είναι σε θύρα εισόδου:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);

  // ορισμός όλων των pins που συνδέονται στον ελεγκτή μοτέρ ως εξόδου
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // pin εισόδου με ενεργοποιημένη την εσωτερική pullup αντίσταση
  pinMode(switch1Pin, INPUT_PULLUP);
  pinMode(switch2Pin, INPUT_PULLUP);
}

void loop()
{
  // διαβάζουμε τη θύρα του κουμπιού και αποθηκεύουμε την κατάστασή του:
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);

  if (buttonState1 == HIGH)  // αν είναι πατημένο το 1ο κουμπί:
  {
    // πήγαινε το φορείο μέχρι τη μία άκρη...
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    analogWrite(enA,200);   // ορισμός της ταχύτητας από 0~255

    // περίμενε μέχρι να φτάσει (κενός βρόχος)...
    while ( digitalRead(switch2Pin) == LOW )
      ;

    // και μετά stop
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
    analogWrite(enA,0);
  }
  else if (buttonState2 == HIGH)  // αλλιώς αν είναι πατημένο το 2ο κουμπί:
  {
    // πήγαινε το φορείο μέχρι την άλλη άκρη...
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    analogWrite(enA,200);   // ορισμός της ταχύτητας από 0~255

    // περίμενε μέχρι να φτάσει (κενός βρόχος)...
    while ( digitalRead(switch1Pin) == LOW )
      ;

    // και μετά stop
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
    analogWrite(enA,0);
  }
  delay(10);
}
