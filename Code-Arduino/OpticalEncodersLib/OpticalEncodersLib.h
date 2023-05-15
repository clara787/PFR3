#define R_CLOCKWISE_PIN           4
#define R_COUNTER_CLOCKWISE_PIN   5
#define L_CLOCKWISE_PIN           6
#define L_COUNTER_CLOCKWISE_PIN   7
//PPR : nombre d'impulsions par révolution
#define PPR                       3000.0
#define WHEEL_DIAM                12.065
#define PI                        3.14

//Fonction initialisation encodeurs
void initEncoders();

//Fonction interruption encodeur droite
void rightInterruptFunction();

//Fonction interruption encodeur gauche
void leftInterruptFunction();

//Fonction qui calcul la distance effectuée
float counterToCm(int counter);

float getLeftCounter();
float getRightCounter();