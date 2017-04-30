/**
 *  Mini curso Iot
 *  Conectar o Arduino a  um broker Mqtt e receber
 *  e monitorar os dados no seu browser
 *  Material necessário:
 *  - 1 Arduino Uno
 *  - 1 Esp-01
 *  - HC-SR 01
 *   - 3 Resistor 1 kOhm
 *   -
 *
 * Author: João Campos
 */

#include <Arduino.h>
//#include <Time.h>
#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include "SoftwareSerial.h"
#include <PubSubClient.h>


WiFiEspClient espClient;

PubSubClient client(espClient);
//
// Insira aqui o ip do computador aonde está alojado o broker mqtt
IPAddress server(10, 163, 99, 190);
// Insira os pinos RX e TX do ESP 01 que definiu aqui nos pinos 11 e 10 respectiviamente
SoftwareSerial soft(10, 11); // RX, TX

// função que será chamada sempre que uma mensagem mqtt é recebida
void callback(char*, byte* , unsigned int );
void reconnect();
// dados do roteador
char ssid[] = "TP-LINK_CBC0F0";           // your network SSID (name)
char pass[] = "";           // your network password
int status = WL_IDLE_STATUS;   // the Wifi radio's status




unsigned long timerLast = 0;
char msg[100];
using namespace std;


void setup(){



  Serial.begin(115200);
  // Iniciar a comunicação com o esp
  soft.begin(9600);
  // Oassar a instância serial do arduino para o wifi
  WiFi.init(&soft);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("Shield Wifi não está presente");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Ligando ao Wifi");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }


  Serial.println("Você está conectado à rede!");

  // conectando ao servidor mqtt
  client.setServer(server, 1883);
  client.setCallback(callback);



}

void loop(){
  long timer = millis();
  double distancia;


// manda mensagem a cada um segundo da distancia
 if(timer-timerLast>1000){
    timerLast = timer;
    // se cliente conectado
    // mandar mensagem da distancia
    if(client.connected()){
      String distanceMsg = String(distancia);
      client.publish("MINICURSO/LED/ESTADO/GET", distanceMsg.c_str());
    }

 }

 if (!client.connected()) {
   // implementar função reconnect
   reconnect();
 }
 client.loop();



}

double getDistance(){


}





void reconnect() {
  // Entra no loop até conectar
  while (!client.connected()) {
    Serial.print("Ligando ao Broker Mqtt");
    // Attempt to connect, just a name to identify the client
    if (client.connect("minicurso")) {
      Serial.println("Ligado");
      //
      // subscrever aos dados que necessitamos
      // Nós subscrevemos ao valor
      client.subscribe("MINICURSO/LED/ESTADO/SET");
    } else {
      Serial.print("Falhou, rc=");
      Serial.print(client.state());
      Serial.println(" Tente outra vez em 5 segundos");
      // Esperar 5 segundos para voltar a conectar
      delay(5000);
    }
  }
}




// função que é chamada quando existe uma mensagem com o tópico que subscrevemos
// topic --> tópicos que foram subscritos
// payload -->
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem chegou [");
  Serial.print(topic);
  Serial.print("] ");

  switch (topic) {
    case "MINICURSO/LED/ESTADO/SET":
    String msg = String(payload);
    //
    DigitalWrite(LED, msg.toInt());
    // Publicar que o estado do LED mudou
    client.publish("MINICURSO/LED/ESTADO/GET", msg.c_str());

  }

}
