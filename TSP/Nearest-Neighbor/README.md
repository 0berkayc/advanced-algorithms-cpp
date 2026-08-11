# Traveling Salesperson Problem (TSP) - Nearest Neighbor (En Yakın Komşu) Yaklaşımı

Bu doküman, **Gezgin Satıcı Problemi'nin (Traveling Salesperson Problem - TSP)** en temel ve hızlı sezgisel (heuristic) çözüm yöntemlerinden biri olan **Nearest Neighbor (En Yakın Komşu / Açgözlü)** yaklaşımının teorik esaslarını, çalışma prensibini, sözde kodunu ve karmaşıklık analizini içermektedir.

---

## 1. Problemin Tanımı, Zorluğu ve Geliştirilme Amacı

* **Problem Tanımı:** Verilen $N$ adet şehir ve şehirler arasındaki mesafe matrisine göre; bir satıcının her şehre **tam olarak bir kez** uğraması ve başladığı şehre geri dönmesi şartıyla katetmesi gereken **en kısa (minimum) toplam yolu** bulma problemidir.
* **Zorluk Sınıfı:** **NP-Hard** zorluk sınıfındadır.
* **Geliştirilme Amacı:** Kaba kuvvet (Brute Force) yöntemlerinin $O(N!)$ faktöriyel karmaşıklığı nedeniyle $N > 12$ değerlerinde kilitlenmesine karşılık; gerçek zamanlı sistemlerde binlerce şehri kapsayan haritalarda dahi **milisaniyeler içinde makul ve hızlı bir rota** üretebilmek amacıyla geliştirilmiştir.

---

## 2. Hangi Eksikliği Gidermektedir ve Avantajları Nelerdir?

* **Giderdiği Eksiklik:** Kesin çözüm veren algoritmaların (Brute Force, DP, Branch and Bound) gerektirdiği yüksek işlemci yükünü ve devasa bellek ihtiyacını ortadan kaldırır. Zaman maliyetini üstel/faktöriyel seviyeden polinom seviyesine ($O(N^2)$) indirger.
* **Önceki Yöntemlere Göre Avantajları:**
* **Işık Hızında Çalışması:** Polinom zamanda ($O(N^2)$) çalışarak binlerce şehri içeren problemleri saliseler içinde çözer.
* **Uygulama Kolaylığı:** Kodlanması, anlaşılması ve entegre edilmesi en kolay TSP algoritmasıdır.
* **Düşük Bellek İhtiyacı:** Dev durum tabloları tutmaz, özyineleme yığını kullanmaz.
* **İyi Bir Başlangıç Noktası (Initial Solution) Olması:** 2-Opt veya 3-Opt gibi daha gelişmiş yerel arama sezgisellerine girdi olarak verilmek üzere hızlıca bir taslak rota oluşturur.



### Hangi Durumda Tercih Edilir?

* Anlık kargo/kurye rota optimizasyonu gibi milisaniyeler içinde cevap üretilmesi gereken sistemlerde.
* Şehir sayısının çok büyük olduğu ($N > 1000$) ve kesin optimal çözüm yerine %80–%85 verimlilikte hızlı bir çözümün yeterli olduğu senaryolarda.

### Hangi Durumda Tercih EDİLMEZ?

* Kuruşu kuruşuna %100 kesin (optimal) çözümün zorunlu olduğu kritik sistemlerde.
* **Miyopik (Açgözlü) karar mekanizması** nedeniyle haritanın son adımlarında mecburen çok uzaktaki şehirlere sıçrama riskinin (bad local minima) kabul edilemediği durumlarda.

---

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir (Greedy / Açgözlü Yaklaşım)

> *"Şu an bulunduğun şehirden henüz ziyaret etmediğin en yakın şehire git."*

### Çalışma Prensibi

1. **Başlangıç:** Bir başlangıç şehri seçilir (genellikle $0$. şehir) ve `visited` dizisinde "ziyaret edildi" olarak işaretlenir.
2. **En Yakın Şehri Arama:** Bulunulan şehirden, henüz ziyaret edilmemiş tüm şehirler arasındaki mesafeler taranır.
3. **Seçim ve İlerleme:** En küçük mesafeye sahip ziyaret edilmemiş şehir seçilir, maliyet eklenir, yeni konuma geçilir ve şehir ziyaret edildi olarak işaretlenir.
4. **Döngü:** Ziyaret edilmeyen şehir kalmayana kadar 2. ve 3. adımlar tekrarlanır.
5. **Turu Tamamlama:** Tüm şehirler gezildikten sonra, son şehirden tekrar **başlangıç şehrine** dönülerek kapalı tur tamamlanır.

---

## 4. Veri Yapıları

* **`dist` (2D Dizi / Matris):** $N \times N$ boyutunda şehirler arası mesafeleri tutan matris (`dist[i][j]`).
* **`visited` (1D Boolean Dizi):** Hangi şehirlerin gezildiğini takip eden $N$ elemanlı mantıksal dizi.
* **`path` (1D Dizi):** Gezilen şehirlerin sırasını tutan vektör.
* **`total_cost` (Tamsayı Variable):** Biriken toplam rota mesafesini saklayan sayaç.

---

## 5. Sözde Kod (Pseudo-Code)

```text
TSPNearestNeighbor(dist_matrix, start_city):
    N ← dist_matrix.size()
    visited ← N boyutunda FALSE dolu dizi
    path ← boş liste
    total_cost ← 0

    current_city ← start_city
    path.push(current_city)
    visited[current_city] ← TRUE

    HER step İÇİN 1'den N-1'e KADAR:
        nearest_city ← -1
        min_dist ← INT_MAX

        HER next_city İÇİN 0'dan N-1'e KADAR:
            EĞER visited[next_city] == FALSE VE dist_matrix[current_city][next_city] < min_dist İSE:
                min_dist ← dist_matrix[current_city][next_city]
                nearest_city ← next_city

        visited[nearest_city] ← TRUE
        path.push(nearest_city)
        total_cost ← total_cost + min_dist
        current_city ← nearest_city

    // Başlangıç noktasına dönüş
    total_cost ← total_cost + dist_matrix[current_city][start_city]
    path.push(start_city)

    DÖNDÜR total_cost, path

```

---

## 6. Zaman Karmaşıklığı

* **Best Case:** $O(N^2)$
* **Average Case:** $O(N^2)$
* **Worst Case:** $O(N^2)$
* **Açıklama:** Dış döngü $N-1$ kere çalışır. İç döngü her adımda henüz ziyaret edilmemiş şehirleri bulmak için tüm matris satırını ($N$ adet eleman) tarar. Toplam adım sayısı $\frac{N(N-1)}{2}$ işlemdir, bu da $O(N^2)$ (Karesel Zaman) karmaşıklığına denk gelir.



---

## 7. Alan Karmaşıklığı

* **Ek Bellek Kullanımı (Auxiliary Space):** $O(N)$
* **Kullanılan Veri Yapılarının Etkisi:**
* **`dist` Matrisi:** $N \times N$ boyutunda $O(N^2)$ alan kaplar (Girdi verisi).
* **`visited` ve `path` Dizileri:** Yalnızca $N$ elemanlı 1D vektörler olduğu için ekstra bellek yükü $O(N)$'dir. Çağrı yığını kullanılmaz.