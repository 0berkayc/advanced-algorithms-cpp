# Maximum Flow - Ford-Fulkerson Metodu

Bu doküman, bir yönlü şebeke ağında **Kaynaktan ($S$) Hedefe ($T$)** ulaştırılabilecek **Maksimum Akış (Maximum Flow)** miktarını bulmayı sağlayan temel **Ford-Fulkerson** metodunun teorik esaslarını, çalışma prensibini, sözde kodunu ve karmaşıklık analizini içermektedir.

---

## 1. Problemin Tanımı, Zorluğu ve Geliştirilme Amacı

* **Problem Tanımı:** Yönlü, kenarları kapasitelerle sınırlandırılmış bir çizgede (graf); kaynak düğümden ($S$) çıkan ve hedef düğüme ($T$) ulaşan akışın, kenar kapasitelerini aşmadan ve ara düğümlerde akışın korunması kuralına ($\sum \text{giren} = \sum \text{çıkan}$) uyarak elde edebileceği **maksimum toplam debiyi/miktarı** bulma problemidir.
* **Zorluk Sınıfı:** **P** (Polinom Zamanda Çözülebilir) sınıfındadır.
* **Geliştirilme Amacı:** Ağ yapılarındaki (su/petrol boru hatları, veri/internet şebekeleri, lojistik rotaları) toplam iletim kapasitesini ve sistemi sınırlayan ana darboğazları (**Max-Flow Min-Cut Teoremi**) matematiksel bir metotla kesin olarak hesaplamaktır.

---

## 2. Hangi Eksikliği Gidermektedir ve Avantajları Nelerdir?

* **Giderdiği Eksiklik:** Klasik açgözlü (greedy) yaklaşımın en büyük zayıflığı, ilk adımlarda "kötü/yanlış" bir yol seçtiğinde kilitlenmesi ve optimal sonuca ulaşamamasıdır. Ford-Fulkerson, **Geri Kenar (Backward Edge)** mekanizması getirerek algoritmanın geçmişte yaptığı yanlış akış tercihlerini **iptal etme / yönünü değiştirme** imkanı tanır.
* **Önceki Yöntemlere Göre Avantajları:**
* Algoritmanın ve çatısının (framework) anlaşılması son derece basittir.
* Graf teorisindeki **Maksimum Akış = Minimum Kesit (Max-Flow Min-Cut)** teoreminin temel yapısını oluşturur.



### Hangi Durumda Tercih Edilir?

* Akış kapasitelerinin **küçük tamsayılar** olduğu durumlarda.
* Akış mantığının ve kalıntı graf yapısının temel düzeyde simüle edileceği problemlerde.

### Hangi Durumda Tercih EDİLMEZ?

* Kenar kapasitelerinin çok büyük sayılardan ($1.000.000+$) oluştuğu durumlarda (kötü yol seçimleri nedeniyle algoritma milyonlarca adım çalışabilir).
* Kapasitelerin irrasyonel sayılar içerdiği teorik durumlarda (algoritma sonsuz döngüye girebilir).
* Bu tür olumsuz senaryolarda yol aramayı BFS ile sabitleyen **Edmonds-Karp ($O(V \cdot E^2)$)** veya Seviye Grafı kullanan **Dinic ($O(V^2 \cdot E)$)** algoritmaları tercih edilir.

---

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir (Kalıntı Graf ve Geri Kenar)

> *"Kaynaktan hedefe giden ve henüz dolmamış borulardan akışı gönder. Ancak gönderdiğin her akış miktarı kadar ters yöne sanal bir 'geri kenar' aç ki, ileride bu akışı geri alıp başka yola kaydırma hakkın olsun."*

### Çalışma Prensibi

1. **Kalıntı Graf (Residual Graph - $G_f$):** İleri yönlü kenarlarda kalan boş kapasiteleri $c_f(u,v)$, geri yönlü kenarlarda ise iptal edilebilir akış miktarlarını $c_f(v,u)$ saklar.
2. **Artırıcı Yol (Augmenting Path) Bulma:** Kalıntı graf üzerinde $S$'den $T$'ye giden ve tüm kenarlarının kalan kapasitesi $c_f(u,v) > 0$ olan bir yol (DFS veya BFS ile) aranır.
3. **Şişe Boynu (Bottleneck) Tespiti:** Bulunan yol üzerindeki kenarların kalan kapasitelerinin en küçüğü belirlenir ($b = \min \{c_f(u,v)\}$).
4. **Kapasite Güncellemesi:**
* Yol üzerindeki ileri kenarların kapasitesinden $b$ düşülür: $c_f(u,v) \leftarrow c_f(u,v) - b$
* Yol üzerindeki geri kenarların kapasitesine $b$ eklenir: $c_f(v,u) \leftarrow c_f(v,u) + b$


5. **Döngü:** $S$'den $T$'ye ulaşan boş bir artırıcı yol kalmayana kadar adımlar tekrarlanır.

---

## 4. Veri Yapıları

* **`capacity` (2D Dizi / Matris veya Komşuluk Listesi):** $V \times V$ boyutundaki kalıntı matrisi. $u \to v$ arasındaki anlık kalan kapasiteyi ve $v \to u$ arasındaki geri alma kapasitesini saklar.
* **`parent` (1D Dizi):** $V$ boyutunda dizi. Arama sırasında hangi düğüme hangi ebeveyn düğümden gelindiğini kaydederek hedeften kaynağa doğru rotanın çıkarılmasını sağlar.
* **`visited` (1D Dizi):** $V$ boyutunda boolean dizi. Yol arama (DFS/BFS) esnasında düğümlerin tekrar ziyaret edilerek sonsuz döngüye girilmesini önler.

---

## 5. Sözde Kod (Pseudo-Code)

```text
FordFulkerson(graph, source, sink):
    V ← graph.node_count
    capacity ← graph.capacity_matrix  // Kalıntı kapasiteleri tutar
    max_flow ← 0

    Sonsuz Döngü:
        visited ← V boyutunda FALSE ile dolu dizi
        parent ← V boyutunda -1 ile dolu dizi

        // 1. Kalıntı grafta S'den T'ye giden artırıcı yol ara (DFS)
        has_path ← DFS(source, sink, visited, parent, capacity)

        EĞER has_path == FALSE İSE:
            KÖK DÖNGÜDEN ÇIK  // Artırıcı yol kalmadı, maksimum akışa ulaşıldı

        // 2. Yol üzerindeki şişe boynu (bottleneck) kapasitesini bul
        path_flow ← INF
        v ← sink
        WHILE v != source:
            u ← parent[v]
            path_flow ← MIN(path_flow, capacity[u][v])
            v ← u


        // 3. Kalıntı grafı güncelle (İleri kenardan düş, geri kenara ekle)
        v ← sink
        WHILE v != source:
            u ← parent[v]
            capacity[u][v] ← capacity[u][v] - path_flow
            capacity[v][u] ← capacity[v][u] + path_flow
            v ← u

        max_flow ← max_flow + path_flow

    DÖNDÜR max_flow

FONKSİYON DFS(u, t, visited, parent, capacity):
    EĞER u == t İSE DÖNDÜR TRUE  // Hedefe ulaşıldı

    visited[u] ← TRUE

    HER v İÇİN 0'dan V-1'e KADAR:
        EĞER visited[v] == FALSE VE capacity[u][v] > 0 İSE:
            parent[v] ← u
            EĞER DFS(v, t, visited, parent, capacity) == TRUE İSE:
                DÖNDÜR TRUE

    DÖNDÜR FALSE
```

---

## 6. Zaman Karmaşıklığı

* **Best Case:** $O(E)$
* *Açıklama:* Kaynaktan hedefe giden tek bir yolun tüm akışı tek adımda taşıdığı ve başka yolun bulunmadığı senaryo.


* **Average / Worst Case:** $O(E \cdot \vert{}f_{max}\vert{})$
* *Açıklama:* $E$ kenar sayısı, $\vert{}f_{max}\vert{}$ ise elde edilen toplam maksimum akış miktarıdır. Yol arama işlemi (DFS) $O(E)$ sürer. Tamsayı kapasitelerde her adımda akış en az 1 birim artabileceği için en fazla $\vert{}f_{max}\vert{}$ adet yol bulunabilir.
* *Kötü Senaryo:* Kapasiteler çok yüksekse ve DFS sürekli 1 birim taşıyan uzun yolları seçerse algoritma $\vert{}f_{max}\vert{}$ defa döngüye girer.



---

## 7. Alan Karmaşıklığı

* **Ek Bellek Kullanımı (Auxiliary Space):**
* **Matris Gösterimi İle:** $O(V^2)$
* **Komşuluk Listesi İle:** $O(V + E)$


* **Kullanılan Veri Yapılarının Etkisi:**
* $V \times V$ matrisi kullanıldığında $O(1)$ sürede doğrudan hücre erişimi sağlanır, ancak bellek kullanımı $O(V^2)$ olur.
* Düğüm sayısı ($V$) çok büyük, kenar sayısı ($E$) az olan seyrek (sparse) graflarda matris yerine komşuluk listesi tutulması bellek tüketimini $O(V + E)$ seviyesine düşürür.
