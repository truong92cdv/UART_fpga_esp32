#include <WiFi.h> // For ESP32
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

// Replace with your Wi-Fi credentials and ThingsBoard token
const char* ssid = "Nhat Truong";
const char* password = "vonhattruong";
const char* token = "2h6530dey1xs5tbejqrs";

const int ledPin = 2; // Define the LED pin (D4)
const int buttonPin = 0; // Define the button pin (D3)

EspSoftwareSerial::UART mySerial;

WiFiClient espClient;
PubSubClient client(espClient);
char thingsboardServer[] = "demo.thingsboard.io";

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);
    Serial.begin(9600);
    mySerial.begin(9600, EspSoftwareSerial::SWSERIAL_8O1, 13, 15); // RX: GPIO4(D7), TX: GPIO5(D8) - 8 data bits, odd parity, 1 stop bit
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
    client.setServer(thingsboardServer, 1883);
    client.setCallback(callback);
    reconnect();
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    if (mySerial.available() > 0) {
        // Read the incoming byte
        char incomingByte = mySerial.read();
        // Print the received byte as ASCII character
        Serial.print("Receice: ");
        Serial.println(incomingByte);
        
        // Check if the incoming byte is 0x55
        if (incomingByte == 0x55) {
            // Toggle the LED on pin 2 and send the status to ThingsBoard
            sendLedStatus(digitalRead(ledPin) == HIGH);
            digitalWrite(ledPin, !digitalRead(ledPin));
        }
    }

    if (digitalRead(buttonPin) == LOW) {
        mySerial.write(0x55);
        Serial.println("Transmit: U ");
        delay(1000); // Debounce delay
    }
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Connecting to ThingsBoard...");
        if (client.connect("ESP32_Client", token, NULL)) {
            Serial.println("connected");
            client.subscribe("v1/devices/me/rpc/request/+");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, payload, length);
    const char* method = doc["method"];
    if (strcmp(method, "setLight1") == 0) {
        bool ledState = doc["params"];
        mySerial.write(0x55);
        Serial.println("Transmit: U ");
    }
}

void sendLedStatus(bool state) {
    StaticJsonDocument<200> doc;
    doc["value1"] = state;
    char buffer[256];
    size_t n = serializeJson(doc, buffer);
    client.publish("v1/devices/me/attributes", buffer, n);
}
