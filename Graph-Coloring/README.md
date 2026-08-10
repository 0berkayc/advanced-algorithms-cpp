# Graph Coloring (Çizge Boyama) Algoritmaları

Bu modül, NP-Hard sınıfında yer alan **Graph Coloring (Çizge Boyama)** probleminin sezgisel (heuristic) çözüm yaklaşımlarını içermektedir.

---

## 📁 Proje Dizin Yapısı

```text
NP-Hard-Approximation/Graph-Coloring/
├── README.md                   # Modül genel dokümantasyonu (Bu dosya)
├── Greedy/                     # Welsh-Powell (Statik Dereceli Sezgisel)
│   ├── main.cpp
│   └── README.md
└── DSatur/                     # DSatur (Dinamik Doygunluk Dereceli Sezgisel)
    ├── main.cpp
    └── README.md

```

---

## 📌 Problem Tanımı

**Graph Coloring**, yönlendirilmemiş bir $G = (V, E)$ çizgesinde, birbirine kenarla bağlı (komşu) hiçbir iki düğüm aynı rengi almayacak şekilde tüm düğümlere renk atanması ve bu işlem yapılırken kullanılan **toplam renk sayısının minimuma (Kromatik Sayı — $\chi(G)$)** indirilmesi problemidir.

* **Zorluk Sınıfı:** **NP-Hard** (Optimizasyon versiyonu). Verilen $k$ sayısı için çizgenin $k$ renkle boyanabilirliğini belirlemek **NP-Complete**'tir.
* **Uygulama Alanları:**
* Derleyici Tasarımı (Register Allocation)
* Ders ve Sınav Programı Çizelgeleme (Exam Scheduling)
* Kablosuz Ağlarda Frekans Ataması (Frequency Assignment)
* Harita Boyama ve Kaynak Yönetimi



---

## Uygulanan Algoritmalar ve Karşılaştırma

Modül kapsamında iki farklı sezgisel yaklaşım C++17 ile geliştirilmiştir:

1. **Greedy (Welsh-Powell):** Düğümleri boyama işleminden önce **statik** olarak derecelerine (komşu sayılarına) göre sıralar.
2. **DSatur (Degree of Saturation):** Her boyama adımında komşularda kullanılan farklı renklerin sayısını takip ederek **dinamik** bir doygunluk derecesi ($\text{sat}$) hesaplar ve en kısıtlı düğümü seçer.

### Algoritma Karşılaştırma Tablosu

| Özellik | Greedy (Welsh-Powell) | DSatur (Brélaz) |
| --- | --- | --- |
| **Sıralama Stratejisi** | Statik (Boyama öncesi 1 kez) | Dinamik (Her adımda yeniden hesaplanır) |
| **Seçim Kriteri** | Orijinal Derece (Komşu Sayısı) | Doygunluk Derecesi ($\text{sat}$) + Orijinal Derece |
| **Zaman Karmaşıklığı** | $O(\vert{}V\vert{} \log \vert{}V\vert{} + \vert{}E\vert{})$ | $O(\vert{}V\vert{}^2)$ (Dizi ile) / $O((\vert{}V\vert{} + \vert{}E\vert{}) \log \vert{}V\vert{})$ (Heap ile) |
| **Alan Karmaşıklığı** | $O(\vert{}V\vert{} + \vert{}E\vert{})$ | $O(\vert{}V\vert{}^2 + \vert{}E\vert{})$ |
| **Çözüm Kalitesi** | Hızlıdır, ancak karmaşık yapılarda renk harcar | Sezgiseller içinde **Optimuma en yakın** olanıdır |
| **Ideal Kullanım** | Devasa ağlar ($N > 10.000$), hızlı yanıt ihtiyacı | Orta ve büyük ölçekli karmaşık çizgeler |

---

## 🛠️ Derleme ve Çalıştırma

Her algoritma kendi dizini içinde bağımsız C++ projeleri olarak derlenebilir.

### 1. Greedy (Welsh-Powell) Algoritmasını Çalıştırma

```bash
cd Greedy
g++ -std=c++17 main.cpp -o app
./app

```

### 2. DSATUR Algoritmasını Çalıştırma

```bash
cd DSATUR
g++ -std=c++17 main.cpp -o app
./app

```

---