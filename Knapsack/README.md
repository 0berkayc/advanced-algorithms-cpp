# 0/1 Knapsack Problemi

Bu modül, bilgisayar bilimlerinin en temel **NP-Complete** problemlerinden biri olan **0/1 Knapsack (Sırt Çantası)** probleminin 5 farklı algoritma tekniği ile gerçekleştirilmiş C++ çözümlerini ve teorik analizlerini içermektedir.

0/1 Knapsack probleminde eşyalar bölünemediği ($0$ veya $1$ seçeneği) için tek bir evrensel çözüm yöntemi her senaryoda mükemmel çalışmaz. Girdi boyutlarına ($N$), çanta kapasitesine ($W$), eşya değerlerine ($V$) ve kesinlik ihtiyacına göre doğru algoritmanın seçilmesi gerekir.

---

## 1. Genel Karşılaştırma Tablosu

| Algoritma | Yaklaşım Tipi | Doğruluk Garantisi | Zaman Karmaşıklığı (Worst Case) | Alan Karmaşıklığı | Kritik Bağımlılık / Sınır | İdeal Kullanım Senaryosu |
| --- | --- | --- | --- | --- | --- | --- |
| **Pure Recursive** | Kaba Kuvvet (Brute-Force) | %100 Optimal | $O(2^N)$ | $O(N)$ | $N \le 20$ (Üstel Patlama) | Doğruluk ispatı, eğitim ve baseline testler |
| **Memoization** | Top-Down DP (Not Etme) | %100 Optimal | $O(N \cdot W)$ | $O(N \cdot W) + O(N)$ | $W$ kapasitesi (Stack Overflow riski) | Seyrek durum uzaylarında sadece gerekli durumları çözme |
| **Bottom-Up DP** | Tabulation (Tablo Yöntemi) | %100 Optimal | $O(N \cdot W)$ | $O(N \cdot W)$ $\rightarrow$ $$*O(W)* | $W$ kapasitesi (Pseudo-Polynomial) | Küçük/orta $W$ değerlerinde maksimum işlemci hızı |
| **Branch & Bound** | Best-First Search & Pruning | %100 Optimal | $O(2^N)$ *(Pratikte çok hızlı)* | $O(2^N)$ | Değer/Ağırlık oran varyansı | $W$ kapasitesinin devasa ($10^9$) olduğu kesin çözümler |
| **FPTAS** | Value Scaling + Value DP | $(1 - \epsilon) \cdot \text{OPT}$ | $O\left(N^3 \cdot \frac{1}{\epsilon}\right)$ | $O\left(N^2 \cdot \frac{1}{\epsilon}\right)$ | Hata toleransı ($\epsilon$) kabulü | $W$ ve $V$ devasa olduğunda polimom zamanda hızlı çözüm |

---

## 2. Algoritmaların Detaylı Mimarisi ve Davranış Analizi

### 1. Pure Recursive (Saf Özyineleme)

* **Mantık:** İkili karar ağacı (Binary Decision Tree) üzerinde her eşya için "AL" veya "ALMA" seçeneklerini kaba kuvvetle dener.
* **Güçlü Yönü:** Kodlaması en basit olan, doğruluk garantisi %100 olan temel referans algoritmasıdır.
* **Zayıf Yönü:** Tekrar eden alt problemleri (Overlapping Subproblems) hafızada tutmadığı için $N > 25$ olduğunda kilitlenir.

### 2. Memoization (Top-Down DP)

* **Mantık:** Özyinelemeli arama yapısını korur; ancak hesapladığı her $(i, capacity)$ durumunu 2D bir matrise kaydeder. Aynı duruma tekrar gelindiğinde $O(1)$ sürede matristen okur.
* **Güçlü Yönü:** Tüm durum matrisini doldurmak zorunda kalmaz, sadece arama ağacının uğradığı gerekli durumları çözer.
* **Zayıf Yönü:** Derin özyinelemelerde `Call Stack Overflow` riski taşır ve fonksiyon çağırma maliyeti (`function call overhead`) vardır.

### 3. Bottom-Up DP (Tabulation)

* **Mantık:** Özyinelemeyi tamamen kaldırır. $0$ eşya ve $0$ kapasiteden başlayarak $(N+1) \times (W+1)$ boyutundaki tabloyu döngülerle aşağıdan yukarıya doldurur.
* **Güçlü Yönü:** İşlemci önbelleği (CPU Cache) dostudur, çok hızlı çalışır. $1D$ dizi dönüşümü ile bellek ihtiyacı $O(W)$'ye düşürülebilir. Stack overflow riski yoktur.
* **Zayıf Yönü:** $W$ kapasitesi $10^9$ gibi büyük bir sayı olduğunda bellek matrisi oluşturulamaz (Pseudo-polynomial kısıtı).

### 4. Branch and Bound (Dallan ve Sınırla)

* **Mantık:** Arama ağacında ilerlerken Kesirli (Fractional) Knapsack sezgisiyle bir "Üst Sınır" (Upper Bound) hesaplar. Bu tavan değer, elde bulunan güncel rekordan küçükse o dalı tek adımda budar (pruning).
* **Güçlü Yönü:** Çalışma süresi **$W$ kapasitesinden tamamen bağımsızdır**. Dinamik Programlama'nın havlu attığı devasa çanta boyutlarında milisaniyeler içinde %100 kesin optimal sonucu bulur.
* **Zayıf Yönü:** En kötü senaryoda (hiçbir dalın budanamadığı durumlar) karmaşıklığı $O(2^N)$'e düşer.

### 5. FPTAS (Fully Polynomial-Time Approximation Scheme)

* **Mantık:** Eşya değerlerini belirlenen hata toleransına ($\epsilon$) göre hesaplanan bir $K$ ölçekleme faktörüne bölerek küçültür. Küçültülmüş değerlerle Değer Tabanlı DP çalıştırır.
* **Güçlü Yönü:** Hem $W$ kapasitesinden hem de orijinal $V$ değerlerinden tamamen bağımsızdır. **Kesin Polinom Zamanda ($O(N^3 / \epsilon)$)** çalışır.
* **Zayıf Yönü:** Tam %100 kesin sonucu vermez; ancak $(1 - \epsilon) \cdot \text{OPT}$ alt sınır garantisi sunar (örneğin $\epsilon = 0.05$ ise en az %95 doğruluk).

---

## 3. Algoritma Seçim Rehberi (Decision Flowchart)

Geliştireceğiniz sistemde hangi algoritmayı seçeceğinize karar vermek için aşağıdaki mantıksal akışı takip edebilirsiniz:

```text
[Başlangıç: 0/1 Knapsack Problemi]
       │
       ├─── Eşya Sayısı Çok Küçük mü? (N <= 20)
       │       └──> EVET: Pure Recursive veya Herhangi Bir Yöntem
       │
       └─── Kesin (%100 Optimal) Çözüm Şart mı?
               │
               ├──> EVET:
               │      │
               │      ├─── Çanta Kapasitesi (W) Makul mu? (W <= 10^7)
               │      │       └──> EVET: Bottom-Up DP (Tabulation) [1D Optimizasyonlu]
               │      │
               │      └─── Çanta Kapasitesi (W) Devasa mı? (W >= 10^8)
               │              └──> EVET: Branch and Bound (Dallan ve Sınırla)
               │
               └──> HAYIR (%1 - %5 Hata Toleransı Kabul Edilebilir):
                      │
                      └──> Çanta Kapasitesi (W) ve Değerler (V) Çok Devasa:
                              └──> EVET: FPTAS (Değer Ölçekleme Yaklaşımı)

```

---

## 4. Karmaşıklık ve Performans Özeti

```text
ZAMAN PERFORMANSI (Çalışma Hızı):
   Küçük W için  : Bottom-Up DP > Memoization > Branch & Bound > FPTAS > Pure Recursive
   Devasa W için : Branch & Bound >= FPTAS >>> (DP ve Recursive Kilitlenir)

BELLEK TÜKETİMİ:
   En Düşük Bellek : Bottom-Up DP (1D Opt. - O(W)) < Pure Recursive (O(N))
   En Yüksek Bellek: Memoization (O(N*W)) / Branch & Bound (Kuyruk - O(2^N))

```

---