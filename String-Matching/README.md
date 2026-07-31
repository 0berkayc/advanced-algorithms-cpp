# String Matching (Metin İçi Desen Arama) Algoritmaları

Metin içi desen arama (String Matching), bilgisayar bilimlerinin en temel ve sık karşılaşılan problemlerinden biridir. Bu proje kapsamında, metin arama probleminin çözümünde kullanılan farklı yaklaşımlar (Kaba Kuvvet, Hash Tabanlı ve Ön Ek Tabanlı) C++ dilinde uygulanmış, zaman ve alan karmaşıklıkları açısından karşılaştırmalı olarak incelenmiştir.

---

## 1. Problem Tanımı ve Matematiksel Altyapı

* **Metin (Text - $T$):** $N$ uzunluğundaki karakter dizisi ($T[0 \dots N-1]$).
* **Kalıp (Pattern - $P$):** $M$ uzunluğundaki aranan karakter dizisi ($P[0 \dots M-1]$).
* **Amaç:** $P$'nin $T$ içerisinde geçtiği tüm $s$ kayma (shift) değerlerini ($0 \le s \le N - M$) ve $T[s \dots s+M-1] = P[0 \dots M-1]$ şartını sağlayan başlangıç indekslerini tespit etmektir.
* **Zorluk Sınıfı:** **$P$ (Polynomial Time)** sınıfındadır.

---

## 2. Uygulanan Algoritmalar

Klasör içerisinde her biri farklı matematiksel ve mantıksal yaklaşımlara sahip 3 temel algoritma yer almaktadır:

### 1. Naive String Matching (Kaba Kuvvet)
* **Klasör:** [`/Naive`](./Naive/)
* **Mantık:** Metin üzerinde $M$ uzunluğunda bir kayan pencere (sliding window) oluşturur ve her adımda pencere içindeki karakterleri $P$ ile teker teker kıyaslar.
* **Avantajı:** Herhangi bir ön işleme (pre-processing) süresi ve ek bellek gerektirmez. Sezgiseldir.
* **Dezavantajı:** Yanlış eşleşmelerde geçmiş bilgiyi kullanmaz, her defasında karakterleri baştan kıyaslar.

### 2. Rabin-Karp Algorithm (Rolling Hash)
* **Klasör:** [`/Rabin-Karp`](./Rabin-Karp/)
* **Mantık:** Karakterleri tek tek kıyaslamak yerine $P$'nin ve metindeki $M$ uzunluğundaki pencerelerin sayısal **Hash** değerlerini kıyaslar. Pencere sağa kaydıkça **Rolling Hash** formülüyle $O(1)$ sürede yeni hash hesaplanır.
* **Avantajı:** Aday olmayan pencereleri harflere bakmadan $O(1)$ sürede eler. Çoklu desen aramalarında (Multiple Pattern Matching) oldukça etkilidir.
* **Dezavantajı:** Hash çakışmaları (Spurious Hit) durumunda Naive yöntemdeki gibi karakter kontrolüne düşer.

### 3. Z-Algorithm
* **Klasör:** [`/Z-Algorithm`](./Z-Algorithm/)
* **Mantık:** Metin ve kalıbı `Pattern + "$" + Text` şeklinde birleştirir. $Z$-dizisi ve **Z-Kutusu (Z-Box)** hafızasını kullanarak geçmişte doğrulanmış eşleşmeleri tekrar kontrol etmez.
* **Avantajı:** En kötü durumda (worst-case) bile kesin olarak **$O(N + M)$ doğrusal zaman** garantisi verir.
* **Dezavantajı:** Birleştirilmiş metin ve $Z$-dizisi için girdinin boyutuyla orantılı ek bellek harcar.

---

## 3. Algoritmaların Karşılaştırma Matrisi

| Algoritma | Ön İşleme Süresi | Best Case | Average Case | Worst Case | Ek Bellek (Auxiliary Space) |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Naive** | $O(0)$ | $O(N)$ | $O(N + M)$ | $O(N \times M)$ | $O(1)$ |
| **Rabin-Karp** | $O(M)$ | $O(N)$ | $O(N + M)$ | $O(N \times M)$ | $O(1)$ |
| **Z-Algorithm** | $O(N + M)$ | $O(N + M)$ | $O(N + M)$ | $O(N + M)$ | $O(N + M)$ |

---

## 4. Kullanım Senaryoları ve Tercih Rehberi

```text
                           [String Matching Problemi]
                                       |
           +---------------------------+---------------------------+
           |                                                       |
 [Bellek Kısıtlı / Küçük Girdi]                           [Yüksek Performans / Büyük Veri]
           |                                                       |
    (Naive / O(1) Space)                                           |
                                               +-------------------+-------------------+
                                               |                                       |
                                     [Çoklu Desen / Plagiarism]               [Genetik / DNA Dizilimi]
                                               |                                       |
                                         (Rabin-Karp)                             (Z-Algorithm)
```

* **Naive Algoritması:** Bellek kullanımının kesinlikle $O(1)$ kalması gereken gömülü sistemlerde ve girdinin/kalıbın çok küçük olduğu durumlarda tercih edilir.
* **Rabin-Karp Algoritması:** Karakter çeşitliliğinin fazla olduğu doğal dil metinlerinde ve metin içinde aynı anda birden fazla kalıbın arandığı intihal/plagiarism analizlerinde tercih edilir.
* **Z-Algorithm:** En kötü durumda bile zaman garantisi aranan, DNA/RNA dizilim analizleri gibi tekrarlayan karakterlerin çok olduğu devasa veri setlerinde tercih edilir.

---

## 5. Dizin Yapısı

```text
String-Matching/
├── README.md                   # Genel bilgilendirme ve karşılaştırma dokümanı
├── Naive/
│   ├── main.cpp                # Naive algoritması C++ kaynak kodu
│   └── README.md               # Naive algoritması detaylı analizi
├── Rabin-Karp/
│   ├── main.cpp                # Rabin-Karp algoritması C++ kaynak kodu
│   └── README.md               # Rabin-Karp algoritması detaylı analizi
└── Z-Algorithm/
    ├── main.cpp                # Z-Algorithm C++ kaynak kodu
    └── README.md               # Z-Algorithm detaylı analizi
```