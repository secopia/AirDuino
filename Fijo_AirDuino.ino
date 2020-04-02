// Import required libraries
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//For the SDS011 Particulate Matter sensor

#include <SoftwareSerial.h>
SoftwareSerial SDS_Serial(D1, D2);
unsigned int Pm25 = 0;
unsigned int Pm10 = 0;


// WiFi parameters
/const char* ssid = " ";                    //Wifi SSID
/const char* password = " ";            //Wifi password



void ProcessSerialSDSData() {
  uint8_t mData = 0;
  uint8_t i = 0;
  uint8_t mPkt[10] = {0};
  uint8_t mCheck = 0;
  while (SDS_Serial.available() > 0) { 
    mData = SDS_Serial.read();
    delay(2); //wait until packet is received
    if(mData == 0xAA) { //first headerbyte ok
      mPkt[0] =  mData;
      mData = SDS_Serial.read();
      if(mData == 0xC0) { //second headerbyte ok
        mPkt[1] =  mData;
        mCheck = 0;
        for(i=0;i<6;i++) { //data reception and crc calculation
           mPkt[i+2] = SDS_Serial.read();
           delay(2);
           mCheck += mPkt[i+2];
        }
        mPkt[8] = SDS_Serial.read(); //get crc
        delay(1);
        mPkt[9] = SDS_Serial.read();
        if(mCheck == mPkt[8]) { //crc ok?
          Pm25 = (uint16_t)mPkt[2] | (uint16_t)(mPkt[3]<<8);
          Pm10 = (uint16_t)mPkt[4] | (uint16_t)(mPkt[5]<<8);
          //one good packet received
          return;
        }
      }      
    }
  } 
}


String data() {

  // read mq135 sensor data
  int mq135_adc = analogRead(A0);                                                //Read the analog output of the MQ
  float mq135_voltaje = mq135_adc * (5.0 / 1023.0);                              //Convert the reading into a voltage value
  float mq135_resistencia = 1000 * ((5 - mq135_voltaje) / mq135_voltaje);        //Calculate Rs with an RL of 1k
  double CO2 = 245 * pow(mq135_resistencia / 5463, -2.26);                       //Calculate the concentration of CO2
  double NO = 132.6 * pow(mq135_resistencia / 5463, -2.74);                      //Calculate the concentration of NO
  double NH3 = 161.7 * pow(mq135_resistencia / 5463, -2.26);                     //Calculate the concentration of NH3
  String mq135_umbral = "false";

  //read pm10 & pm2.5
  SDS_Serial.listen();
  ProcessSerialSDSData();


  String datos = "CO2=" + String(CO2) + "&NH3=" + String(NH3) + "&NO=" + String(NO) + "&PM25=" + String(Pm25/10) + "&PM10=" + String(Pm10/10);

  return datos;
}


void setup() {
  SDS_Serial.begin(9600);
  Serial.begin(115200);
  
// Attempt to connect to WiFi
  Serial.println();
  Serial.print("Connecting to WiFi network: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;
    http.begin("http://" <-- Add here the url + data()); //Specify request destination
    int httpCode = http.GET();            //Send the request
    Serial.println("Sending GET!");
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.println("Error:");
      Serial.println(httpCode);
    }
    http.end();
  }
  delay(3600000); //Set delay (3600000ms = 1H)
}
