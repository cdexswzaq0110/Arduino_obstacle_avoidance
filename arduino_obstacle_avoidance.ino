#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const int trigPin = 11;    // 超音波感測器 Trig腳接 Arduino pin 11
const int echoPin = 12;    // 超音波感測器 Echo 腳接 Arduino pin 12
const int speakerPin = 7;  // 蜂鳴器 + 腳接 Arduino pin 7
const int TX  = 10;
const int RX  = 9;
const int ESPTX  = 2;
const int ESPRX  = 3;
SoftwareSerial ESPserial(ESPRX, ESPTX); // 建立軟體序列埠來與 ESP-01S 通訊
SoftwareSerial BT(RX, TX);

const int delay_time = 1000; //delay 1000 ms for every measurment
long duration, distance;   // 宣告計算距離時需要用到的變數
const char* ssid = "Huang_(:3｣∠)_YmM2MTRl";
const char* password = "12345678";
ESP8266WebServer server(80);

void setup() {
  Serial.begin(9600);         // 設定序列埠監控視窗和 Arduino資料傳輸速率為 9600 bps // 初始化藍牙模塊串口
  ESPserial.begin(115200);      // 設定 ESP-01S 資料傳輸速率為 115200 bps ;       
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connecting to WiFi");
  Serial.println(WiFi.localIP());
  Serial.println();

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");

  pinMode(trigPin, OUTPUT);    // Arduino 對外啟動距離感測器Trig腳，射出超音波 
  pinMode(echoPin, INPUT);     // 超音波被障礙物反射後，Arduino讀取感測器Echo腳的時間差
  pinMode(speakerPin, OUTPUT); // Arduino對蜂鳴器送出電壓，使其鳴叫
}

void loop() {  
  measureDistance();

  // 根據距離控制蜂鳴器
  if (distance > 5 && distance <= 100) {       // 距離介於5到100公分，蜂鳴器斷斷續續叫 
    digitalWrite(speakerPin, HIGH);
    delay(500);
    digitalWrite(speakerPin, LOW);
    delay(500);
  } else {
    digitalWrite(speakerPin, LOW);
  }
  server.handleClient();
}

void measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5); // 等待超音波模組穩定
  digitalWrite(trigPin, HIGH); // 發送10微秒的觸發脈衝
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // 等待回波並測量回波時間
  distance = (duration / 2) / 29.1; //distance = ( half of time of back and forth )x( wave velocity(use 0.034 cm per microsecond) )

  if (distance != 0) {
    // 在串列監視器上顯示結果
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    delay(delay_time);

    // 將距離數據轉換為整數並打包
    int sendData = (int) (distance * 100); // 將距離乘以100並轉換為整數
    byte packet[3]; 
    packet[0] = 97; // 發送到手機的鍵值
    packet[1] = sendData / 256; // 將 sendData 分成兩個1字節的包
    packet[2] = sendData % 256;

    // 檢查藍牙是否可用
    if(BT.available() > 0) {
      if(BT.read() == 97) { // 檢查是否接收到來自手機的鍵值
        Serial.println("succeed!");
        for(int i = 0; i < 3; i++) 
          BT.write(packet[i]); // 發送packet到手機
      }
    }
  }
}

void handleRoot() {
  measureDistance();
  String message = "Distance: ";
  message += distance;
  message += " cm";
  server.send(200, "text/plain", message);
}