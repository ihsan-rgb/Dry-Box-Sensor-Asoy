#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 3 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

#define LED_1 18 
#define LED_2 5
#define LED_3 17
#define LED_4 16

float h = 0;
float t = 0;

void setup() {

  Serial.begin(115200);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  dht.begin();

  Serial.print("Humidity");
  Serial.print("(%) ");
  Serial.print("Temperature ");
  Serial.print("(C) ");

}

void loop() {

  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();
  
  bool t_status = (t >= 70);
  bool h_status = (h >= 55);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  displayOled();

  digitalWrite(LED_1, h_status);
  digitalWrite(LED_2, !h_status);
  digitalWrite(LED_3, t_status);
  digitalWrite(LED_4, !t_status);

  Serial.println();
  Serial.print(h);
  Serial.print(" ");
  Serial.print(t);
  Serial.print(" ");

}

void displayOled() {

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(0.5);
  display.setCursor(5, 8);
  display.print("Humidity:");
  display.setCursor(80, 8);
  display.print(h);
  display.print("%");
  display.setCursor(5, 20);
  display.print("Temperature:");
  display.setCursor(80, 20);
  display.print(t);
  display.print((char)247); // degree symbol
  display.print("C");
  display.display();

}