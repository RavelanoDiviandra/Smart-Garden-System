# Smart Garden System: Dual Sensor Adaptive Logic 

Sistem kontrol penyiraman tanaman otomatis (*Smart Garden*) berbasis mikrokontroler. Sistem ini menggunakan logika adaptif berdasarkan intensitas cahaya lingkungan (Siang/Malam) dan menerapkan metode penstabilan data (Moving Average) serta batas Histeresis untuk mencegah osilasi pada pompa air (*Relay Chatter*).

## Arsitektur Perangkat Keras
Sistem ini dirancang untuk dijalankan pada arsitektur **Arduino Uno/Nano** (AVR).

### Kebutuhan Komponen:
1. Arduino Uno / Nano
2. Sensor Kelembapan Tanah (Analog)
3. Sensor Cahaya (LDR)
4. Modul Relay 1-Channel (Active LOW)
5. LCD I2C 16x2
6. Pompa Air DC mini & Selang

### Matriks Pengkabelan (Pinout)
| Komponen | Pin Arduino | Tipe Logika |
| :--- | :--- | :--- |
| Sensor Tanah | `A3` | Analog Input |
| Sensor LDR | `A1` | Analog Input |
| Relay Pompa | `7` | Digital Output (Active LOW) |
| LED Indikator | `13` | Digital Output (Active HIGH) |
| LCD I2C (SDA) | `A4` | I2C Data |
| LCD I2C (SCL) | `A5` | I2C Clock |

## Parameter Kalibrasi Sistem
Sistem ini beroperasi dengan parameter adaptif yang mengubah ambang batas kekeringan tanah berdasarkan waktu:
* **Ambang Batas LDR (Senja):** `> 700` (Status berubah menjadi MALAM)
* **Batas Aktif Pompa (SIANG):** Tanah `> 800` (Kering)
* **Batas Aktif Pompa (MALAM):** Tanah `> 950` (Lebih kering, menghemat air di malam hari)
* **Histeresis (Batas Matikan Pompa):** `Batas Aktif - 100` (Mencegah pompa hidup-mati secara berulang dalam waktu cepat)

## Deployment (Instalasi)

### Opsi 1: Menggunakan PlatformIO (Direkomendasikan)
1. Kloning repositori ini.
2. Buka folder proyek menggunakan Visual Studio Code dengan ekstensi PlatformIO.
3. Pastikan mikrokontroler terhubung ke komputer.
4. Eksekusi perintah `Build` dan `Upload`. Dependensi `LiquidCrystal_I2C` akan diunduh secara otomatis sesuai konfigurasi pada file `platformio.ini`.

### Opsi 2: Menggunakan Arduino IDE
1. Unduh repositori ini sebagai file ZIP lalu ekstrak.
2. Pindahkan seluruh kode sumber yang ada di dalam `src/main.cpp` ke dalam file baru bernama `Smart_Garden.ino`.
3. Buka file `Smart_Garden.ino` menggunakan Arduino IDE.
4. Instal pustaka wajib melalui **Library Manager** (`Sketch` -> `Include Library` -> `Manage Libraries...`):
   * Cari dan instal **LiquidCrystal I2C** (Pastikan alamat I2C Anda sesuai, misal: `0x27`).
5. Pilih *Board* (Arduino Uno/Nano) dan *Port* yang sesuai pada menu `Tools`.
6. Klik tombol **Upload**.
