#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

WiFiServer server(80);

bool res_status = false;

const char* ssid = "";                    //Wifi SSID
const char* password = "";                //Wifi password

const char* nodejs_ip = "";               //Node.js server IP
const int nodejs_port = 8000;             //Node.js server PORT

bool send_get(WiFiClient cl, String msq_GET) {

  if (msq_GET != "") {
    String ip = "";

    if (cl.connect(nodejs_ip, nodejs_port)) {

      Serial.println("connected to Nodejs");

      cl.println("GET /" + msq_GET + " HTTP/1.1");
      cl.println("Host: " + (String)nodejs_ip);
      cl.println("Connection: close");
      cl.println();
      String req = cl.readStringUntil('\r');
      cl.flush();
      Serial.println(req);

      HTTPClient http;
      http.begin("http://" + (String)nodejs_ip + "/" + (String)msq_GET);
      http.setAuthorization("user", "password");
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      http.POST("title=foo&body=bar&userId=1");
      http.addHeader("Content-Type", "text/plain");
      int httpCode = http.GET();                  //Send the request
      String payload = http.getString();          //Get the response payload
      Serial.println(httpCode);                   //Print HTTP return code
      Serial.println(payload);                    //Print request response payload
      http.end();

      if (httpCode == 200) {
        return true;
      } else {
        return false;
      }


    } else {
      Serial.println("connection failed - Nodejs");
      return false;
    }

  }

}


bool read_mq135(WiFiClient cl) {

  int mq135_adc = analogRead(A0);                                                //Read the analog output of the MQ
  float mq135_voltaje = mq135_adc * (5.0 / 1023.0);                              //Convert the reading into a voltage value
  float mq135_resistencia = 1000 * ((5 - mq135_voltaje) / mq135_voltaje);        //Calculate Rs with an RL of 1k
  double CO2 = 245 * pow(mq135_resistencia / 5463, -2.26);                       //Calculate the concentration of co2
  double NO = 132.6 * pow(mq135_resistencia / 5463, -2.74);                      //Calculate the concentration of NO
  double NH3 = 161.7 * pow(mq135_resistencia / 5463, -2.26);                     //Calculate the concentration of NH3
  String mq135_umbral = "false";

  res_status = send_get(cl, String("saveData?CO2=") + CO2 + "&NH3=" + NH3 + "&NO=" + NO);

  return res_status;
}

void setup() {
  Serial.begin(115200);
  
  // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started at...");
  Serial.println(WiFi.localIP());
}

void loop() {

  // Check if a client has connected
  WiFiClient client = server.available();

  Serial.println("Reading data!");
  res_status = read_mq135(client);

  //Set delay
  delay(150000);
}
