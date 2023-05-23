#define R_CLOCKWISE_PIN           2
#define R_COUNTER_CLOCKWISE_PIN   4
#define L_CLOCKWISE_PIN           3
#define L_COUNTER_CLOCKWISE_PIN   5
//PPR : nombre d'impulsions par révolution
#define PPR                       3000.0
#define WHEEL_DIAM                12.065
#define PI                        3.14

volatile long rightCounter = 0;
volatile long leftCounter = 0;

void setup() {
  pinMode(R_CLOCKWISE_PIN, INPUT);
  pinMode(R_COUNTER_CLOCKWISE_PIN, INPUT);
  pinMode(L_CLOCKWISE_PIN, INPUT);
  pinMode(L_COUNTER_CLOCKWISE_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(R_CLOCKWISE_PIN), rightInterruptFunction, RISING);
  attachInterrupt(digitalPinToInterrupt(L_CLOCKWISE_PIN), leftInterruptFunction, RISING);

  Serial.begin(9600);
  Serial.println("Prêt");
}

void loop() {  
  Serial.print("Compteur encodeur droite : ");
  Serial.println(rightCounter);
  Serial.print("Distance (cm) : ");
  Serial.println(counterToCm(rightCounter));
  delay(1000);

  //majPosition(20, 20);
}

//Fonction interruption encodeur droite
void rightInterruptFunction(){
  int rightAChannel = digitalRead(R_CLOCKWISE_PIN);
  int rightBChannel = digitalRead(R_COUNTER_CLOCKWISE_PIN);

  if(rightAChannel == rightBChannel){
    rightCounter--;
  }

  else{
    rightCounter++;
  }
}

//Fonction interruption encodeur gauche
void leftInterruptFunction(){
  int leftAChannel = digitalRead(L_CLOCKWISE_PIN);
  int leftBChannel = digitalRead(L_COUNTER_CLOCKWISE_PIN);

  if(leftAChannel == leftBChannel){
    leftCounter--;
  }

  else{
    leftCounter++;
  }
}

//Fonction qui calcul la distance effectuée
float counterToCm(long counter){
  float angle = counter / PPR * 360.0; // Calcul angle effectué
  return (angle*PI/180) * (WHEEL_DIAM/2); //Calcul distance parcourue
}