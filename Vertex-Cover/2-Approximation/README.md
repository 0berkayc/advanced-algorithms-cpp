# Vertex Cover - 2-Approximation Algoritması

Bu doküman, **Vertex Cover (Düğüm Kaplama)** probleminin 2-Approximation sezgisel algoritması ile çözümünü, teorik altyapısını, zaman/alan karmaşıklığını ve kullanım senaryolarını detaylandırmaktadır.

---

## 1. Problemin Tanımı ve Zorluğu

* **Problem Tanımı:** Yönlendirilmemiş bir $G = (V, E)$ çizgesinde, çizgedeki **her bir kenarın ($e = (u, v) \in E$) en az bir ucunu kapsayacak** şekilde seçilen **minimum sayıda** düğüm kümesini ($V' \subseteq V$) bulma problemidir.
* **Zorluk Sınıfı:** **NP-Hard** (Optimizasyon versiyonu). Karar versiyonu ise **NP-Complete**'tir.
* **Hesaplama Zorluğu:** Çizgedeki düğüm sayısı ($\vert{}V\vert{}$) arttıkça kesin (optimum) çözümü veren algoritmalarda olası tüm alt kümelerin taranması üstel zaman ($O(2^{\vert{}V\vert{}})$) gerektirir. Bu durum, orta ve büyük ölçekli çizgelerde kesin çözümün pratik bir sürede elde edilmesini imkansız kılar.

---

## 2. Algoritmanın Geliştirilme Amacı ve Kullanım Durumları

### Geliştirilme Amacı ve Giderdiği Eksiklik

Kesin çözüm sunan algoritmalar (Brute Force, Backtracking, Branch and Bound vb.), NP-Hard problemler için büyük girdi boyutlarında kilitlenir. 2-Approximation algoritması, **kesin çözümden kısmen taviz vererek polinomsal zamanda ($O(\vert{}V\vert{} + \vert{}E\vert{})$) çalışan** hızlı bir sezgisel yöntem sunmak amacıyla geliştirilmiştir.

### Önceki/Kesin Yöntemlere Göre Avantajları

* **Hız:** Üstel karmaşıklık ($O(2^N)$) yerine doğrusal/polinomsal karmaşıklık sunar.
* **Teorik Garanti:** Sonucun ne kadar kötü olabileceğine dair matematiksel bir üst sınır sunar. Algoritmanın bulduğu düğüm sayısı, mutlak minimum sonucun (OPT) en fazla **2 katı** olabilir ($\vert{}C\vert{} \le 2 \cdot \text{OPT}$).

### Hangi Durumda Tercih Edilir?

* Çizgedeki düğüm ve kenar sayısının çok fazla olduğu (örneğin sosyal ağlar, devasa ağ altyapıları) ve kesin çözüm hesaplamasının saatler/günler süreceği senaryolarda.
* Gerçek zamanlı veya kaynak kısıtı olan sistemlerde kabul edilebilir bir hata payı ile hızlı karar alınması gerektiğinde.

### Hangi Durumda Tercih Edilmez?

* Düğüm sayısının küçük olduğu ($\vert{}V\vert{} \le 30-40$) ve kesin çözümü hesaplamanın makul sürede bittiği durumlarda.
* Kullanılacak düğüm sayısının maliyetinin son derece kritik olduğu ve %100 optimalliğin zorunlu olduğu (örneğin donanım tasarımı veya yüksek maliyetli tesis konumlandırma) hassas mühendislik problemlerinde.

---

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir

Algoritma, çizge üzerindeki **bağımsız kenarlar (maximal matching)** mantığına dayanır. Rastgele/sıradan kapsanmamış bir kenar seçilir ve bu kenarın her iki ucu da kaplamaya eklenir.

### Çalışma Prensibi

1. Henüz kapsanmamış herhangi bir $e = (u, v)$ kenarı seçilir.
2. Seçilen kenarın **her iki ucu da** ($u$ ve $v$) kaplama kümesine ($C$) eklenir.
3. Çizgedeki $u$ veya $v$ düğümlerine bağlı **tüm kenarlar** kapsandı olarak işaretlenir (veya çizgeden silinir).
4. Çizgede kapsanmamış kenar kalmayana kadar adımlar tekrarlanır.

#### Neden $2 \cdot \text{OPT}$ Garanti Edilir?

Algoritmanın seçtiği kenarlar birbiriyle düğüm paylaşmayan bağımsız kenarlar kümesidir ($M$). Mükemmel bir çözüm ($\text{OPT}$), bu bağımsız kenarların her birini örtebilmek için **en az 1 düğüm** seçmek zorundadır ($\text{OPT} \ge \vert{}M\vert{}$). Bizim algoritmamız ise her bağımsız kenar için **2 düğüm** seçtiğinden:

$$\vert{}C\vert{} = 2 \cdot \vert{}M\vert{} \le 2 \cdot \text{OPT}$$

---

## 4. Veri Yapıları

Algoritmada kullanılan temel C++ veri yapıları ve işlevleri şunlardır:

* `std::vector<std::pair<int, int>> edges`: Çizgedeki kenarları $(u, v)$ çiftleri halinde saklamak için kullanılır.
* `std::vector<bool> visitedEdges`: Her bir kenarın kapsanıp kapsanmadığını $O(1)$ sürede takip etmek için kullanılan durum vektörüdür.
* `std::vector<int> cover`: Kaplama için seçilen düğümlerin tutulduğu sonuç vektörüdür.
* `int uncoveredCount`: Kapsanmamış kenar sayısını tutan sayaçtır. Döngü sonlanma koşulunu denetler.

---

## 5. Sözde Kod (Pseudo-Code)

```text
approxVertex(numVertices, edges):

    numVertices (düğüm sayısı), edges (kenarlar listesi)
    cover (seçilen düğümler listesi)

    cover ← []                                     // Seçilen düğümler listesi
    numEdges ← edges boyutu                        // Toplam kenar sayısı
    uncoveredCount ← numEdges                      // Kapsanmamış kenar sayısı
    visitedEdges ← numEdges boyutunda Yanlış dizisi // Kenar kapsanma durumları

    DÖNGÜ (uncoveredCount ≠ 0 İKEN):
        selectedEdgeIndex ← -1

        // Kapsanmamış ilk kenarın indeksini bul
        HER i İÇİN 0'dan (numEdges - 1)'e KADAR:
            EĞER visitedEdges[i] == Yanlış İSE:
                selectedEdgeIndex ← i
                DÖNGÜDEN ÇIK

        EĞER selectedEdgeIndex == -1 İSE:
            DÖNGÜDEN ÇIK                           // Kapsanmamış kenar kalmadı

        u ← edges[selectedEdgeIndex].first         // Seçilen kenarın 1. ucu
        v ← edges[selectedEdgeIndex].second        // Seçilen kenarın 2. ucu

        cover listesine u ekle
        cover listesine v ekle

        // u veya v düğümüne bağlı olan kapsanmamış kenarları kapat
        HER j İÇİN 0'dan (numEdges - 1)'e KADAR:
            node1 ← edges[j].first
            node2 ← edges[j].second

            EĞER visitedEdges[j] == Yanlış VE (node1 == u VEYA node1 == v VEYA node2 == u VEYA node2 == v) İSE:
                visitedEdges[j] ← Doğru
                uncoveredCount ← uncoveredCount - 1

    DÖNDÜR cover

```

---

## 6. Zaman Karmaşıklığı

* **Best Case — $O(\vert{}V\vert{} + \vert{}E\vert{})$:** Çizge yapısına bağlı olarak kenarların büyük çoğunluğu ilk birkaç adımda eleniyorsa ve komşuluk ilişkileri hızlı taranıyorsa.
* **Average Case — $O(\vert{}V\vert{} + \vert{}E\vert{})$:** İdeal bir bitişiklik listesi (adjacency list) yapısı ile taranan düğüm ve kenarlar toplamı kadar adımda sonlanır.
* **Worst Case — $O(\vert{}E\vert{}^2)$:** Düz kenar listesi (`std::vector<std::pair<int, int>>`) üzerinde her kenar seçiminde tüm liste tekrar taranıyorsa üst sınır $O(\vert{}E\vert{}^2)$ olur. Çizge komşuluk listesi yapısı ile yazıldığında worst-case karmaşıklığı da $O(\vert{}V\vert{} + \vert{}E\vert{})$ seviyesine düşer.

---

## 7. Alan Karmaşıklığı

* **Ek Bellek Kullanımı (Auxiliary Space):** $O(\vert{}V\vert{} + \vert{}E\vert{})$
* **Kullanılan Veri Yapılarının Etkisi:**
* `edges` dizisi: Çizgedeki tüm kenarlar için $O(\vert{}E\vert{})$ alan kullanır.
* `visitedEdges` dizisi: Her kenarın durumunu tutmak için $O(\vert{}E\vert{})$ boyutunda boolean yer kaplar.
* `cover` dizisi: Seçilen düğümler saklanır. Algoritma en fazla $2 \cdot \text{OPT} \le \vert{}V\vert{}$ sayıda düğüm seçeceği için kapladığı alan $O(\vert{}V\vert{})$ ile sınırlıdır.



---

## 8. Örnek Çalıştırma ve Derleme

### Derleme

```bash
g++ -std=c++17 main.cpp -o app
./app

```

### Örnek Çıktı

```text
Seçilen Düğümler (Vertex Cover): 0 1 2 3 4 5 
Kullanılan Toplam Düğüm Sayısı: 6

```

*(Not: Bu çizge için mutlak optimum çözüm 3 düğümdür (`{0, 3, 4}`). Algoritmanın bulduğu 6 değeri, $2 \cdot \text{OPT}$ teorik üst sınırını tam olarak doğrulamaktadır.)*