# 0/1 Knapsack Problemi - FPTAS (Fully Polynomial-Time Approximation Scheme)

Bu doküman, **0/1 Knapsack (Sırt Çantası)** probleminin **FPTAS (Tam Polinom Zamanlı Yaklaşım Şeması)** tekniği ile çözümünün teorik esaslarını, değer ölçekleme (scaling) mantığını, veri yapılarını, sözde kodunu ve karmaşıklık analizini içermektedir.

---

## 1. Problemin Tanımı, Zorluğu ve Geliştirilme Amacı

* **Problem Tanımı:** Elimizde belirli bir $W$ ağırlık kapasitesine sahip bir çanta ve her birinin ağırlığı ($w_i$) ile değeri ($v_i$) olan $N$ adet eşya bulunmaktadır. Amaç, çanta kapasitesini aşmadan toplam değeri **maksimum** yapmaktır. Eşyalar parçalanamaz ($0/1$ kısıtı).
* **Zorluk Sınıfı:** **NP-Complete** zorluk sınıfındadır.
* **Geliştirilme Amacı:** Dinamik Programlama (DP) çözümlerinin zaman karmaşıklığı $O(N \cdot W)$ (ağırlık tabanlı) veya $O(N^2 \cdot V_{max})$ (değer tabanlı) şeklindedir. Çanta kapasitesi $W$ veya eşya değerleri $V_{max}$ çok devasa sayılara ulaştığında ($10^9$ gibi), DP algoritmaları bellek yetersizliği ve yüksek işlem süresi nedeniyle kilitlenir.
**FPTAS**, kabul edilebilir küçük bir hata toleransı ($\epsilon$) karşılığında, girdi boyutlarından ($W$ veya $V$) tamamen bağımsız, **kesin polinom zamanda ($O(N^3 / \epsilon)$)** çalışan yakın-optimal bir çözüm üretmek amacıyla geliştirilmiştir.

---

## 2. Hangi Eksikliği Gidermektedir ve Avantajları Nelerdir?

* **Giderdiği Eksiklik:** Klasik DP'nin kapasite veya değer büyüklüğüne bağlı kalma (**Pseudo-Polynomial**) zayıflığını giderir. Ayrıca Branch and Bound gibi algoritmaların en kötü senaryoda $O(2^N)$ üstel zamana düşme riskini ortadan kaldırır.
* **Önceki Yöntemlere Göre Avantajları:**
* **Kapasiteden ($W$) Bağımsız Çalışma:** Çanta kapasitesi ne kadar büyük olursa olsun algoritmanın çalışma süresini etkilemez.
* **Matematiksel Doğruluk Garantisi:** Algoritmanın bulduğu çözüm keyfi bir tahmin değildir; kesin optimal çözümün ($\text{OPT}$) en az $(1 - \epsilon)$ katı olacağını garanti eder:

$$\text{Bulunan Değer} \ge (1 - \epsilon) \cdot \text{OPT}$$


* **Esnek Performans Kontrolü:** Hata toleransı $\epsilon$ küçüldükçe doğruluk artar; $\epsilon$ büyüdükçe algoritma hızlanır.



### Hangi Durumda Tercih Edilir?

* Çanta kapasitesinin ($W$) veya eşya değerlerinin ($V$) çok büyük olduğu, klasık DP'nin belleğe sığmadığı durumlarda.
* %100 kesin optimal çözüm yerine %95–%99 doğrulukta bir çözümün milisaniyeler içinde elde edilmesinin yeterli olduğu endüstriyel senaryolarda.

### Hangi Durumda Tercih EDİLMEZ?

* Kuruşu kuruşuna **%100 kesin (optimal)** çözümün şart olduğu durumlarda.
* $N$ eşya sayısının ve $W$ kapasitesinin zaten küçük olduğu ve standart DP'nin sorunsuz çalıştığı durumlarda.

---

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir (Değer Ölçekleme - Value Scaling)

> *"Devasa eşya değerlerinin sonundaki basamakları/küsüratları hesaplanabilir bir ölçekleme faktörü ($K$) ile bölüp küçült. Küçültülmüş değerlerle Değer Tabanlı DP çalıştır ve sonucu tekrar $K$ ile çarparak gerçek değere dönüştür."*

### Çalışma Prensibi

1. **En Büyük Değerin Bulunması:** Eşyalar arasındaki maksimum değer ($V_{max}$) tespit edilir.
2. **Ölçekleme Faktörünün ($K$) Hesaplanması:** Kullanıcının belirlediği hata payı $\epsilon$ ($0 < \epsilon < 1$) ve eşya sayısı $N$ kullanılarak $K$ faktörü türetilir:

$$K = \frac{\epsilon \cdot V_{max}}{N}$$


3. **Değerlerin Küçültülmesi:** Her bir eşyanın değeri $K$'ye bölünerek aşağı yuvarlanır:

$$v_i' = \lfloor \frac{v_i}{K} \rfloor$$


4. **Değer Tabanlı DP (Value-Based DP):** Yeni küçültülmüş değerler ($v_i'$) kullanılarak min-ağırlık tabanlı DP matrisi/dizisi doldurulur. $dp[v]$, $v$ ölçeklenmiş değerine ulaşmak için gereken **minimum ağırlığı** saklar.
5. **Geri Ölçekleme (Descaling):** Kapasite $W$'yi aşmayan en yüksek ölçeklenmiş değer $v_{best}'$ bulunur. Gerçek yaklaşık değer şu şekilde hesaplanır:

$$\text{Approximate Real Value} = \text{round}(v_{best}' \cdot K)$$



---

## 4. Veri Yapıları

* **`Item` Structure:** Eşyanın `weight`, `value` ve hesaplanan `scaled_value` ($v_i'$) bilgilerini tutar.
* **`std::vector<int> dp` (1D Dizi):** Değer tabanlı DP durumunu saklar.
* `dp[v]`: $v$ ölçeklenmiş değerini elde etmek için ihtiyaç duyulan **minimum ağırlığı** temsil eder. Başlangıçta tüm elemanlar sonsuz ($\infty$) ile ilklendirilir; `dp[0] = 0` yapılır.



---

## 5. Sözde Kod (Pseudo-Code)

```text
solveScaledDP(W, items, max_scaled_val):
    N ← items.size()
    max_possible_value ← N * max_scaled_val
    
    // dp[v]: v değerine ulaşmak için gereken MINIMUM AĞIRLIK
    dp ← max_possible_value + 1 boyutunda SONSUZ (INF) ile dolu dizi
    dp[0] ← 0

    HER item İÇİN items İÇİNDE:
        HER v İÇİN max_possible_value'dan item.scaled_value'ya GERİYE DOĞRU:
            dp[v] ← MIN(dp[v], dp[v - item.scaled_value] + item.weight)

    // Kapasite W'yi aşmayan en yüksek değeri bul
    best_scaled_value ← 0
    HER v İÇİN max_possible_value'dan 0'a GERİYE DOĞRU:
        EĞER dp[v] <= W İSE:
            best_scaled_value ← v
            DÖNGÜDEN ÇIK

    DÖNDÜR best_scaled_value

knapsackFPTAS(W, items, epsilon):
    N ← items.size()
    V_max ← MAX(items[i].value)

    K ← (epsilon * V_max) / N
    EĞER K < 1.0 İSE K ← 1.0

    // Değerleri ölçekle
    max_scaled_val ← 0
    HER item İÇİN items İÇİNDE:
        item.scaled_value ← FLOOR(item.value / K)
        max_scaled_val ← MAX(max_scaled_val, item.scaled_value)

    best_scaled_val ← solveScaledDP(W, items, max_scaled_val)
    approx_real_value ← ROUND(best_scaled_val * K)

    DÖNDÜR approx_real_value

```

---

## 6. Zaman Karmaşıklığı

* **Best Case:** $O\left(N^3 \cdot \frac{1}{\epsilon}\right)$
* **Average Case:** $O\left(N^3 \cdot \frac{1}{\epsilon}\right)$
* **Worst Case:** $O\left(N^3 \cdot \frac{1}{\epsilon}\right)$
* **Açıklama:** Değer tabanlı DP matrisinin maksimum değeri $N \cdot V_{max}' = N \cdot \lfloor \frac{N}{\epsilon} \rfloor = O(N^2 / \epsilon)$ mertebesindedir. $N$ adet eşya için bu tablonun taranması toplam süreyi $O(N^3 / \epsilon)$ yapar. Karmaşıklık $W$ kapasitesinden veya ham $V$ değerlerinden **tamamen bağımsızdır** ve polinomiyaldir.



---

## 7. Alan Karmaşıklığı

* **Ek Bellek Kullanımı (Auxiliary Space):** $O\left(N^2 \cdot \frac{1}{\epsilon}\right)$
* **Kullanılan Veri Yapılarının Etkisi:**
* **`dp` Dizisi:** $1D$ alan optimizasyonu ile kullanılan dizinin boyutu $O(N^2 / \epsilon)$ kadardır.
* **Özyineleme Yığını (Call Stack):** Özyineleme kullanılmadığı için $O(1)$'dir.



---