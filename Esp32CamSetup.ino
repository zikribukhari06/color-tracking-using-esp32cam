#include "esp_camera.h"

// Definisi pin khusus untuk modul AI-Thinker ESP32-CAM
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

void setup() {
  Serial.begin(115200);
  Serial.println("\nMemulai inisialisasi kamera...");

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
  
  // PENGATURAN KRUSIAL UNTUK COLOR TRACKING:
  config.pixel_format = PIXFORMAT_RGB565; // Format mentah agar bisa kita ekstrak warnanya
  config.frame_size = FRAMESIZE_QQVGA;    // Resolusi 160x120. Semakin kecil = semakin cepat diproses
  config.fb_count = 1;                    // Kita cuma butuh 1 buffer memori karena diproses on-the-fly

  // Mulai inisialisasi
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Kamera gagal inisialisasi dengan error 0x%x", err);
    return;
  }
  Serial.println("Kamera berhasil diinisialisasi!");
}

void loop() {
  // 1. Tangkap frame dari sensor kamera
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Gagal mengambil frame");
    return;
  }

  // Cek apakah data benar-benar masuk
  Serial.printf("Berhasil jepret! Resolusi: %dx%d | Ukuran data: %d bytes\n", fb->width, fb->height, fb->len);

  // 2. WAJIB: Kembalikan buffer ke sistem. 
  // Kalau ini lupa, ESP32 akan langsung crash karena kehabisan memori.
  esp_camera_fb_return(fb);
  
  delay(1000); // Delay sementara untuk tes Serial Monitor
}
