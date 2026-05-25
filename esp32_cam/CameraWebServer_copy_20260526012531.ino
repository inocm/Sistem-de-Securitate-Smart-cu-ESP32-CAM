#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

const char* ssid = "ASUS-TUF 4682";
const char* password = "zcartu2003";
const char* botToken = "8675891819:AAFRc4bP1e3wz3Dt_RbKHn_2BC59yUTMtRM";
const char* chatID = "7602329347";

#define TRIGGER_PIN 12

WebServer server(80);
bool alarmActive = false;
String cameraIP = "";

void startCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;
  config.fb_location = CAMERA_FB_IN_DRAM;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed: 0x%x\n", err);
  } else {
    Serial.println("Camera init OK!");
  }
}

void sendMessageTelegram(String message) {
  Serial.println("Trimit mesaj pe Telegram...");
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  String url = "https://api.telegram.org/bot" + String(botToken) + "/sendMessage?chat_id=" + String(chatID) + "&text=" + message;
  http.begin(client, url);
  http.setTimeout(30000);
  int httpCode = http.GET();
  Serial.printf("Telegram response: %d\n", httpCode);
  http.end();
}

void handleCapture() {
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    server.send(500, "text/plain", "Camera capture failed");
    return;
  }
  server.sendHeader("Content-Type", "image/jpeg");
  server.sendHeader("Content-Length", String(fb->len));
  server.send_P(200, "image/jpeg", (const char*)fb->buf, fb->len);
  esp_camera_fb_return(fb);
}

void handleRoot() {
  String html = "<html><body>";
  html += "<h2>ESP32-CAM Sistem Securitate</h2>";
  html += "<img src='/capture' style='width:100%'>";
  html += "<br><a href='/capture'>Refresh poza</a>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, INPUT_PULLDOWN);
  startCamera();

  WiFi.begin(ssid, password);
  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  cameraIP = WiFi.localIP().toString();
  Serial.print("Camera Ready! http://");
  Serial.println(cameraIP);

  server.on("/", handleRoot);
  server.on("/capture", handleCapture);
  server.begin();
}

void loop() {
  server.handleClient();

  if (digitalRead(TRIGGER_PIN) == HIGH) {
    if (!alarmActive) {
      alarmActive = true;
      Serial.println("Intrus detectat!");
      String msg = "INTRUS%20DETECTAT%20Acceseaza%20camera:%20http://" + cameraIP;
      sendMessageTelegram(msg);
      delay(30000);
      alarmActive = false;
    }
  } else {
    alarmActive = false;
  }
}