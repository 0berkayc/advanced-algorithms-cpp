# Maximum Flow - Dinic Algoritması

Bu doküman, bir yönlü şebeke ağında **Kaynaktan ($S$) Hedefe ($T$)** ulaştırılabilecek **Maksimum Akış (Maximum Flow)** miktarını hesaplayan, **Seviye Grafı (Level Graph)** ve **Engelleme Akışı (Blocking Flow)** konseptlerini birleştirerek $O(V^2 \cdot E)$ zaman karmaşıklığına ulaşan **Dinic** algoritmasının teorik esaslarını, çalışma prensibini, sözde kodunu ve karmaşıklık analizini güncellenmiş değişken isimleriyle içermektedir.

---

## 1. Problemin Tanımı, Zorluğu ve Geliştirilme Amacı

* **Problem Tanımı:** Yönlü, kenarları kapasitelerle sınırlandırılmış bir çizgede (graf); kaynak düğümden ($S$) çıkan ve hedef düğüme ($T$) ulaşan akışın, kenar kapasitelerini aşmadan ve ara düğümlerde akışın korunması kuralına ($\sum \text{giren} = \sum \text{çıkan}$) uyarak elde edebileceği **maksimum toplam debiyi/miktarı** bulma problemidir.
* **Zorluk Sınıfı:** **P** (Polinom Zamanda Çözülebilir) sınıfındadır.
* **Geliştirilme Amacı:** Yevgeniy Dinic tarafından 1970 yılında; Ford-Fulkerson ($O(E \cdot \vert{}f_{max}\vert{})$) ve Edmonds-Karp ($O(V \cdot E^2)$) algoritmalarının büyük ve yoğun graflardaki performans sınırlamalarını aşmak amacıyla geliştirilmiştir. Dinic algoritması, akışı tek bir yolla değil, **seviye seviye toplu halde (Engelleme Akışı)** göndererek teorik karmaşıklığı **$O(V^2 \cdot E)$** seviyesine düşürmüştür.

---

## 2. Hangi Eksikliği Gidermektedir ve Avantajları Nelerdir?

* **Giderdiği Eksiklik:** Edmonds-Karp algoritması her BFS adımında sadece tek bir en kısa artırıcı yol bulup grafı günceller. Dinic algoritması ise aynı uzunluktaki (seviyedeki) **tüm artırıcı yolları tek bir fazda (DFS ile)** bularak doymuş hale getirir. Ayrıca `ptr` (dynamic pointer) dizisi sayesinde tıkanmış/dolmuş kenarları tekrar tekrar tarama israfını ortadan kaldırır.
* **Önceki Yöntemlere Göre Avantajları:**
* **Hız ve Ölçeklenebilirlik:** Genel graflarda $O(V^2 \cdot E)$ sınırıyla Edmonds-Karp'tan kat kat daha hızlı çalışır.
* **Birim Ağlarda Üstünlük:** Kenar kapasitelerinin $1$ olduğu (Bipartite Matching / İki Parçalı Eşleştirme gibi) ağlarda **$O(E \sqrt{V})$** karmaşıklığına düşerek Hopcroft-Karp algoritmasının performansına ulaşır.
* **Sektör Standardı:** Yarışmalı programlamada ve yüksek performanslı grafik kütüphanelerinde en yaygın kullanılan Maksimum Akış çözümüdür.



### Hangi Durumda Tercih Edilir?

* Büyük ölçekli, düğüm ($V > 1.000$) ve kenar ($E > 10.000$) sayısı fazla olan karmaşık şebeke problemlerinde.
* Maksimum İki Parçalı Eşleştirme (Maximum Bipartite Matching) problemlerinde.
* Zaman kısıtının çok dar olduğu performans odaklı yazılım sistemlerinde.

### Hangi Durumda Tercih EDİLMEZ?

* Çok küçük ölçekli graflarda ($V < 100$) kodlama karmaşıklığı yaratmamak adına daha basit olan Edmonds-Karp veya Ford-Fulkerson tercih edilebilir.
* Ancak genel performans ve ölçeklenebilirlik açısından Dinic algoritması hemen her Maksimum Akış senaryosunda en güçlü tercihtir.

---

## 3. Algoritmanın Çalışma Mantığı

Dinic algoritması iki ana bileşenin döngüsel olarak çalıştırılmasına dayanır:

### 1. Seviye Grafı Oluşturma (Level Graph - BFS)

Kaynaktan ($S$) başlayarak BFS çalıştırılır. Her düğüme kaynaktan olan en kısa kenar mesafesi ($level[v]$) atanır. Seviye grafı, yalnızca kaynaktan hedefe doğru 1 adım ileriye giden ($level[v] = level[u] + 1$) ve kalan kapasitesi `edge.capacity - edge.flow > 0` olan kenarları içerir.

### 2. Engelleme Akışı Bulma (Blocking Flow - DFS)

Oluşturulan Seviye Grafı üzerinde $S$'den $T$'ye ulaşan yollar kalmayana kadar DFS çalıştırılır. DFS sadece $level[v] = level[u] + 1$ şartına uyan kenarları takip eder.

### 3. Dinamik İşaretçi / Dynamic Pointer (`ptr` Array)

DFS aramalarında tıkandığı veya kapasitesi dolduğu anlaşılan kenarlar `ptr[u]` değeri artırılarak atlanır. Aynı Seviye Grafı fazındaki sonraki DFS çağrıları, dolmuş kenarları baştan itibaren **tekrar incelemez**, kaldığı komşudan devam eder ($O(1)$ amortize erişim süresi).

---

## 4. Veri Yapıları

* **`Edge` Yapısı:** `to` (hedef düğüm), `flow` (geçen akış), `capacity` (toplam kapasite) ve `reverse` (geri kenarın `neighbor[to]` içerisindeki indeks adresi) bilgilerini tutan nesne.
* **`neighbor` (Komşuluk Listesi):** `std::vector<std::vector<Edge>>` formatında tüm düğümlerin bağlı komşularını/kenarlarını tutan $O(V + E)$ alanlı ana liste.
* **`level` (1D Dizi):** $V$ boyutunda tamsayı dizisi. BFS ile hesaplanan $S$'ye olan en kısa mesafeleri ($level[v]$) tutar.
* **`ptr` (1D Dizi):** $V$ boyutunda tamsayı dizisi. DFS sırasında her düğümün işlenen son komşusunun indeksini tutarak $O(1)$ atlama sağlar.
* **`queue` (Kuyruk):** BFS algoritmasının katman katman ilerlemesini sağlayan $O(V)$ alanlı kuyruk.

---

## 5. Sözde Kod (Pseudo-Code)

```text
VERİ YAPISI Edge:
    to: Tamsayı
    flow: Tamsayı
    capacity: Tamsayı
    reverse: Tamsayı

Dinic(graph, source, sink):
    V ← graph.node_count
    neighbor ← graph.neighbor_list
    max_flow ← 0

    // Seviye grafı oluşturulabildiği sürece ana döngü çalışır
    BFS(source, sink, level, neighbor) == TRUE EŞİT OLDUĞU SÜRECE:
        ptr ← V boyutunda 0 ile dolu dizi

        // O seviye grafındaki tüm akışı (Blocking Flow) çek
        (pushed ← DFS(source, sink, INF, level, ptr, neighbor)) > 0 OLDUĞU SÜRECE:
            max_flow ← max_flow + pushed

    DÖNDÜR max_flow

BFS(source, sink, level, neighbor):
    level dizisini -1 ile doldur
    level[source] ← 0

    q ← boş Kuyruk (Queue)
    q.push(source)

    q BOŞ OLMADIĞI SÜRECE:
        u ← q.pop()

        HER edge İÇİN neighbor[u] LİSTESİNDEKİ:
            v ← edge.to
            EĞER (edge.capacity - edge.flow > 0) VE level[v] == -1 İSE:
                level[v] ← level[u] + 1
                q.push(v)

    DÖNDÜR level[sink] != -1

DFS(u, sink, pushed, level, ptr, neighbor):
    EĞER pushed == 0 İSE DÖNDÜR 0
    EĞER u == sink İSE DÖNDÜR pushed

    tr[u] < neighbor[u].size() OLDUĞU SÜRECE:
        edge ← neighbor[u][ptr[u]]
        v ← edge.to

        // Sadece 1 üst seviyedeki ve kalan kapasitesi olan düğümlere git
        EĞER level[u] + 1 == level[v] VE (edge.capacity - edge.flow > 0) İSE:
            tr_pushed ← DFS(v, sink, MIN(pushed, edge.capacity - edge.flow), level, ptr, neighbor)

            EĞER tr_pushed > 0 İSE:
                edge.flow ← edge.flow + tr_pushed
                neighbor[v][edge.reverse].flow ← neighbor[v][edge.reverse].flow - tr_pushed
                DÖNDÜR tr_pushed

        ptr[u] ← ptr[u] + 1  // Kenar tıkandıysa veya doymuşsa sonraki komşuya geç

    DÖNDÜR 0
```

---

## 6. Zaman Karmaşıklığı

* **Best Case:** $O(E)$
* *Açıklama:* $S$'den $T$'ye giden hiçbir yolun olmadığı veya ilk Seviye Grafında $T$'ye ulaşılamadığı durumdur.


* **Average Case:** $O(V^2 \cdot E)$
* **Worst Case:** $O(V^2 \cdot E)$
* *Açıklama:*
1. Seviye grafının seviyesi her BFS fazında kesin olarak en az $1$ artar. Bu nedenle BFS en fazla $O(V)$ defa çalıştırılabilir.
2. Her bir Seviye Grafı fazı üzerinde engelleme akışının (DFS) bulunması `ptr` optimizasyonu sayesinde toplam $O(V \cdot E)$ zaman alır.
3. Toplam Zaman Karmaşıklığı: $O(V) \times O(V \cdot E) = \mathbf{O(V^2 \cdot E)}$.




* **Birim Ağlar / Bipartite Matching Özel Durumu:** $O(E \sqrt{V})$
* Bütün kenar kapasitelerinin $1$ olduğu ağ yapılarında (İki Parçalı Eşleştirme gibi) Seviye Grafı sayısı en fazla $O(\sqrt{V})$ olacağından karmaşıklık **$O(E \sqrt{V})$** seviyesine düşer.



---

## 7. Alan Karmaşıklığı

* **Ek Bellek Kullanımı (Auxiliary Space):** $O(V + E)$
* **Kullanılan Veri Yapılarının Etkisi:**
* Komşuluk listesi (`neighbor`) graftaki tüm düğüm ve kenarları $O(V + E)$ alan ile tutar.
* `level` ve `ptr` dizileri bellekte $O(V)$ kadar yer kaplar.
* BFS araması için kullanılan `queue` yapısı en fazla $O(V)$ düğüm depolar.
* Toplam bellek tüketimi düğüm ve kenar sayısıyla doğru orantılı olarak son derece verimli bir biçimde **$O(V + E)$** ile sınırlandırılmıştır.