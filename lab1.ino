#define BLYNK_TEMPLATE_ID "TMPL6lgB8-NcG"
#define BLYNK_TEMPLATE_NAME "c1test"
#define BLYNK_AUTH_TOKEN "ayO873GzWkOIYCMbMdAOKmcdnluYHAmW"

#include <BlynkSimpleEsp8266.h>  // Thư viện Blynk cho ESP8266
#include <ESP8266WiFi.h>         // Thư viện WiFi cho ESP8266
#include <DHT.h>    //thu vien dth

// Thông tin mạng WiFi của bạn
char ssid[] = "Cct1";  // Thay thế bằng SSID WiFi của bạn
char pass[] = "11111111";  // Thay thế bằng mật khẩu WiFi của bạn

// Khai báo chân kết nối LED và Relay
int ledPin = D1;
int relayPin = D3;

//khai bao chan dht
#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define TEMP_VPIN V0
#define HUMIDITY_VPIN V3

//khai bao SRF05
long duration;
int distance;
#define echoPin D4
#define trigPin D5


void setup() {
  //led
  pinMode(ledPin, OUTPUT);
  //relay
  pinMode(relayPin, OUTPUT);
  //srf05
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //dth
  Serial.begin(115200);
  dht.begin();
  // Kết nối Wi-Fi và Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();  // Chạy Blynk để đồng bộ trạng thái với server
  delay(1); 
  //khoang cach
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  //do time phan hoi song sieu am
  duration = pulseIn(echoPin, HIGH);
  //ct tinh khoang cach
  distance = duration * 0.0344 / 2;
  //gui du lieu ben blynk
  Blynk.virtualWrite(V4, distance);



  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
    if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
   Blynk.virtualWrite(TEMP_VPIN, temperature);
  Blynk.virtualWrite(HUMIDITY_VPIN, humidity);
 delay(1000);
}

// Hàm điều khiển bật/tắt Relay thông qua Blynk
BLYNK_WRITE(V1)
{
  int relayState = param.asInt();  // Nhận giá trị từ nút nhấn ảo V1
  digitalWrite(relayPin, relayState);  
}

// Hàm điều khiển bật/tắt LED thông qua Blynk
BLYNK_WRITE(V2)
{
  int ledState = param.asInt();  // Nhận giá trị từ nút nhấn ảo V2
  digitalWrite(ledPin, ledState);  // Bật/tắt LED theo giá trị nhận được
}



// ket noi: 
//  -- led +GND
//         +D1
//  -- RELAY + GND
//           +D3
//           +3.3
//   -- DHT +GND
//           +3.3
//           +d2
//   -- srf05 + GND
//             + 5V
//             +echo d4
//             +trip d5
