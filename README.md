# 🛸 ESP32-CAM Color Detection & Tracking for Drone Vision

Repositori ini berisi *source code* untuk menyulap **ESP32-CAM** menjadi *vision sensor* pendamping *flight controller* (seperti Pixhawk). Sistem ini dirancang untuk mendeteksi objek dengan warna tertentu (contoh: bola merah), melacak posisinya, dan menghitung deviasi ($\Delta X, \Delta Y$) objek tersebut dari titik tengah kamera.

Proyek ini sangat berfokus pada **optimasi memori dan kecepatan komputasi**, mengingat keterbatasan RAM pada ESP32.

## 🧠 Mekanisme Dasar & Cara Kerja

Karena kita berurusan dengan memori mikrokontroler yang kecil, *image processing* di sini tidak menggunakan *library* berat seperti OpenCV, melainkan pemrosesan raw memori secara *on-the-fly*.

### 1. Zero-Copy Memory Access (Pengolahan Tanpa Duplikasi)
Kamera diatur untuk menangkap gambar dengan format mentah **RGB565** pada resolusi **QQVGA (160x120)**. Saat *frame* berhasil ditangkap, data gambar (~38.4KB) langsung dialirkan via DMA ke PSRAM. Program **tidak pernah** menyalin data ini ke *array* baru di RAM internal. Kita menggunakan variabel pointer (`fb->buf`) murni hanya untuk "mengintip" dan mengeksekusi data memori tersebut tepat di tempatnya berada.

### 2. On-the-Fly HSV Conversion
Warna RGB sangat rentan terhadap perubahan cahaya lingkungan. Oleh karena itu, kita menggunakan format **HSV (Hue, Saturation, Value)**. 
Alih-alih mengonversi seluruh piksel gambar di awal, program membaca 16-bit (2 *byte*) data RGB565 per piksel, mengekstrak nilai R, G, B via operasi *bitwise* (`>>` dan `&`), lalu langsung mengonversinya menjadi nilai HSV detik itu juga menggunakan fungsi konversi ringan (*Pass by Reference*).

### 3. Thresholding (Penyaringan Target)
Setelah 1 piksel berhasil diubah ke format HSV, nilai tersebut langsung diuji dengan *Thresholding*.
- Apakah *Hue* sesuai dengan warna target (misal merah: di sekitar 0° atau 360°)?
- Apakah *Saturation* dan *Value* cukup pekat/terang?
Jika piksel lolos filter, koordinat letak piksel tersebut ($X, Y$) langsung diakumulasikan. Jika tidak, piksel tersebut diabaikan. **Sistem tidak pernah menyimpan koordinat atau warna piksel yang tidak penting.**

### 4. Centroid & Calculation Deviasi ($\Delta X, \Delta Y$)
Setelah 19.200 piksel (1 layar QQVGA penuh) selesai dipindai, program mencari rata-rata dari seluruh koordinat $X$ dan $Y$ piksel target yang berhasil dikumpulkan. Hasil rata-rata ini adalah **Titik Pusat (Centroid)** objek. 
Titik ini kemudian dikurangi dengan kordinat tengah resolusi layar (80, 60) untuk menghasilkan nilai deviasi $\Delta X$ dan $\Delta Y$ yang akan dikirimkan ke Pixhawk sebagai panduan navigasi *drone*.

## 🛠️ Kebutuhan Hardware
- **ESP32-CAM** (Modul AI-Thinker)
- Modul FTDI / CP2102 USB to TTL (Untuk proses *upload* kode)
- Kabel Jumper secukupnya
- *Flight Controller* (Pixhawk) - *Fase Integrasi*

## 🚀 Cara Instalasi & Penggunaan
1. Buka Arduino IDE dan pastikan sudah menginstal *Board Package* untuk ESP32.
2. Pilih *board* **"AI Thinker ESP32-CAM"**.
3. Buka file *sketch* `.ino` dari repo ini.
4. Hubungkan ESP32-CAM ke FTDI (*Pastikan pin GPIO 0 di-jumper ke GND saat upload*).
5. *Upload* kode. Setelah selesai, cabut *jumper* GPIO 0 dan tekan tombol *Reset*.
6. Buka Serial Monitor dengan *baud rate* **115200** untuk melihat hasil tracking posisi objek.

## 📌 Status Proyek (To-Do List)
- [x] Inisialisasi ESP32-CAM & Memory Handling
- [x] Ekstraksi RGB565 ke HSV secara *On-the-fly*
- [x] Logika Thresholding warna 
- [x] Perhitungan Centroid & Delta Deviasi
- [ ] Implementasi protokol komunikasi Serial/MAVLink ke Pixhawk
- [ ] Uji coba pergerakan *drone* berdasarkan data visual
