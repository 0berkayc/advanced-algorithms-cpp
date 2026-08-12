# Traveling Salesperson Problem (TSP) Çözüm Modülü - Algoritma Karşılaştırma ve Mimari Rehber

Bu modül, bilgisayar bilimlerinin en temel **NP-Hard** problemlerinden biri olan **Gezgin Satıcı Problemi'nin (Traveling Salesperson Problem - TSP)** 4 farklı algoritma tekniği ile gerçekleştirilmiş C++ çözümlerini ve teorik analizlerini içermektedir.

TSP'de olası rota sayısı şehir sayısı ($N$) arttıkça faktöriyel ($N!$) olarak büyüdüğü için tek bir evrensel algoritma tüm senaryolarda mükemmel çalışmaz. Şehir sayısı ($N$), kabul edilebilir zaman kısıtı ve kesinlik (optimal) ihtiyacına göre doğru algoritmanın seçilmesi gerekir.

---

## 1. Genel Karşılaştırma Tablosu

| Algoritma | Yaklaşım Tipi | Doğruluk Garantisi | Zaman Karmaşıklığı (Worst Case) | Alan Karmaşıklığı | Pratik $N$ Sınırı | İdeal Kullanım Senaryosu |
| --- | --- | --- | --- | --- | --- | --- |
| **Brute Force** | Kaba Kuvvet (Permütasyon) | %100 Optimal | $O(N!)$ | $O(N)$ | $N \le 12$ | Küçük veri setleri, doğruluk ispatı ve unit testler |
| **Nearest Neighbor** | Açgözlü Sezgisel (Greedy) | Yaklaşık (%80–%90) | $O(N^2)$ | $O(N)$ | $N \le 100.000+$ | Anlık kargo/kurye rotalaması, 2-Opt öncesi taslak rota üretimi |
| **Branch & Bound** | Best-First Search + Pruning | %100 Optimal | $O(N^2 \cdot N!)$ | $O(N^3 \cdot 2^N)$ | $N \le 25$ | DP bellek sınırına takılmadan kesin optimal çözüm arayışı |
| **Held-Karp** | Dinamik Programlama + Bitmask | %100 Optimal | $O(N^2 \cdot 2^N)$ | $O(N \cdot 2^N)$ | $N \le 22$ | $N \le 20$ bandında kesin çözümü veren en kararlı ve hızlı yöntem |

---

## 2. Algoritmaların Detaylı Mimarisi ve Davranış Analizi

### 1. Brute Force (Kaba Kuvvet)

* **Mantık:** 0. şehri sabit tutup geriye kalan $N-1$ şehrin olası tüm permütasyonlarını dener. Her permütasyon için toplam tur maliyetini hesaplayıp en küçüğünü rekor olarak saklar.
* **Güçlü Yönü:** Kodlaması son derece basittir. Matematiksel olarak %100 kesin (optimal) sonucu bulur.
* **Zayıf Yönü:** Arama uzayı $O(N!)$ faktöriyel hızda büyüdüğü için $N > 12$ olduğunda kilitlenir.

### 2. Nearest Neighbor (En Yakın Komşu)

* **Mantık:** Bulunulan şehirden bencilce (greedy) bir karar vererek henüz ziyaret edilmemiş **en yakın komşu şehre** atlar. Tüm şehirler gezilince başlangıç noktasına döner.
* **Güçlü Yönü:** Polinom zamanda ($O(N^2)$) çalışır. Yüz binlerce şehri saliseler içinde çözebilir.
* **Zayıf Yönü:** Miyopiktir (geleceği görmez). İlk adımlarda kısa mesafeleri tükettiği için haritanın sonlarına doğru uzaktaki şehirlere uzun sıçramalar yapmak zorunda kalabilir. Optimali garanti etmez.

### 3. Branch and Bound (Dallan ve Sınırla - İndirgenmiş Matris)

* **Mantık:** Her adımda satır/sütun indirgemesi yaparak o düğüm için bir **Alt Sınır ($LB - Lower\ Bound$)** hesaplar. Hesaptaki $LB$, eldeki mevcut rekor maliyetten (`best_cost`) daha kötüyse o daldan aşağıya **hiç inmez (pruning)**. Min-Öncelikli Kuyruk (`priority_queue`) ile en potansiyelli düğümü öne alır.
* **Güçlü Yönü:** Kesin optimal çözümü verir. Güçlü $LB$ budaması sayesinde $N!$ olan arama uzayının %90'ından fazlasını hiç girmeden eler.
* **Zayıf Yönü:** Her düğümde $N \times N$ matris kopyalaması yaptığı için bellek tüketimi oldukça yüksektir.

### 4. Held-Karp (Dinamik Programlama + Bitmask)

* **Mantık:** Gezilen şehir kümesini tamsayı bitleri ile ifade eder (`bitmask`). Tekrar eden alt problemleri (Overlapping Subproblems) `dp[mask][u]` tablosunda saklar. Aynı şehir kümesi gezilip aynı $u$ noktasına gelindiğinde geçmişi tekrar hesaplamaz.
* **Güçlü Yönü:** $N \le 20$ bandındaki kesin çözümlerde **teorik ve pratik olarak en hızlı algoritmadır**. Süresi veri içeriğinden bağımsız olarak kararlıdır.
* **Zayıf Yönü:** Durum tablosu boyutu ($2^N \times N$) üstel olarak büyüdüğü için $N \ge 23$ seviyelerinde RAM sınırına (`Out of Memory`) takılır.

---

## 3. Algoritma Seçim Rehberi (Decision Flowchart)

Geliştirilecek sistemde hangi TSP algoritmasının seçileceğine karar vermek için aşağıdaki mantıksal akış takip edilebilir:

```text
[Başlangıç: TSP Problemi]
       │
       ├─── Şehir Sayısı Çok mu Küçük? (N <= 12)
       │       └──> EVET: Brute Force (En Basit Kesin Çözüm)
       │
       └─── Kesin (%100 Optimal) Çözüm Şart mı?
               │
               ├──> EVET:
               │      │
               │      ├─── Şehir Sayısı N <= 22 aralığında mı?
               │      │       └──> EVET: Held-Karp (DP + Bitmask) [En Hızlı Kesin Çözüm]
               │      │
               │      └─── Şehir Sayısı N = 23 - 28 aralığında mı?
               │              └──> EVET: Branch and Bound (İndirgenmiş Matris)
               │
               └──> HAYIR (Milisaniyeler İçinde Hızlı Çözüm Yeterli):
                      │
                      └──> Nearest Neighbor (Açgözlü Sezgisel)

```

---

## 4. Karmaşıklık ve Performans Özeti

```text
ZAMAN PERFORMANSI (Çalışma Hızı):
   N <= 12 için : Held-Karp > Branch & Bound > Nearest Neighbor > Brute Force
   N > 25 için  : Nearest Neighbor >>> (Diğer hepsi zaman veya bellek sınırına takılır)

BELLEK TÜKETİMİ:
   En Düşük Bellek : Brute Force (O(N)) == Nearest Neighbor (O(N))
   En Yüksek Bellek: Branch & Bound (O(N^3 * 2^N)) > Held-Karp (O(N * 2^N))

```