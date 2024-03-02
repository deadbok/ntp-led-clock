NTPServer: "dk.pool.ntp.org"
timeZone: "CET-1CEST,M3.5.0,M10.5.0/3")
time: "00:00:00"
brightness: 4
SSID: "Ir6pGk2ccdqloaCABJ8pV7kkh7UTkkIi"
hostname: "clock1947093"

// Stream& input;

StaticJsonDocument<384> doc;

DeserializationError error = deserializeJson(doc, input);

if (error) {
  Serial.print(F("deserializeJson() failed: "));
  Serial.println(error.f_str());
  return;
}

const char* NTPServer = doc["NTPServer"]; // "dk.pool.ntp.org"
const char* timeZone = doc["timeZone"]; // "CET-1CEST,M3.5.0,M10.5.0/3"
const char* time = doc["time"]; // "00:00:00"
int brightness = doc["brightness"]; // 4
const char* SSID = doc["SSID"]; // "Ir6pGk2ccdqloaCABJ8pV7kkh7UTkkIi"
const char* hostname = doc["hostname"]; // "clock1947093"
