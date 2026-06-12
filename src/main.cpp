#include <Arduino.h>
#include <Stepper.h>
#include <SoftwareSerial.h> // Biblioteca para criar a porta do Wi-Fi

// --- 1. CONFIGURAÇÃO DO WI-FI ---
// O pino 2 recebe (RX) e o pino 3 envia (TX)
SoftwareSerial moduloWiFi(2, 3); 

String nomeDaRede = "realme C63";
String senhaDaRede = "50309010";

// --- 2. CONFIGURAÇÃO DO MOTOR ---
const int passosPorVolta = 2048; 
Stepper meuMotor(passosPorVolta, 8, 10, 9, 11);

int andarAtual = 0; 
float alturaPorAndarCm = 13.0; 
float diametroCarretelCm = 2.0; 
float circunferencia = diametroCarretelCm * 3.14159;
int passosPorAndar = (alturaPorAndarCm / circunferencia) * passosPorVolta;

// Função simples para enviar comandos para a peça do Wi-Fi
void enviarComandoWiFi(String comando, int tempoEspera) {
  moduloWiFi.println(comando);
  long int tempo = millis();
  while ((tempo + tempoEspera) > millis()) {
    while (moduloWiFi.available()) {
      char c = moduloWiFi.read();
      Serial.print(c); // Mostra na tela o que o Wi-Fi respondeu
    }
  }
}

void setup() {
  Serial.begin(9600); // Velocidade do computador
  moduloWiFi.begin(9600); // Velocidade do módulo Wi-Fi
  
  meuMotor.setSpeed(12); // Força do motor

  Serial.println("--- INICIANDO SISTEMA ---");
  Serial.println("Ligando a antena Wi-Fi...");

  // Envia os comandos (chamados de comandos AT) para conectar na rede
  enviarComandoWiFi("AT+RST", 2000); // Reinicia a peça
  enviarComandoWiFi("AT+CWMODE=1", 1000); // Prepara para conectar
  
  String comandoConexao = "AT+CWJAP=\"" + nomeDaRede + "\",\"" + senhaDaRede + "\"";
  Serial.print("Tentando conectar no Wi-Fi: ");
  Serial.println(nomeDaRede);
  
  // Espera 7 segundos tentando conectar
  enviarComandoWiFi(comandoConexao, 7000); 

  Serial.println("\n--- TUDO PRONTO! ---");
  Serial.println("Elevador no Terreo (0). Digite o andar destino (0, 1, 2 ou 3):");
}

void loop() {
  // Mostra qualquer erro ou mensagem do Wi-Fi na tela
  if (moduloWiFi.available()) {
    Serial.write(moduloWiFi.read());
  }

  // Lógica de subir e descer o elevador
  if (Serial.available() > 0) {
    int andarDestino = Serial.parseInt();
    
    while(Serial.available() > 0) {
      Serial.read(); // Limpa a memória
    }

    if (andarDestino >= 0 && andarDestino <= 3) {
       int andaresParaMover = andarDestino - andarAtual;
       int passosTotais = andaresParaMover * passosPorAndar;

       Serial.print(">>> Movendo para o andar ");
       Serial.println(andarDestino);

       meuMotor.step(passosTotais); 
       andarAtual = andarDestino;
       
       Serial.println("CHEGOU! Digite o proximo andar:");
       
       // NO FUTURO: Aqui nós vamos colocar o código para 
       // avisar a sua API na internet que o elevador chegou!
       
    } else {
       // Se digitar letra ou número errado, ele avisa
    }
  }
}