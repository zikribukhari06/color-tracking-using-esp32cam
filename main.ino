#include "esp_camera.h"

// Pin mapping untuk AI-Thinker ESP32-CAM
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

bool initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk     = XCLK_GPIO_NUM;
  config.pin_pclk     = PCLK_GPIO_NUM;
  config.pin_vsync    = VSYNC_GPIO_NUM;
  config.pin_href     = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;

  config.xclk_freq_hz = 20000000;          // 20 MHz
  config.pixel_format = PIXFORMAT_RGB565;  // format mentah, mudah diolah
  config.frame_size   = FRAMESIZE_QQVGA;   // 160x120
  config.fb_count     = 1;
  config.fb_location  = CAMERA_FB_IN_PSRAM;
  config.grab_mode    = CAMERA_GRAB_LATEST; // selalu ambil frame terbaru

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Kamera gagal init, error 0x%x\n", err);
    return false;
  }

  // Tuning sensor: matikan auto-adjust agar warna lebih konsisten
  sensor_t *s = esp_camera_sensor_get();
  s->set_whitebal(s, 1);   // auto white balance (nanti bisa kita matikan)
  s->set_exposure_ctrl(s, 1);
  s->set_gain_ctrl(s, 1);
  return true;
}

void setup() {
  Serial.begin(115200);
  delay(500);
  if (!initCamera()) {
    while (true) delay(1000);
  }
  Serial.println("Kamera siap!");
}

void loop() {
  static uint32_t frameCount = 0;
  static uint32_t tStart = millis();

  camera_fb_t *fb = esp_camera_fb_get();   // minta frame buffer
  if (!fb) {
    Serial.println("Gagal ambil frame");
    return;
  }

  // Cuma info saat frame pertama
  if (frameCount == 0) {
    Serial.printf("Lebar: %d, Tinggi: %d, Ukuran: %u byte\n",
                  fb->width, fb->height, fb->len);
  }

  esp_camera_fb_return(fb);                // WAJIB dikembalikan!
  frameCount++;

  // Hitung FPS tiap 2 detik
  if (millis() - tStart >= 2000) {
    Serial.printf("FPS: %.1f\n", frameCount * 1000.0f / (millis() - tStart));
    frameCount = 0;
    tStart = millis();
  }
}
