#include <Servo.h> 
#include <Stepper.h>
int stepsPerRevolution = 64;  // για το stepper motor 28BYJ-48
int maxSteps = 3500;   // ο αριθμός βημάτων που πρέπει να κάνει ο κινητήρας για να ανοίξει πλήρως η τέντα (εξαρτάται από το μήκος της τέντας)
int pos = 0;      // η τρέχουσα θέση της τέντας: 0 κλειστή, 1 μισάνοιχτη, 2 ανοιχτή
Stepper myStepper(stepsPerRevolution, 3, 4, 5, 6);
int steps = 0;

Servo servo; // Δημιουργία ενός αντικειμένου servo για τον έλεγχο του κινητήρα servo 
int eLDRPin = A0; // Εκχώρηση pins στις φωτοαντιστάσεις 
int wLDRPin = A1; 
int eastLDR = 0; //Δημιουργία μεταβλητών για αποθήκευση τιμών φωτοαντιστάσεων
int westLDR = 0;
int difference = 0; // Δημιουργία μεταβλητής για σύγκριση των τιμών των φωτοαντιστάσεων 
int error = 10; // Μεταβλητή για εάν υπάρχει αξιοσημείωτη διαφορά μεταξύ των δύο φωτοαντιστάσεων
int servoSet = 130; //Variable for position of servo - will be different for each device 
int trigPin = 12;    // Trigger
int echoPin = 11;    // Echo
int led = 10;   //Φωτισμός
long duration, cm, inches;

void setup() { 
  servo.attach(9); // συνδέει το αντικείμενο servo στον ακροδέκτη της φωτοαντίστασης
  //Αρχικοποηση σειριακης 
  Serial.begin (9600);
  //ΟΡΙΣΜΟΣ ΣΗΜΕΙΩΝ ΕΙΣΟΔΟΥ-ΕΞΟΔΟΥ
  pinMode(trigPin, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // ορισμός της ταχύτητας του stepper motor της τέντας σε rpm
  myStepper.setSpeed(450);
} 

void loop() { 
  eastLDR = analogRead(eLDRPin); //Διάβασμα τιμών των φωτοαντιστάσεων
  westLDR = analogRead(wLDRPin);
  if (eastLDR < 400 && westLDR < 400) { // Ελέγχει αν υπάρχει χαμηλός φωτισμός και στις 2 φωτοαντιστάσεις
    while (servoSet <=140 && servoSet>=15) 
    { // σε αυτή την περίπτωση το πάνελ κινείται ανατολικά προς το ηλιοβασίλεμα
      servoSet ++; 
      servo.write(servoSet);
      delay(100); 
    } 
  } 
  difference = eastLDR - westLDR ; //Έλεγχος της διαφοράς 
  if (difference > 10) { // Στέλνει το πάνελ προς τη φωτοαντίσταση με την υψηλότερη ένδειξη
    if (servoSet <= 140) {  
      servoSet ++; 
      servo.write(servoSet); 
    }
  } 
  else if (difference < -10) { 
    if (servoSet >= 15) { 
      servoSet --;
      servo.write(servoSet);
    }
  } Serial.print(eastLDR); // Η σειριακή οθόνη μπορεί να είναι χρήσιμη για εντοπισμό σφαλμάτων 
  Serial.print(" - "); 
  Serial.print(westLDR); //Χρησιμοποιείται το για ελέγξετε αν οι φωτοαντιστάσεις έχουν μέγαλη διαφορά στην τιμή τους. Εάν ναι, διορθώνει την τιμή σφάλματος
  Serial.print(" - "); 
  Serial.print(difference);
  Serial.print(" - "); 
  Serial.print(servoSet); // Προσαρμογή ρυθμίσεων servo, για μεγιστοποίηση της διαθέσιμης ταλάντευση 
  Serial.print(" - "); 
  Serial.println(".");
 
 // Ο αισθητήρας ενεργοποιείται από ψηλό παλμό 10 ή περισσότερων μικροδευτερολέπτων
  // Δίνει ένα σύντομο παλμό χαμηλού επιπέδου για να εξασφαλισθεί καθαρός ψηλός παλμός
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Διαβάζει το σήμα από τον αισθητήρα: ένας υψηλός παλμό του οποίου
  // η διάρκεια είναι ο χρόνος (σε μικροδευτερόλεπτα) από την αποστολή
  // του ping μέχρι τη λήψη της ηχώ από ένα αντικείμενο-εμπόδιο.

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // Μετατροπή χρόνου σε απόσταση
  cm = (duration/2) / 29.1;     // ΔΙΑΙΡΕΣΗ ΜΕ 29.1 Η ΠΟΛΛΑΠΛΑΣΙΑΣΜΟΣ ΜΕ 0.0343
  //Ελέγχει αν διέρχεται αντικείμενο σε απόσταση μικρότερη από 20cm και σκοτάδι
  if (cm<20 && westLDR<700)
  {
      digitalWrite(led, HIGH);//Ανάβει ο φωτισμός
      delay(2000); //καθυστερηση 2 sec
  }
  else
  {
      digitalWrite(led, LOW);//Σβήνει ο φωτισμός
  }
  Serial.print("Apostasi: ");
  Serial.print(cm);
  Serial.println("cm");
  

  // έλεγχος για την τέντα:
  steps = 0;
  if( pos == 0 )    // αν η τέντα είναι κλειστή...
  {
    if( westLDR > 600 )   // και αν υπάρχει φως,
    {
      pos = 1;            // τότε η τέντα ανοίγει ως τη μέση
      steps = maxSteps/2;
    }
  }
  else if( pos == 1 )     // αν η τέντα είναι μισάνοιχτη...
  {
    if( westLDR > 850 )   // και αν υπάρχει πολύ φως,
    {
      pos = 2;            // τότε η τέντα ανοίγει πλήρως.
      steps = maxSteps/2;
    }
    else if( westLDR < 600 )  // αλλά αν δεν έχει φως,
    {
      pos = 0;            // τότε η τέντα κλείνει.
      steps = -maxSteps/2;
    }
  }
  else if( pos == 2 )     // αν η τέντα είναι τελείως ανοιχτή...
  {
    if( westLDR < 850 )   // και αν δεν έχει πολύ φως,
    {
      pos = 1;            // τότε η τέντα κλείνει μέχρι τη μέση.
      steps = -maxSteps/2;
    }
  }
  
  Serial.print("steps=");
  Serial.println(steps);
  Serial.print("pos=");
  Serial.println(pos);  

  myStepper.step(steps);

  delay(100);
}
