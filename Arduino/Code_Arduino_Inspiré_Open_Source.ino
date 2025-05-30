//Bluetooth
#include <SoftwareSerial.h>
#define rxPin 8 // Correspondant à la broche tx du module bluetooth
#define txPin 7 // Correspondant à la broche Rx du module bluetooth
#define baudrate 9600 
SoftwareSerial mySerial(rxPin ,txPin); //Definition du software serial


//OLED ATTENTION, la librarie utilisée (d'origine) consomme énormement de mémoire RAM en buffer.
// Si le programme ne lance rien, même pas le setup, le problème peut venir de là.
// On peut soit baisser le nombre de pixels dans l'écran, soit utiliser d'autres librairies moins gourmandes facilement trouvables sur internet.
#include <Adafruit_SSD1306.h>
#define nombreDePixelsEnLargeur 128        // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64        // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1          // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED     0x3C        // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)
Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);  


// MESURE Pin pour mesure de la tension du capteur //////////
const int capteurgraphitePin = A0;  // Pin A0
float calibre = pow(10,-6); // pour avoir des Mohms
// Variables pour éviter le delay 
unsigned long previousMillis = 0;        // will store last time LED was updated
// Variable pour cadencer l'acquisition
const long interval = 500;           // interval at which to blink (milliseconds) for MesureINST


// MESURE FLEX SENSOR
const int flexPin = A1;      // Pin connected to voltage divider output
const float VCC = 5;      // voltage at Ardunio 5V line
const float R_DIV = 10000.0;  // resistor used to create a voltage divider
const float flatResistance = 30000.0; // resistance when flat
const float bendResistance = 90000.0;  // resistance at 90 deg

//ENCODEUR ROTATOIRE
#define encoder0PinA  2  //CLK Output A Do not use other pin for clock as we are using interrupt
#define encoder0PinB  4  //DT Output B
#define Switch 5 // Switch connection if available

volatile int encoder0Pos = 1;

// Digital Potentiometer
#include <SPI.h>
const byte csPin           = 11;      // MCP42100 chip select pin
const int  maxPositions    = 256;     // wiper can move from 0 to 255 = 256 Positions
const long rAB             = 52200;   // 50k pot resistance between terminals A and B, mais pour ajuster au multimètre, je mets 47500
const byte rWiper          = 125;     // 125 ohms pot wiper resistance
const byte pot0            = 0x11;    // pot0 addr // B 0001 0001
const byte pot0Shutdown    = 0x21;    // pot0 shutdown // B 0010 0001
long R3;                              // Valeur de R3 calculé en sortir du potentiomètre
int PotPos;                           // Valeur de la position du potentiomètre entre 0 et 255



// MENU ///////////////////////////
int Position = 1; // Position par défaut dans le menu
int OK=0;  //Indicateur si SWITCH enclecnhé
String text1="1 : Graphite"; // Les différents menus
String text3="2 : Flex Sensor";
String text4="3 : Calibration";

// Permet de gérer le potentiomètre
void setPotWiper(int addr, int pos) {
  pos = constrain(pos, 0, 255);            // limit wiper setting to range of 0 to 255
  digitalWrite(csPin, LOW);                // select chip
  SPI.transfer(addr);                      // configure target pot with wiper Position
  SPI.transfer(pos);
  digitalWrite(csPin, HIGH);               // de-select chip

  R3 = ((rAB * pos) / maxPositions ) + rWiper; // Calcul de R3 (résistance entre wiper et B)
  Serial.print(F(" Resistance wiper to B terminal: "));
  Serial.print(R3);
}

void setup() {
    // Initialisation de l'écran OLED
  if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
    while(1);   
  mySerial.begin(baudrate); // Initialiser le port bluetooth
  Serial.begin(baudrate); // Initialiser le port série
  pinMode(encoder0PinA, INPUT); 
  digitalWrite(encoder0PinA, HIGH);       // turn on pullup resistor

  pinMode(encoder0PinB, INPUT); 
  digitalWrite(encoder0PinB, HIGH);       // turn on pullup resistor

  pinMode(Switch,INPUT_PULLUP);

  attachInterrupt(0, doEncoder, RISING); // encoder pin on interrupt 0 - pin2

  Serial.begin (9600);

  digitalWrite(csPin, HIGH);        // chip select default to de-selected
  SPI.begin();
  delay(5000);
  PotPos=Calibration();             // Calibration du potentiomètre en fonction du capteur
}


void loop() {
  ChoixMenu(); // Affiche le menu et ses choix en boucle
}


void ChoixMenu(){
  // // MENU
  unsigned long currentMillis = millis ();
  if (currentMillis - previousMillis >= 500) 
  {
    previousMillis = currentMillis;
    ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
    ecranOLED.setTextSize(1);                           // Taille du texte
    ecranOLED.setCursor(0, 0);                          // Placement du texte
    ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);   // Couleur du texte, et couleur du fond
    //setPotWiper(pot0,7);
    Valider_Menu();
   // Fonction primordiale : vérifie si les boutons physiques ne sont pas appuyé et ajuste la variable position, OK et OK_TEL (si un bouton sur tél n'est pas cliqué)
    if(Position==1)                       // Si nous sommes en position 1, affiche le texte correspondant (effet de surbrillance)
    {
      ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      ecranOLED.println(text1);
      ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
      ecranOLED.println(text3);
      ecranOLED.println(text4);
      ecranOLED.display();                            // Transfert le buffer à l'écran

      if (OK==1){
        OK=0;
        MesureINST();
      }
    
    }

    else if (Position==2)// Si nous sommes en position 3, affiche le texte correspondant (effet de surbrillance)
    {
      ecranOLED.println(text1);
      ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      ecranOLED.println(text3);
      ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK); 
      ecranOLED.println(text4);
      ecranOLED.display();                            // Transfert le buffer à l'écran
      if (OK==1){
        OK=0;
        FlexSensor();
      }
     
    }
    else if (Position==3)// Si nous sommes en position 4, affiche le texte correspondant (effet de surbrillance)
    {
      ecranOLED.println(text1);
      ecranOLED.println(text3);
      ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE); 
      ecranOLED.println(text4);
      ecranOLED.display();

      if (OK==1){
        OK=0;
        Calibration();
      }
         
    }
  }
}

void doEncoder() {
  if (digitalRead(encoder0PinA)==HIGH && digitalRead(encoder0PinB)==HIGH) {
    encoder0Pos++;
    Position=encoder0Pos;
    Serial.println (encoder0Pos, DEC);  //Angle = (360 / Encoder_Resolution) * encoder0Pos
  } else if (digitalRead(encoder0PinA)==HIGH && digitalRead(encoder0PinB)==LOW) {
    encoder0Pos--;
    Position=encoder0Pos;
    Serial.println (encoder0Pos, DEC);  //Angle = (360 / Encoder_Resolution) * encoder0Pos
  }
}



void FlexSensor(){
  int ADCflex;
  float Vflex;
  float Rflex;
  ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
  ecranOLED.setTextSize(1);                   // Taille des caractères (1:1, puis 2:1, puis 3:1)
  ecranOLED.setCursor(0, 0);
  ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);   // Couleur du texte, et couleur du fond
  ecranOLED.println(F("Mesure Flex Sensor:"));
  ecranOLED.display();    
  delay(1000);
  do
  {
  // Read the ADC, and calculate voltage and resistance from it
  ADCflex = analogRead(flexPin);
  Vflex = ADCflex * VCC / 1024.0;
  Rflex = R_DIV * (VCC / Vflex - 1.0); // Calcul de la résistance du flex
  // Use the calculated resistance to estimate the sensor's bend angle:
  // float angle = map(Rflex, flatResistance, bendResistance, 0, 90.0);
  // Serial.println("Bend: " + String(angle) + " degrees");
  delay(500);
  DisplayAndTransmitter(Rflex,3);
  Valider_Menu();
  }while (Position==2);
  
     
  ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
  ecranOLED.setCursor(0, 0);
  ecranOLED.setTextSize(1);
  ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);   // Couleur du texte, et couleur du fond                 
  ecranOLED.println(text1);
  ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  ecranOLED.println(text3);
  ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK); 
  ecranOLED.println(text4);
  ecranOLED.display();
  delay(1000);
}


void MesureINST()
{
  float R4=100000;
  int Vcc=5;
  float R2=100000;
  int R1=10000;
  float Vadc=0;
  float Res=-1;
  // Affichage de mesureINST avant les mesures réelles, comme cela, l'utilisateur peut arrêter d'appuyer sur le bouton OK et cela évite de le faire sortir du programme
  ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
  ecranOLED.setTextSize(1);                   // Taille des caractères (1:1, puis 2:1, puis 3:1)
  ecranOLED.setCursor(0, 0);
  ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);   // Couleur du texte, et couleur du fond
  ecranOLED.println(F("Mesure instantannee : "));
  ecranOLED.display();    
  delay(1000);
  do
  {    
    // MESURE
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      Vadc = analogRead(capteurgraphitePin)*5.0/1024.0; // Valeur lu en voltage sortant du capteur
      Res=(R2*(1+R4/R3)*(Vcc/Vadc)-R2-R1)*calibre; // Calcul de la résistance
      Serial.print("Vadc = ");
      Serial.println(Vadc*1024/5);
      DisplayAndTransmitter(Res,1); // Transmet la donnée à afficher à l'ordi, à l'écran et au bluetooth
      Valider_Menu();
      }
  }while(Position==1); 
  
  
  ecranOLED.clearDisplay();                                   // Efface l'intégralité du buffer
  ecranOLED.setCursor(0, 0);
  ecranOLED.setTextSize(1);                 
  ecranOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);   // Couleur du texte, et couleur du fond
  ecranOLED.println(text1);
  ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);   // Couleur du texte, et couleur du fond
  ecranOLED.println(text3);
  ecranOLED.println(text4);
  ecranOLED.display();    
  delay(1000);
 
} 

void DisplayAndTransmitter(float VALUE, int choix){
  char ResASCII[10];
  dtostrf(VALUE, 5, 2, ResASCII); //Transforme le float en chaîne de caractères
  ecranOLED.clearDisplay();                                   // Efface l'intégralité du buffer
  ecranOLED.setTextSize(1);                   // Taille des caractères (1:1, puis 2:1, puis 3:1)
  ecranOLED.setCursor(0, 0);
  ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);   // Couleur du texte, et couleur du fond
  if(choix==1){ecranOLED.println(F("Mesure instantanee :"));}
  else if(choix==2){ecranOLED.println(F("Mesure moyennee :"));}
  else if(choix==3){ecranOLED.println(F("Mesure Flex Sensor :"));dtostrf(VALUE, 5, 0, ResASCII);}
  else if(choix==4){ecranOLED.println(F("Calibration en cours"));}
  ecranOLED.setTextSize(3);                   // Ecris en gros
  ecranOLED.setCursor(0, 30); // Ecris à gauche de l'écran
  ecranOLED.print(ResASCII); // Envoyer sur l'écran la valeur
  mySerial.write(ResASCII); // Envoyer sur le port bluetooth la valeur acquise
  Serial.println(ResASCII); // Afficher sur le port série la valeur de la tension mesuré en bytes
  ecranOLED.display();                            // Transfert du buffer à l'écran
  delay(100);

}


void Valider_Menu(){
  if (digitalRead(Switch)==LOW){
    OK=1;
  }
}




//Calibration basée sur un principe de dichotimie. Essaie de se placer au centre de la plage des 1024 valeurs.
int Calibration(){
  ecranOLED.clearDisplay();                                   // Efface l'intégralité du buffer
  ecranOLED.setTextSize(1);                   // Taille des caractères (1:1, puis 2:1, puis 3:1)
  ecranOLED.setCursor(0, 0);
  ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);   // Couleur du texte, et couleur du fond
  ecranOLED.println(F("Calibration en cours"));
  ecranOLED.display();    
  delay(2000);   
  int Vadc=0;
  int tolerance=40; //Tolerance : 512+-20
  int V_Cible = 512;
  int a = 0;
  int b = 255;
  int ite=0;
  while(abs(Vadc - V_Cible) > tolerance)
  {
    Vadc=0;
    ite++;
    PotPos = (a + b)/2; // Prend le milieu  des 2 valeurs précédentes
    setPotWiper(pot0,int(PotPos));
    for(int i=0;i<3;i++)
    {
      Vadc += analogRead(capteurgraphitePin);
      delay(200);
    }
    Vadc = int(Vadc/3);
    Serial.print(F("Position Pota :"));
    Serial.print(PotPos);
    Serial.print(F(". Valeur lue :"));
    Serial.println(Vadc); // Afficher sur le port série la valeur de la tension mesuré en bytes
    DisplayAndTransmitter(PotPos,4);
    if(Vadc > V_Cible){ // on change le minimum ou le maximum
      a = PotPos;}
    else{
      b = PotPos;}
    if(ite>12){break;}
  }
  Serial.print(F("Finie. Pota en "));
  Serial.println(PotPos); // Afficher sur le port série la valeur de la tension mesuré en bytes
  return PotPos;

}
