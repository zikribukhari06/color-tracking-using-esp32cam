// ... (kode jepret kamera sebelumnya) ...

  uint8_t *buf = fb->buf;
  int width = fb->width;
  int height = fb->height;

  // Kita lompat per 2 index (i += 2) karena 1 piksel butuh 2 byte
  for (int i = 0; i < fb->len; i += 2) {
    
    // 1. Gabungkan 2 byte menjadi 1 variabel 16-bit (1 piksel utuh)
    // Catatan: Jika warna aslinya nanti terlihat aneh/terbalik, tukar posisinya menjadi: (buf[i+1] << 8) | buf[i]
    uint16_t pixel = (buf[i] << 8) | buf[i + 1];

    // 2. Ekstrak nilai Merah, Hijau, Biru dan konversi ke skala 0-255
    // - R: Ambil 5 bit pertama (0xF800), geser ke kanan 8 kali
    uint8_t r = (pixel & 0xF800) >> 8; 
    
    // - G: Ambil 6 bit tengah (0x07E0), geser ke kanan 3 kali
    uint8_t g = (pixel & 0x07E0) >> 3; 
    
    // - B: Ambil 5 bit terakhir (0x001F), geser ke kiri 3 kali
    uint8_t b = (pixel & 0x001F) << 3; 

    // --- Nanti konversi RGB ke HSV ditaruh di sini ---

  } // Akhir dari loop piksel

  // WAJIB: Kembalikan memory buffer
  esp_camera_fb_return(fb);
  // ...
