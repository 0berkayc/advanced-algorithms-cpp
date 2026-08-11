# Sudoku Algoritmaları

Bu modül, Kısıt Sağlama Problemleri (Constraint Satisfaction Problem - CSP) arasında en popüler olan **Sudoku Solver** probleminin C++17 ile geliştirilmiş iki farklı **Backtracking (Geri Adım Atma)** çözüm yaklaşımını içermektedir.

---

## Proje Dizin Yapısı

```text
Sudoku/
├── README.md          # Modül genel dokümantasyonu
├── Backtracking/      # Klasik 2D Matris Tabanlı Backtracking
│   ├── main.cpp
│   └── README.md
└── MRV/              # MRV (Minimum Remaining Values) Sezgisel Çözümü
    ├── main.cpp
    └── README.md

```

---

## Problem Tanımı

**Sudoku Solver Problemi**, $9 \times 9$ boyutundaki kısmen doldurulmuş bir matrisi belirli kurallara uyarak eksiksiz tamamlama problemidir.

* **Kısıtlar:**
1. **Satır Kısıtı:** Her satırda $1$'den $9$'a kadar rakamlar yalnızca **1 kez** bulunabilir.
2. **Sütun Kısıtı:** Her sütunda $1$'den $9$'a kadar rakamlar yalnızca **1 kez** bulunabilir.
3. **$3 \times 3$ Blok Kısıtı:** Tahtadaki 9 adet $3 \times 3$'lük alt bloğun her birinde $1$'den $9$'a kadar rakamlar yalnızca **1 kez** bulunabilir.


* **Zorluk Sınıfı:** Genelleştirilmiş $N \times N$ Sudoku problemi **NP-Complete** zorluk sınıfındadır.
* **Arama Uzayı:** $M$ boş hücre sayısı olmak üzere ($M \approx 60$), ham kaba kuvvet (brute force) arama uzayı $9^M \approx 9^{60} \approx 10^{57}$ olası kombinasyondur.

---

## Uygulanan Yöntemler ve Karşılaştırma

Modül kapsamında iki farklı çözüm stratejisi uygulanmıştır:

1. **Standart Backtracking (Sıralı Arama):** Tahtadaki ilk boş hücreden (`0`) başlayarak soldan sağa, yukarıdan aşağıya doğru sırayla ilerler. Hücrelere $1$'den $9$'a kadar kurala uyan ilk rakamı koyar; tıkandığında geri adım atar (backtrack).
2. **MRV Sezgisi (Minimum Remaining Values):** *"Fail-First" (İlk Önce Başarısız Ol)* prensibini uygular. Sıradaki ilk hücreyi seçmek yerine, o anki tahta durumunda **yazılabilecek aday rakam sayısı en az olan (en kısıtlı) hücreyi** seçerek arama ağacının dallanma faktörünü drastik biçimde küçültür.

### Algoritma Karşılaştırma Tablosu

| Özellik | Standart Backtracking | MRV (Minimum Remaining Values) Sezgisi |
| --- | --- | --- |
| **Hücre Seçim Stratejisi** | Sıralı Arama (Soldan sağa, yukarıdan aşağı) | Sezgisel Arama (En az adayı kalan hücre) |
| **Çalışma Prensibi** | Kör tarama (Blind Search) | *Fail-First* (İlk Önce Başarısız Ol / Erken Budama) |
| **Arama Ağacı Boyutu** | Geniş (Derin çıkmaz sokaklara saplanabilir) | **Minimum** (Çıkmaz sokakları anında tespit eder) |
| **Arama Maliyeti (Adım Başı)** | $O(1)$ sıradaki hücre seçimi | $O(M)$ en az adaylı hücreyi bulmak için tarama |
| **Çalışma Hızı** | Kolay bulmacalarda hızlı, zorlarda yavaş | Zor ve karmaşık bulmacalarda **%90 - %99 daha hızlı** |
| **Kullanım Amacı** | Temel kısıt kontrolü ve algoritma mantığı | Zor Sudoku bulmacaları ve karmaşık CSP problemleri |

---

## 🛠️ Derleme ve Çalıştırma

Her iki yöntem de kendi klasörleri içinde bağımsız C++ projeleri olarak derlenebilir.

### 1. Standart Backtracking Çözümü

```bash
cd Standard-Backtracking
g++ -std=c++17 main.cpp -o app
./app

```

### 2. MRV Sezgisel Çözüm

```bash
cd MRV-Heuristic
g++ -std=c++17 main.cpp -o app
./app

```

---