#define BLYNK_TEMPLATE_ID "TMPLTijQwpoW"
#define BLYNK_TEMPLATE_NAME "Air Monitoring System"
#define BLYNK_AUTH_TOKEN "vjGxEroYOh3Mr30-YCOIiGMx6FtpVGz-"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte degree_symbol[8] = {
  0b00111,
  0b00101,
  0b00111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "OnePlus 9R";
char pass[] = "umar1234";

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensor() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  float g = analogRead(A0);

  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (isnan(g)) {
    Serial.println("Failed to read from Gas sensor!");
    return;
  }

  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V2, g);

  Serial.print("Temperature : ");
  Serial.println(t);
  Serial.print("Humidity : ");
  Serial.println(h);
  Serial.print("Gas Value: ");
  Serial.println(g);
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(1000L, sendSensor);

  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Air Quality");
  lcd.setCursor(3, 1);
  lcd.print("Monitoring");
  delay(1000);
  lcd.clear();
}

void loop() {
  Blynk.run();
  timer.run();

  float t = dht.readTemperature();
  float h = dht.readHumidity();
  float g = analogRead(A0);

  lcd.setCursor(0, 0);
  lcd.print("Temperature ");
  lcd.setCursor(0, 1);
  lcd.print(t);
  lcd.setCursor(6, 1);
  lcd.write(1);
  lcd.createChar(1, degree_symbol);
  lcd.setCursor(7, 1);
  lcd.print("C");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humidity ");
  lcd.print(h);
  lcd.print("%");
  delay(1000);

  if (g <= 300) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Gas Value: ");
    lcd.print(g);
    lcd.setCursor(0, 1);
    lcd.print("Fresh Air");
    Serial.println("Fresh Air");
    delay(1000);
    lcd.clear();
  } else if (g > 300) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Gas Value: ");
    lcd.print(g);
    lcd.setCursor(0, 1);
    lcd.print("Bad Air");
    Serial.println("Bad Air");
    delay(1000);
    lcd.clear();
  }

  if (g > 300) {
    Blynk.logEvent("pollution_alert", "Bad Air");
  }
}