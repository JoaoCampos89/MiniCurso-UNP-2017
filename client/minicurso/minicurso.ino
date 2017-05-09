/**
 *  Mini curso Iot
 *  Conectar o ESP 12 E a  um broker Mqtt e receber
 *  e monitorar os dados no  browser
 *  Material necessário:
 *  - 1 Esp8266 12 E
 *  - 1 LDR
 *   - 3 Resistor 1 kOhm
 *   -
 *
 * Author: João Campos
 */
// ETAPA_1.0:

// biblioteca do Arduino
  #include <Arduino.h>
// biblioteca do Esp8266WiFi
  #include <ESP8266WiFi.h> 
//  biblioteca do protocolo mqtt
  #include <PubSubClient.h>
//  biblioteca do sensor 
 

// ETAPA_2.0:

// refere-se ao cliente 
  WiFiClient espClient;   
 // esta função publica de sub escreve o ciente 
  PubSubClient client(espClient);  
 // Ip do computador aonde está alojado o broker mqtt 
  IPAddress server(192, 168, 0, 102); 
 //  pinos do Arduino conectado ao sensor ultrassonico 
   
 //ETAPA_2.1
 // função que será chamada sempre que uma mensagem mqtt é recebida
void callback(char*, byte* , unsigned int );
// função que lê o valor analógico do sensor LDR
float getLdrRead();
// função responsável por reconectar o cliente a rede WiFi
void reconnect();
// nome da rede WiFi que irá se conectar (SSID)
char ssid[ ] = "MiniCurso" ;
// senha da rede WiFi que irá se conectar(password)
char password[ ] = " ";     
 // Estado do rádio WiFi    
int status = WL_IDLE_STATUS; 

 //ETAPA_2.2
 
 // Coloque o nome da sua equipe
 char EQUIPE[ ] = "EQUIPE1";
 // pino_2  do NodeMcu ou Witty
 int LED = 2;  // 
 unsigned long timerLast = 0;
// vetor message
char msg[100];
 using namespace std;

// ETAPA_3.0
void setup() 
 {
   
// velocidade de comunicação serial 
   Serial.begin(115200);  
  // Iniciar a comunicação com o ESP
  WiFi.mode(WIFI_STA); 
  // Conecte-se à rede WPA / WPA2
  WiFi.begin(ssid, password);
  // tentativa de conectar à rede de Wifi
  while (WiFi.status() != WL_CONNECTED) {
 // Aguarde 0,5 segundos para conexão
   delay(500);
// mostrar na porta serial
   Serial.print(".");    }

// ETAPA_3.1

  // mostrar a string na porta serial
  Serial.println(" ");
  // mostrar  a string WiFi connected na porta serial
  Serial.println( "WiFi connected" );
  // mostrar  a string IP address na porta serial
  Serial.println( "IP address: " );
  // mostrar o IP da rede local  na porta serial
  Serial.println(WiFi.localIP());
  //mostrar na porta serial que está conectado
  Serial.println("Você está conectado à rede!");
  // conectando ao servidor, utilizando a porta 1883 com protocolo mqtt
  client.setServer(server, 1883);
  // Recebe os dados e os trata utilizando o protocolo mqtt
  client.setCallback(callback);

}//end

// ETAPA_4.0
void loop(){
// utilizando a função millis()
  long timer = millis();

   // manda mensagem a cada três segundos 
     if (timer-timerLast>3000) {
       timerLast = timer;
       // se cliente conectado, manda a mensagem de leitura do ldr
         if (client.connected())  {
          Serial.println("Publishing data");
         float ldrRead = getLdrRead() ;

// ETAPA_4.1


    String LdrMsg = String(ldrRead);
  
     String publication = String(EQUIPE)+String("/LDR/ESTADO/GET");
    // mostra no monitor serial a publicação
      Serial.println(publication);     
       // Publicação do cliente 
       client.publish(publication.c_str(), LdrMsg.c_str());
        }  
  }
    // caso cliente não conectado implementa a função reconnect    
    if (!client.connected()) {
        reconnect();      }
    // loop no cliente
    client.loop();   
  
 }//end
 
//ETAPA_5.0
float getLdrRead() {
  return analogRead(A0);
}

//ETAPA_6.0

void reconnect() {
    // testa se está conectado 
   while (!client.connected()) {
     // monitor serial mostra broker Mqtt  
      Serial.print("Ligando ao Broker Mqtt");
      // conecta a ssid do cliente 
      if (client.connect("minicurso")) {
        Serial.println("Ligado");
        // subscrever os dados que necessitous
        String subscription = String(EQUIPE) +String("/LED/ESTADO/SET");
        client.subscribe(subscription.c_str());
}
// ETAPA_6.1
else {
      // mostrar no monitor serial
      Serial.print("Falhou, rc=");
      Serial.print(client.state());
      Serial.println(" Tente outra vez em 5 segundos");
      // Esperar 5 segundos para voltar a conectar
      delay(5000);
    }
  }
}

// ETAPA_7.0

void callback(char* topic, byte* payload, unsigned int length) {

    Serial.print("Mensagem chegou [");
    Serial.print(topic);
    Serial.print("] ");
   // armazena o vetor constituído por caracteres
    char payloadMsg[length+1];
    for (int i = 0; i < length; i++) {
    payloadMsg[i] = (char)payload[i];
  }
  // o vetor recebe o valor em caracteres (tabela ASCII)
  payloadMsg[length] = '\0';
  String topicArrived = String(topic);

//ETAPA_7.1

// O tópico recebido corresponde aquele que nós estamos esperando
  String topicString =  String(EQUIPE) + String("/LED/ESTADO/SET");
   // testa a igualdade dos caracteres
  if (topicArrived == topicString) {
     String msg = String(payloadMsg);
    // mostrar no monitor serial o estado do LED valor inteiro
    Serial.print("Mudando Estado do Led");
    // mostra o  valor inteiro da mensagem tratada pelo callback       
    Serial.print(msg.toInt());
    // escreve o valor do Led
    digitalWrite(LED, msg.toInt());
    // Publicar que o estado do LED mudou
    String publication = String(EQUIPE) + String("/LED/ESTADO/GET");
    client.publish(publication.c_str(), msg.c_str());
  }
}





