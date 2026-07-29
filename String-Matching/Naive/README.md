# Naive String Matching Algoritması

## 1. Problem Tanımı ve Zorluğu
* **Problem Tanımı:** Verilen bir $N$ uzunluğundaki metin (`text`) içerisinde, $M$ uzunluğundaki bir kalıbın (`pattern`) geçtiği tüm başlangıç indekslerini bulma problemidir.
* **Zorluk Sınıfı:** $P$ (Polynomial Time) zorluk sınıfındadır. Metin üzerinde kayan pencere mantığıyla doğrusal veya karesel zaman karmaşıklığında tam çözüm sunar.

## 2. Algoritmanın Geliştirilme Amacı
* **Bu algoritma neden geliştirilmiştir?** Metin içi desen arama problemlerine yönelik en temel, sezgisel ve uygulanması en kolay çözümü sağlamak amacıyla geliştirilmiştir.
* **Hangi eksikliği gidermektedir?** Herhangi bir karmaşık matematiksel ön işleme (pre-processing) adımı veya ek veri yapısı gereksinimi olmadan arama yapabilmeyi sağlar. Karşılaştırma bazlı arama yöntemleri için temel referans (baseline) algoritmadır.
* **Önceki yöntemlere göre avantajı nedir?** Tamamen sezgisel olduğu için kodlama karmaşıklığı sıfıra yakındır. Metin veya kalıp üzerinde ön hazırlık süresi ($O(M)$ veya $O(N)$ tablosu vb.) gerektirmez.
* **Hangi durumda tercih edilir?** 
  * Metin ($N$) ve aranan kalıp ($M$) boyutları küçük olduğunda.
  * Bellek (RAM) kullanımının son derece kısıtlı olduğu sistemlerde.
  * Algoritmanın hızlıca prototiplenmesi ve doğrulanması gereken durumlarda.
* **Hangi durumda tercih edilmez?** 
  * Çok büyük boyutlu metinlerde veya veritabanı seviyesindeki aramalarda.
  * Birbiriyle eşleşen çok sayıda tekrar eden karakter içeren dizilimlerde (Örn: `AAAAAAAAB` metni içinde `AAAB` aramak) zaman karmaşıklığı tavan yaptığı için tercih edilmez.

## 3. Algoritmanın Çalışma Mantığı
* **Temel Fikir:** Kayan pencere (sliding window) yaklaşımıyla kalıbı metnin başından sonuna kadar her pozisyonda hizalamak ve karakter karakter karşılaştırmaktır.
* **Çalışma Prensibi:** 
  1. Metnin $0$. indeksinden başlanarak kalıp metin üzerine hizalanır.
  2. Kalıbın karakterleri soldan sağa doğru metindeki karşılıklarıyla teker teker kıyaslanır.
  3. Herhangi bir karakterde uyuşmazlık (mismatch) yakalandığı an pencere $1$ birim sağa kaydırılır ve kıyaslama baştan başlar.
  4. Kalıbın tüm karakterleri eşleştiğinde ($j = M$), başlangıç indeksi ($i$) sonuç listesine kaydedilir.
* **Veri Yapıları:** 
  * `std::string`: Metin ve aranan kalıp verilerini bellekte tutmak için.
  * `std::vector<int>`: Bulunan eşleşme indekslerini dinamik olarak saklamak için.

### Sözde Kod (Pseudo-Code)
```text
n = text uzunluğu
m = pattern uzunluğu
matches = boş liste

0' dan (n-m)' ye kadar her i değeri için:
    j = 0
    j < m ve text[i + j] ile pattern[j] eşit olduğu sürece:
        j' yi bir artır

    eğer j = m ise:
        i indexini matches listesine ekle

matches listesini döndür.
```
## 4. Zaman Karmaşıklığı (Time Complexity)
Geliştirilen C++ koddaki döngü yapılarına göre analiz:
* **Best Case — $O(N)$:** Metnin her konumundaki ilk karakter, kalıbın ilk karakteriyle eşleşmezse içteki `while` döngüsü her adımda yalnızca $1$ kez çalışır. Toplamda dıştaki `for` döngüsü kadar ($N - M + 1 \approx N$) işlem yapılır.
* **Average Case — $O(N + M)$:** Doğal dillerdeki metinlerde uyuşmazlıklar genellikle ilk $1$-$2$ karakterde yakalandığı için ortalama çalışma süresi doğrusala çok yakındır.
* **Worst Case — $O(N \times M)$:** Metin ve kalıp aynı karakter tekrarlarından oluşuyorsa (Örn: Metin: `AAAAAAAAAAAAAAAA`, Kalıp: `AAAB`), içteki `while` döngüsü her $i$ adımı için $M$ kez çalışır ve $O((N - M + 1) \times M) \approx O(N \times M)$ karmaşıklığına ulaşır.

## 5. Alan Karmaşıklığı (Space Complexity)
* **Ek Bellek Kullanımı (Auxiliary Space):** $O(1)$
  * Arama işlemi sırasında ekstra hiçbir tablo, matris veya yardımcı ağaç yapısı oluşturulmaz. Arama doğrudan girdi verileri üzerinde gerçekleştirilir.
* **Kullanılan Veri Yapılarının Etkisi:**
  * `text` ve `pattern` değişkenleri salt okunur (`const reference`) olarak fonksiyona geçildiği için kopyalama yapılmaz, ekstra alan kaplamaz.
  * Yalnızca bulunan eşleşme indekslerini döndürmek amacıyla kullanılan `std::vector<int>` yapısı, $K$ kadar eşleşme durumu için $O(K)$ kadar dinamik bellek alanı kaplar ($K \le N$).
