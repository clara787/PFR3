#define R_CLOCKWISE_PIN           4
#define R_COUNTER_CLOCKWISE_PIN   5
#define L_CLOCKWISE_PIN           6
#define L_COUNTER_CLOCKWISE_PIN   7

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
  majPosition();
  // put your main code here, to run repeatedly:
  Serial.print("Compteur encodeur droite : ");
  Serial.println(rightCounter);
  Serial.print("Compteur encodeur gauche : ");
  Serial.println(leftCounter);
  delay(1000);
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

void majPosition(){
  int posX = 20;
  int posY = 20;

  
}