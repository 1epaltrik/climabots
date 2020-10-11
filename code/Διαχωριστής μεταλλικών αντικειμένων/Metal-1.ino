// Αρχείο metal-1.ino

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

void setup()
{
  // ορίζουμε ότι τα κουμπιά είναι σε θύρα εισόδου:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);

  // ορισμός όλων των pins που συνδέονται στον ελεγκτή μοτέρ ως εξόδου
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
}

void loop()
{
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
    // πήγαινε προς τη μία κατεύθυνση
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // ορισμός της ταχύτητας από το εύρος 0~255
    analogWrite(enA, 240);
  }
  else if (buttonState2 == HIGH)  // αλλιώς αν είναι πατημένο το 2ο κουμπί:
  {
    // πήγαινε προς την άλλη κατεύθυνση
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // ορισμός της ταχύτητας από το εύρος 0~255
    analogWrite(enA, 220);
  }
}
