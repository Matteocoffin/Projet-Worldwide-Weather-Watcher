#define LEDPIN1 7
#define PUSHPIN1 2

#define LEDPIN2 8
#define PUSHPIN2 3


#define PIN_LED_G 10
#define PIN_LED_B 11
#define PIN_LED_R 9


int randNumber = 0;
int data;
int mode = 0;
int *etatmode = NULL;
int *modeprecedant = NULL;


byte* debut = NULL;
byte bloqueur = 1;
byte* t = NULL;
unsigned long dureeAntiRebond;

int capteur_temp = 0;





void setup()
{
  
  pinMode(LEDPIN1, OUTPUT); 
  pinMode(PUSHPIN1, INPUT);
  pinMode(LEDPIN2, OUTPUT); 
  pinMode(PUSHPIN2, INPUT);
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  displayColor(0, 0, 0);
  initialisation_interrup();
}

void loop()
{
  if (*etatmode == 1)
  {
    displayColor(100, 220, 255);
    delay(1000);
  }
  if (*etatmode == 2)
  {
    displayColor(255,0,255);
    acquisition();
    delay(1000);
  }
  if (*etatmode == 3)
  {
    displayColor(255,255,0);
    delay(1000);
  }
  if (*etatmode == 4)
  {
    displayColor(0,0,255);
    delay(1000);
  }
  delay(1000);
}


void initialisation_interrup()
{
  attachInterrupt(digitalPinToInterrupt(PUSHPIN2),vert,CHANGE);
  attachInterrupt(digitalPinToInterrupt(PUSHPIN1),rouge,CHANGE);
}

void rouge()
{
  Serial.begin(9600);
  if( *t != 1){
    t = &bloqueur;
    dureeAntiRebond = 5000 + millis(); 
  }
  unsigned long date = millis();
  if (date > dureeAntiRebond)
  {
    digitalWrite(LEDPIN1,HIGH);
    if(mode == 3){
      Serial.print("mode standard");
      Serial.print("\t");
      mode = 2;
      etatmode = &mode;
      t = NULL;
    }
    if(mode == 0){
      Serial.print("mode configuration");
      Serial.print("\t");
      mode = 1;
      etatmode = &mode;
      t = NULL;
    }
    if(mode == 4){
      Serial.print("mode précedant");
      Serial.print("\t");
      mode = *modeprecedant;
      etatmode = &mode;
      Serial.print(mode);
      t = NULL;
    }
    if(mode == 2){
      Serial.print("mode maintenance");
      Serial.print("\t");
      modeprecedant = &mode;
      mode = 4;
      etatmode = &mode;
      t = NULL;
    }
    t = NULL;
  }
  if (digitalRead(PUSHPIN1)== LOW){
    t = NULL;
  }
}

void vert()
{
  Serial.begin(9600);
  if( *t != 1){
    t = &bloqueur;
    dureeAntiRebond = 5000 + millis(); 
  }
  unsigned long date = millis();
  if (date > dureeAntiRebond)
  {
    digitalWrite(LEDPIN2,HIGH);
    if(mode == 3){
      Serial.print("mode maintenance");
      Serial.print("\t");
      modeprecedant = &mode;
      mode = 4;
      etatmode = &mode;
      t = NULL;
    }
    if(mode == 2){
      Serial.print("mode economique");
      Serial.print("\t");
      mode = 3;
      etatmode = &mode;
      t = NULL;
    }
    if(mode == 0){
      Serial.print("mode standard");
      Serial.print("\t");
      mode = 2;
      etatmode = &mode;
      t = NULL;
    }
    t = NULL;
  }
  if (digitalRead(PUSHPIN2)== LOW){
    t = NULL;
  }
}

void displayColor(byte r, byte g, byte b) {

  analogWrite(PIN_LED_R, ~r);
  analogWrite(PIN_LED_G, ~g);
  analogWrite(PIN_LED_B, ~b);
}
  


void acquisition(){
  int reading = analogRead(capteur_temp);
  Serial.begin(9600);
  float volt = reading * 5.0;
  volt = volt / 1024.0;
  float temperature = (volt - 0.5) * 100 ;
  Serial.print(temperature); 
  Serial.println(" degrees C");
}

void demarrage()
{
  if(*debut != 1)
  {
    delay(2000);
    mode = 2;
    etatmode = &mode;
    debut = &bloqueur;
  }
}
