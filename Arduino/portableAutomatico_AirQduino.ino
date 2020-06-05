// Import required libraries
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <SoftwareSerial.h>//incluimos SoftwareSerial
#include <TinyGPS.h>//incluimos TinyGPS

TinyGPS gps;//Declaramos el objeto gps
SoftwareSerial serialgps(D1,D8);//Declaramos el pin 4 Tx y 3 Rx

// Import lcd library
#include <LiquidCrystal.h>

const int rs = D2, en = D3, d4 = D4, d5 = D5, d6 = D6, d7 = D7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// WiFi parameters
const char* ssid = "MiCasa";
const char* password = "2507VeGaMoLiNa142317309_FYFJR.";

// const char* ssid = "Andared";
// const char* password = "llevalatararaunvestidoblancollenodecascabeles";

int btn = D0, v = 0, ov = 0;

//Declaramos la variables para la obtención de datos


String mqdata() {

  int mq135_adc = analogRead(A0);                                                //Read the analog output of the MQ
  float mq135_voltaje = mq135_adc * (5.0 / 1023.0);                              //Convert the reading into a voltage value
  float mq135_resistencia = 1000 * ((5 - mq135_voltaje) / mq135_voltaje);        //Calculate Rs with an RL of 1k
  double CO2 = 245 * pow(mq135_resistencia / 5463, -2.26);                       //Calculate the concentration of CO2
  double NO = 132.6 * pow(mq135_resistencia / 5463, -2.74);                      //Calculate the concentration of NO
  double NH3 = 161.7 * pow(mq135_resistencia / 5463, -2.26);                     //Calculate the concentration of NH3
  String mq135_umbral = "false";


  String datos = "CO2=" + String(CO2) + "&NH3=" + String(NH3) + "&NO=" + String(NO);
  return datos;
}

String datos() {

  // read mq135 sensor data
  int mq135_adc = analogRead(A0);                                                //Read the analog output of the MQ
  float mq135_voltaje = mq135_adc * (5.0 / 1023.0);                              //Convert the reading into a voltage value
  float mq135_resistencia = 1000 * ((5 - mq135_voltaje) / mq135_voltaje);        //Calculate Rs with an RL of 1k
  double CO2 = 245 * pow(mq135_resistencia / 5463, -2.26);                       //Calculate the concentration of CO2
  double NO = 132.6 * pow(mq135_resistencia / 5463, -2.74);                      //Calculate the concentration of NO
  String mq135_umbral = "false";

  String datos = "CO2=" + String(CO2) + " NO=" + String(NO);

  return datos;
}


String gpsdata(){
  bool newData = false;
  for (unsigned long start = millis(); millis() - start < 500;)
  {
    while (serialgps.available())
    {
      char c = serialgps.read();
      if (gps.encode(c))
        newData = true;
    }
  }

  if (newData)
  {
    float latitude, longitude;
    gps.f_get_position(&latitude, &longitude);
    String datosgps = "&LA=" + String(latitude,5) + "&LO=" + String(longitude,5);
    return datosgps;
  }  else {
    String datosgps = "&LA=x&LO=x";
    return datosgps;
  }
}


String gpsdatos(){
  bool newData = false;
  for (unsigned long start = millis(); millis() - start < 500;)
  {
    while (serialgps.available())
    {
      char c = serialgps.read();
      if (gps.encode(c))
        newData = true;
    }
  }

  if (newData)
  {
    float latitude, longitude;
    gps.f_get_position(&latitude, &longitude);
    String datosgps = "LA=" + String(latitude,2) + " LO=" + String(longitude,2);
    return datosgps;
  } 
}


String data() {
  String datos = mqdata() + gpsdata();
  return datos;
}



void sData(){

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://airqualitypdv.herokuapp.com/inserts/p?" + data());
    int httpCode = http.GET();
    Serial.println("Sending GET!");
    lcd.setCursor(0,1);
    lcd.clear();
    lcd.print("Sending GET!");
    delay(300);
    lcd.clear();

    if (httpCode > 0) { 
      String payload = http.getString();
      Serial.println(payload);
      lcd.setCursor(0,1);
      lcd.print(payload);
      delay(300);
      lcd.clear();
    } else {
      Serial.println("Error:");
      Serial.println(httpCode);
      lcd.setCursor(0,1);
      lcd.print(httpCode);
      delay(300);
      lcd.clear();
    }
    http.end();
  }
}


void setup() {
  Serial.begin(115200);
  serialgps.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Connecting WiFi:");
  lcd.setCursor(0,1);
  lcd.print(ssid);
  Serial.println();
  Serial.print("Connecting to WiFi network: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.print("Connected!");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connected!");
  delay(500);
  lcd.clear();
}


void loop() 
{
  lcd.setCursor(0,0);
  lcd.print(datos());
  lcd.setCursor(0,1);
  lcd.print(gpsdatos());
  v = digitalRead(btn);
  oout:
  if ((v == HIGH) && (ov == LOW)){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Recording...");
    v=0;
    while(true) {
      sData();
      lcd.setCursor(0,0);
      lcd.print("Recording...");
      v = digitalRead(btn);
      for (int i = 0; i < 120; ++i) {
        delay(500);
        v = digitalRead(btn);
        if ((v == HIGH) && (ov == LOW)){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("the recording ended");
          v=0;
          goto oout;
        }
      }
    }
    delay(100);
    lcd.clear();
  }
  ov = v;
  delay(500);
}
