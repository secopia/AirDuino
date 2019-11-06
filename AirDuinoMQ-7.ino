#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define led_pin D0

String ID = " -MQ7";

WiFiServer server(80);

bool res_status = false;

//Network data to connect arduino to Node.js

const char* ssid = "";                    //Wifi SSID
const char* password = "";                //Wifi password

const char* nodejs_ip = "";               //Node.js server IP
const int nodejs_port = 8000;             //Node.js server PORT


//This function is to make a GET request to send data to Node.js
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
      Serial.println("Data updated correctly");                   //Print HTTP return code
      http.end();

      if(httpCode == 200){
        return true;
      }else{
        return false;
      }
      
      
    } else {
      Serial.println("connection failed - Nodejs");
      blink();
      return false;
    }

  }

}


bool read_mq7(WiFiClient cl){

  int mq7_adc = analogRead(A0);                                         //Read the analog output of the MQ
  float mq7_voltaje = mq7_adc * (5.0 / 1023.0);                         //Convert the reading into a voltage value
  float mq7_resistencia = 1000*((5-mq7_voltaje)/mq7_voltaje);           //Calculate Rs with an RL of 1k
  double CO = 233.9*pow(mq7_resistencia/5463, -1.40);                   //Calculate the concentration of CO
  String mq7_umbral = "false";

  res_status = send_get(cl, String("saveDataMQ7?ID=") + ID + "&CO=" + CO);

  return res_status;
}


void blink() {
  for (int i = 0; i < 10; ++i)
  {
    digitalWrite(led_pin, HIGH);
    delay(250);
    digitalWrite(led_pin, LOW);
    delay(250);
  }
  digitalWrite(led_pin, LOW);
}

void setup() {
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
  Serial.begin(115200);
  
  // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(led_pin, HIGH);
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  digitalWrite(led_pin, LOW);
 
  // Start the server
  server.begin();
  Serial.println("Server started at...");
  Serial.println(WiFi.localIP());
}
 
void loop() {

  // Check if a client has connected
  WiFiClient client = server.available();

  Serial.println("Reading data!");
  res_status = read_mq7(client);

  //Set delay
  delay(150000);
}
