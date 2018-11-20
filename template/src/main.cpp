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
#define comptePulseTheo 3200
#define PINBLANCHE 6
#define PINCOULEUR 7
double speedGauche = 0.42;
double speedDroite = 0.4;
#include <time.h>
#include <stdlib.h>
int servoMoteurDroit = 1;
int servoMoteurGauche = 0;
int inputButtonStart = 6;
int inputButtonDifficulte = 5;
int count = 1;
int positionX = 0;
int positionY = 0;
int orientation = 0;
int IRCentrePin = 13;
int IRGauchePin = 12;
int IRDroitePin = 11;

//Lumière
#define LED_PIN 10
#define NUM_LEDS 120
#define BRIGHTNESS 100
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

//#define PI 3.1415926
// -> defines...
// L'ensemble des fonctions y ont acces

/* ****************************************************************************
Vos propres fonctions sont creees ici
**************************************************************************** */
struct JEU
{
    int blancheX;
    int blancheY;
    int couleurX;
    int couleurY;
};

void MoveServoMoteur(int servo, int angle)
{
    //Les meilleurs valeurs sont 10 et 170 degrés pour tenir les balles et de 90 degrés pour les relaché
    SERVO_SetAngle(servo, angle);
}

int LireDistanceInfraRobot(int sensor)
{
    return ROBUS_ReadIR(sensor);
}

void Suivre_la_ligne()
{
    MOTOR_SetSpeed(RIGHT, 0.21);
    MOTOR_SetSpeed(LEFT, 0.2);

    delay(500);

    int centre = 0;
    int gauche = 0;
    int droite = 0;

     while(centre < 600 && gauche < 500 && droite < 500)
    {
        centre = analogRead(IRCentrePin);
        gauche = analogRead(IRGauchePin) * 1.5;
        droite = analogRead(IRDroitePin) * 1.5;

        if(gauche > 500)
        {
            Serial.println("Gauche");
            MOTOR_SetSpeed(RIGHT, 0.2);
            MOTOR_SetSpeed(LEFT, 0);
        }
        if(droite > 500)
        {
            Serial.println("Droite");
            MOTOR_SetSpeed(RIGHT, 0);
            MOTOR_SetSpeed(LEFT, 0.2);
        }
    }

    delay(5000);
    /*
    static int derniereDirectionEnregistree = 0;
    Serial.println("Fonction Suivre_la_ligne");

    int lectureInitiale;

    if (analogRead(IRDevantPin) < 600)
    {
        lectureInitiale = 0;
        MOTOR_SetSpeed(LEFT, 0.1);
        MOTOR_SetSpeed(RIGHT, 0.2);
        while (analogRead(IRDevantPin) < 600)
            delay(50);
    }
    else
        lectureInitiale = 1;

    if ((analogRead(IRDevantPin) < 600) && derniereDirectionEnregistree == 1)
    {
        MOTOR_SetSpeed(LEFT, 0.1);
        MOTOR_SetSpeed(RIGHT, 0.2);
        while (analogRead(IRDevantPin) < 600)
            delay(50);
    }

    while (analogRead(IRExterieurPin) * 1.5 < 400)
    {
        MOTOR_SetSpeed(LEFT, 0.2);
        MOTOR_SetSpeed(RIGHT, 0.1);

        derniereDirectionEnregistree = 1;

        if (lectureInitiale == 1)
        {
            while (analogRead(IRDevantPin) > 600)
            {
                delay(50);
            }
        }
        else
        {
            while (analogRead(IRDevantPin) < 600)
            {
                delay(50);
            }
        }

        if (analogRead(IRExterieurPin) * 1.5 > 400)
        {
            break;
        }

        MOTOR_SetSpeed(LEFT, 0.1);
        MOTOR_SetSpeed(RIGHT, 0.2);

        derniereDirectionEnregistree = 0;

        if (lectureInitiale == 1)
        {
            while (analogRead(IRDevantPin) < 600)
            {
                delay(50);
            }
        }
        else
        {
            while (analogRead(IRDevantPin) > 600)
            {
                delay(50);
            }
        }
    }
    */
    /*
        int lecture = 1000;
        lecture = analogRead(IRInterieurPin);
        
            int lecture1, lecture2;
            lecture =  analogRead(IRInterieurPin);
            delay (50);
            lecture1 =  analogRead(IRExterieurPin);
            delay (50);
            lecture2 =  analogRead(IRDevantPin);
            delay (50);
             
            Serial.print("lecture :");
            Serial.println(lecture );
            Serial.print("lecture1 : ");
            lecture1= lecture1*1.5;
            Serial.println(lecture1 );
            Serial.print("lecture2 : ");
            Serial.println(lecture2 );
            delay (2000);
    
    delay(100);
    MOTOR_SetSpeed(LEFT, 0.);
    MOTOR_SetSpeed(RIGHT, 0.);
    delay(1000);*/
}

JEU GetPositionBalles(int difficulte)
{
    int nbrAleatoire;
    JEU balles;

    int facile[6][2][2] = {{{1, 1}, {0, 0}},
                           {{1, 1}, {0, 2}},
                           {{3, 1}, {4, 0}},
                           {{3, 1}, {4, 2}},
                           {{1, 1}, {4, 0}},
                           {{3, 1}, {0, 2}}};

    int moyen[4][2][2] = {{{1, 1}, {2, 0}},
                          {{3, 1}, {2, 2}},
                          {{3, 2}, {4, 1}},
                          {{1, 0}, {0, 1}}};

    int difficile[4][2][2] = {{{0, 1}, {3, 1}},
                              {{0, 1}, {4, 1}},
                              {{4, 1}, {0, 1}},
                              {{3, 1}, {1, 1}}};

    difficulte = 1;
    if (difficulte == 0)
    {
        nbrAleatoire = rand() % 6;
        Serial.println(nbrAleatoire);
        nbrAleatoire = 2;
        balles.blancheX = facile[nbrAleatoire][0][0];
        balles.blancheY = facile[nbrAleatoire][0][1];
        balles.couleurX = facile[nbrAleatoire][1][0];
        balles.couleurY = facile[nbrAleatoire][1][1];
        return balles;
    }
    else if (difficulte == 1)
    {
        nbrAleatoire = rand() % 4;
        nbrAleatoire = 1;
        balles.blancheX = moyen[nbrAleatoire][0][0];
        balles.blancheY = moyen[nbrAleatoire][0][1];
        balles.couleurX = moyen[nbrAleatoire][1][0];
        balles.couleurY = moyen[nbrAleatoire][1][1];
        return balles;
    }
    else
    {
        nbrAleatoire = rand() % 4;
        nbrAleatoire = 2;
        balles.blancheX = difficile[nbrAleatoire][0][0];
        balles.blancheY = difficile[nbrAleatoire][0][1];
        balles.couleurX = difficile[nbrAleatoire][1][0];
        balles.couleurY = difficile[nbrAleatoire][1][1];
        return balles;
    }
}

void RejoindreLigne()
{
    Serial.println("Fonction RejoindreLigne :");
    MOTOR_SetSpeed(RIGHT, 0.2);
    MOTOR_SetSpeed(LEFT, 0.21);

    int centre = 0;
    int gauche = 0;
    int droite = 0;

    while(centre < 600 && gauche < 500 && droite < 500)
    {
        centre = analogRead(IRCentrePin);
        gauche = analogRead(IRGauchePin) * 1.5;
        droite = analogRead(IRDroitePin) * 1.5;
    }
}

void CalculerPositionX(int positionX, int orientation)
{
    Serial.println("Fonction CalculerPositionX :");
    if (orientation == 1)
    {
        positionX = positionX + 1;
    }
    else if (orientation == 3)
    {
        positionX = positionX - 1;
    }
}

void CalculerPositionY()
{
    Serial.println("Fonction CalculerPositionY :");
    if (orientation == 0)
    {
        positionY = positionY + 1;
    }
    else if (orientation == 2)
    {
        positionY = positionY - 1;
    }
}

void TournerDroite()
{
    Serial.println("Fonction TournerDroite :");
    MOTOR_SetSpeed(RIGHT, -0.1);
    MOTOR_SetSpeed(LEFT, 0.3);
    delay(500);
    int hasALine = 0;
    while (hasALine < 400)
    {
        hasALine = analogRead(IRGauchePin) * 1.5;
    }
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
    orientation = (orientation + 1) % 4;
}

void TournerGauche()
{
    Serial.println("Fonction TournerGauche :");
    MOTOR_SetSpeed(RIGHT, 0.3);
    MOTOR_SetSpeed(LEFT, -0.1);
    delay(500);
    int hasALine = 0;
    while (hasALine < 400)
    {
        hasALine = analogRead(IRDroitePin) * 1.5;
        Serial.println(hasALine);
    }
    MOTOR_SetSpeed(RIGHT, 0);
    MOTOR_SetSpeed(LEFT, 0);
    orientation = (orientation - 1) % 4;
}

void positionnerBalle(int balleX, int balleY)
{
    Serial.println("Fonction positionnerBalle :");
    //Ce while fait rejoindre la colonne en question
    while (positionX != balleX)
    {
        Suivre_la_ligne();
        CalculerPositionX(positionX, orientation);
        Serial.print("BalleX :");
        Serial.println(balleX);
        Serial.print("positionX :");
        Serial.println(positionX);
        Serial.print("BalleY :");
        Serial.println(balleY);
        Serial.print("positionY :");
        Serial.println(positionY);
    }
    //Ce if fait tourner selon le besoin
    if (positionY < balleY)
    {
        if (orientation == 1)
        {
            TournerGauche();
        }
        if (orientation == 3)
        {
            TournerDroite();
        }
    }
    else if (positionY > balleY)
    {
        if (orientation == 1)
        {
            TournerDroite();
        }
        if (orientation == 3)
        {
            TournerGauche();
        }
    }
    //Ce while faite avancer jusqu'à la ligne en question
    while (positionY != balleY)
    {
        Suivre_la_ligne();
        CalculerPositionY();
        Serial.print("positionX :");
        Serial.println(positionX);
        Serial.print("positionY :");
        Serial.println(positionY);
    }
}

void retourRobot(int positionX, int positionY, int orientation)
{
    while (positionY != 0)
    {
        Suivre_la_ligne();
        CalculerPositionY();
    }
    
    if(positionX == 1)
    {
        TournerGauche();
        Suivre_la_ligne();
        TournerDroite();
    }
    if(positionX == 2)
    {
        TournerDroite();
        Suivre_la_ligne();
        TournerDroite();
    }

    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);

    //Code pour retourner dans son enclos
    //ici
}

void LancerPartie(int difficultee)
{
    Serial.println("Fonction LancerPartie :");

    positionX = 0;
    positionY = 0;
    orientation = 1;

    JEU balles;

    balles = GetPositionBalles(difficultee);

    Serial.println(balles.blancheX);
    Serial.println(balles.blancheY);
    Serial.println(balles.couleurX);
    Serial.println(balles.couleurY);

    RejoindreLigne();
    positionX++;

    TournerGauche();

    RejoindreLigne();
    positionY++;

    TournerDroite();

    if (balles.blancheX > balles.couleurX)
    {
        positionnerBalle(balles.blancheX, balles.blancheY);

        if (balles.couleurX < positionX)
        {
            if (orientation == 2)
            {
                TournerDroite();
            }
            else
            {
                while (orientation != 3)
                {
                    TournerGauche();
                }
            }
        }
        delay(1000);
        SERVO_SetAngle(servoMoteurGauche, 90);
        delay(1000);
    }

    positionnerBalle(balles.couleurX, balles.couleurY);

    if(orientation == 0)
    {
        TournerGauche();
    }
    if(orientation == 2)
    {
        TournerDroite();
    }

    /*if (balles.blancheY > balles.couleurY)
    {
        TournerGauche();
    }
    else
    {
        TournerDroite();
        if (balles.couleurY == balles.blancheY)
        {
            TournerDroite();
        }
    }
    SERVO_SetAngle(servoMoteurDroit, 90);
    delay(2000);

    if (balles.blancheX > balles.couleurX)
    {
        positionnerBalle(balles.blancheX, balles.blancheY);
        if (balles.couleurY < balles.blancheY)
        {
            TournerGauche();
        }
        else
        {
            TournerDroite();
            if (balles.couleurY == balles.blancheY)
            {
                TournerDroite();
            }
        }

        SERVO_SetAngle(servoMoteurGauche, 90);
        delay(2000);
    }*/

    retourRobot(positionX, positionY, orientation);
}

bool buttonPress(int analogButtonInput)
{
    int press = analogRead(analogButtonInput);

    if (press == 1023)
    {
        int outputPress = 1023;

        while (outputPress >= 1000)
        {
            outputPress = analogRead(analogButtonInput);
            Serial.println(outputPress);
        }
        return true;
    }
    return false;
}

void LEDSTRIP_Level(int level)
{
    int color;

    if (level == 0)
        color = CRGB::Blue;
    if (level == 1)
        color = CRGB::Yellow;
    if (level == 2)
        color = CRGB::Pink;

    memset(leds, 0, NUM_LEDS * 3);
    for (int i = 0; i < 60; i++)
    {

        //leds[i-1] = CRGB::Black;
        leds[i] = color;

        FastLED.show();
        delay(10);
    }

    for (int i = BRIGHTNESS; i >= 0; i--)
    {
        FastLED.setBrightness(i);
        FastLED.show();
        delay(10);
    }

    FastLED.setBrightness(BRIGHTNESS);
}

int choixDifficulte()
{
    int start = 0;
    int difficulte = 0;

    while (!start)
    {
        start = buttonPress(inputButtonStart);

        bool buttonDifficulteTrigger = buttonPress(inputButtonDifficulte);

        if (buttonDifficulteTrigger)
        {
            difficulte++;
            Serial.println(difficulte % 3);
            LEDSTRIP_Level(difficulte % 3);
        }
    }
    return difficulte;
}

/* ****************************************************************************
Fonctions d'initialisation (setup)
**************************************************************************** */
// -> Se fait appeler au debut du programme
// -> Se fait appeler seulement un fois
// -> Generalement on y initilise les varibbles globales

void setup()
{
    BoardInit();
    //pinMode(LED_BUILTIN, OUTPUT);
    pinMode(11, INPUT);
    pinMode(12, INPUT);
    pinMode(13, INPUT);
    pinMode(14, INPUT);
    pinMode(15, INPUT);
    Serial.begin(9600);

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    MoveServoMoteur(servoMoteurGauche, 10);
    MoveServoMoteur(servoMoteurDroit, 170);
}

/* ****************************************************************************
Fonctions de boucle infini (nloop())
**************************************************************************** */
// -> Se fait appeler perpetuellement suite au "setup"

void loop()
{
        Serial.println(analogRead(IRCentrePin));
        Serial.println(analogRead(IRGauchePin));
        Serial.println(analogRead(IRDroitePin));
        delay(2000);
    //MOTOR_SetSpeed(RIGHT, 0.2);
    //MOTOR_SetSpeed(LEFT, 0.21);
    //delay(20000);
    /*int lecture11 = analogRead(11);
    int lecture12 = analogRead(12);
    int lecture13 = analogRead(13);
    int lecture14 = analogRead(14);
    int lecture15 = analogRead(15);

    Serial.print("lecture11 :");
    Serial.println(lecture11);
    Serial.print("lecture12 : ");
    Serial.println(lecture12);
    Serial.print("lecture13 : ");
    Serial.println(lecture13);
    Serial.print("lecture14 : ");
    Serial.println(lecture14);
    Serial.print("lecture15 : ");
    Serial.println(lecture15);
    delay(2000);*/

    //Attend le choix de difficulté de l'utilisateur
    //int difficulte = choixDifficulte() % 3;

    //int gauche = analogRead(IRGauchePin);
    //Serial.println(gauche);
    //LancerPartie(0);
    //Suivre_la_ligne();
}