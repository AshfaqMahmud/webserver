#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>

const char* ssid     = "STARK_Gigabit";
const char* password = "ashfaq1234!!!!";

ESP8266WebServer server(80);

// Infer MIME type
String getContentType(String path) {
  if (path.endsWith(".html")) return "text/html";
  if (path.endsWith(".css")) return "text/css";
  if (path.endsWith(".js")) return "application/javascript";
  if (path.endsWith(".png")) return "image/png";
  if (path.endsWith(".jpg") || path.endsWith(".jpeg")) return "image/jpeg";
  if (path.endsWith(".ico")) return "image/x-icon";
  if (path.endsWith(".gif")) return "image/gif";
  return "text/plain";
}

void listLittleFS() {
  Serial.println("----- LittleFS file list -----");
  Dir dir = LittleFS.openDir("/");
  while (dir.next()) {
    Serial.print("FILE: ");
    Serial.print(dir.fileName());
    Serial.print(" | SIZE: ");
    Serial.println(dir.fileSize());
  }
  Serial.println("------------------------------");
}

void handleFileRequest() {
  String path = server.uri();
  Serial.println();
  Serial.println("=== Incoming request ===");
  Serial.print("URI: ");
  Serial.println(path);

  if (path.endsWith("/")) path += "index.html";

  Serial.print("Mapped path: ");
  Serial.println(path);

  // Does the file exist?
  bool exists = LittleFS.exists(path);
  Serial.print("LittleFS.exists(): ");
  Serial.println(exists ? "YES" : "NO");

  if (!exists) {
    Serial.println("File does NOT exist → sending 404");
    server.send(404, "text/plain", "404 Not Found");
    return;
  }

  File file = LittleFS.open(path, "r");

  if (!file) {
    Serial.println("ERROR: File exists but failed to open!");
    server.send(500, "text/plain", "500 Internal Error");
    return;
  }

  String contentType = getContentType(path);
  Serial.print("Content-Type: ");
  Serial.println(contentType);

  Serial.println("Streaming file to client...");
  server.streamFile(file, contentType);
  file.close();

  Serial.println("Done.");
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("Mounting LittleFS...");

  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount FAILED!");
    return;
  }
  Serial.println("LittleFS mount OK");

  listLittleFS();  // Print directory contents

  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }

  Serial.println();
  Serial.print("Connected. IP address: ");
  Serial.println(WiFi.localIP());

  server.onNotFound(handleFileRequest);
  server.begin();

  Serial.println("HTTP server started.");
}

void loop() {
  server.handleClient();
}
