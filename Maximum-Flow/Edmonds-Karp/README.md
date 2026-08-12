# Maximum Flow - Edmonds-Karp Algoritması

Bu doküman, bir yönlü şebeke ağında **Kaynaktan ($S$) Hedefe ($T$)** ulaştırılabilecek **Maksimum Akış (Maximum Flow)** miktarını hesaplayan, Ford-Fulkerson metodunun **BFS (En Kısa Yol)** tabanlı somutlaştırması olan **Edmonds-Karp** algoritmasının teorik esaslarını, çalışma prensibini, sözde kodunu ve karmaşıklık analizini içermektedir.

---

## 1. Problemin Tanımı, Zorluğu ve Geliştirilme Amacı

* **Problem Tanımı:** Yönlü, kenarları kapasitelerle sınırlandırılmış bir çizgede (graf); kaynak düğümden ($S$) çıkan ve hedef düğüme ($T$) ulaşan akışın, kenar kapasitelerini aşmadan ve ara düğümlerde akışın korunması kuralına ($\sum \text{giren} = \sum \text{çıkan}$) uyarak elde edebileceği **maksimum toplam debiyi/miktarı** bulma problemidir.
* **Zorluk Sınıfı:** **P** (Polinom Zamanda Çözülebilir) sınıfındadır.
* **Geliştirilme Amacı:** Standart Ford-Fulkerson metodunda DFS kullanılması durumunda oluşan $O(E \cdot \vert{}f_{max}\vert{})$ karmaşıklığını ortadan kaldırmaktır. Edmonds-Karp algoritması, artırıcı yol aramada **BFS (En Kısa Yol)** kullanarak algoritma süresini akış/kapasite büyüklüklerinden tamamen bağımsız kılmış ve **güçlü polinom zamanda (strongly polynomial time)** garanti çözüm sunmak amacıyla geliştirilmiştir.

---

## 2. Hangi Eksikliği Gidermektedir ve Avantajları Nelerdir?

* **Giderdiği Eksiklik:** Ford-Fulkerson metodunun kenar kapasitelerine ($\vert{}f_{max}\vert{}$) bağlı kalması zayıflığını giderir. Kapasitelerin $1.000.000+$ gibi büyük tamsayılar olduğu veya irrasyonel sayılardan oluştuğu durumlarda Ford-Fulkerson'un kilitlenmesini ya da sonsuz döngüye girmesini engeller.
* **Önceki Yöntemlere Göre Avantajları:**
* **Kapasiteden Bağımlısızlık:** Çalışma süresi akış değerinden değil, sadece düğüm ($V$) ve kenar ($E$) sayısından etkilenir.
* **Garantili Üst Sınır:** Zaman karmaşıklığı kesin olarak **$O(V \cdot E^2)$** ile sınırlandırılmıştır.
* **En Az Kenarlı Yol (Shortest Path):** Her adımda kaynaktan hedefe giden en az kenar sayısına sahip artırıcı yol seçilir.



### Hangi Durumda Tercih Edilir?

* Kenar kapasitelerinin çok yüksek olduğu veya dinamik olarak değiştiği ağ yapılarında.
* Dinic algoritmasının getirdiği karmaşıklığa ihtiyaç duyulmayan, orta ölçekli graflarda ($V \le 1.000$, $E \le 10.000$) basit ve güvenilir bir Maksimum Akış çözümü gerektiğinde.

### Hangi Durumda Tercih EDİLMEZ?

* Düğüm ve kenar sayısının çok yüksek olduğu büyük ölçekli ve yoğun (dense) graflarda ($O(V \cdot E^2)$ süresi yavaş kalabilir).
* Bu tür büyük graflarda Seviye Grafı (Level Graph) ve Engelleme Akışı (Blocking Flow) kullanan **Dinic Algoritması ($O(V^2 \cdot E)$)** veya **Push-Relabel** algoritmaları tercih edilir.

---

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir (BFS ile En Kısa Yol Arama)

> *"Kalıntı graf üzerinde kaynaktan hedefe giden yolları ararken derinlemesine (DFS) değil, sığlamasına (BFS) ilerle. Her adımda EN AZ kenar sayısına sahip yolu seç ki, bir kenar en fazla $O(V)$ kez darboğaz olabilsin ve toplam adım sayısı $O(V \cdot E)$ ile sınırlansın."*

### Çalışma Prensibi

1. **Kalıntı Graf (Residual Graph - $G_f$):** İleri yönlü kalan kapasiteler $c_f(u,v) = c(u,v) - f(u,v)$ ve geri yönlü iptal kapasiteleri $c_f(v,u) = f(u,v)$ saklanır.
2. **BFS ile Artırıcı Yol Arama:** $S$'den $T$'ye sadece kalan kapasitesi $c_f(u,v) > 0$ olan kenarlar kullanılarak BFS çalıştırılır. BFS ilk kez $T$'ye ulaştığında bulunan yol, kenar sayısı bakımından **en kısa artırıcı yoldur**.
3. **Şişe Boynu (Bottleneck) Tespiti:** Bulunan yol üzerindeki kenarların kalan kapasitelerinin en küçüğü belirlenir ($b = \min \{c_f(u,v)\}$).
4. **Kapasite Güncellemesi:**
* Yol üzerindeki ileri kenarlardan $b$ düşülür: $c_f(u,v) \leftarrow c_f(u,v) - b$
* Yol üzerindeki geri kenarlara $b$ eklenir: $c_f(v,u) \leftarrow c_f(v,u) + b$


5. **Döngü:** BFS ile $S$'den $T$'ye ulaşan boş bir yol kalmayana kadar adımlar tekrarlanır.

---

## 4. Veri Yapıları

* **`capacity` (2D Dizi / Matris veya Komşuluk Listesi):** Kalıntı kapasiteleri saklayan matris/liste.
* **`queue` (Kuyruk):** BFS algoritmasının katman katman (level-by-level) ilerlemesini sağlayan $O(V)$ alanlı kuyruk yapısı.
* **`parent` (1D Dizi):** BFS ağacında her düğümün ebeveynini tutar; hedefe ulaşıldığında $T \to S$ rotasını geriye doğru takip etmeyi sağlar.

---

## 5. Sözde Kod (Pseudo-Code)

```text
EdmondsKarp(graph, source, sink):
    V ← graph.node_count
    capacity ← graph.capacity_matrix
    max_flow ← 0

    parent ← V boyutunda -1 ile dolu dizi

    // Kalıntı grafta BFS ile yol bulunduğu sürece döngü devam eder
     BFS(source, sink, parent, capacity) == TRUE OLDUĞU SÜRECE:
        
        // 1. Şişe boynu (bottleneck) kapasitesini bul
        path_flow ← INF
        v ← sink
        v != source OLDUĞU SÜRECE:
            u ← parent[v]
            path_flow ← MIN(path_flow, capacity[u][v])
            v ← u

        // 2. Kalıntı grafı güncelle (İleri kenardan düş, geri kenara ekle)
        v ← sink
        v != source OLDUĞU SÜRECE:
            u ← parent[v]
            capacity[u][v] ← capacity[u][v] - path_flow
            capacity[v][u] ← capacity[v][u] + path_flow
            v ← u

        max_flow ← max_flow + path_flow

    DÖNDÜR max_flow

FONKSİYON BFS(source, sink, parent, capacity):
    parent dizisini -1 ile doldur
    parent[source] ← source

    q ← boş Kuyruk (Queue)
    q.push(source)

    q BOŞ OLMADIĞI SÜRECE:
        u ← q.pop()

        HER v İÇİN 0'dan V-1'e KADAR:
            // Henüz ziyaret edilmediyse VE kalan kapasite > 0 ise
            EĞER parent[v] == -1 VE capacity[u][v] > 0 İSE:
                parent[v] ← u
                EĞER v == sink İSE:
                    DÖNDÜR TRUE  // Hedefe ulaşan en kısa yol bulundu
                q.push(v)

    DÖNDÜR FALSE  // Hedefe ulaşan yol kalmadı

```

---

## 6. Zaman Karmaşıklığı

* **Best Case:** $O(E)$
* *Açıklama:* Kaynaktan hedefe ulaşan ilk BFS yolunda tüm akışın taşındığı veya $S$'den $T$'ye hiç yolun olmadığı durumdur.


* **Average Case:** $O(V \cdot E^2)$
* **Worst Case:** $O(V \cdot E^2)$
* *Açıklama:*
1. Tek bir BFS araması $O(E)$ sürer.
2. BFS her adımda en kısa yolu seçtiği için, her bir kenar en fazla $O(V)$ defa bir yolun "şişe boynu" (bottleneck) olabilir.
3. Toplam kenar sayısı $E$ olduğuna göre, bulunabilecek maksimum artırıcı yol sayısı $O(V \cdot E)$ ile sınırlandırılmıştır.
4. Toplam Zaman Karmaşıklığı: $O(E) \times O(V \cdot E) = \mathbf{O(V \cdot E^2)}$.





---

## 7. Alan Karmaşıklığı

* **Ek Bellek Kullanımı (Auxiliary Space):**
* **Matris Gösterimi İle:** $O(V^2)$
* **Komşuluk Listesi İle:** $O(V + E)$


* **Kullanılan Veri Yapılarının Etkisi:**
* BFS araması için kullanılan `queue` ve `parent` dizisi hafızada sadece $O(V)$ kadar ek yer tutar.
* $V \times V$ kalıntı matrisi yerine komşuluk listesi (`std::vector<Edge>`) kullanıldığında toplam alan $O(V + E)$ seviyesine çekilebilir.