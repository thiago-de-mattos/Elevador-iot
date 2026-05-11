#include <Arduino.h>
#include <Stepper.h>
#include <Servo.h>

Stepper elevador(2048, 8, 9, 10, 11);
Servo travaPorta;

const int BTN_ANDAR1 = 5;  // Azul
const int BTN_ANDAR2 = 6;  // Verde
const int BTN_ANDAR3 = 7;  // Amarelo
const int BTN_TERREO = A4; // Preto
const int BTN_CARGA  = A5; // Branco

const int PASSOS_POR_ANDAR = 2048;
int andarAtual = 0;

void irParaAndar(int destino);

void setup() {
  Serial.begin(9600);
  elevador.setSpeed(15);
  travaPorta.attach(3);

  pinMode(BTN_ANDAR1, INPUT_PULLUP);
  pinMode(BTN_ANDAR2, INPUT_PULLUP);
  pinMode(BTN_ANDAR3, INPUT_PULLUP);
  pinMode(BTN_TERREO, INPUT_PULLUP);
  pinMode(BTN_CARGA,  INPUT_PULLUP);

  travaPorta.write(0);
  Serial.println("Sistema iniciado. Pressione um botao.");
}

void loop() {
  if (digitalRead(BTN_ANDAR1) == LOW) { delay(50); if (digitalRead(BTN_ANDAR1) == LOW) irParaAndar(1); }
  if (digitalRead(BTN_ANDAR2) == LOW) { delay(50); if (digitalRead(BTN_ANDAR2) == LOW) irParaAndar(2); }
  if (digitalRead(BTN_ANDAR3) == LOW) { delay(50); if (digitalRead(BTN_ANDAR3) == LOW) irParaAndar(3); }
}

void irParaAndar(int destino) {
  if (destino == andarAtual) {
    Serial.println("Ja esta neste andar!");
    return;
  }

  Serial.print("Indo para o andar "); Serial.println(destino);
  travaPorta.write(0);
  delay(500);

  int passos = (destino - andarAtual) * PASSOS_POR_ANDAR;
  elevador.step(passos);
  andarAtual = destino;

  Serial.println("Chegou! Abrindo porta...");
  travaPorta.write(90);
  delay(3000);

  Serial.println("Fechando porta e voltando ao terreo...");
  travaPorta.write(0);
  delay(500);

  elevador.step(-(andarAtual * PASSOS_POR_ANDAR));
  andarAtual = 0;
  Serial.println("Pronto. Aguardando proximo botao.");
}