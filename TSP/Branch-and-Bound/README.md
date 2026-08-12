# Traveling Salesperson Problem (TSP) - Branch and Bound (Dallan ve Sınırla) Yaklaşımı

Bu doküman, **Gezgin Satıcı Problemi'nin (Traveling Salesperson Problem - TSP)** indirgenmiş matris (Reduced Matrix) ve Alt Sınır (Lower Bound - $LB$) hesabı kullanarak kesin çözüme ulaşan **Branch and Bound (Dallan ve Sınırla)** algoritmasının teorik esaslarını, çalışma prensibini, sözde kodunu ve karmaşıklık analizini içermektedir.

---

## 1. Problemin Tanımı, Zorluğu ve Geliştirilme Amacı

* **Problem Tanımı:** Verilen $N$ adet şehir ve şehirler arasındaki mesafe matrisine göre; bir satıcının her şehre **tam olarak bir kez** uğraması ve başladığı şehre geri dönmesi şartıyla katetmesi gereken **en kısa (minimum) toplam yolu** bulma problemidir.
* **Zorluk Sınıfı:** **NP-Hard** zorluk sınıfındadır.
* **Geliştirilme Amacı:** Kaba Kuvvet (Brute Force) algoritmasının $O(N!)$ olan üstel arama uzayını taranabilir seviyeye çekmektir. Algoritmanın amacı, %100 kesin (optimal) çözüm garantisinden ödün vermeden, başarı vadetmeyen milyonlarca rotayı henüz yolun başındayken matematiksel olarak **budamak (pruning)** ve arama uzayını sıkıştırmaktır.

---

## 2. Hangi Eksikliği Gidermektedir ve Avantajları Nelerdir?

* **Giderdiği Eksiklik:** Kaba Kuvvet yönteminin tüm permütasyonları körü körüne deneme zayıflığını ortadan kaldırır. Sezgisel (Heuristic) algoritmaların aksine **yaklaşık değil, %100 kesin (optimal) sonucu** garanti eder.
* **Önceki Yöntemlere Göre Avantajları:**
* **Matematiksel Budama (Pruning):** Her düğümde hesaplanan Alt Sınır ($LB$), o daldan aşağı inildiğinde elde edilebilecek en iyi ihtimali verir. Eğer $LB \ge \text{best\_cost}$ ise o dalın altındaki tüm olasılıklar tek adımda çöpe atılır.
* **Akıllı Arama (Best-First Search):** Min-Öncelikli Kuyruk (Min-Heap) kullanarak her adımda teorik olarak en düşük $LB$'ye (en yüksek potansiyele) sahip düğümü öne alır. Böylece kaliteli turlara erkenden ulaşarak `best_cost` rekorunu hızla düşürür.
* **Kapasiteden ($W$) Bağımsızlık:** Dinamik Programlama gibi yöntemlerin aksine çanta kapasitesi veya mesafe büyüklüklerinden etkilenmez.



### Hangi Durumda Tercih Edilir?

* %100 kesin optimal çözümün şart olduğu durumlarda.
* Kaba Kuvvet'in kilitlendiği, ancak arama ağacının indirgenmiş matrislerle güçlü bir şekilde budanabildiği orta ölçekli haritalarda ($N \le 25$).

### Hangi Durumda Tercih EDİLMEZ?

* $N > 30-40$ olduğu ve bellek ile işlemci sınırlarının aşıldığı durumlarda.
* Her düğümde $N \times N$ matris kopyalama ve indirgeme maliyeti yüksek olduğu için saliseler içinde cevap verilmesi gereken kargo/kurye dağıtım sistemlerinde (bu durumlarda 2-Opt, LKH veya Karınca Kolonisi tercih edilir).

---

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir (İndirgenmiş Matris ve Alt Sınır - LB)

> *"Her şehre 1 kez girilmek ve 1 kez çıkılmak zorundadır. Matristeki her satır ve sütundan minimum elemanları çıkararak matrisi indirge. Çıkarılan bu değerlerin toplamı, o haritada ulaşılabilecek teorik alt sınırı ($LB$) verir. Önündeki dalın $LB$ değeri elindeki rekor `best_cost` değerinden kötüleştiği an o yoldan gitme."*

### Çalışma Prensibi

1. **Satır ve Sütun İndirgeme:** Matrisin her satırındaki en küçük eleman tespit edilip o satırdan çıkarılır. Ardından oluşan yeni matrisin her sütunundaki en küçük eleman tespit edilip çıkarılır. Eksiltilen tüm değerlerin toplamı kök düğümün $LB_0$ değerini oluşturur.
2. **Dallanma ($u \to v$):** $u$ şehrinden $v$ şehrine geçildiğinde:
* $u$. satır tamamen $\infty$ yapılır (Artık $u$'dan çıkılamaz).
* $v$. sütun tamamen $\infty$ yapılır (Artık $v$'ye girilemez).
* `matris[v][0]` hücresi $\infty$ yapılır (Erken dönüş engellenir).


3. **Yeni $LB$ Hesabı:** Güncellenen matris tekrar indirgenir ve yeni indirgeleme masrafı ($r$) eklenir:

$$LB_{\text{yeni}} = LB_{\text{eski}} + \text{matris}[u][v] + r$$


4. **Budama ve İlerleme:** Öncelikli kuyruk (Min-Heap) en küçük $LB$'li düğümü tepede tutar. Çekilen düğümün $LB$'si eldeki `best_cost` değerinden büyükse dal budanır. Tüm şehirler gezildiğinde bulunan tur optimal çözümdür.

---

## 4. Veri Yapıları

* **`Node` Struct:** Arama ağacındaki her bir adımı temsil eder:
* `reduced_matrix`: O düğüme ait $N \times N$ indirgenmiş matris kopyası.
* `cost`: Düğümün hesaplanan Alt Sınırı ($LB$).
* `city`: Bulunulan güncel şehir id'si.
* `level`: Ziyaret edilen toplam şehir sayısı.
* `path`: İzlenen şehir sırasını tutan liste.


* **`std::priority_queue<Node>` (Min-Heap):** Düğümleri $LB$ değerlerine göre küçükten büyüğe sıralayan öncelikli kuyruk.

---

## 5. Sözde Kod (Pseudo-Code)

```text
reduceMatrix(matrix):
    reduction_cost ← 0
    N ← matrix.size()

    // 1. Satır İndirgeme
    HER i İÇİN 0'dan N-1'e KADAR:
        row_min ← matrix[i] satırındaki MINIMUM eleman
        EĞER row_min != INF VE row_min != 0 İSE:
            reduction_cost ← reduction_cost + row_min
            matrix[i] satırındaki her elemandan row_min çıkar

    // 2. Sütun İndirgeme
    HER j İÇİN 0'dan N-1'e KADAR:
        col_min ← matrix[][j] sütunundaki MINIMUM eleman
        EĞER col_min != INF VE col_min != 0 İSE:
            reduction_cost ← reduction_cost + col_min
            matrix[][j] sütunundaki her elemandan col_min çıkar

    DÖNDÜR reduction_cost

FONKSİYON solveTSPBranchAndBound(dist_matrix):
    N ← dist_matrix.size()
    pq ← Min-Priority Queue (Node.cost değerine göre sıralı)

    root ← yeni Node (city=0, level=0, path=[0], reduced_matrix=dist_matrix)
    root.cost ← reduceMatrix(root.reduced_matrix)
    pq.push(root)

    pq BOŞ OLMADIĞI SÜRECE:
        min_node ← pq.top()
        pq.pop()

        u ← min_node.city

        // Tüm şehirler gezildiyse optimal çözüm bulunmuştur
        EĞER min_node.level == N - 1 İSE:
            DÖNDÜR min_node.cost, min_node.path

        // Komşu şehirlere dallan
        HER v İÇİN 0'dan N-1'e KADAR:
            EĞER min_node.reduced_matrix[u][v] != INF İSE:
                child ← yeni Node
                child.reduced_matrix ← min_node.reduced_matrix kopyala
                
                // Matrisi güncelle (u satırı, v sütunu ve v->0 hücresi INF yapılır)
                u satırını INF yap
                v sütununu INF yap
                child.reduced_matrix[v][0] ← INF

                transition_cost ← min_node.reduced_matrix[u][v]
                new_reduction ← reduceMatrix(child.reduced_matrix)
                child.cost ← min_node.cost + transition_cost + new_reduction

                pq.push(child)

```

---

## 6. Zaman Karmaşıklığı

* **Best Case:** $O(N^3)$
* *Açıklama:* Algoritmanın hiç yanlış dala sapmadan doğrudan optimal tura gittiği senaryodur. $N$ adım boyunca her düğümde $O(N^2)$ matris indirgemesi yapılır.


* **Average Case:** $O(N^2 \cdot 2^N)$
* *Açıklama:* Güçlü $LB$ budaması sayesinde $N!$ olan arama uzayının %90'ından fazlası elenir ve ortalama çalışma süresi üstel seviyede kalır.


* **Worst Case:** $O(N^2 \cdot N!)$
* *Açıklama:* Tüm şehir mesafelerinin birbirine eşit veya simetrik olduğu, $LB$ değerlerinin budama yapmaya yetmediği en kötü senaryodur.



---

## 7. Alan Karmaşıklığı

* **Ek Bellek Kullanımı (Auxiliary Space):** $O(N^3 \cdot 2^N)$
* **Kullanılan Veri Yapılarının Etkisi:**
* **`priority_queue` ve Matris Kopyaları:** Öncelikli kuyrukta bekleyen her bir `Node`, kendisiyle birlikte $N \times N$ boyutunda bağımsız bir indirgenmiş matris kopyası saklar. Bu durum, Branch and Bound yönteminin bellek tüketimini diğer algoritmalara kıyasla oldukça yükseltir.