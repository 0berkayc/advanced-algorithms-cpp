# Bin Packing Problemi ve Sezgisel (Heuristic) Yaklaşımlar

## 1. Problemin Tanımı
**Bin Packing (Kutu Paketleme)**, bilgisayar bilimleri ve yöneylem araştırmasında klasik bir kombinatoryal optimizasyon problemidir. 

Temel amaç:
* Sabit $C$ kapasitesine sahip özdeş kutulara (bins),
* Farklı boyutlardaki $N$ adet nesneyi,
* **Hiçbir kutunun kapasitesini aşmayacak şekilde minimum sayıda kutu kullanarak** yerleştirmektir.

---

## 2. Karmaşıklık Sınıfı (NP-Hard)
Bin Packing problemi **NP-Hard** zorluk sınıfındadır.
* **Problem Neden Zordur?** $N$ nesne sayısı büyüdükçe nesnelerin kutulara dağılabileceği olası kombinasyon sayısı üstel ($O(2^N)$ veya $O(N!)$) olarak artar.
* **Kesin Çözümün Maliyeti:** Tüm olası durumları deneyen Dinamik Programlama veya Backtracking gibi yöntemler, küçük eleman gruplarında bile kilitlenir ve evrenin yaşından daha uzun sürede sonuç üretebilir.
* **Sezgisel (Approximation) Yaklaşımlar:** Gerçek hayatta optimum sonucu makul sürede bulmak imkansız olduğundan, **First-Fit**, **Best-Fit** ve **FFD** gibi polinom zamanda ($O(N^2)$ veya $O(N \log N)$) optimuma çok yakın sonuçlar veren sezgisel algoritmalar tercih edilir.

---

## 3. Uygulanan Algoritmalar ve Karşılaştırma

Bu modül altında geliştirilen 3 temel algoritmanın çalışma prensipleri ve performans özetleri:

### A. First-Fit (FF)
* **Mantık:** Gelen her nesne için açık olan kutular soldan sağa taranır ve sığdığı **ilk kutuya** yerleştirilir. Sığmıyorsa yeni kutu açılır.
* **Çalışma Tipi:** Online (Akış Anında)
* **Zaman Karmaşıklığı:** $O(N^2)$ (Ağaç veri yapılarıyla $O(N \log N)$)
* **Alan Karmaşıklığı:** $O(K)$ ($K \le N$)

### B. Best-Fit (BF)
* **Mantık:** Gelen her nesne için tüm açık kutular taranır ve nesne yerleştiğinde **en az artık boşluk (minimum remaining space)** bırakacak kutu seçilir.
* **Çalışma Tipi:** Online (Akış Anında)
* **Zaman Karmaşıklığı:** $O(N^2)$ (Ağaç veri yapılarıyla $O(N \log N)$)
* **Alan Karmaşıklığı:** $O(K)$

### C. First-Fit Decreasing (FFD)
* **Mantık:** Nesneler öncelikle **büyükten küçüğe sıralanır**, ardından standart First-Fit uygulanır. Kaba ve yerleşimi zor olan nesneler önden iskelet oluşturur, küçük nesneler ise aradaki boşluklara kayar.
* **Çalışma Tipi:** Offline (Çevrimdışı / Sıralama Zorunlu)
* **Zaman Karmaşıklığı:** $O(N \log N + N^2) = O(N^2)$
* **Alan Karmaşıklığı:** $O(N + K)$

---

## 4. Algoritma Karşılaştırma Tablosu

| Özellik | First-Fit (FF) | Best-Fit (BF) | First-Fit Decreasing (FFD) |
| :--- | :--- | :--- | :--- |
| **Algoritma Türü** | Online | Online | Offline |
| **Ön İşlem (Sıralama)** | Yok | Yok | Var ($O(N \log N)$) |
| **Çözüm Kalitesi** | Makul | Başarılı | **En İyi (Optimuma Çok Yakın)** |
| **Garantili Üst Sınır** | $\le 1.7 \cdot \text{OPT} + 1$ | $\le 1.7 \cdot \text{OPT} + 1$ | $\mathbf{\le 1.22 \cdot \text{OPT} + 1}$ |
| **Kullanım Alanı** | Canlı ağ paket akışı | Bellek Yönetimi (Memory Allocation) | Lojistik / Konteyner Yükleme |

---

## 5. Dizin Yapısı

```text
Bin-Packing/
├── README.md               # Genel bilgilendirme dokümanı (Bu dosya)
├── First-Fit/
│   ├── main.cpp            # First-Fit C++ uygulaması
│   └── README.md           # First-Fit özel dokümantasyonu
├── Best-Fit/
│   ├── main.cpp            # Best-Fit C++ uygulaması
│   └── README.md           # Best-Fit özel dokümantasyonu
└── FFD/
    ├── main.cpp            # First-Fit Decreasing C++ uygulaması
    └── README.md           # FFD özel dokümantasyonu
```