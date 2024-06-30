
#include <Servo.h>

Servo myServo;

const int photoResistorPin = A0;
const int ledPin = 2; // LED del pin 
const int pwmPin = 10;
const int servoPin = A1; // Pin para controlar el servomotor 
const int buttonPin = 4; // Pin del pulsador

const int trigPin = 9;  // Pin TRIG del sensor HC-SR04
const int echoPin = 10; // Pin ECHO del sensor HC-SR04

long duration;
int distanceCm;
bool buttonState = false;
bool lastButtonState = false;

void setup() {
  pinMode(photoResistorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(pwmPin, OUTPUT);
  
  myServo.attach(servoPin);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  pinMode(buttonPin, INPUT_PULLUP); // Configura el pin del pulsador con una resistencia pull-up interna
  
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(photoResistorPin);
  int ledIntensity = map(sensorValue, 0, 1023, 0, 255);
  analogWrite(pwmPin, ledIntensity);
  
  if (sensorValue > 511) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  // Leer el estado del pulsador
  buttonState = digitalRead(buttonPin);
  
  // Detectar el cambio de estado del pulsador
  if (buttonState != lastButtonState) {
    if (buttonState == LOW) { // Pulsador presionado (estado bajo)
      bool ledInitialState = digitalRead(ledPin); // Guarda el estado inicial del LED
      for (int i = 0; i < 3; i++) {
        digitalWrite(ledPin, HIGH);
        delay(500);
        digitalWrite(ledPin, LOW);
        delay(500);
      }
      digitalWrite(ledPin, ledInitialState); // Restaura el estado inicial del LED
    }
    lastButtonState = buttonState;
  }

  // Generar un pulso de 10us en el pin TRIG del sensor HC-SR04
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Leer la duración del pulso en el pin ECHO y calcular la distancia en centímetros
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.034 / 2;

  // Mapear la distancia medida a un rango para el servomotor (0 a 180 grados)
  int servoPosition = map(distanceCm, 0, 200, 0, 180);
  servoPosition = constrain(servoPosition, 0, 180);
  myServo.write(servoPosition);

  // Imprimir información en el Monitor Serial
  Serial.print("Intensidad detectada de luz: ");
  Serial.print(sensorValue);
  Serial.print(" | Intensidad emitida por el LED: ");
  Serial.print(ledIntensity);
  Serial.print(" | Distancia medida: ");
  Serial.print(distanceCm);
  Serial.print(" cm | Posicion del servomotor: ");
  Serial.println(servoPosition);

  delay(100);
}