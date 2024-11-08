#include <WiFi.h>

#define PIN_SWITCH 19
#define PIN_SWITCH_F1 18
#define PIN_SWITCH_F2 5

#define PIN_BUZZER 23

bool sudahPutus[3];

const char *ssid = "dansat";          // Wi-Fi SSID
const char *password = "77040109";    // Wi-Fi password
const char *serverIP = "192.168.4.1"; // The IP address of the ESP32 server
const int serverPort = 80;            // The port of the ESP32 server

WiFiClient client;

void buzzerRun()
{
  digitalWrite(PIN_BUZZER, LOW);
  delay(2000);
  digitalWrite(PIN_BUZZER, HIGH);
}
void setup()
{
  Serial.begin(115200); // Start Serial communication
  delay(1000);          // Wait for the serial monitor to open

  pinMode(PIN_SWITCH, INPUT_PULLUP);
  pinMode(PIN_SWITCH_F1, INPUT_PULLUP);
  pinMode(PIN_SWITCH_F2, INPUT_PULLUP);

  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, HIGH);

  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); // Print the IP address of the client (ESP32)

  sudahPutus[0] = false;
  sudahPutus[1] = false;
  sudahPutus[2] = false;

  delay(2000);
  Serial.println("READY..........");
  buzzerRun();
}

void loop()
{
  // Check if the serial monitor has input
  if (Serial.available() > 0)
  {
    String command = Serial.readStringUntil('\n'); // Read command from serial
    command.trim();                                // Remove any trailing spaces or newline characters

    if (command == "BUKA" || command == "TUTUP")
    {
      if (client.connect(serverIP, serverPort))
      {
        // Send the command to the server
        client.println(command);
        Serial.println("Sent command: " + command);

        // Wait for a response from the server
        String response = client.readStringUntil('\n');
        Serial.println("Response from server: " + response);

        // Close the connection
        client.stop();
      }
      else
      {
        Serial.println("Failed to connect to the server.");
      }
    }
    else
    {
      Serial.println("Invalid command. Use 'BUKA' to open or 'TUTUP' to close.");
    }
  }

  if (digitalRead(PIN_SWITCH) == HIGH && !sudahPutus[0])
  {
    if (client.connect(serverIP, serverPort))
    {
      client.println("BUKA");
      Serial.println("Sent command: BUKA");
      String response = client.readStringUntil('\n');
      Serial.println("Response from server: " + response);
      client.stop();
    }

    Serial.println("==========");
    Serial.println(sudahPutus[0]);
    Serial.println("kabel 1 benar terlah putus, data dikirim.....");
    sudahPutus[0] = true;
  }

  if (digitalRead(PIN_SWITCH_F1) == HIGH && !sudahPutus[1])
  {
    Serial.println("==========");
    Serial.println(sudahPutus[1]);
    Serial.println("kabel 2 salah terlah putus.....");
    sudahPutus[1] = true;

    buzzerRun();
  }
  if (digitalRead(PIN_SWITCH_F2) == HIGH && !sudahPutus[2])
  {
    Serial.println("==========");
    Serial.println(sudahPutus[2]);
    Serial.println("kabel 3 salah terlah putus.....");
    sudahPutus[2] = true;

    buzzerRun();
  }
}
