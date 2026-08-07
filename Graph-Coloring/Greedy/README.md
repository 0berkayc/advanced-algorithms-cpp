# Graph Coloring - Welsh-Powell (Greedy) Algoritması

Bu doküman, **Graph Coloring (Çizge Boyama)** probleminin en bilinen sezgisel çözümlerinden biri olan **Welsh-Powell (Greedy / First-Fit)** algoritmasını, teorik altyapısını, zaman/alan karmaşıklığını ve kullanım senaryolarını detaylandırmaktadır.

---

## 1. Problemin Tanımı ve Zorluğu

* **Problem Tanımı:** Yönlendirilmemiş bir $G = (V, E)$ çizgesinde, birbirine kenarla bağlı (komşu) hiçbir iki düğüm aynı rengi almayacak şekilde tüm düğümlere renk atanması ve bu işlem yapılırken **toplam kullanılan renk sayısının minimuma (Kromatik Sayı — $\chi(G)$)** indirilmesi problemidir.
* **Zorluk Sınıfı:** **NP-Hard** (Optimizasyon versiyonu). Karar versiyonu ($k \ge 3$ renk ile boyanabilirlik) **NP-Complete**'tir.
* **Hesaplama Zorluğu:** Çizgedeki düğüm sayısı ($\vert{}V\vert{}$) arttıkça, çakışmasız ve kromatik sayıyı tam veren çözümü bulmak olası kombinasyonların çokluğu nedeniyle üstel zaman gerektirir.

---

## 2. Algoritmanın Geliştirilme Amacı ve Kullanım Durumları

### Geliştirilme Amacı ve Giderdiği Eksiklik

Klasik / Rastgele Sıralı Greedy (First-Fit) algoritması, düğümleri gelişigüzel bir sırayla boyar. Düğümlerin işlenme sırası çıktıyı doğrudan etkiler ve rastgele seçimler gereksiz yere çok fazla renk harcanmasına yol açar.

**Welsh-Powell sezgiseli**, en çok komşusu olan (en yüksek dereceli) düğümlerin en kısıtlayıcı düğümler olduğu fikrinden doğmuştur. Düğümleri derecelerine göre büyükten küçüğe sıralayarak boyama kısıtını en baştan çözer ve rastgele sıralı Greedy'ye kıyasla renk kullanımını ciddi oranda düşürür.

### Önceki / Rastgele Yöntemlere Göre Avantajları

* **Hız ve Etkinlik:** Polinomsal zamanda ($O(\vert{}V\vert{} \log \vert{}V\vert{} + \vert{}E\vert{})$) çalışır. Devasa çizgelerde bile milisaniyeler içinde sonuç verir.
* **Sezgisel Başarı:** En yüksek dereceli düğümleri öne alması sayesinde, pratikte optimum çözüme ($\chi(G)$) sıklıkla çok yakın veya tam eşit sonuçlar üretir.

### Hangi Durumda Tercih Edilir?

* Binlerce düğüm ve kenardan oluşan devasa ağlarda.
* Derleyici tasarımı (Register Allocation), ders/sınav çizelgeleme ve kablosuz frekans atama gibi **hızlı yanıt verilmesi gereken gerçek zamanlı sistemlerde**.
* Tam optimumun şart olmadığı, "yeterince iyi ve hızlı" bir boyamanın kabul edildiği durumlarda.

### Hangi Durumda Tercih Edilmez?

* Hata toleransının sıfır olduğu ve kesinlikle kromatik sayının ($\chi(G)$) bulunmasının şart olduğu hassas optimizasyon problemlerinde (Bu durumlarda Exact Backtracking veya DSatur tercih edilmelidir).

---

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir

> *"Aynı anda çok fazla düğüme bağlı olan (yüksek dereceli) zor düğümleri en başta boya. Böylece renk kısıtı en fazla olan elemanlar ilk adımlarda elenmiş olur."*

### Çalışma Prensibi

1. Çizgedeki tüm düğümlerin dereceleri (komşu sayıları) hesaplanır.
2. Düğümler derecelerine göre **büyükten küçüğe** sıralanır.
3. Sıralı listedeki her bir düğüm $u$ için:
* $u$'nun boyanmış komşularının renkleri kontrol edilir ve yasaklanır.
* $u$'ya komşularının kullanmadığı **boşta olan en küçük renk numarası ($0, 1, 2, \dots$)** atanır.


4. Tüm düğümler boyanana kadar işlem devam eder.

---

## 4. Veri Yapıları

Algoritmada kullanılan temel C++ veri yapıları ve işlevleri şunlardır:

* `const std::vector<std::vector<int>>& neighbors`: Bitişiklik Listesi (Adjacency List). Komşuluk ilişkilerini ve düğüm derecelerini (`neighbors[u].size()`) $O(1)$ sürede tutar.
* `std::vector<int> color`: Düğümlere atanan renk numaralarını tutan $N$ boyutlu dizi (başlangıçta `-1`).
* `std::vector<int> nodes`: Düğüm indekslerini sıralamak üzere saklayan dizi.
* `std::vector<bool> available`: Her boyama adımında boyanacak düğümün komşularının kullandığı renkleri `false` yaparak yasaklayan ve en küçük kullanılabilir rengi tespit eden geçici kontrol dizisi.

---

## 5. Sözde Kod (Pseudo-Code)

```text
greedyColoring(numVertices, neighbors):
numVertices (Düğüm sayısı), neighbors (Bitişiklik Listesi)
color (Her düğümün renk numarasını tutan dizi)

    renk ← numVertices boyutunda -1 ile doldurulmuş dizi
    düğümler ← [0, 1, 2, ..., numVertices - 1]

    // 1. Düğümleri derecelerine göre azalan sırada sırala
    düğümler dizisini neighbors[a].size() > neighbors[b].size() kuralına göre sırala

    // 2. Düğümleri sırayla boya
    HER u İÇİN düğümler İÇİNDEKİ:
        kullanılabilir ← numVertices boyutunda Doğru ile doldurulmuş dizi

        // u'nun komşularının renklerini yasakla
        HER v İÇİN neighbors[u] İÇİNDEKİ:
            EĞER renk[v] ≠ -1 İSE:
                kullanılabilir[renk[v]] ← Yanlış


        // Kullanılabilecek en küçük boş rengi bul (0, 1, 2...)
        atananRenk ← 0
        HER cr İÇİN 0'dan (numVertices - 1)'e KADAR:
            EĞER kullanılabilir[cr] == Doğru İSE:
                atananRenk ← cr
                DÖNGÜDEN ÇIK


        renk[u] ← atananRenk

    DÖNDÜR renk
BİTİR

```

---

## 6. Zaman Karmaşıklığı

* **Düğüm Sıralama Süresi:** $O(\vert{}V\vert{} \log \vert{}V\vert{})$
* **Düğümleri Boyama Döngüsü:** Tüm düğümler ($\vert{}V\vert{}$) ve her düğümün komşuları ($\vert{}E\vert{}$) taranır. En küçük renk arama döngüsü worst-case'de $O(\vert{}V\vert{})$ sürer.

| Durum | Karmaşıklık | Açıklama |
| --- | --- | --- |
| **Best Case** | $O(\vert{}V\vert{} \log \vert{}V\vert{} + \vert{}E\vert{})$ | Çizge az sayıda renk ile boyanabiliyorsa renk arama döngüsü $O(1)$ sürer. |
| **Average Case** | $O(\vert{}V\vert{} \log \vert{}V\vert{} + \vert{}V\vert{} + \vert{}E\vert{})$ | Pratikte kullanılan renk sayısı kısıtlı olduğundan polinomsal zaman alır. |
| **Worst Case** | $O(\vert{}V\vert{} \log \vert{}V\vert{} + \vert{}V\vert{}^2)$ | Tam çizge (Complete Graph $K_N$) gibi durumlarda kullanılan renk sayısı $\vert{}V\vert{}$'ye ulaşır. |

---

## 7. Alan Karmaşıklığı

* **Ek Bellek Kullanımı (Auxiliary Space):** $O(\vert{}V\vert{} + \vert{}E\vert{})$
* **Kullanılan Veri Yapılarının Etkisi:**
* `neighbors` (Çizge Yapısı): $O(\vert{}V\vert{} + \vert{}E\vert{})$ alan kaplar.
* `color` ve `nodes` Dizileri: Düğüm sayısı kadar $O(\vert{}V\vert{})$ alan kaplar.
* `available` Dizisi: Geçici kontrol için $O(\vert{}V\vert{})$ alan kaplar.



---

## 8. Örnek Derleme ve Çalıştırma

### Derleme

```bash
g++ -std=c++17 main.cpp -o app
./app

```

### Örnek Çıktı

```text
Düğüm Renk Atamaları:
Düğüm 0 -> Renk 2
Düğüm 1 -> Renk 0
Düğüm 2 -> Renk 1
Düğüm 3 -> Renk 2
Düğüm 4 -> Renk 0
Toplam Kullanılan Renk Sayısı: 3

```