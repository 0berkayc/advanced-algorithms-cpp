# Vertex Cover - Exact (Backtracking) Algoritması

Bu doküman, **Vertex Cover (Düğüm Kaplama)** probleminin kesin ve %100 mutlak optimum çözümünü sunan **Exact (Backtracking / Geri İzleme)** algoritmasını, teorik altyapısını, zaman/alan karmaşıklığını ve kullanım senaryolarını detaylandırmaktadır.

---

## 1. Problemin Tanımı ve Zorluğu

* **Problem Tanımı:** Yönlendirilmemiş bir $G = (V, E)$ çizgesinde, çizgedeki **her bir kenarın ($e = (u, v) \in E$) en az bir ucunu kapsayacak** şekilde seçilen **mutlak minimum sayıda** düğüm kümesini ($V' \subseteq V$) bulma problemidir.
* **Zorluk Sınıfı:** **NP-Hard** (Optimizasyon versiyonu). Karar versiyonu ise **NP-Complete**'tir.
* **Hesaplama Zorluğu:** Çizgedeki tüm olası düğüm alt kümelerinin Brute Force ile taranması üstel zaman ($O(2^{\vert{}V\vert{}})$) gerektirir. Küçük düğüm sayılarında hesaplanabilir olsa da $N$ büyüdükçe arama uzayı astronomik hızla büyür.

---

## 2. Algoritmanın Geliştirilme Amacı ve Kullanım Durumları

### Geliştirilme Amacı ve Giderdiği Eksiklik

Sezgisel (Approximation veya Greedy) algoritmalar hızlı çalışsalar da her zaman en küçük kaplamayı veremezler (örneğin 2-Approximation algoritması gerçek sonucun 2 katına kadar fazladan düğüm seçebilir). Exact algoritması, **tavizsiz biçimde mutlak en küçük düğüm sayısını (%100 optimallik)** garanti etmek amacıyla geliştirilmiştir.

Düz Brute Force yöntemine kıyasla, **budama (pruning)** mekanizmaları kullanarak gereksiz arama dallarını erkenden keser ve çözüm arama süresini ciddi oranda kısaltır.

### Önceki/Sezgisel Yöntemlere Göre Avantajları

* **%100 Mutlak Çözüm:** Yanılma veya hata payı yoktur. Elde edilen çözüm teorik olarak seçilebilecek en küçük düğüm kümesidir.
* **Akıllı Budama (Pruning):** O anki arama dalı daha önce bulunmuş en iyi çözümden daha kötü bir boyuta ulaştığında aramayı derhal keserek zaman kazanır.

### Hangi Durumda Tercih Edilir?

* Düğüm sayısının küçük veya orta ölçekli olduğu ($\vert{}V\vert{} \le 30-40$) durumlarda.
* Hata toleransının sıfır olduğu, fazladan tek bir düğümün bile yüksek donanım/maliyet yükü getirdiği hassas mühendislik ve optimizasyon problemlerinde.
* Sezgisel algoritmaların başarımını (yaklaşım oranını) test etmek için doğrulama ve kıyaslama referansı olarak.

### Hangi Durumda Tercih Edilmez?

* Devasa çizge yapılarında ($\vert{}V\vert{} > 50$), üstel zaman karmaşıklığı nedeniyle bilgisayar kaynakları kilitleneceği ve hesaplama günlerce sürebileceği için tercih edilmez.
* Gerçek zamanlı yanıt verilmesi gereken sistemlerde.

---

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir

Algoritma şu matematiksel mantığa dayanır:
Çizgedeki herhangi bir $e = (u, v)$ kenarını kapatabilmek için **ya $u$ düğümü ya da $v$ düğümü (veya her ikisi)** seçilmek zorundadır.

Bu kural ikili bir karar ağacı (binary search tree) oluşturur. Algoritma sırayla $u$'yu seçtiğimiz senaryoyu ve $v$'yi seçtiğimiz senaryoyu özyinelemeli olarak dener.

### Çalışma Prensibi

1. **Budama Kontrolü:** O anki daldaki `currentCover` boyutu, şu ana kadar bulunmuş en iyi çözüm olan `bestCover` boyutuna ulaştıysa veya geçtiyse o dal terk edilir.
2. **Taban Koşulu:** Kapsanmamış kenar kalmadıysa (`uncoveredCount == 0`), yeni bir optimum çözüm bulunmuş demektir ve `bestCover = currentCover` güncellenmesi yapılır.
3. Kapsanmamış ilk kenar $e = (u, v)$ seçilir.
4. **DAL 1 ($u$ Seçimi):** $u$ düğümü `currentCover`'a eklenir, $u$'ya bağlı kapsanmamış kenarlar `visitedEdges` üzerinde kapatılır ve fonksiyon özyinelemeli olarak çağrılır.
5. **Geri Adım (Backtrack U):** $u$ için yapılan tüm kenar değişiklikleri geri alınır ve $u$ listeden çıkarılır.
6. **DAL 2 ($v$ Seçimi):** $v$ düğümü `currentCover`'a eklenir, $v$'ye bağlı kapsanmamış kenarlar `visitedEdges` üzerinde kapatılır ve fonksiyon özyinelemeli olarak çağrılır.
7. **Geri Adım (Backtrack V):** $v$ için yapılan tüm kenar değişiklikleri geri alınır ve $v$ listeden çıkarılır.

---

## 4. Veri Yapıları

Algoritmada kullanılan temel C++ veri yapıları ve işlevleri şunlardır:

* `const std::vector<std::pair<int, int>>& edges`: Çizgedeki sabit kenar listesini $(u, v)$ çiftleri halinde tutar.
* `std::vector<bool>& visitedEdges`: Kenarların kapsanma durumunu $O(1)$ sürede kontrol etmek için kullanılan referans dizisi.
* `std::vector<int>& currentCover`: Özyinelemeli arama sırasında o anda aktif olan daldaki seçilmiş düğümler kümesi.
* `std::vector<int>& bestCover`: Tüm dallar arasında şu ana kadar bulunmuş en az düğümlü en iyi çözümü saklayan vektör.
* `std::vector<int> newlyVisitedU / newlyVisitedV`: Her bir dallanma adımında sadece o düğüm tarafından kapatılan kenarların indekslerini saklar. Backtracking (geri adım) esnasında bu indeksler kullanılarak durum eski haline getirilir.

---

## 5. Sözde Kod (Pseudo-Code)

```text
ALGORİTMA Kesin-Vertex-Cover-Yardımcı(numVertices, edges, visitedEdges, uncoveredCount, currentCover, bestCover)

numVertices, edges, visitedEdges, uncoveredCount, currentCover, bestCover

    // 1. Budama (Pruning)
    EĞER currentCover boyutu >= bestCover boyutu İSE:
        DÖNDÜR

    // 2. Taban Koşulu (Base Case)
    EĞER uncoveredCount == 0 İSE:
        bestCover ← currentCover
        DÖNDÜR

    // 3. Kapsanmamış ilk kenarı bul
    selectedEdgeIndex ← -1
    HER i İÇİN 0'dan (edges boyutu - 1)'e KADAR:
        EĞER visitedEdges[i] == Yanlış İSE:
            selectedEdgeIndex ← i
            DÖNGÜDEN ÇIK

    EĞER selectedEdgeIndex == -1 İSE:
        EĞER currentCover boyutu < bestCover boyutu İSE:
            bestCover ← currentCover
        DÖNDÜR

    u ← edges[selectedEdgeIndex].birinciDüğüm
    v ← edges[selectedEdgeIndex].ikinciDüğüm

    // --------------------------------------------------
    // DAL 1: u Düğümünü Seç
    // --------------------------------------------------
    currentCover listesine u ekle
    yeniKapananKenarlarU ← Boş Liste

    HER i İÇİN 0'dan (edges boyutu - 1)'e KADAR:
        EĞER visitedEdges[i] == Yanlış VE (edges[i].birinciDüğüm == u VEYA edges[i].ikinciDüğüm == u) İSE:
            visitedEdges[i] ← Doğru
            yeniKapananKenarlarU listesine i ekle

    Kesin-Vertex-Cover-Yardımcı(numVertices, edges, visitedEdges, uncoveredCount - yeniKapananKenarlarU boyutu, currentCover, bestCover)

    // Geri Adım (Backtrack U)
    HER edgeIdx İÇİN yeniKapananKenarlarU İÇİNDEKİ:
        visitedEdges[edgeIdx] ← Yanlış
    currentCover listesinden u çıkar

    // --------------------------------------------------
    // DAL 2: v Düğümünü Seç
    // --------------------------------------------------
    currentCover listesine v ekle
    yeniKapananKenarlarV ← Boş Liste

    HER i İÇİN 0'dan (edges boyutu - 1)'e KADAR:
        EĞER visitedEdges[i] == Yanlış VE (edges[i].birinciDüğüm == v VEYA edges[i].ikinciDüğüm == v) İSE:
            visitedEdges[i] ← Doğru
            yeniKapananKenarlarV listesine i ekle

    Kesin-Vertex-Cover-Yardımcı(numVertices, edges, visitedEdges, uncoveredCount - yeniKapananKenarlarV boyutu, currentCover, bestCover)

    // Geri Adım (Backtrack V)
    HER edgeIdx İÇİN yeniKapananKenarlarV İÇİNDEKİ:
        visitedEdges[edgeIdx] ← Yanlış
    currentCover listesinden v çıkar

```

---

## 6. Zaman Karmaşıklığı

* **Best Case — $O(\vert{}V\vert{} + \vert{}E\vert{})$:** Çizge yapısı öyle denk gelir ki ilk denenen daldan hemen optimum çözüm bulunur ve diğer tüm dallar budama (`currentCover.size() >= bestCover.size()`) sayesinde anında elenir.
* **Average Case — $O(1.44^k \cdot \vert{}E\vert{})$:** Budama (pruning) sayesinde tüm ikili ağaç taranmaz. Ortalama durumda Brute Force $O(2^{\vert{}V\vert{}})$ zamanının çok altında pratik bir hız sergiler ($k = \vert{}\text{OPT}\vert{}$ seçilen en küçük düğüm sayısıdır).
* **Worst Case — $O(2^{\vert{}V\vert{}} \cdot \vert{}E\vert{})$:** Budamanın devreye giremediği son derece karmaşık veya tam çizgelerde (Complete Graph) arama ağacının tüm ikili opsiyonları taranır ve üstel (exponential) zaman karmaşıklığı oluşur.

---

## 7. Alan Karmaşıklığı

* **Ek Bellek Kullanımı (Auxiliary Space):** $O(\vert{}V\vert{} + \vert{}E\vert{})$
* **Kullanılan Veri Yapılarının Etkisi:**
* **Özyineleme Yığıtı (Recursion Stack):** Ağacın maksimum derinliği çizgedeki düğüm sayısı ile sınırlı olduğundan en fazla $O(\vert{}V\vert{})$ derinliğe ulaşır.
* `visitedEdges` ve `edges`: Tüm kenarları tuttuğu için $O(\vert{}E\vert{})$ yer kaplar.
* `currentCover` ve `bestCover`: En fazla $\vert{}V\vert{}$ düğüm barındırabileceği için $O(\vert{}V\vert{})$ alan kaplar.
* `newlyVisitedU / V`: Her özyineleme katmanında geçici olarak oluşturulur, toplam ek alan $O(\vert{}E\vert{})$ sınırını geçmez.



---

## 8. Örnek Çalıştırma ve Derleme

### Derleme

```bash
g++ -std=c++17 main.cpp -o app
./app

```

### Örnek Çıktı

```text
Kesin Optimum Düğümler (Exact Vertex Cover): 0 3 4 
Minimum Düğüm Sayısı: 3

```

*(Not: 2-Approximation algoritmasının aynı çizge üzerinde bulduğu 6 düğümlü çözüme kıyasla Exact algoritma mutlak minimum olan **3 düğümlü** optimum çözümü hesaplamıştır.)*