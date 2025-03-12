#include <WiFi.h>
#include <PubSubClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "YourBlynkAuthToken";
char ssid[] = "YourWiFiSSID";
char pass[] = "YourWiFiPassword";

WiFiClient espClient;
PubSubClient client(espClient);

#define relayPin 5  // Pin connected to relay module

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Blynk.begin(auth, ssid, pass);  // Initialize Blynk app
  client.setServer("mqtt.eclipse.org", 1883);  // MQTT broker
  pinMode(relayPin, OUTPUT);  // Set relay pin as output
}

void loop() {
  Blynk.run();  // Run Blynk app
  if (!client.connected()) {
    reconnect();  // Reconnect to MQTT broker if disconnected
  }
  client.loop();
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      Serial.println("Connected to MQTT broker");
    } else {
      delay(5000);  // Wait for 5 seconds before retrying
    }
  }
}

// Virtual Pin (Blynk) to control light on/off
BLYNK_WRITE(V1) {
  int pinValue = param.asInt();  // Get value from Blynk app
  if (pinValue == 1) {
    digitalWrite(relayPin, HIGH);  // Turn on the light
  } else {
    digitalWrite(relayPin, LOW);  // Turn off the light
  }
}
