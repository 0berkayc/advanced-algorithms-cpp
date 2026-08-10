# Graph Coloring - DSATUR (Degree of Saturation) Algoritması

Bu doküman, **Graph Coloring (Çizge Boyama)** probleminin dinamik kısıt tabanlı en popüler sezgisel çözümlerinden biri olan **DSATUR (Degree of Saturation)** algoritmasını, teorik altyapısını, zaman/alan karmaşıklığını ve kullanım senaryolarını detaylandırmaktadır.

---

## 1. Problemin Tanımı ve Zorluğu

* **Problem Tanımı:** Yönlendirilmemiş bir $G = (V, E)$ çizgesinde, komşu olan hiçbir iki düğüm aynı rengi almayacak şekilde tüm düğümlere renk atanması ve kullanılan **toplam renk sayısının minimuma (Kromatik Sayı — $\chi(G)$)** indirilmesi problemidir.
* **Zorluk Sınıfı:** **NP-Hard** (Optimizasyon versiyonu). Verilen bir $k$ sayısı için çizgenin $k$ renkle boyanabilir olup olmadığını belirleme kararı ise **NP-Complete**'tir.
* **Geliştirilme Amacı:** Daniel Brélaz tarafından 1979 yılında geliştirilen DSATUR; statik sıralama yapan sezgisellerin yetersiz kaldığı durumlarda, boyama süreci ilerledikçe kısıtları **dinamik olarak yeniden hesaplayarak** kromatik sayıya daha yakın sonuçlar elde etmek amacıyla tasarlanmıştır.

---

## 2. Hangi Eksikliği Gidermektedir?

Welsh-Powell gibi klasik Greedy yöntemleri düğümleri boyama işleminden **önce tek bir kez (statik)** sıralar. Ancak boyama süreci ilerledikçe, başlangıçta az komşusu olan bir düğümün komşuları farklı farklı renklere boyanabilir ve o düğüm bir anda "en çok kısıtlanan düğüm" haline gelebilir.

Statik yöntemler bu durumu fark edemez. **DSATUR**, her boyama adımında komşularda kullanılan **farklı renklerin sayısını ($\text{sat}$ - doygunluk derecesi)** takip ederek, boyama anında **en çaresiz / en kısıtlı kalmış düğümü** tespit eder ve bu eksikliği ortadan kaldırır.

---

## 3. Avantajları ve Kullanım Durumları

### Önceki Yöntemlere Göre Avantajları

* **Dinamik Kısıt Analizi:** Her adımda çizgenin anlık durumuna göre karar verir.
* **Yüksek Başarım Oranı:** Sezgisel (heuristic) yöntemler arasında **optimum kromatik sayıya ($\chi(G)$) en sık ulaşan veya en yakın sonucu veren** algoritmalardan biridir.
* **Akıllı Eşitlik Bozma (Tie-Breaking):** Doygunluk dereceleri eşit olduğunda orijinal dereceye bakarak kararsızlığı ortadan kaldırır.

### Hangi Durumda Tercih Edilir?

* Tam kesin çözüm veren (Exact Backtracking) algoritmaların üstel zaman karmaşıklığı ($O(2^V)$) nedeniyle kilitlendiği orta ve büyük ölçekli karmaşık çizgelerde.
* Derleyici yazmaç tahsisi (Register Allocation), sınav/ders takvimi oluşturma ve frekans atama gibi **yüksek kaliteli boyama performansının kritikli taşıdığı** uygulamalarda.

### Hangi Durumda Tercih Edilmez?

* Milyonlarca düğüm içeren devasa çizgelerde her adımda dinamik arama yapmanın getirdiği yük fazla gelebilir (bu durumda $O(\vert{}V\vert{} \log \vert{}V\vert{} + \vert{}E\vert{})$ olan statik Welsh-Powell daha hızlıdır).
* Sınır şartlarının %100 kesin matematiksel optimumu gerektirdiği durumlarda (Exact / ILP çözümleri kullanılmalıdır).

---

## 4. Algoritmanın Çalışma Mantığı

### Temel Fikir

> *"Her adımda, komşuları tarafından en çok farklı renkle kuşatılmış (doygunluk derecesi en yüksek) düğümü boya."*

### Çalışma Prensibi

1. Tüm düğümler boyanmamış (`color = -1`) olarak başlatılır.
2. İlk adımda tüm düğümlerin doygunluk derecesi ($\text{sat}$) 0 olduğu için **en yüksek orijinal dereceye** sahip düğüm seçilir ve `0` rengi atanır.
3. Sonraki her adımda, henüz boyanmamış düğümler arasından:
* **1. Öncelik:** Doygunluk derecesi ($\text{sat}(v)$ = komşularındaki farklı renk sayısı) **en yüksek** düğüm seçilir.
* **2. Öncelik (Eşitlik Durumunda):** Orijinal komşu sayısı (derecesi) **en yüksek** düğüm seçilir.


4. Seçilen düğüme, komşularının kullanmadığı **en küçük tamsayı rengi ($0, 1, 2, \dots$)** atanır.
5. Boyanan düğümün komşularının doygunluk kümelerine bu yeni renk eklenir.
6. Tüm düğümler boyanana kadar 3, 4 ve 5. adımlar tekrarlanır.

---

## 5. Veri Yapıları

* `const std::vector<std::vector<int>>& neighbors`: Bitişiklik Listesi (Adjacency List). Komşuluk ilişkilerini saklar.
* `std::vector<int> color`: Düğümlerin atanan renk numaralarını tutan dizi (başlangıçta `-1`).
* `std::vector<std::set<int>> neighbor_colors`: Her düğümün komşularında kullanılan **farklı renklerin kümesini** saklar. `neighbor_colors[u].size()` ifadesi doğrudan $u$ düğümünün doygunluk derecesini ($\text{sat}$) verir.
* `std::vector<bool> used`: Seçilen düğümün komşu renklerini işaretleyip en küçük boş rengi bulan geçici kontrol dizisi.

---

## 6. Sözde Kod (Pseudo-Code)

```text
dsaturColoring(num_vertices, neighbors)
num_vertices (Düğüm sayısı), neighbors (Bitişiklik Listesi)
color (Her düğümün renk numarasını tutan dizi)

    renk ← num_vertices boyutunda -1 ile doldurulmuş dizi
    komşu_renkler ← num_vertices boyutunda boş kümeler dizisi

    DÖNGÜ step ← 0'dan (num_vertices - 1)'e KADAR:
        en_iyi_düğüm ← -1
        maks_sat ← -1
        maks_derece ← -1

        // 1. En yüksek doygunluğa sahip boyanmamış düğümü seç
        HER u İÇİN 0'dan (num_vertices - 1)'e KADAR:
            EĞER renk[u] ≠ -1 İSE DEVAM ET

            sat ← komşu_renkler[u].size()
            derece ← neighbors[u].size()

            EĞER sat > maks_sat VEYA (sat == maks_sat VE derece > maks_derece) İSE:
                maks_sat ← sat
                maks_derece ← derece
                en_iyi_düğüm ← u

        // 2. Seçilen düğümün komşu renklerini işaretle
        kullanılan ← num_vertices boyutunda Yanlış ile doldurulmuş dizi
        HER v İÇİN neighbors[en_iyi_düğüm] İÇİNDEKİ:
            EĞER renk[v] ≠ -1 İSE:
                kullanılan[renk[v]] ← Doğru

        // 3. Kullanılabilir en küçük boş rengi bul
        atanan_renk ← 0
        DÖNGÜ kullanılan[atanan_renk] == Doğru OLDUĞU SÜRECE:
            atanan_renk ← atanan_renk + 1


        renk[en_iyi_düğüm] ← atanan_renk

        // 4. Komşuların doygunluk kümelerini güncelle
        HER v İÇİN neighbors[en_iyi_düğüm] İÇİNDEKİ:
            komşu_renkler[v].ekle(atanan_renk)

    DÖNDÜR renk

```

---

## 7. Zaman Karmaşıklığı

* **Düğüm Seçim Döngüsü:** $\vert{}V\vert{}$ adım sürer.
* **En İyi Düğümü Arama:** Her adımda boyanmamış düğümler taranır $\rightarrow O(\vert{}V\vert{})$.
* **Renk Atama ve Küme Güncelleme:** Komşular gezilip `set` eklemesi yapılır $\rightarrow O(\vert{}E\vert{} \log \vert{}V\vert{})$ toplamda.

| Durum | Karmaşıklık | Açıklama |
| --- | --- | --- |
| **Best Case** | $O(\vert{}V\vert{}^2)$ | Seyrek çizgelerde arama baskındır. |
| **Average Case** | $O(\vert{}V\vert{}^2 + \vert{}E\vert{} \log \vert{}V\vert{})$ | Dizi tabanlı seçim ve `std::set` güncellemeleri ile tipik çalışma süresi. |
| **Worst Case** | $O(\vert{}V\vert{}^2)$ | Öncelikli kuyruk (Binary Heap / Max-Heap) veri yapısı kullanılarak worst-case $O((\vert{}V\vert{} + \vert{}E\vert{}) \log \vert{}V\vert{})$ seviyesine çekilebilir. |

---

## 8. Alan Karmaşıklığı

* **Ek Bellek Kullanımı (Auxiliary Space):** $O(\vert{}V\vert{}^2 + \vert{}E\vert{})$
* **Kullanılan Veri Yapılarının Etkisi:**
* `neighbors`: $O(\vert{}V\vert{} + \vert{}E\vert{})$ alan kaplar.
* `color`: $O(\vert{}V\vert{})$ alan kaplar.
* `neighbor_colors`: En kötü senaryoda her düğüm için $O(\vert{}V\vert{})$ renk saklanabileceğinden $O(\vert{}V\vert{}^2)$ alan kaplar.



---

## 9. Örnek Derleme ve Çalıştırma

### Derleme

```bash
g++ -std=c++17 main.cpp -o app
./app

```

### Örnek Çıktı

```text
düğüm renk atamaları:
düğüm 0 -> renk 2
düğüm 1 -> renk 0
düğüm 2 -> renk 1
düğüm 3 -> renk 2
düğüm 4 -> renk 0
toplam kullanılan renk sayısı: 3

```