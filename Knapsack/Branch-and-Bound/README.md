# 0/1 Knapsack Problemi - Branch and Bound Yaklaşımı

Bu doküman, **0/1 Knapsack** probleminin **Branch and Bound** tekniği ile çözümlenmesinin teorik esaslarını, budama (pruning) mantığını, veri yapılarını, sözde kodunu ve karmaşıklık analizini içermektedir.

---

## 1. Problemin Tanımı, Zorluğu ve Geliştirilme Amacı

* **Problem Tanımı:** Elimizde belirli bir $W$ ağırlık kapasitesine sahip bir çanta ve her birinin ağırlığı ($w_i$) ile değeri ($v_i$) olan $N$ adet eşya bulunmaktadır. Amaç, çanta kapasitesini aşmadan toplam değeri **maksimum** yapmaktır. Eşyalar parçalanamaz ($0/1$ kısıtı).
* **Zorluk Sınıfı:** **NP-Complete** zorluk sınıfındadır.
* **Geliştirilme Amacı:** Klasik Geri Adım Atma (Backtracking) ve Kaba Kuvvet (Brute-Force) algoritmaları çözüme ulaşırken arama ağacının tüm kollarını "kör" bir şekilde tarar. Dinamik Programlama (DP) ise çanta kapasitesi $W$ çok büyüdüğünde $O(N \cdot W)$ boyutunda devasa bellek matrislerine ihtiyaç duyar ve kilitlenir.
**Branch and Bound**, geniş arama uzayını akıllı bir matematiksel üst sınır (Upper Bound) tahmini ile daraltmak ve DP'nin bellek darboğazına girdiği büyük kapasiteli problemlerde dahi **hızlı ve kesin (optimal)** sonuca ulaşmak amacıyla geliştirilmiştir.

---

## 2. Hangi Eksikliği Gidermektedir ve Avantajları Nelerdir?

* **Giderdiği Eksiklik:** Klasik özyinelemeli yöntemlerin, sonucunun kötü olacağı en başından belli olan umutsuz dallara girip zaman kaybetmesi eksikliğini giderir. Ayrıca Dinamik Programlama'nın **kapasite değerine ($W$) bağımlı olma (Pseudo-Polynomial)** zayıflığını aşar.
* **Önceki Yöntemlere Göre Avantajı:**
* **Umutsuz Dalları Anında Budama (Pruning):** Her adımda yapay olarak hesaplanan hileli üst sınır (Upper Bound), elimizdeki dürüst rekor değerden (`maxProfit`) küçükse o dalın altındaki tüm olasılıklar $O(1)$ sürede çöpe atılır.
* **En Potansiyelli Yolu Öne Alma (Best-First Search):** Maksimum Öncelikli Kuyruk (Max-Priority Queue) kullanılarak vaat ettiği değer en yüksek olan düğüm en önce açılır. Rekor değer en başta çok yükseldiği için kalan dalların %99'u budanır.



### Hangi Durumda Tercih Edilir?

* Çanta kapasitesinin ($W$) çok büyük olduğu ($W = 10^9$ gibi) ve Dinamik Programlama matrisinin belleğe sığmadığı durumlarda.
* Eşya sayısının orta/büyük ölçekte olduğu ve kesin (optimal) çözümün istendiği senaryolarda.

### Hangi Durumda Tercih Edilmez?

* Eşyaların değer ve ağırlık oranlarının birbirine çok yakın olduğu "Kötü Senaryo (Worst Case)" durumlarında. Bu senaryoda budama mekanizması işleyemez ve algoritma $O(2^N)$ kaba kuvvet aramasına dönüşür.
* Sadece yaklaşık (near-optimal) bir çözümün yeterli olduğu ve çok daha hızlı çalışan sezgisel/yaklaşım algoritmalarının (örneğin FPTAS veya Greedy) tercih edildiği durumlar.

---

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir (Best-First Search & Upper Bound Pruning)

> *"Bir yola girmeden önce, eşyaları parçalamak serbest olsaydı (Fractional Knapsack) o yoldan en fazla ne kadar hayali değer çıkacağını hesapla. Bu hayali tavan değer elindeki dürüst rekordan düşükse, o yola hiç girme ve anında buda"*

### Çalışma Prensibi

1. **Ön Sıralama:** Tüm eşyalar birim değerlerine ($v_i / w_i$) göre büyükten küçüğe sıralanır.
2. **Kök Düğümün Kurulması:** Kök düğüm ilklendirilir. Eşyaları parçalama mantığıyla bu düğüm için teorik tavan değer (`bound`) hesaplanır ve Max-Priority Queue'ya atılır.
3. **Kuyruktan Düğüm Seçimi:** Kuyruğun en tepesindeki (en yüksek `bound` değerine sahip) düğüm çekilir.
4. **Budama Kontrolü:** Çekilen düğümün `bound` değeri elimizdeki güncel `maxProfit` değerinden küçük veya eşitse, bu dal ilerletilmeden çöpe atılır (budanır).
5. **Dallanma (Branching):** Düğüm umut vaat ediyorsa sıradaki eşya için iki çocuk düğüm oluşturulur:
* **AL (Take) Dalı:** Ağırlık ve değer güncellenir. Kapasite aşılmadıysa ve değer `maxProfit`'ten büyükse yeni rekor kaydedilir.
* **ALMA (Skip) Dalı:** Ağırlık ve değer değişmez.


6. **Kuyruğa Ekleme:** Her iki çocuk düğümün `bound` değerleri hesaplanır. `bound > maxProfit` şartını sağlayan düğümler tekrar kuyruğa eklenir; sağlayamayanlar anında elenir.

---

## 4. Veri Yapıları

* **`Item` Structure:** Eşyaların id, ağırlık (`weight`), değer (`value`) verilerini saklar.
* **`Node` Structure:** Arama ağacındaki bir durumu temsil eder:
* `level`: Hangi eşya seviyesinde olunduğu.
* `profit`: O ana kadarki toplam gerçek değer.
* `weight`: O ana kadarki toplam gerçek ağırlık.
* `bound`: O daldan ulaşılabilecek maksimum teorik üst sınır.


* **`std::priority_queue<Node>` (Max-Heap):** Düğümleri `bound` değerlerine göre büyükten küçüğe sıralayan öncelikli kuyruk yapısı.

---

## 5. Sözde Kod (Pseudo-Code)

```text
calculateBound(u, N, W, items):
    EĞER u.weight >= W İSE DÖNDÜR 0
    profit_bound ← u.profit
    j ← u.level + 1
    totweight ← u.weight

    X_DÖNGÜSÜ (j < N VE totweight + items[j].weight <= W):
        totweight ← totweight + items[j].weight
        profit_bound ← profit_bound + items[j].value
        j ← j + 1

    // Kalan boşluğa sığmayan ilk eşyayı parçalayarak tavanı tamamla
    EĞER j < N İSE:
        profit_bound ← profit_bound + (W - totweight) * (items[j].value / items[j].weight)
    
    DÖNDÜR profit_bound


knapsackBranchAndBound(W, items):
    items dizisini (value / weight) oranına göre AZALAN sırada sırala
    PQ ← Max-Priority Queue (Düğümleri 'bound' değerine göre sıralar)

    u ← Yeni Düğüm (level: -1, profit: 0, weight: 0)
    u.bound ← calculateBound(u, N, W, items)
    PQ.push(u)

    maxProfit ← 0

    X_DÖNGÜSÜ (PQ boş değilken):
        u ← PQ.top()
        PQ.pop()

        // BUDAMA 1: Tavan değer rekoru kıramıyorsa geç
        EĞER u.bound <= maxProfit İSE DEVAM ET

        v.level ← u.level + 1

        // --- SEÇENEK 1: Eşyayı AL ---
        v.weight ← u.weight + items[v.level].weight
        v.profit ← u.profit + items[v.level].value

        EĞER v.weight <= W VE v.profit > maxProfit İSE:
            maxProfit ← v.profit

        v.bound ← calculateBound(v, N, W, items)
        EĞER v.bound > maxProfit İSE PQ.push(v) // BUDAMA 2

        // --- SEÇENEK 2: Eşyayı ALMA ---
        v.weight ← u.weight
        v.profit ← u.profit
        v.bound ← calculateBound(v, N, W, items)
        EĞER v.bound > maxProfit İSE PQ.push(v) // BUDAMA 3

    DÖNDÜR maxProfit

```

---

## 6. Zaman Karmaşıklığı

* **Best Case:** $O(N \log N)$
* *Açıklama:* Başlangıçtaki eşya sıralaması $O(N \log N)$ sürer. Arama ağacının ilk sol dalında doğrudan optimal çözüme ulaşıldığında ve kalan tüm kollar anında budandığında algoritma neredeyse lineer adımla biter.


* **Average Case:** $O(2^N)$ teorik üst sınıra sahip olmasına rağmen, budama mekanizması sayesinde ortalama çalışma süresi klasik özyinelemeli çözümlere göre **milisaniyeler seviyesine düşer**.
* **Worst Case:** $O(2^N)$
* *Açıklama:* Hiçbir dalın budanamadığı (üst sınırların hep yüksek kaldığı) en kötü senaryoda, açılan toplam düğüm sayısı ikili arama ağacının tamamını ($2^N$) kapsar.



---

## 7. Alan Karmaşıklığı

* **Ek Bellek Kullanımı (Auxiliary Space):** $O(2^N)$
* **Kullanılan Veri Yapılarının Etkisi:**
* **`priority_queue` (Öncelikli Kuyruk):** Budanamayan aktif düğümler kuyrukta tutulur. En kötü senaryoda kuyruktaki düğüm sayısı $O(2^N)$ seviyesine ulaşabilir.
* **`items` Dizisi:** Eşyaları saklayan $N$ elemanlı dizi $O(N)$ alan kaplar.
