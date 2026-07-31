# Rabin-Karp (Rolling Hash) Algoritması

## 1. Problemin Tanımı ve Zorluğu
* **Problem Tanımı:** Verilen $N$ uzunluğundaki bir metin (`text`) içerisinde, $M$ uzunluğundaki bir kalıbın (`pattern`) tüm başlangıç indekslerini bulma problemidir.
* **Zorluk Sınıfı:** $P$ (Polynomial Time) sınıfındadır. Karakter dizilerini doğrudan kıyaslamak yerine sayısal hash (özet) değerlerini kullanarak arama yapar.

## 2. Algoritmanın Geliştirilme Amacı
* **Bu algoritma neden geliştirilmiştir?** Naive (Kaba Kuvvet) algoritmasındaki her pencere kayışında harflerin teker teker kıyaslanması zorunluluğunun yarattığı performans kaybını engellemek, bunun yerine kıyaslamayı matematiksel bir hash değeri üzerinden tek adımda yapmak için geliştirilmiştir.
* **Hangi eksikliği gidermektedir?** Metin içindeki "aday dahi olamayacak" yanlış pencereleri, karakterlerine hiç bakmadan $O(1)$ sürede eleyebilme eksikliğini giderir.
* **Önceki yöntemlere göre avantajı nedir?** En büyük avantajı **Rolling Hash (Kayan Hash)** yapısıdır. Pencere metin üzerinde bir birim kaydığında yeni pencerenin hash değeri sıfırdan hesaplanmaz; çıkan karakterin etkisi formülden çıkarılıp yeni karakterin etkisi eklenerek anında ($O(1)$ sürede) bulunur.
* **Hangi durumda tercih edilir?**
  * Metin içindeki karakter çeşitliliğinin fazla olduğu durumlarda (hash çakışmasını azaltır).
  * Tek bir metin içinde aynı anda birden fazla kalıbın (multiple pattern matching) arandığı senaryolarda.
  * İntihal (plagiarism) analizi yapan sistemlerde.
* **Hangi durumda tercih edilmez?**
  * Metin ve kalıbın sürekli aynı karakterlerden oluştuğu durumlarda (Örn: `text="AAAAA"`, `pattern="AA"`). Bu durum sürekli hash çakışmasına (Spurious Hit) sebep olur ve algoritma karakterleri yine de tek tek kontrol etmek zorunda kalır. Ekstra yapılan matematiksel hash işlemleri (modulo, çarpma) nedeniyle bu senaryoda Naive algoritmadan daha yavaş çalışır.

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir
Metindeki her $M$ uzunluğundaki pencerenin sayısal bir özetini (hash) oluşturmak ve bu değeri kalıbın hash değeri ile kıyaslamaktır. Sadece hash değerleri eşit çıkarsa karakterler harf harf doğrulanır.

### Çalışma Prensibi
1. Belirlenen bir taban değeri (örn: ASCII için $d=256$) ve taşmayı (overflow) önlemek için büyük bir asal sayı ($q=101$) seçilir.
2. Kalıbın ve metnin ilk $M$ uzunluğundaki penceresinin hash değerleri hesaplanır.
3. Pencere metin üzerinde sağa doğru kaydırılır.
4. Hash değerleri eşleşirse: Yalancı eşleşme (Spurious Hit) riskine karşı karakterler teker teker kıyaslanır. Uyuşuyorsa indeks kaydedilir.
5. Pencere sağa kaydıkça "Rolling Hash" formülü kullanılarak yeni pencerenin hash değeri $O(1)$ sürede güncellenir.

### Veri Yapıları
* `std::string`: Metin ve aranan kalıbı (pattern) bellekte tutmak için.
* Sabit (Primitive) Tipler (`int` vb.): Hash değerlerini, taban ($d$) ve modulo ($q$) değerlerini tutmak için.
* `std::vector<int>`: Bulunan gerçek eşleşmelerin indekslerini saklamak ve döndürmek için.

### Sözde Kod (Pseudo-Code)
```text
RABIN-KARP-SEARCH(Metin, Kalıp, d, q):
    n = Metin'in uzunluğu
    m = Kalıp'ın uzunluğu
    h = (d^(m-1)) mod q
    pHash = 0
    tHash = 0
    Eşleşmeler = boş liste

    İlk m karakter için pHash ve tHash değerlerini hesapla

    0'dan (n - m)'ye kadar her i değeri için:
        Eğer pHash == tHash ise:
            Metin[i ... i+m-1] ile Kalıp eşleşiyor mu harf harf kontrol et
            Eğer tüm harfler eşitse:
                i indeksini Eşleşmeler listesine ekle
        
        Eğer i < (n - m) ise:
            tHash = (d * (tHash - Metin[i] * h) + Metin[i + m]) mod q
            Eğer tHash negatifse:
                tHash = tHash + q

    Eşleşmeler listesini döndür
```

## 4. Zaman Karmaşıklığı
* **Best Case — O(N):** Hash fonksiyonunun kusursuz çalıştığı ve hiç yalancı eşleşme (Spurious Hit) yaşanmadığı durumdur. Döngü N kez çalışır, her adımda hash O(1) sürede kontrol edilip güncellenir. Karakter kıyaslama döngüsüne hiç girilmez.
* **Average Case — O(N+M):** Genel metinlerde nadiren hash çakışması yaşanır. Başlangıçtaki ilk hash hesaplaması O(M) sürer, ardından metin boyunca O(1) güncellemelerle gidilir. Ortalama süre doğrusaldır.
* **Worst Case — O(N×M):** Tüm pencerelerin hash değerinin aynı çıktığı senaryodur. Her adımda mecburen tüm karakterler tek tek kıyaslanır. Rolling hash için yapılan matematiksel işlemlerin sabit maliyeti eklendiği için pratikte Naive algoritmadan daha maliyetlidir.

## 5. Alan Karmaşıklığı
* **Ek Bellek Kullanımı: O(1)**
  * Arama işlemi için ekstra bir tablo, matris veya yardımcı veri yapısı oluşturulmaz. Yalnızca 
  `pHash`, `tHash`, `h` gibi sayılar için birkaç değişken kullanılır.
* **Kullanılan Veri Yapılarının Etkisi:**
  * Fonksiyona parametre olarak gelen `text` ve `pattern` referansla alındığı için bellekte tekrar kopyalanmaz. Eşleşmeleri saklamak için kullanılan sonuç listesi (K adet eşleşme için) O(K) kadar yer kaplar.