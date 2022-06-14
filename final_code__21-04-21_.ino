#include <Wire.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <HTTPClient.h>
const int I2C_ERROR = 10;
const char* ssid     = "TECNO SPARK 5 Air";
const char* password = "jumaca45";

// no support for hhtps!
const char* serverName  = "http://machine.dbmsystem.com/post-data.php";
// Keep this API Key value to be compatible with the PHP code provided in the project page.
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key
String apiKeyValue = "tPmAT5Ab3j7F9";
boolean busActivity = false;
int error = 0;
int systolic = 0;
int diastolic = 0;
int hr = 0;
int EN = 2;
void setup() {
   // Enable ESP32 Chip 
   pinMode(EN,OUTPUT);
   digitalWrite(EN, HIGH);
   delay(20);
   digitalWrite(EN, LOW);
   delay(100); //stabalize the ESP32  
  // LED pin
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  delay(100); //add a small delay
  Serial.begin(115200);
  Wire.begin(21, 22); // initialise things
  delay(10); //add a small delay
  Serial.println("Wire initialized");

  // Connect to Wifi (on setup)
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Turn the LED on (active low) so user knows WiFi is up
  digitalWrite(4, LOW);

  // We just need one byte, but 4 is the minimum.
  EEPROM.begin(4);
}

void loop() {

  int readCount = 0;
  int ourCount = 0;
  int addr = 0x00;

  // Trigger on SDA activity, user might have stored a new measurement
  busActivity = false;
  attachInterrupt(21, busActivityDetected, CHANGE);

  Serial.println("Waiting for bus activity.");

  // Lets take a nap while we wait.
  while (busActivity == false) {
    yield();
  }

  // Ok, forget the interrupts for now, there's work to do!
  detachInterrupt(21);

  // Delay a bit so the monitor's MCU can finish talking
  delay(500);

  // Get current measurement count
  readCount = getMeasurementCount();

  // The user might have turned off the unit causing an interrupt
  if (error == I2C_ERROR) {
    Serial.println("Can't communicate with the chip.");
    return;
  }

  // Check count in our own eeprom, so we know if we have sent this data before
  ourCount = EEPROM.read(0x00);

  if (readCount != 0 && readCount != ourCount) {

    Serial.println("New measurement detected!");

    // Get last measurement
    addr = (readCount - 1) * 4; // address for last measurement
    Serial.print("Reading from 0x");
    Serial.println(addr, HEX);
    refreshLastMeasurements(addr);

    // Upload to data.sparkfun.com
    //uploadMeasurements();

    // Store updated count in eeprom
    EEPROM.write(0x00, readCount);
    EEPROM.commit();

    // Blink led so user knows everything is dandy
    blinkLED();

    // Print it for debugging
    Serial.print("Your last measurement is ");
    Serial.print(systolic, DEC);
    Serial.print(" mmHG X ");
    Serial.print(diastolic, DEC);
    Serial.print(" mmHG and ");
    Serial.print(hr, DEC);
    Serial.println(" bpm. Bye!");
    get_value(); //send data to database
  } else if (readCount == 0 && ourCount != 0) {

    // BPM's memory was probably erased by the user
    // So, update the count in EEPROM to keep it in sync
    EEPROM.write(0x00, 0);
    EEPROM.commit();
    Serial.println("Memory was reset - syncing EEPROM counter.");
    // Blink it
    blinkLED();

  } else {
    Serial.println("Hmm, I2C bus activity detected, but nothing to do.");
  }
}

void blinkLED() {

  for (int i = 0; i < 4; i++) {
    digitalWrite(4, HIGH);  // Turn the LED off
    delay(100);
    digitalWrite(4, LOW);   // Turn the LED on (active low)
    delay(100);
  }
}
void busActivityDetected() {

  busActivity = true;
}

int getMeasurementCount() {

  int addr = 0x00;
  byte rdata = 0x00;

  // Get current number of measurements at 0xFF
  Wire.beginTransmission(0x50);
  Wire.write(0xFF);
  Wire.endTransmission();
  Wire.requestFrom(0x50, 1);

  if (Wire.available()) {
    rdata = Wire.read();
    error = 0;
  } else {
    error = I2C_ERROR;
    // Wait a little for things to settle
    delay(300);
    return 0;
  }

  if (rdata == 0) {
    Serial.println("Nothing stored.");
    return 0;
  }

  return rdata;
}

void refreshLastMeasurements(int addr) {

  byte rdata = 0x00;

  // read measurement
  Wire.beginTransmission(0x50);
  Wire.write(addr);
  Wire.endTransmission();
  Wire.requestFrom(0x50, 4);

  if (Wire.available() != 4) {
    Serial.println("Less than 4 bytes found?");
    return;
  }

  hr = Wire.read();
  diastolic = Wire.read();
  rdata = Wire.read();
  systolic = Wire.read();
  // systolic is stored halved
  systolic *= 2;
  // if this byte is 0x80, add 1 to make it odd
  if (rdata == 0x80)
    systolic += 1;
}

void get_value() {
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(serverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(systolic)
                             + "&value2=" + String(diastolic) + "&value3=" + String(hr) + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
    
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  delay(100);
  exit(0);  



}
