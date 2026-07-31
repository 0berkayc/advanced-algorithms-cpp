# Jarvis's March Algoritması

## 1. Problemin Tanımı ve Zorluğu
* **Problem Tanımı:** 2 boyutlu düzlemde verilen $N$ adet noktayı tamamen kapsayan, en dıştaki noktalardan oluşan en küçük dış bükey çokgeni (Convex Hull) bulma problemidir.
* **Zorluk Sınıfı:** **$P$ (Polynomial Time)** zorluk sınıfındadır. Polinom zamanda kesin çözümü elde edilir.

## 2. Algoritmanın Geliştirilme Amacı
* **Bu algoritma neden geliştirilmiştir?** Sezgisel ve uygulaması en basit geometrik kapsayan kabuk algoritması olarak geliştirilmiştir. Fiziksel hediye paketleme mantığını simüle eder.
* **Hangi eksikliği gidermektedir?** Noktaların tüm olası kombinasyonlarını deneyen aşırı yavaş yaklaşımlar yerine geometrik yönelim (orientation/cross product) hesabıyla dış noktaları adım adım seçer.
* **Avantajı nedir?** Çıktıya duyarlı (output-sensitive) bir algoritmadır. Eğer sınır üzerindeki köşe noktası sayısı ($H$) toplam nokta sayısına ($N$) göre çok az ise oldukça hızlı çalışır.
* **Hangi durumda tercih edilir?** 
  * Kodlama kolaylığı ve düşük karmaşıklık istendiğinde.
  * Nokta sayısı çok fazla olsa bile sonucun az sayıda dış köşeden oluşacağı bilinen durumlarda.
* **Hangi durumda tercih edilmez?**
  * Tüm noktaların (veya büyük çoğunluğunun) dış sınırda yer aldığı en kötü durum senaryolarında (örneğin tüm noktaların bir çember üzerinde dizilmesi). Bu durumda performansı $O(N^2)$ seviyesine düşer.

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir
1. X koordinatı en küçük olan (en soldaki) nokta kesinlikle dış sınırda yer alır. Bu nokta başlangıç noktası olarak seçilir.
2. Saat yönünün tersine (CCW) tüm noktalar taranarak en dışta kalan bir sonraki köşe noktası bulunur.
3. Bulunan yeni köşe noktaları sırayla eklenir ve başlangıç noktasına dönülene kadar "paketleme" işlemine devam edilir.

### Çalışma Prensibi
1. `l` = En soldaki noktanın indeksi bulunur.
2. `p = l` yapılır (Mevcut köşe).
3. `p` noktasından çıkılarak rastgele bir `q` adayı seçilir.
4. Tüm `i` noktaları için `orientation(p, i, q)` kontrolü yapılır. Eğer `i` noktası `p-q` doğrusunun daha da solundaysa (CCW), yeni aday `q = i` yapılır.
5. Bulunan `q` noktası kabuğa eklenir ve `p = q` yapılarak döngü `p == l` olana kadar sürdürülür.

### Veri Yapıları
* `struct Point`: Noktaların $(x, y)$ koordinatlarını tutmak için.
* `std::vector<Point>`: Tüm noktaları ve bulunan kabuk (hull) noktalarını depolamak için.

### Sözde Kod (Pseudo-Code)
```text
JARVIS-MARCH(Noktalar):
    n = Noktalar dizisinin boyutu
    Eğer n < 3 ise geriye boş liste döndür

    1. En soldaki noktayı bul (l)
    l = 0
    1'den n-1'e kadar her i için:
        Eğer Noktalar[i].x < Noktalar[l].x ise: l = i
        Eğer Noktalar[i].x == Noktalar[l].x VE Noktalar[i].y < Noktalar[l].y ise: l = i

    Kabuk = boş liste
    p = l
    Tekrar et:
        Kabuk listesine Noktalar[p] ekle
        q = (p + 1) mod n
        
        0'dan n-1'e kadar her i için:
            Eğer YÖNİLİM(Noktalar[p], Noktalar[i], Noktalar[q]) == CCW ise:
                q = i
        p = q
    p != l Olduğu Sürece Devam Et

    Geriye Kabuk listesini döndür
```

## 4. Zaman Karmaşıklığı (Time Complexity)
$N$ toplam nokta sayısı, $H$ ise kapsayan kabuk üzerindeki köşe noktası sayısı olmak üzere:
* **Best Case — $O(N)$:** Eğer $H = 3$ ise (kabuk sadece 3 noktadan oluşuyorsa) 3 tur atılır ve doğrusal zaman sürer.
* **Average Case — $O(N \cdot H)$:** Genelde $H \ll N$ olduğu için $N$ ile $H$'nin çarpımı kadar adım sürer.
* **Worst Case — $O(N^2)$:** Tüm noktaların dış sınır üzerinde olduğu durumlarda ($H = N$) karmaşıklık $O(N^2)$ seviyesine çıkar.

## 5. Alan Karmaşıklığı (Space Complexity)
* **Ek Bellek Kullanımı (Auxiliary Space): $O(H)$**
  * Sadece kapsayan kabuğu oluşturan $H$ adet noktayı saklamak için ek `vector` kullanılır. Orijinal nokta dizisi üzerinde ekstra bir kopyalama yapılmaz.