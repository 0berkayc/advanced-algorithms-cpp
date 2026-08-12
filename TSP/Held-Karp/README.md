# Traveling Salesperson Problem (TSP) - Held-Karp Yaklaşımı

Bu doküman, **Gezgin Satıcı Problemi'nin (Traveling Salesperson Problem - TSP)** **Dinamik Programlama (Dynamic Programming - DP)** ve **Bitmask (Bit Maskeleme)** teknikleri kullanılarak kesin optimal çözüme ulaştırıldığı **Held-Karp** algoritmasının teorik esaslarını, çalışma prensibini, sözde kodunu ve karmaşıklık analizini içermektedir.

---

## 1. Problemin Tanımı, Zorluğu ve Geliştirilme Amacı

* **Problem Tanımı:** Verilen $N$ adet şehir ve şehirler arasındaki mesafe matrisine göre; bir satıcının her şehre **tam olarak bir kez** uğraması ve başladığı şehre geri dönmesi şartıyla katetmesi gereken **en kısa (minimum) toplam yolu** bulma problemidir.
* **Zorluk Sınıfı:** **NP-Hard** zorluk sınıfındadır.
* **Geliştirilme Amacı:** Kaba Kuvvet (Brute Force) algoritmasının $O(N!)$ olan zaman karmaşıklığını, **Dinamik Programlama** prensibiyle tekrar eden alt problemleri (Overlapping Subproblems) hafızaya kaydederek **$O(N^2 \cdot 2^N)$** seviyesine düşürmek ve kesin çözümü garanti eden en hızlı algoritmayı elde etmektir.

---

## 2. Hangi Eksikliği Gidermektedir ve Avantajları Nelerdir?

* **Giderdiği Eksiklik:** Kaba Kuvvet algoritmasının aynı şehir kümeleri için aynı alt rotaları defalarca baştan hesaplama (redunant computation) zayıflığını tamamen ortadan kaldırır.
* **Önceki Yöntemlere Göre Avantajları:**
* **Devasa Hız Artışı:** $N = 20$ şehirlik bir haritada Kaba Kuvvet $\approx 2.4 \times 10^{18}$ işlem yaparken, Held-Karp sadece $\approx 4 \times 10^8$ işlemle %100 kesin optimal sonucu bulur.
* **$O(1)$ Doğrudan Erişim:** Şehir alt kümeleri tamsayı mantıksal bitleri (bitmask) ile ifade edildiği için durum tablosundaki (`dp[mask][u]`) değerlere diziler üzerinden $O(1)$ sürede erişilir.
* **Kararlı Performans:** Branch and Bound algoritması gibi verinin yapısına (matris içeriğine) göre zaman değiştirmestir; en kötü senaryoda bile süresi sabittir.



### Hangi Durumda Tercih Edilir?

* %100 kesin optimal sonucun şart olduğu ve $N \le 20 - 22$ aralığındaki şehir haritalarında.
* Küçük/orta ölçekli problemlerde en hızlı kesin çözücüye ihtiyaç duyulduğunda.

### Hangi Durumda Tercih EDİLMEZ?

* $N > 23$ olduğu durumlarda. Durum tablosunun boyutu ($2^N \times N$) RAM kapasitesini (üstsel bellek sınırı) aşacağı için `Out of Memory` hatası verir.
* Milisaniyeler seviyesinde cevap üretilmesi gereken binlerce şehirlik büyük ölçekli lojistik ve kargo dağıtım senaryolarında (bu durumlarda LKH, 2-Opt veya Karınca Kolonisi tercih edilir).

---

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir (Aynı Küme, Aynı Son Şehir = Tekrar Etme!)

> *"Gezilen şehir kümesi $S$ ve bulunulan son şehir $u$ aynı ise, bu duruma gelene kadar hangi rotalardan geçildiğinin bir önemi yoktur. Önemli olan o anki $S$ kümesine $u$ şehrinde sonlanacak şekilde ulaşmanın MINIMUM maliyetidir."*

### Çalışma Prensibi

1. **Bitmasking ile Küme Temsili:** $N$ elemanlı bir şehir kümesi $N$ bitlik bir tamsayı ile ifade edilir. $i$. bitin `1` olması $i$. şehrin gezildiğini gösterir (Örn: `1101` $\rightarrow$ 0, 2 ve 3. şehirler gezildi).
2. **DP Durumu (State):** `dp[mask][u]`, `mask` ile belirtilen şehirler gezilmiş ve en son $u$ şehrinde bulunuluyor iken katledilen minimum mesafedir.
3. **Geçiş Denklemi:**

$$dp[mask \mid (1 \ll v)][v] = \min \Big( dp[mask \mid (1 \ll v)][v], \; dp[mask][u] + dist[u][v] \Big)$$



*(Burada $v \notin mask$ şartı vardır).*
4. **Geriye Doğru Yol Çıkarma (Backtracking):** Tüm şehirler gezildikten sonra (`full_mask = 2^N - 1`), son bulunulan $v$ şehrinden $0$'a dönüş mesafesi eklenerek en kısa tur bulunur. `parent[mask][u]` tablosu kullanılarak izlenen yol geriye doğru çıkarılır.

---

## 4. Veri Yapıları

* **`dist` (2D Dizi / Matris):** $N \times N$ boyutunda şehirler arası mesafeleri tutan matris (`dist[i][j]`).
* **`dp` (2D Dizi):** $2^N \times N$ boyutunda durum tablosu. `dp[mask][u]` değerini saklar.
* **`parent` (2D Dizi):** $2^N \times N$ boyutunda izlenen rotayı geriye doğru yeniden oluşturmak için kullanılan ebeveyn tablosu.

---

## 5. Sözde Kod (Pseudo-Code)

```text
TSPHeldKarp(dist_matrix):
    N ← dist_matrix.size()
    num_states ← 2^N

    dp ← num_states x N boyutunda INF ile dolu 2D matris
    parent ← num_states x N boyutunda -1 ile dolu 2D matris

    // Base Case: Sadece 0. şehir gezildi
    dp[1][0] ← 0

    // Tüm mask durumlarını tara
    HER mask İÇİN 1'den num_states-1'e KADAR:
        HER u İÇİN 0'dan N-1'e KADAR:
            // u şehri mask içinde yoksa veya u durumuna ulaşılamadıysa atla
            EĞER (mask & (1 << u)) == 0 VEYA dp[mask][u] == INF İSE:
                DEVAM ET

            // Ziyaret edilmemiş bir v şehrine geç
            HER v İÇİN 0'dan N-1'e KADAR:
                EĞER (mask & (1 << v)) != 0 İSE: // v zaten gezildiyse atla
                    DEVAM ET

                next_mask ← mask | (1 << v)
                new_cost ← dp[mask][u] + dist_matrix[u][v]

                EĞER new_cost < dp[next_mask][v] İSE:
                    dp[next_mask][v] ← new_cost
                    parent[next_mask][v] ← u

    // Başlangıç noktasına (0. şehre) dönüş hesabı
    full_mask ← num_states - 1
    min_cost ← INF
    last_city ← -1

    HER v İÇİN 1'den N-1'e KADAR:
        EĞER dp[full_mask][v] != INF İSE:
            total_cost ← dp[full_mask][v] + dist_matrix[v][0]
            EĞER total_cost < min_cost İSE:
                min_cost ← total_cost
                last_city ← v

    // Parent tablosundan rotayı çıkar (Backtracking)
    path ← boş liste
    curr_mask ← full_mask
    curr_city ← last_city

    path.push(0)
    curr_city != -1 EŞİT İKEN:
        path.push(curr_city)
        prev_city ← parent[curr_mask][curr_city]
        curr_mask ← curr_mask XOR (1 << curr_city)
        curr_city ← prev_city

    path dizisini TERSİNE ÇEVİR

    DÖNDÜR min_cost, path


```

---

## 6. Zaman Karmaşıklığı

* **Best Case:** $O(N^2 \cdot 2^N)$
* **Average Case:** $O(N^2 \cdot 2^N)$
* **Worst Case:** $O(N^2 \cdot 2^N)$
* **Açıklama:** Toplam $2^N$ adet alt küme durumu vardır. Her durum için $N$ adet şehir son konum olabilir ($N \cdot 2^N$ toplam durum). Her durumdan henüz gezilmeyen diğer $N$ şehre geçiş denenir. Dolayısıyla toplam işlem sayısı $N \cdot N \cdot 2^N = O(N^2 \cdot 2^N)$ olur.



---

## 7. Alan Karmaşıklığı

* **Ek Bellek Kullanımı (Auxiliary Space):** $O(N \cdot 2^N)$
* **Kullanılan Veri Yapılarının Etkisi:**
* **`dp` ve `parent` Tabloları:** Her biri $2^N \times N$ boyutunda matrislerdir. $N = 20$ için bu tablolar hafızada yaklaşık $80 \text{ MB}$ yer tutarken, $N = 30$ olduğunda bellek ihtiyacı yüzlerce GB seviyesine çıkarak bellek sınırına (Memory Wall) takılır.