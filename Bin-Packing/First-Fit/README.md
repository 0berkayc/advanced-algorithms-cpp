# First-Fit (Bin Packing) Algoritması

## 1. Problemin Tanımı ve Zorluğu
* **Problem Tanımı:** Sabit $C$ kapasitesine sahip kutulara (bins), farklı boyutlardaki $N$ adet nesneyi, hiçbir kutunun kapasitesini aşmayacak şekilde **minimum sayıda kutu kullanarak** yerleştirme problemidir.
* **Zorluk Sınıfı:** **NP-Hard** zorluk sınıfındadır. Girdi boyutu ($N$) büyüdükçe kesin (optimum) çözümü veren kombinasyon sayısı üstel olarak arttığından ($O(2^N)$ veya $O(N!)$), polinom zamanda kesin çözümünü bulmak imkansız derecede yüksek hesaplama maliyeti çıkarır.

## 2. Algoritmanın Geliştirilme Amacı
* **Bu algoritma neden geliştirilmiştir?** NP-Hard olan Bin Packing probleminde mükemmel/optimum çözümü aramak yerine, saniyeler veya milisaniyeler içinde mükemmele yakın %95'lik "yeterince iyi" bir çözümü (sezgisel/approximation) üretmek amacıyla geliştirilmiştir.
* **Hangi eksikliği gidermektedir?** Kaba kuvvet (Brute-Force / Backtracking) yaklaşımların $N > 30$ gibi küçük nesne gruplarında dahi kilitlenmesi ve saatlerce/günlerce çalışması eksikliğini giderir.
* **Önceki yöntemlere göre avantajı nedir?** 
  * Karar verme mekanizması çok basittir ve işleme maliyeti son derece düşüktür.
  * Nesnelerin tamamını önceden bilmeye veya sıralamaya ihtiyaç duymaz. Akış anında (*Online Algorithm*) gelen nesneleri anında işleyebilir.
* **Hangi durumda tercih edilir?** 
  * Verinin anlık olarak aktığı ve sırayla işlenmesi gerektiği durumlarda (örneğin bellek yönetimi, canlı ağ paket dağıtımı).
  * Hızlı yanıt süresinin, optimum kutu sayısından daha kritik olduğu senaryolarda.
* **Hangi durumda tercih edilmez?**
  * Nesnelerin tamamı önceden biliniyorsa ve sıralama imkanı varsa (bu durumda sıralama yapabilen *First-Fit Decreasing* çok daha yüksek verim sağlar).
  * Kullanılacak kutu sayısındaki %1'lik bir iyileşmenin bile devasa finansal tasarruf sağladığı durumlarda.

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir
Gelen her bir nesne için mevcut açık kutular soldan sağa doğru taranır. Nesnenin sığdığı **ilk kutuya** atama yapılır ve arama anında sonlandırılır. Eğer nesne mevcut açık kutuların hiçbirine sığmıyorsa, yeni bir kutu açılır.

### Çalışma Prensibi
1. Açık kutuların kalan kapasitelerini takip etmek için boş bir liste/dizi (`bins`) oluşturulur.
2. `items` dizisindeki her bir nesne sıra ile alınır.
3. Soldan sağa doğru `bins` dizisi taranır:
   * Eğer $items[i] \le bins[j]$ ise nesne $j$. kutuya konur, kalan kapasite $bins[j] = bins[j] - items[i]$ olarak güncellenir ve iç döngü sonlandırılır.
4. Eğer mevcut kutuların hiçbiri bu nesneyi alamazsa, `bins` dizisinin sonuna $C - items[i]$ kalan kapasitesiyle yeni bir kutu eklenir.

### Veri Yapıları
* `std::vector<int> items`: Yerleştirilecek nesne boyutlarını tutan dinamik dizi.
* `std::vector<int> bins`: Açık olan kutuların anlık kalan kapasitelerini saklayan dinamik dizi.

### Sözde Kod (Pseudo-Code)
```text
FIRST-FIT(items, binCapacity):
    bins = boş dizi (açık kutuların kalan kapasitelerini tutar)

    DÖNGÜ i = 0'dan items.size - 1'e kadar:
        isPlaced = FALSE (nesne yerleşti mi takibi)

        DÖNGÜ j = 0'dan bins.size - 1'e kadar:
            EĞER items[i] <= bins[j] İSE:
                bins[j] = bins[j] - items[i]  // Kapasiteyi düş
                isPlaced = TRUE
                DÖNGÜDEN ÇIK (break)          // Nesne yerleşti, j döngüsünü bitir

        EĞER isPlaced == FALSE İSE:
            bins dizisine (binCapacity - items[i]) değerini yeni kutu olarak ekle

    GERİYE bins.size DÖNDÜR (kullanılan toplam kutu sayısı)
```

## 4. Zaman Karmaşıklığı (Time Complexity)
$N$ nesne sayısını, $K$ ise açılan toplam kutu sayısını temsil etmek üzere ($K \le N$):
* **Best Case — $O(N)$:** Tüm nesnelerin çok küçük olduğu veya her nesnenin her zaman ilk kutuya ($bins[0]$) sığdığı durum. İç döngü sadece 1 kez çalışır.
* **Average Case — $O(N \cdot K)$ / $O(N^2)$:** Ortalama senaryoda her nesne için açık olan $K$ adet kutunun bir kısmı taranır.
* **Worst Case — $O(N^2)$:** Nesne boyutlarının büyük olduğu ve sürekli yeni kutu açıldığı durum. $i$. nesne için $i-1$ adet kutunun tamamı taranır.
  * *İyileştirme Notu:* Kutuların kalan alanları bir *Self-Balancing Binary Search Tree* veya *Segment Tree* veri yapısında tutulursa en kötü zaman karmaşıklığı $O(N \log N)$ seviyesine düşürülebilir.

## 5. Alan Karmaşıklığı (Space Complexity)
* **Ek Bellek Kullanımı (Auxiliary Space): $O(K)$**
  * Sadece açılan kutuların kalan kapasitelerini saklamak için $K$ elemanlı `bins` dizisi kullanılır ($K \le N$). En kötü senaryoda her nesne için ayrı kutu açılırsa $O(N)$ alan harcanır.