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

#include <ESP8266WiFi.h>
#include <PubSubClient.h>


WiFiClient espClient;

PubSubClient client(espClient);
//
// Insira aqui o ip do computador aonde está alojado o broker mqtt
IPAddress server(192, 168, 0, 102);


// Usar com o witty
#define RED_PIN      7   // RGB Light pin at Arduino pin 3
#define GREEN_PIN    8   // RGB Light pin at Arduino pin 5
#define BLUE_PIN     9   // RGB Light pin at Arduino pin 6
#define BUTTON_PIN   4   // button
#define LDR_PIN      A0


// função que será chamada sempre que uma mensagem mqtt é recebida
void callback(char*, byte* , unsigned int );
void reconnect();
// dados do roteador
char ssid[] = "MiniCurso";           // your network SSID (name)
char password[] = "";           // your network password
int status = WL_IDLE_STATUS;   // the Wifi radio's status

// Coloque o nome da sua equipe
char EQUIPE[] = "EQUIPE1";

int LED = 2;


unsigned long timerLast = 0;
char msg[100];
using namespace std;


void setup(){

  //pinMode(RED_PIN, OUTPUT);

  Serial.begin(115200);
  // Iniciar a comunicação com o esp
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  Serial.println("Você está conectado à rede!");

  // conectando ao servidor mqtt
  client.setServer(server, 1883);
  client.setCallback(callback);



}

void loop(){
  long timer = millis();
  double distancia;


// manda mensagem a cada um segundo da distancia
 if(timer-timerLast>3000){
    timerLast = timer;
    // se cliente conectado
    // mandar mensagem da distancia
    if(client.connected()){
      Serial.println("Publishing data");
      float ldrRead =  analogRead(LDR_PIN);
      // String distanceMsg = String(getDistancia());
      String distanceMsg = String(ldrRead);
      String publication = String(EQUIPE) + String("/LDR/ESTADO/GET");
      Serial.println(publication);
      client.publish(publication.c_str(), distanceMsg.c_str());
    }

 }

 if (!client.connected()) {
   // implementar função reconnect
   reconnect();
 }
 client.loop();



}
/**
 * [getDistancia retorna o valor de distância medida]
 * @return [description]]
 * TODO Implementar lógica do sensor de distância
 */
double getDistancia(){
  return 10;

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
      String subscription = String(EQUIPE) + String("/LED/ESTADO/SET");
      client.subscribe(subscription.c_str());
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

  char payloadMsg[length+1];
  for (int i = 0; i < length; i++) {
   payloadMsg[i] = (char)payload[i];
  }
  payloadMsg[length] = '\0';
  String topicArrived = String(topic);

 // O tópico recebido corresponde aquele que nós estamos esperando
  String topicString =  String(EQUIPE) + String("/LED/ESTADO/SET");
 
  if(topicArrived == topicString){
     String msg = String(payloadMsg);
    //
    Serial.print("Mudando Estado do Led");
    Serial.print(msg.toInt());
    digitalWrite(RED_PIN, msg.toInt());
    // Publicar que o estado do LED mudou

    String publication = String(EQUIPE) + String("/LED/ESTADO/GET");
    client.publish(publication.c_str(), msg.c_str());
  }



}
