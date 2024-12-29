#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <IRremote.h>
#include <Ticker.h>
#include "secrets/wifi.h"
#include "secrets/mqtt.h"
#include "ca_cert.h"

const char *ssid = WiFiSecrets::ssid;
const char *password = WiFiSecrets::pass;
const char *broker = MQTT::broker;
const int port = MQTT::port;
const char *username = MQTT::username;
const char *mqtt_password = MQTT::password;
const char *pir_topic = "esp32/pir";
const char *air_topic = "esp32/air_quality";
const char *soil_topic = "esp32/soil_moisture";
const char *dht_topic = "esp32/dht";
const char *ir_topic = "esp32/led_control";
const char *mode_topic = "esp32/mode";
const char *relay_topic = "esp32/relay";
#define PIR_PIN 39
#define LED_PIR_PIN 23  
#define SOIL_PIN 33
#define MQ135_PIN 34    
#define DHT_PIN 32
#define DHT_TYPE DHT11
#define IR_RECV_PIN 22
#define IR_LED_PIN 5    
#define RELAY_PIN 4     
WiFiClientSecure tlsClient;
PubSubClient mqttClient(tlsClient);
DHT dht(DHT_PIN, DHT_TYPE);
IRrecv irrecv(IR_RECV_PIN);
decode_results results;

Ticker sensorTicker;
bool irLedState = LOW;
bool manualControl = false; 
void setup_wifi();
void mqttCallback(char *topic, byte *payload, unsigned int length);
void publishSensorData();
void handleIRCommand();
void mqttReconnect();
void mqttCallback(char *topic, byte *payload, unsigned int length) {
    String message = "";
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    if (String(topic) == ir_topic) {
        if (message == "ON") {
            digitalWrite(IR_LED_PIN, HIGH);
            irLedState = HIGH;
            Serial.println("Node-RED: LED ON");
        } else if (message == "OFF") {
            digitalWrite(IR_LED_PIN, LOW);
            irLedState = LOW;
            Serial.println("Node-RED: LED OFF");
        }
    } if (String(topic) == relay_topic) {
        if (message == "RELAY_ON") {
            manualControl = true;
            digitalWrite(RELAY_PIN, LOW);
        } else if (message == "RELAY_OFF") {
            manualControl = true;
            digitalWrite(RELAY_PIN, HIGH);
        }
    }
    if (String(topic) == mode_topic)
    {
        if (message == "MANUAL")
        {
            manualControl = true;
            Serial.println("Switched to MANUAL mode");
        }
        else if (message == "AUTO")
        {
            manualControl = false;
            Serial.println("Switched to AUTO mode");
        }
    }
}

void publishSensorData() {
    int motionDetected = digitalRead(PIR_PIN);
    mqttClient.publish(pir_topic, motionDetected == HIGH ? "1" : "0");
    Serial.print("PIR Motion: ");
    Serial.println(motionDetected == HIGH ? "Detected" : "No Motion");

    if (motionDetected == HIGH) {
        digitalWrite(LED_PIR_PIN, HIGH);  
                Serial.println("LED PIR ON");
    } else {
        digitalWrite(LED_PIR_PIN, LOW);  
        Serial.println("LED PIR OFF");
    }
    delay(100);


    int mq135Value = analogRead(MQ135_PIN); 
    float airQuality = map(mq135Value, 0, 4095, 0, 100); 
    String airData = String(airQuality, 2);
    mqttClient.publish(air_topic, airData.c_str());
    Serial.print("Air Quality (MQ-135): ");
    Serial.print(airQuality);
    Serial.println("%");

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    if (!isnan(humidity) && !isnan(temperature)) {
        String dhtData = "Humidity: " + String(humidity, 2) + "%, Temperature: " + String(temperature, 2) + "C";
        mqttClient.publish(dht_topic, dhtData.c_str());
        Serial.print("DHT11 - Humidity: ");
        Serial.print(humidity);
        Serial.print("%, Temperature: ");
        Serial.print(temperature);
        Serial.println("°C");
    } else {
        Serial.println("DHT11: Failed to read data");
    }


    int soilValue = analogRead(SOIL_PIN);
    float soilMoisture = (soilValue / 4095.0) * 100;


    if (!manualControl) {
        if (soilMoisture > 80.0) {
            digitalWrite(RELAY_PIN, LOW); 
            Serial.println("Relay ON ");
        } else {
            digitalWrite(RELAY_PIN, HIGH); 
            Serial.println("Relay OFF ");
        }
    }

    String soilData = String(soilMoisture, 2);
    mqttClient.publish(soil_topic, soilData.c_str());
    Serial.print("Soil Moisture: ");
    Serial.print(soilMoisture);
    Serial.println("%");

    Serial.println("----------------------------");
}

void handleIRCommand() {
    if (irrecv.decode(&results)) {
        unsigned long decCode = results.value;

        if (decCode == 16724175) {
            digitalWrite(IR_LED_PIN, HIGH);
            irLedState = HIGH;
            mqttClient.publish(ir_topic, "ON");
            Serial.println("IR Remote: LED ON");
        } else if (decCode == 16738455) {
            digitalWrite(IR_LED_PIN, LOW);
            irLedState = LOW;
            mqttClient.publish(ir_topic, "OFF");
            Serial.println("IR Remote: LED OFF");
        }

        Serial.print("IR Code: ");
        Serial.println(decCode, DEC); 
        irrecv.resume();
    }
}


void mqttReconnect() {
    while (!mqttClient.connected()) {
        Serial.print("Connecting to MQTT...");
        if (mqttClient.connect("esp32-client", username, mqtt_password)) {
            Serial.println("connected");
            mqttClient.subscribe(ir_topic);
            mqttClient.subscribe(mode_topic); 
            mqttClient.subscribe(relay_topic);
            Serial.println("Subscribed to topics:");
            Serial.println(ir_topic);
            Serial.println(relay_topic);
            Serial.println(mode_topic);
        } else {
            Serial.print("failed, rc=");
            Serial.println(mqttClient.state());
            delay(5000);
        }
    }
}


void setup() {
    Serial.begin(115200);

    pinMode(PIR_PIN, INPUT);
    pinMode(LED_PIR_PIN, OUTPUT);
    pinMode(IR_LED_PIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH);  

    setup_wifi();
    tlsClient.setCACert(ca_cert);
    mqttClient.setServer(broker, port);
    mqttClient.setCallback(mqttCallback);

    dht.begin();
    irrecv.enableIRIn();

    sensorTicker.attach(2, publishSensorData); 
}


void loop() {
    if (!mqttClient.connected()) {
        mqttReconnect();
    }
    mqttClient.loop();
    handleIRCommand();
}


void setup_wifi() {
    delay(10);
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
}
