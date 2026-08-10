# N-Queens (N-Vezir) Algoritmaları

Bu modül, kombinatorik arama ve kısıt sağlama problemlerinin (Constraint Satisfaction Problem) klasikleri arasında yer alan **N-Queens (N-Vezir)** probleminin C++17 ile geliştirilmiş iki farklı **Backtracking (Geri Adım Atma)** çözüm yaklaşımını içermektedir.

---

## Proje Dizin Yapısı

```text
Backtracking/N-Queens/
├── README.md                   # Modül genel dokümantasyonu
├── Backtracking/      # Standart 1D Dizi tabanlı Backtracking (Tahta Çizimli)
│   ├── main.cpp
│   └── README.md
└── Bitmask/                 # Bitwise Optimizasyonlu Backtracking (Hızlı Sayım)
    ├── main.cpp
    └── README.md

```

---

## Problem Tanımı

**N-Queens Problemi**, $N \times N$ boyutundaki bir satranç tahtasına **$N$ adet veziri**, hiçbir vezir bir diğerini tehdit etmeyecek şekilde yerleştirme problemidir.

* **Kısıtlar:** İki vezir aynı **satırda**, aynı **sütunda** veya aynı **çaprazda** yer alamaz.
* **Zorluk Sınıfı:** Combinatorial Search / Constraint Satisfaction Problem.
* **Arama Uzayı:** $N \times N$ kareden $N$ kare seçme olasılığı kaba kuvvetle $\binom{N^2}{N}$ kadardır. Budama (pruning) ile arama alanı daraltılarak $O(N!)$ karmaşıklığına indirgenir.

---

## Uygulanan Yöntemler ve Karşılaştırma

Modül kapsamında iki farklı Backtracking yaklaşımı uygulanmıştır:

1. **Standart Backtracking (1D Dizi Tabanlı):** Satır satır ilerler, $row$. satırdaki vezirin sütununu `board[row] = col` şeklinde tutar. Her adımda $O(N)$ süren `isSafe` kontrolü yapar ve **geçerli tahta dizilimlerini ekrana çizer**.
2. **Bitmasking Optimizasyonu (Bitsel İşlemler):** Sütun ve çapraz kısıtlarını tamsayı bitleri (`cols`, `left_diags`, `right_diags`) üzerinde saklar. Çakışma kontrolünü $O(1)$ sürede yaparak **sadece toplam çözüm sayısını maksimum hızda** hesaplar.

### Algoritma Karşılaştırma Tablosu

| Özellik | Standart Backtracking | Bitmasking Optimizasyonu |
| --- | --- | --- |
| **Tahta Temsili** | 1D Dizi (`std::vector<int>`) | Tamsayı Bit Maskeleri (`int`) |
| **Çakışma Kontrolü** | $O(N)$ (Döngü ile `isSafe`) | $O(1)$ (Bitsel Operatörler `&, |, ~`) |
| **Temel Kullanım Amacı** | Çözüm dizilimlerini ekrana bastırma / görselleştirme | Yüksek performansla **sadece toplam çözüm sayısını** bulma |
| **Çalışma Hızı** | Standart (I/O & Döngü maliyetli) | **10x - 50x daha hızlı** (CPU register seviyesinde) |
| **Bellek Kullanımı** | $O(N)$ (Dizi + Call Stack) | $O(N)$ (Sadece Call Stack) |
| **I/O Etkisi** | Tahtaları (`Q . . .`) yazdırır | Sadece sayı basar (I/O gecikmesi yok) |

---

## Derleme ve Çalıştırma

Her iki yöntem de kendi klasörleri içinde bağımsız C++ projeleri olarak derlenebilir.

### 1. Standart Backtracking (Tahta Görselleştirmeli)

```bash
cd Backtracking
g++ -std=c++17 main.cpp -o app
./app

```

### 2. Bitmasking Optimizasyonlu (Hızlı Çözüm Sayımı)

```bash
cd Bitmask
g++ -std=c++17 main.cpp -o app
./app

```

---