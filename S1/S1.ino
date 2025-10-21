#include <WiFi.h>
#include <PubSubClient.h>

const String SSID = "FIESC_IOT_EDU";
const String PSWD = "8120gv08";

const String brokerUrl = "test.mosquitto.org";  //URL do broker   (servidor)
const int port = 1883;                          //Porta do Broker (servidor)

WiFiClient espClient;                           //Criando Cliente WiFi
PubSubClient mqttClient(espClient);             //Criando Cliente MQTT

void connectToWifi();
void connectToBroker();

void setup() {
  Serial.begin(115200);
  connectToWifi();
  connectToBroker();
}

void loop() {

  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Conexão WiFi perdida!");
    connectToWifi();
  }
  if(!mqttClient.connected()){
    Serial.println("Conexão MQTT perdida!");
    connectToBroker();
  }
  if(Serial.available()>0){
    String msg = Serial.readStringUntil('\n');
    msg = "Joao: "+msg;
    mqttClient.publish("AulaIoTSul/Chat",msg.c_str()); //envia msgs
  }

  mqttClient.loop();
}


void connectToBroker(){
  Serial.println("Conectando ao broker...");
  mqttClient.setServer(brokerUrl.c_str(),port);
  String userId = "ESP-JOAO";
  userId += String(random(0xffff), HEX);
  while(!mqttClient.connected()){
    mqttClient.connect(userId.c_str());
    Serial.print(".");
    delay(2000);
  }
  mqttClient.subscribe("AulaIoT/msg");
  mqttClient.setCallback(callback);
  Serial.println("Conectado com sucesso!");
}

void callback(char* topic, byte* payload, unsigned long length){
  String resposta = "";
  for(int i = 0; i < length; i++){
    resposta += (char) payload[i];
  }
  Serial.println(resposta);
}

void connectToWifi(){
  Serial.println("Iniciando conexão com rede WiFi");
  while(WiFi.status() != WL_CONNECTED){
    WiFi.begin(SSID,PSWD);
    Serial.print(".");
    delay(2000);
  }
  Serial.println("\nConectado!");
}