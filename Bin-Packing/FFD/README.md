# First-Fit Decreasing (FFD) Algoritması

## 1. Problemin Tanımı ve Zorluğu
* **Problem Tanımı:** Sabit $C$ kapasitesine sahip kutulara (bins), farklı boyutlardaki $N$ adet nesneyi, hiçbir kutunun kapasitesini aşmayacak şekilde **minimum sayıda kutu kullanarak** yerleştirme problemidir.
* **Zorluk Sınıfı:** **NP-Hard** zorluk sınıfındadır. Girdi boyutu ($N$) büyüdükçe kesin (optimum) çözümü veren kombinasyon sayısı üstel olarak arttığından ($O(2^N)$ veya $O(N!)$), polinom zamanda kesin çözümünü bulmak imkansız derecede yüksek hesaplama maliyeti çıkarır.

## 2. Algoritmanın Geliştirilme Amacı
* **Bu algoritma neden geliştirilmiştir?** FFD, standart First-Fit algoritmasının girdi sırasına bağımlı yapısını kırarak pratik sonuçları **optimum çözüme en yakın** (yaklaşık olarak en fazla $1.22 \cdot \text{OPT} + 1$) seviyeye çekmek amacıyla geliştirilmiş çevrimdışı (*offline*) bir sezgisel algoritmadır.
* **Hangi eksikliği gidermektedir?** Küçük nesnelerin önden yerleşip kutuların üst kısımlarında düzensiz boşluklar bırakması ve sona kalan büyük nesnelerin bu boşluklara sığmayıp gereksiz yere yeni kutular açması eksikliğini giderir.
* **Önceki yöntemlere göre avantajı nedir?** 
  * Yerleştirmesi zor olan kaba/büyük nesneleri başa alıp kutulara "iskelet" olarak oturtur.
  * Küçük nesneleri sona bıraktığı için bu nesneler büyüklerden kalan küçük girintilere "dolgu malzemesi" gibi rahatça yerleşir.
  * Standart First-Fit ve Best-Fit yöntemlerine kıyasla çok daha yüksek oranda minimum kutu sayısını (optimumu) yakalar.
* **Hangi durumda tercih edilir?** 
  * Tüm nesnelerin boyutlarının önceden bilindiği ve dizinin sıralanmasına imkan olan çevrimdışı (*offline*) senaryolarda.
  * Kutu tasarrufunun (malzeme, lojistik veya sunucu maliyeti) üst düzey önem taşıdığı uygulamalarda.
* **Hangi durumda tercih edilmez?**
  * Verilerin anlık olarak aktığı ve gelen verinin anında işlenmesi gereken akış (*online*) sistemlerinde (çünkü sıralama yapmak için tüm verinin gelmesini beklemek gerekir).

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir
Nesneler döngüye girmeden önce **büyükten küçüğe (azalan sırada)** sıralanır. Ardından sıralanmış bu liste üzerinde standart First-Fit algoritması çalıştırılır: Gelen her nesne için açık olan kutular soldan sağa taranır ve sığdığı ilk kutuya yerleştirilir.

### Çalışma Prensibi
1. Orijinal veriyi korumak amacıyla `items` dizisinin bir kopyası alınır.
2. Kopya dizi $O(N \log N)$ karmaşıklıkla azalan sırada (`std::greater<int>()`) sıralanır.
3. Açık kutuların kalan kapasitelerini takip etmek için boş bir liste/dizi (`bins`) oluşturulur.
4. Sıralanmış dizideki her bir nesne sırayla alınır ve soldan sağa açık kutular taranır:
   * Eğer $sortedItems[i] \le bins[j]$ ise nesne $j$. kutuya konur, kalan kapasite güncellenir ve iç döngüden çıkılır ($break$).
5. Nesne mevcut kutuların hiçbirine sığmazsa, $C - sortedItems[i]$ kalan alanıyla yeni bir kutu açılır.

### Veri Yapıları
* `std::vector<int> items`: Yerleştirilecek nesne boyutlarını tutan dinamik dizi.
* `std::vector<int> sortedItems`: Büyükten küçüğe sıralanmış nesne boyutları dizisi.
* `std::vector<int> bins`: Açık olan kutuların anlık kalan kapasitelerini saklayan dinamik dizi.

### Sözde Kod (Pseudo-Code)
```text
FIRST-FIT-DECREASING(items, binCapacity)
    sortedItems = COPY(items)
    SORT_DESCENDING(sortedItems)  // Büyükten küçüğe sırala

    bins = boş dizi
    n = sortedItems.size

    DÖNGÜ i = 0'dan n - 1'e kadar:
        isPlaced = FALSE

        // Mevcut kutuları soldan sağa tara
        DÖNGÜ j = 0'dan bins.size - 1'e kadar:
            EĞER sortedItems[i] <= bins[j] İSE:
                bins[j] = bins[j] - sortedItems[i]
                isPlaced = TRUE
                DÖNGÜDEN ÇIK (break)

        // Hiçbir kutuya sığmadıysa yeni kutu aç
        EĞER isPlaced == FALSE İSE:
            bins dizisine (binCapacity - sortedItems[i]) ekle

    DÖNDÜR bins.size
```

## 4. Zaman Karmaşıklığı (Time Complexity)
$N$ nesne sayısını, $K$ ise açılan toplam kutu sayısını temsil etmek üzere ($K \le N$):
* **Sıralama Aşaması — $O(N \log N)$:** Hızlı sıralama (Quicksort/Introsort) algoritması maliyeti.
* **Yerleştirme Aşaması — $O(N \cdot K)$ / $O(N^2)$:** Standart First-Fit tarama maliyeti.
* **Toplam Karmaşıklık — Best / Average / Worst Case:**
  * **Best Case — $O(N \log N)$:** Sıralama $O(N \log N)$ sürer, yerleştirme ise elemanların her zaman ilk kutulara sığmasıyla $O(N)$ tamamlanır.
  * **Average & Worst Case — $O(N^2)$:** Standart dizi taraması ile toplam süre $O(N \log N + N^2) = O(N^2)$ olur.
  * *İyileştirme Notu:* Kutuların kalan alanları bir *Max-Heap* veya *Segment Tree* veri yapısı üzerinde tutulursa yerleştirme adımı da $O(N \log N)$ seviyesine çekilerek tüm algoritma en kötü senaryoda bile **$O(N \log N)$** sürede çalıştırılabilir.

## 5. Alan Karmaşıklığı (Space Complexity)
* **Ek Bellek Kullanımı (Auxiliary Space): $O(N + K)$**
  * Sıralanmış verileri saklamak için $N$ elemanlı kopyalama dizisi (`sortedItems`) ve açılan kutuları saklamak için $K$ elemanlı `bins` dizisi kullanılır ($K \le N$). Toplam ek alan $O(N)$ seviyesindedir.