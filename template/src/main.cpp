/*
Projet: CharlUS V0.000001
Equipe: P-16
Auteurs: Gibraltar
Description: Code du brave robot qui ne fais que se perdre
Date: Le jour de la naissance de Gibraltar
*/

/* ****************************************************************************
Inclure les librairies de functions que vous voulez utiliser
**************************************************************************** */

#include <LibRobus.h> // Essentielle pour utiliser RobUS
#include <fastLED.h>
/* ****************************************************************************
Variables globales et defines
**************************************************************************** */
#define comptePulseTheo 3200;
double speedGauche = 0.42;
double speedDroite = 0.4;

//#define PI 3.1415926
// -> defines...
// L'ensemble des fonctions y ont acces

/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */

void Acceleration()
{
    for (int i = 1; i <= 100; i++)
    {
        float pourcentage = i * 0.01;

        MOTOR_SetSpeed(RIGHT, speedDroite * pourcentage);
        MOTOR_SetSpeed(LEFT, speedGauche * pourcentage);
        delay(10);
    }
}

void Decceleration()
{
    for (int i = 100; i >= 1; i--)
    {
        float pourcentage = i * 0.01;

        MOTOR_SetSpeed(RIGHT, speedDroite * pourcentage);
        MOTOR_SetSpeed(LEFT, speedGauche * pourcentage);
        delay(5);
    }
}

int lireDistanceInfraRobot(int sensor)
{
    return ROBUS_ReadIR(sensor);
}

void Avancer_Cm(int distance)
{
    long compteur = 0;

    float circonference = PI * 7.5;
    long pulseParCm = 32000 / circonference;
    long comptePulse = (pulseParCm * (distance));
    comptePulse = comptePulse / 10;

    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);

    speedGauche = 0.42;
    speedDroite = 0.4;
    Acceleration();
    while (compteur < comptePulse)
    {
        delay(5);
        compteur = ENCODER_Read(RIGHT);
    }
    Decceleration();

    delay(200);
}


void Reculer_Cm(int distance)
{
    long compteur = 0;

    float circonference = PI * 7.5;
    long pulseParCm = 32000 / circonference;
    long comptePulse = (pulseParCm * (distance));
    comptePulse = comptePulse / 10;

    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);

    speedGauche = -0.42;
    speedDroite = -0.4;
    Acceleration();
    while (abs(compteur) < comptePulse)
    {
        delay(5);
        compteur = ENCODER_Read(RIGHT);
    }
    Decceleration();

    delay(200);
}

void Tourne_Gauche()
{
    int comptePulse;
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);

    MOTOR_SetSpeed(RIGHT, 0.4);
    MOTOR_SetSpeed(LEFT, 0);

    comptePulse = 0;
    while (comptePulse <= (1.18 * 3200))
    {
        comptePulse = ENCODER_Read(RIGHT);
        delay(10);
    }

    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}

void Tourne_Droite()
{
    int comptePulse;
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);

    MOTOR_SetSpeed(RIGHT, 0);
    MOTOR_SetSpeed(LEFT, 0.4);
    comptePulse = 0;
    while (comptePulse <= (1.18 * 3200))
    {
        comptePulse = ENCODER_Read(LEFT);
        delay(10);
    }
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}
//À modifier pour que le robot tourne sur lui-même
void Demi_Tour()
{
    int comptePulse;
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);

    MOTOR_SetSpeed(RIGHT, -0.4);
    MOTOR_SetSpeed(LEFT, 0.4);
    comptePulse = 0;
    while (comptePulse <= (1.18 * 3200))
    {
        comptePulse = ENCODER_Read(LEFT);
        delay(10);
    }
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);

    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
/*
    MOTOR_SetSpeed(RIGHT, -0.4);
    MOTOR_SetSpeed(LEFT, 0);
    comptePulse = 0;
    while (abs(comptePulse) <= (1.18 * 3200))
    {
        comptePulse = ENCODER_Read(RIGHT);
        delay(10);
    }
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
    */
}

/* ****************************************************************************
Fonctions d'initialisation (setup)
**************************************************************************** */
// -> Se fait appeler au debut du programme
// -> Se fait appeler seulement un fois
// -> Generalement on y initilise les varibbles globales

#define LED_PIN     4
#define NUM_LEDS    120
#define BRIGHTNESS  100
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

void LEDSTRIP_Standby()
{
    memset(leds, 0, NUM_LEDS * 3);
    for(int i = 0 ; i < NUM_LEDS; i++ ) {

    //leds[i-1] = CRGB::Black;
    leds[i] = CRGB::HotPink;
    
    FastLED.show();
    delay(10);
    }

    for(int i =BRIGHTNESS; i > 0 ; i--)
    {
      FastLED.setBrightness( i );
        FastLED.show();
      delay(10);
    }

   FastLED.setBrightness(BRIGHTNESS);
}

void LEDSTRIP_Level(int level)
{
    int color;
    if(level == 0)
        color = CRGB::Green;
    if(level == 1)
        color = CRGB::Yellow;
    if(level == 2)
        color = CRGB::Red;

      // 3 at a time
    for(int i = 3 ; i < NUM_LEDS; i++ ) {
      memset(leds, 0, NUM_LEDS * 3);
       
        leds[i] = color;
        leds[i-1] = color;
        leds[i-2] = color; 

      FastLED.show();
      delay(10);
    }
  
}



void setup()
{
    BoardInit();

     delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    /*pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    SERVO_Enable(1);
    SERVO_Enable(0);*/
}

/* ****************************************************************************
Fonctions de boucle infini (nloop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"


//https://www.tweaking4all.com/hardware/arduino/arduino-ws2812-led/

void loop()
{

    //LEDSTRIP_Standby();
    LEDSTRIP_Level(0);
    LEDSTRIP_Level(1);
    LEDSTRIP_Level(2);

    /*
    delay(1000);
    Tourne_Droite();
    Tourne_Gauche();
    delay(500);
    Demi_Tour();
    delay(1000);
    Avancer_Cm(15);
    delay(1000);
    Reculer_Cm(15);
    delay(5000);
*/
}
