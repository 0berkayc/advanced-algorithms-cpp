# Traveling Salesperson Problem (TSP) - Brute Force (Kaba Kuvvet) Yaklaşımı

Bu doküman, **Gezgin Satıcı Problemi'nin (Traveling Salesperson Problem - TSP)** en temel çözüm yöntemi olan **Brute Force (Kaba Kuvvet)** yaklaşımının teorik esaslarını, çalışma prensibini, sözde kodunu ve karmaşıklık analizini içermektedir.

---

## 1. Problemin Tanımı, Zorluğu ve Geliştirilme Amacı

* **Problem Tanımı:** Verilen $N$ adet şehir ve şehirler arasındaki mesafe matrisine göre; bir satıcının her şehre **tam olarak bir kez** uğraması ve başladığı şehre geri dönmesi şartıyla katetmesi gereken **en kısa (minimum) toplam yolu** bulma problemidir.
* **Zorluk Sınıfı:** **NP-Hard** zorluk sınıfındadır.
* **Geliştirilme Amacı:** Problemin tüm olası durum uzayını taranarak **%100 kesin (optimal) çözümü bulmak** ve geliştirilen diğer karmaşık algoritmaların (Dinamik Programlama, Branch & Bound, Sezgisel Yöntemler) doğruluk testlerini yapabilmek için temel referans noktası (baseline) oluşturmaktır.

---

## 2. Hangi Eksikliği Gidermektedir ve Avantajları Nelerdir?

* **Giderdiği Eksikling:** Sezgisel (Heuristic) ve yaklaşım algoritmalarının yerel minimumlara (Local Optima) takılma veya yaklaşıklık hatası verme riskini tamamen ortadan kaldırır. Problem alanındaki olası hiçbir rotayı atlamaz.
* **Önceki Yöntemlere Göre Avantajları:**
* **%100 Doğruluk Garantisi:** Matematiksel olarak var olan en kısa rotayı bulmama ihtimali %0'dır.
* **Uygulama Kolaylığı:** Karmaşık durum geçişleri, matris indirgemeleri veya veri yapıları gerektirmez; anlaşılması ve kodlanması en basit yöntemdir.
* **Hafıza Dostu Olması:** Dinamik Programlama gibi yöntemlerin aksine devasa durum tablolarını hafızada tutmak zorunda değildir.



### Hangi Durumda Tercih Edilir?

* Şehir sayısının çok küçük olduğu ($N \le 10$ veya $N \le 12$) durumlarda.
* Yazılan daha gelişmiş çözücülerin (Solver) ürettiği sonuçların doğruluğunu küçük veri setlerinde doğrulamak (Unit Test) amacıyla.

### Hangi Durumda Tercih EDİLMEZ?

* $N > 12$ olduğu durumlarda. Faktöriyel büyüme ($(N-1)!$) nedeniyle arama uzayı milyarlara ve trilyonlara ulaşacağı için algoritmanın çalışması saatler veya yıllar sürebilir.

---

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir

> *"Dairesel turlarda başlangıç şehrinin değişmesi rotanın toplam mesafesini değiştirmez. Bu yüzden 0 numaralı şehri sabit tut, geriye kalan $N-1$ şehrin tüm permütasyonlarını oluştur, her birinin toplam mesafesini hesapla ve en küçüğünü rekor olarak sakla."*

### Çalışma Prensibi

1. **Başlangıç Şehrinin Sabitlenmesi:** $0$. şehir sabit başlangıç ve bitiş noktası kabul edilir.
2. **Permütasyon Kümesi:** Kalan $\{1, 2, \dots, N-1\}$ şehirlerinden oluşan bir dizi ilklendirilir ve sıralanır.
3. **Mesafe Hesabı ve Döngü:** Dizi alfabetik/sayısal sıraya göre permütasyonlarına ayrılır (örneğin `std::next_permutation` ile). Her permütasyon için:

$$\text{Mesafe} = \text{dist}[0][p_1] + \text{dist}[p_1][p_2] + \dots + \text{dist}[p_{k}][0]$$



toplamı hesaplanır.
4. **Rekor Güncelleme:** Hesaplanan mesafe, başlangıçta `INT_MAX` (sonsuz) olarak atanan `min_cost` değerinden küçükse `min_cost` ve `best_path` güncellenir.

---

## 4. Veri Yapıları

* **`dist` (2D Dizi / Vector of Vectors):** $N \times N$ boyutunda şehirler arası mesafeleri tutan matris (`dist[i][j]`).
* **`cities` (1D Dizi):** $N-1$ elemanlı, permütasyona girecek şehir id'lerini saklayan vektör.
* **`min_cost` (Tamsayı Variable):** O ana kadar bulunan en kısa mesafeyi saklayan değişken. Başlangıç değeri `INT_MAX`'tır.
* **`best_path` (1D Dizi):** Bulunan en kısa rotanın şehir sırasını tutan liste.

---

## 5. Sözde Kod (Pseudo-Code)

```text
TSPBruteForce(dist_matrix, N):
    cities ← [1, 2, ..., N-1] dizisi
    min_cost ← INT_MAX
    best_path ← boş dizi

    cities dizisini KÜÇÜKTEN BÜYÜĞE SIRALA

    YAP:
        current_cost ← 0
        current_city ← 0

        // Permütasyondaki şehirleri sırayla gez
        HER next_city İÇİN cities İÇİNDE:
            current_cost ← current_cost + dist_matrix[current_city][next_city]
            current_city ← next_city

        // Başlangıç noktası olan 0. şehre dönüşü ekle
        current_cost ← current_cost + dist_matrix[current_city][0]

        // Rekor kontrolü
        EĞER current_cost < min_cost İSE:
            min_cost ← current_cost
            best_path ← [0] + cities + [0]

    OLDUĞU SÜRECE (cities dizisinin SONRAKİ PERMÜTASYONU VARSA)

    DÖNDÜR min_cost, best_path

```

---

## 6. Zaman Karmaşıklığı

* **Best Case:** $O(N!)$
* **Average Case:** $O(N!)$
* **Worst Case:** $O(N!)$
* **Açıklama:** Toplam üretilen permütasyon sayısı $\frac{(N-1)!}{2}$ (simetrik TSP) veya $(N-1)!$ (asimetrik TSP) kadardır. Her permütasyonun mesafesini toplamak $O(N)$ adım sürer. Dolayısıyla toplam karmaşıklık $O(N \cdot (N-1)!) = O(N!)$ (Faktöriyel Zaman) olur.



---

## 7. Alan Karmaşıklığı

* **Ek Bellek Kullanımı (Auxiliary Space):** $O(N)$
* **Kullanılan Veri Yapılarının Etkisi:**
* **`dist` Matrisi:** $N \times N$ boyutunda $O(N^2)$ alan kaplar (Girdi boyutu).
* **`cities` ve `best_path` Dizileri:** Sadece $N$ elemanlı 1D vektörler olduğu için ekstra bellek yükü $O(N)$'dir. Özyineleme yığını kullanılmaz.



---