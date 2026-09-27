#include <math.h> // Diperlukan untuk fungsi fmod()

// Taruh fungsi ini di luar loop(), biasanya di bagian atas sebelum setup()
void rgbToHsv(uint8_t r, uint8_t g, uint8_t b, float &h, float &s, float &v) {
  // Normalisasi nilai RGB dari 0-255 ke pecahan 0.0 hingga 1.0
  float r_f = r / 255.0;
  float g_f = g / 255.0;
  float b_f = b / 255.0;

  // Cari komponen warna yang paling dominan (max) dan paling lemah (min)
  float max_val = max(max(r_f, g_f), b_f);
  float min_val = min(min(r_f, g_f), b_f);
  float delta = max_val - min_val;

  // 1. Hitung Value (Kecerahan ruang)
  v = max_val; 

  if (delta == 0) {
    // Jika komponen warnanya sama semua (abu-abu/hitam/putih murni),
    // artinya tidak ada warna (Hue) yang bisa diukur.
    h = 0;
    s = 0;
  } else {
    // 2. Hitung Saturation (Kepekatan warna)
    s = delta / max_val; 

    // 3. Hitung Hue (Jenis Warna dalam format derajat melingkar 0-360)
    if (max_val == r_f) {
      h = 60.0 * fmod(((g_f - b_f) / delta), 6.0);
    } else if (max_val == g_f) {
      h = 60.0 * (((b_f - r_f) / delta) + 2.0);
    } else if (max_val == b_f) {
      h = 60.0 * (((r_f - g_f) / delta) + 4.0);
    }
    
    // Pastikan derajat Hue tidak bernilai negatif
    if (h < 0) {
      h += 360.0;
    }
  }
}



//===============================================================================//
// ... [kode pemecah 16-bit (pixel) ke R, G, B di dalam loop for] ...
    uint8_t r = (pixel & 0xF800) >> 8; 
    uint8_t g = (pixel & 0x07E0) >> 3; 
    uint8_t b = (pixel & 0x001F) << 3; 

    // Siapkan wadah variabel lokal kosong
    float h, s, v; 
    
    // Panggil fungsinya. Nilai h, s, v akan otomatis terisi nilai yang benar berkat Pass by Reference!
    rgbToHsv(r, g, b, h, s, v);

    // Sampai baris ini, 1 buah piksel yang sedang diperiksa sudah berhasil diubah menjadi format HSV.
