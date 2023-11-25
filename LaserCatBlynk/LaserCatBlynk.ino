#define BLYNK_TEMPLATE_ID "TMPL2ek035q2F"
#define BLYNK_TEMPLATE_NAME "Robô Laser Cat"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

#include "BlynkEdgent.h"
#include <Servo.h>

Servo myservo1;
Servo myservo2;

int velSliderValue = 100;

int ledpin = 16;
int servopin1 = 14;
int servopin2 = 12;

bool modoAleatorioAtivo = false;

void serv() {
  if (modoAleatorioAtivo) {
    int targetPos1 = random(0, 180);
    int targetPos2 = random(0, 180);

    int pos1 = myservo1.read();
    int pos2 = myservo2.read();

    int increment = map(velSliderValue, 0, 250, 0, 12);  // Ajuste conforme necessário

    if (pos1 < targetPos1) {
      pos1 += increment;
      if (pos1 > targetPos1) pos1 = targetPos1;
    } else if (pos1 > targetPos1) {
      pos1 -= increment;
      if (pos1 < targetPos1) pos1 = targetPos1;
    }

    if (pos2 < targetPos2) {
      pos2 += increment;
      if (pos2 > targetPos2) pos2 = targetPos2;
    } else if (pos2 > targetPos2) {
      pos2 -= increment;
      if (pos2 < targetPos2) pos2 = targetPos2;
    }

    myservo1.write(pos1);
    myservo2.write(pos2);
    
    delay((500/increment));  // Aguarde um curto período antes de verificar novamente
  }
}

BLYNK_WRITE(V0) {
  int pinvalue = param.asInt();
  digitalWrite(16, pinvalue);
}

BLYNK_WRITE(V1) {
  int pinvalue1 = param.asInt();
  myservo1.write(pinvalue1);
}

BLYNK_WRITE(V2) {
  int pinvalue2 = param.asInt();
  myservo2.write(pinvalue2);
}

BLYNK_WRITE(V3) {
  int pinvalue3 = param.asInt();

  if (pinvalue3 == 1) {
    Blynk.virtualWrite(V3, HIGH);
    modoAleatorioAtivo = true;
  } else if (pinvalue3 == 0) {
    Blynk.virtualWrite(V3, LOW);
    modoAleatorioAtivo = false;
  }
}

BLYNK_WRITE(V4) {
  velSliderValue = param.asInt();
}

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(ledpin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  BlynkEdgent.begin();
  myservo1.attach(servopin1);
  myservo2.attach(servopin2);
}

void loop() {
  BlynkEdgent.run();
  serv();
}
