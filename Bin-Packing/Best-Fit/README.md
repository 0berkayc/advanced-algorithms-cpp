# Best-Fit (Bin Packing) Algoritması

## 1. Problemin Tanımı ve Zorluğu
* **Problem Tanımı:** Sabit $C$ kapasitesine sahip kutulara (bins), farklı boyutlardaki $N$ adet nesneyi, hiçbir kutunun kapasitesini aşmayacak şekilde **minimum sayıda kutu kullanarak** yerleştirme problemidir.
* **Zorluk Sınıfı:** **NP-Hard** zorluk sınıfındadır. Girdi boyutu ($N$) büyüdükçe kesin (optimum) çözümü veren kombinasyon sayısı üstel olarak arttığından ($O(2^N)$ veya $O(N!)$), polinom zamanda kesin çözümünü bulmak imkansız derecede yüksek hesaplama maliyeti çıkarır.

## 2. Algoritmanın Geliştirilme Amacı
* **Bu algoritma neden geliştirilmiştir?** Best-Fit, nesneleri rastgele veya ilk bulunan yere koymak yerine, yerleştirme sonrası kutuda **en az boşluğu (minimum artan kapasiteyi)** bırakacak kutuyu seçerek kutu içi boşluk parçalanmasını (internal fragmentation) en aza indirmek amacıyla geliştirilmiştir.
* **Hangi eksikliği gidermektedir?** First-Fit algoritmasının solda bulunan ilk uygun kutuya körlemesine yerleştirme yapıp ileriki nesneler için sıkı/uygun boşlukları harcaması eksikliğini giderir.
* **Önceki yöntemlere göre avantajı nedir?** 
  * Kutulardaki artık boşlukları son derece sıkı yönetir.
  * Genellikle First-Fit'e göre daha az artık boşluk bırakır ve sıkı doluluk sağlar.
  * Nesnelerin tamamını önceden bilmeye ihtiyaç duymaz, gelen nesneyi anında işleyebilir (*Online Algorithm*).
* **Hangi durumda tercih edilir?** 
  * Kutularda kalan küçük alanların değerlendirilmesinin kritik olduğu ve parçalanmanın istenmediği bellek yönetimi (örneğin Best-Fit Memory Allocation) veya lojistik senaryolarında.
* **Hangi durumda tercih edilmez?**
  * Nesneleri büyükten küçüğe sıralama imkanımız varsa (bu durumda sıralama yapabilen *First-Fit Decreasing* veya *Best-Fit Decreasing* çok daha iyi performans verir).
  * Çok büyük kutu sayılarında her eleman için tüm kutuların taranması arama maliyetini artırabileceğinden sadece hızın arandığı basit uygulamalarda.

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir
Gelen her bir nesne için mevcut açık kutuların tamamı taranır. Nesnenin sığabileceği kutular arasından, yerleştirme yapıldıktan sonra **geriye en az boşluk bırakacak olan kutu** seçilir. Eğer tam sığan (kalan alanı 0 yapan) bir kutu bulunursa arama anında kesilir. Hiçbir kutuya sığmıyorsa yeni bir kutu açılır.

### Çalışma Prensibi
1. Açık kutuların kalan kapasitelerini takip etmek için boş bir liste/dizi (`bins`) oluşturulur.
2. `items` dizisindeki her bir nesne sıra ile alınır.
3. Mevcut tüm kutular taranır:
   * $remainingSpace = bins[j] - items[i]$ değeri hesaplanır.
   * $remainingSpace \ge 0$ olan kutular arasından en küçük $remainingSpace$ değerini veren $bestBinIndex$ güncellenir.
   * Eğer $remainingSpace == 0$ olursa tam uyum sağlanmıştır, arama anında sonlandırılır ($break$).
4. Tarama sonunda uygun bir kutu ($bestBinIndex \neq -1$) bulunduysa nesne o kutuya yerleştirilir ve kapasite güncellenir.
5. Uygun kutu bulunamadıysa yeni bir kutu açılır.

### Veri Yapıları
* `std::vector<int> items`: Yerleştirilecek nesne boyutlarını tutan dinamik dizi.
* `std::vector<int> bins`: Açık olan kutuların anlık kalan kapasitelerini saklayan dinamik dizi.

### Sözde Kod (Pseudo-Code)
```text
BEST-FIT(items, binCapacity)
    bins = boş dizi

    DÖNGÜ i = 0'dan items.size - 1'e kadar:
        bestBinIndex = -1
        minSpace = binCapacity + 1  // Başlangıçta ulaşılamaz büyük bir değer

        DÖNGÜ j = 0'dan bins.size - 1'e kadar:
            kalanYer = bins[j] - items[i]

            EĞER kalanYer >= 0 İSE:
                EĞER kalanYer == 0 İSE:
                    bestBinIndex = j
                    minSpace = 0
                    DÖNGÜDEN ÇIK (break)  // Tam uyum bulundu döngüyü bitir
                
                AKSİ HÂLDE EĞER kalanYer < minSpace İSE:
                    minSpace = kalanYer
                    bestBinIndex = j      // En iyi adayı güncelle

        // 2. Tarama bitti, karar ver
        EĞER bestBinIndex != -1 İSE:
            bins[bestBinIndex] = bins[bestBinIndex] - items[i]
        AKSİ HÂLDE:
            bins dizisine (binCapacity - items[i]) yeni kutu olarak ekle

    DÖNDÜR bins.size
```

## 4. Zaman Karmaşıklığı (Time Complexity)
$N$ nesne sayısını, $K$ ise açılan toplam kutu sayısını temsil etmek üzere ($K \le N$):
* **Best Case — $O(N)$:** Her gelen nesnenin mevcut bir kutuyu tam doldurduğu ($remainingSpace == 0$) ve ilk aramada bulunduğu durum.
* **Average Case — $O(N \cdot K)$ / $O(N^2)$:** Ortalama senaryoda her nesne için açık olan kutuların tamamı taranarak en küçük artık alan aranır.
* **Worst Case — $O(N^2)$:** Her nesne için mevcut tüm kutuların taranması durumu.
  * *İyileştirme Notu:* Kutuların kalan alanları bir *Self-Balancing Binary Search Tree* (örneğin C++ `std::set` / `std::map`) üzerinde saklanırsa, en az boşluk bırakan kutu $O(\log K)$ sürede bulunabilir ve en kötü zaman karmaşıklığı $O(N \log N)$ seviyesine çekilebilir.

## 5. Alan Karmaşıklığı (Space Complexity)
* **Ek Bellek Kullanımı (Auxiliary Space): $O(K)$**
  * Sadece açılan kutuların kalan kapasitelerini saklamak için $K$ elemanlı `bins` dizisi kullanılır ($K \le N$). En kötü senaryoda $O(N)$ alan harcanır.