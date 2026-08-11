# 0/1 Knapsack Problemi - Bottom-Up DP

Bu doküman, **0/1 Knapsack (Sırt Çantası)** probleminin **Bottom-Up Dynamic Programming** ile çözümünün teorik detaylarını, çalışma prensibini, sözde kodunu ve karmaşıklık analizini içermektedir.

---

## 1. Problemin Tanımı, Zorluğu ve Geliştirilme Amacı

* **Problem Tanımı:** Elimizde belirli bir $W$ ağırlık kapasitesine sahip bir sırt çantası ve her birinin ağırlığı ($w_i$) ile değeri ($v_i$) olan $N$ adet eşya bulunmaktadır. Amaç, çanta kapasitesini aşmadan toplam değeri **maksimum** yapmaktır. Eşyalar bölünemez ($0/1$ kısıtı).
* **Zorluk Sınıfı:** **NP-Complete** zorluk sınıfındadır.
* **Geliştirilme Amacı:** Özyinelemeli (Recursive) çözümlerdeki fonksiyon çağırma maliyetini (`call stack overhead`) ve derleyici seviyesindeki bellek taşması (`stack overflow`) riskini tamamen ortadan kaldırmak amacıyla geliştirilmiştir. Süreci en küçük alt problemden ($0$ eşya, $0$ kapasite) başlatıp yinelemeli (iterative) döngülerle nihai çözüme ulaştırır.

---

## 2. Hangi Eksikliği Gidermektedir ve Avantajları Nelerdir?

* **Giderdiği Eksiklik:** Memoization (Top-Down) yöntemi zaman karmaşıklığını düşürse de, halen özyineleme yığınını (call stack) kullanır. Eşya sayısı $N$ binlere ulaştığında bu durum programın kilitlenmesine veya bellek patlamasına yol açar. Bottom-Up yaklaşımı özyinelemeyi tamamen kaldırarak bu mimari eksikliği giderir.
* **Önceki Yöntemlere Göre Avantajları:**
* **İşlemci Seviyesinde Hız:** Döngüler (for loops) ardışık bellek erişimi yaptığı için işlemci önbelleği (CPU Cache) dostudur ve özyinelemeli yöntemlere göre daha hızlı çalışır.
* **Sıfır Stack Overflow Riski:** Çağrı yığını kullanılmaz.
* **1D Alan Optimizasyonu İmkânı:** Hesaplanmakta olan satır sadece bir önceki satıra ihtiyaç duyduğu için $O(N \cdot W)$ matris alanı, istenirse $O(W)$ boyutunda tek boyutlu bir diziye dönüştürülebilir.



### Hangi Durumda Tercih Edilir?

* Tüm $(i, w)$ durumlarının taranması gereken senaryolarda.
* Derin özyineleme riskinin bulunduğu büyük $N$ değerlerinde.
* Performans ve bellek optimizasyonunun (1D array optimization) kritik olduğu sistemlerde.

### Hangi Durumda Tercih Edilmez?

* Kapasite $W$ değeri çok uçuk olduğunda ($W = 10^9$ gibi pseudo-polynomial sınırların aşıldığı durumlar). Bu durumda alan ve zaman yükü kaldırılamaz.
* Durum uzayının (state space) çok seyrek olduğu ve kombinasyonların büyük kısmının hiç oluşmadığı problemlerde (Top-Down yaklaşımı sadece gerekli durumları çözdüğü için bu senaryoda daha avantajlı olabilir).

---

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir

> *"En basit durum olan '0 eşya ve 0 kapasite' ile başla. Her adımı bir önceki adımın sonuçlarını kullanarak inşa et ve matrisin en sağ alt köşesinde nihai çözüme ulaş."*

### Çalışma Prensibi

1. **Matris Kurulumu:** $(N + 1) \times (W + 1)$ boyutunda bir `dp` matrisi oluşturulur ve tüm hücreler $0$ ile ilklendirilir ($0$. satır ve $0$. sütun taban durumları temsil eder).
2. **Tabloyu Döngü ile Doldurma:** $i$ ($1 \dots N$) ve $w$ ($0 \dots W$) değerleri üzerinden iç içe iki `for` döngüsü çalıştırılır.
3. **Geçiş Denklemi (State Transition):**
* **Eşya sığmıyorsa ($w_i > w$):** Üst satırdaki değer aynen alınır:
$$\text{dp}[i][w] = \text{dp}[i-1][w]$$


* **Eşya sığıyorsa ($w_i \le w$):** Eşyayı ALMA ve AL seçeneklerinden maksimum olanı seçilir:
$$\text{dp}[i][w] = \max(\text{dp}[i-1][w], \; v_i + \text{dp}[i-1][w - w_i])$$




4. **Sonucun Okunması:** Tablo tamamen dolduğunda en sağ alt köcedeki `dp[N][W]` hücresi kesin optimal sonucu verir.

---

## 4. Veri Yapıları

* **`Item` Yapısı:** Eşyaların `weight` (ağırlık) ve `value` (değer) verilerini tutar.
* **`std::vector<std::vector<int>> dp` (2D Matris):** Taban durumdan başlayarak tüm alt problemlerin optimal çözümlerini adım adım biriktiren tablo.
* **Satır ($i$):** İncelenen eşya sayısı ($0 \dots N$).
* **Sütun ($w$):** O an değerlendirilen çanta kapasitesi ($0 \dots W$).



---

## 5. Sözde Kod (Pseudo-Code)

```text
knapsackBottomUp(capacity, items):
    N ← items.size()
    dp ← (N + 1) x (capacity + 1) boyutunda 0 ile dolu matris

    HER i İÇİN 1'den N'e KADAR:
        HER w İÇİN 0'dan capacity'e KADAR:
            EĞER items[i-1].weight <= w İSE:
                skip ← dp[i - 1][w]
                take ← items[i-1].value + dp[i - 1][w - items[i-1].weight]
                dp[i][w] ← MAX(skip, take)
            DEĞİLSE:
                dp[i][w] ← dp[i - 1][w]

    DÖNDÜR dp[N][capacity]

```

---

## 6. Zaman Karmaşıklığı

* **Best Case:** $O(N \cdot W)$
* **Average Case:** $O(N \cdot W)$
* **Worst Case:** $O(N \cdot W)$
* **Açıklama:** Algoritma herhangi bir erken çıkış yapmaksızın $(N + 1) \times (W + 1)$ boyutundaki matrisin tüm hücrelerini sırayla doldurur. Her hücredeki işlem $O(1)$ sürdüğü için karmaşıklık sabit bir şekilde $O(N \cdot W)$ (Pseudo-polynomial) olur.



---

## 7. Alan Karmaşıklığı

* **Ek Bellek Kullanımı (Auxiliary Space):** $O(N \cdot W)$
* *(1D Alan Optimizasyonu uygulanırsa $O(W)$'ye düşürülebilir).*


* **Kullanılan Veri Yapılarının Etkisi:**
* **`dp` Matrisi:** $(N + 1) \times (W + 1)$ boyutunda tablo hafızada $O(N \cdot W)$ kadar yer kaplar.
* **Özyineleme Yığını (Call Stack):** Çağrı yığını kullanılmadığı için $O(1)$'dir.
