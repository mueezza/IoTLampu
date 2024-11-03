#include <WiFi.h>
#include <WiFiClient.h>
//String untuk command
#include <String.h>

//mendeklarasikan lampu dengan Pin
#define Lampu1 5
#define Lampu2 6
#define Lampu3  7
//sett untuk Wifi Server
const char* ssid = "IOT Lampu";
const char* password = "12345678";
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password); 
  server.begin();
  Serial.println("Access Point started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());  //  

  pinMode(Lampu1, OUTPUT);
  pinMode(Lampu2, OUTPUT);
  pinMode(Lampu3, OUTPUT);

  digitalWrite(Lampu1, LOW);
  digitalWrite(Lampu2, LOW);
  digitalWrite(Lampu3, LOW);

}

void loop() {

  String all_command = "";

  WiFiClient client = server.available();

  if (client) {
    String request = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;
        if (c == '\r') {
          // End of line reached, periksa apakah karakter berikutnya adalah baris baru

          Serial.println(request);  // baris perintah HTTP lengkap termasuk GET dan HTTP 1

          // Extract command dari request string
          int start = request.indexOf("GET /") + 5;
          int end = request.indexOf("HTTP/");
          String command = request.substring(start, end);

          //Purify atau membersihkan command
          command.replace("\n", "");
          command.replace("\r", "");
          command.replace(" ", ""); // menghapus semua karakter spasi
          command.replace("\t", ""); // menghapus semua karakter tab
          command.trim();

         

Serial.println(command);

          all_command =  command + " is on";  // Lampu2 on 

          if (command.equals("lampu1")) {
            digitalWrite(Lampu1, HIGH);
            digitalWrite(Lampu2, LOW);
            digitalWrite(Lampu3, LOW);

          }

          if (command.equals("Lampu2")) {
            digitalWrite(Lampu2, HIGH);
            digitalWrite(Lampu1, LOW);
            digitalWrite(Lampu3, LOW);

          }

          if (command.equals("Lampu3")) {
            digitalWrite(Lampu3, HIGH);
            digitalWrite(Lampu1, LOW);
            digitalWrite(Lampu2, LOW);
          }


          if (client.peek() == '\n') {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            String commandWithTags = "<html><body>" + all_command + "</body></html>";
            client.println(commandWithTags);
            break;

          }
        }
      }
    }
  }
}
