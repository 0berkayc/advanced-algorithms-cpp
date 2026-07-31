# Z-Algorithm String Matching Algoritması

## 1. Problemin Tanımı ve Zorluğu
* **Problem Tanımı:** Verilen $N$ uzunluğundaki bir metin (`text`) içerisinde, $M$ uzunluğundaki bir kalıbın (`pattern`) tüm başlangıç indekslerini bulma problemidir.
* **Zorluk Sınıfı:** $P$ (Polynomial Time) zorluk sınıfındadır. Doğrusal zamanlı (linear-time) çalışan ve metin arama işlemlerini optimum sürede çözen bir algoritmadır.

## 2. Algoritmanın Geliştirilme Amacı
* **Bu algoritma neden geliştirilmiştir?** Metin arama işlemlerini, karakterleri defalarca baştan kıyaslamadan (gereksiz tekrarlardan kaçınarak) kesin olarak doğrusal $O(N+M)$ zamanda çözmek için geliştirilmiştir.
* **Hangi eksikliği gidermektedir?** Naive ve Rabin-Karp algoritmalarında görülen en kötü durumdaki (worst-case) $O(N \times M)$ zaman karmaşıklığı problemini giderir. Eşleştiği bilinen karakterleri hafızada tutarak tekrar kontrol edilmesini engeller.
* **Önceki yöntemlere göre avantajı nedir?** KMP algoritmasındaki karmaşık "Kısmi Eşleşme Tablosu" (LPS/PI array) yerine çok daha sezgisel olan "Birleştirme (Concatenation)" ve "Z-Kutusu (Z-Box)" taktiğini kullanır. Kodlaması ve anlaşılması KMP'ye göre daha kolaydır.
* **Hangi durumda tercih edilir?** 
  * Arama süresinin kritik olduğu ve en kötü durumda bile $O(N+M)$ performansın garanti edilmesi gereken sistemlerde.
  * DNA dizilim analizleri (Bioinformatics) gibi devasa ve benzer karakterlerden oluşan veri setlerinde.
* **Hangi durumda tercih edilmez?**
  * Ek bellek (RAM) kullanımının çok kısıtlı olduğu gömülü sistemlerde tercih edilmez. Çünkü Z-Algorithm, hem metin ve kalıbı birleştirerek yeni bir dizi oluşturur hem de metin boyutu kadar ekstra bir Z-dizisi (array) tutar.

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir
Aranan kalıp ile metin aralarına özel bir ayırıcı (örn: `$`) konularak tek bir dizi haline getirilir (`Kalıp + $ + Metin`). Oluşan bu yeni dizinin her indeksinden başlayan alt parçaların, dizinin **en başındaki (prefix)** haliyle ne kadar eşleştiği bulunarak eşleşme yerleri tespit edilir.

### Çalışma Prensibi
1. `S = pattern + "$" + text` şeklinde birleşik bir metin oluşturulur.
2. `S` dizisi üzerinde gezilerek bir `Z` dizisi hesaplanır. `Z[i]` değeri, `S`'nin `i`. indeksinden başlayan alt metnin, `S`'nin en başıyla kaç harf eşleştiğini tutar.
3. Algoritma sağa doğru ilerlerken, geçmişte bulduğu eşleşmeleri bir **Z-Kutusu (Z-Box, `[L, R]` aralığı)** olarak aklında tutar.
4. Eğer ilerlenilen indeks bu kutunun içindeyse, karakterleri tekrar saymak yerine geçmişteki sonucu direkt `Z` dizisine kopyalar.
5. `Z` dizisindeki değerlerden herhangi biri kalıbın uzunluğuna (`M`) eşitse, eşleşme bulunmuş demektir.

### Veri Yapıları
* `std::string`: Kalıbı, ayırıcı karakteri ve metni birleştirip tek bir dizi (`S`) olarak tutmak için.
* `std::vector<int>`: 
  * **Z-Dizisi:** Eşleşme uzunluklarını hafızada tutmak için.
  * **Sonuç Listesi:** Eşleşmelerin başlangıç indekslerini kaydetmek için.

### Sözde Kod (Pseudo-Code)
```text
Z-ALGORITHM(Metin, Kalıp):
    S = Kalıp + "$" + Metin
    n = S'in uzunluğu
    Z = n elemanlı, sıfırlarla dolu dizi
    L = 0, R = 0

    1'den n-1'e kadar her i değeri için:
        Eğer i > R ise: 
            L = i, R = i
            S[R - L] ile S[R] eşit olduğu sürece R'yi artır
            Z[i] = R - L
            R = R - 1
        Değilse: 
            k = i - L
            Eğer Z[k] < R - i + 1 ise:
                Z[i] = Z[k]
            Değilse:
                L = i
                S[R - L] ile S[R] eşit olduğu sürece R'yi artır
                Z[i] = R - L
                R = R - 1

    Eşleşmeler = boş liste
    m = Kalıp'ın uzunluğu
    
    Her i için Z dizisinde:
        Eğer Z[i] == m ise:
            (i - m - 1) indeksini Eşleşmeler listesine ekle

    Eşleşmeler listesini döndür
```

## 4. Zaman Karmaşıklığı (Time Complexity)
Z-Algorithm, Z-Kutusu (`[L, R]`) optimizasyonu sayesinde hiçbir karakteri gereksiz yere birden fazla kez kıyaslamaz. `R` işaretçisi (pointer) metin üzerinde asla geriye doğru gitmez.
* **Best Case — $O(N + M)$:** Eşleşmelerin hiç olmadığı veya hızlıca uyuşmazlık bulunduğu durumlarda metin ve kalıp boyutu kadar işlem yapılır.
* **Average Case — $O(N + M)$:** Doğal dillerdeki genel kullanımlarda doğrusal zamanda tamamlanır.
* **Worst Case — $O(N + M)$:** Naive ve Rabin-Karp'ın aksine, Z-Algorithm tüm metnin ve kalıbın aynı karakterlerden (örn: `AAAAAA`) oluştuğu en kötü senaryoda bile Z-Kutusu geçmişini kullandığı için **$O(N + M)$ sürede yani doğrusal (linear) zamanda** çalışmayı garanti eder.

## 5. Alan Karmaşıklığı (Space Complexity)
* **Ek Bellek Kullanımı (Auxiliary Space): $O(N + M)$**
  * Naive ve Rabin-Karp algoritmaları $O(1)$ ekstra bellek kullanırken, Z-Algorithm yüksek performansını hafıza kullanarak öder. 
  * Kalıp ve metnin birleşmesinden $O(N + M)$ uzunluğunda yeni bir metin (`S`) üretilir.
  * Bu metnin eşleşme değerlerini tutmak için $O(N + M)$ boyutunda bir Z-Dizisi oluşturulur.
* **Kullanılan Veri Yapılarının Etkisi:**
  * Oluşturulan `std::string S` ve `std::vector<int> Z` veri yapıları girdi boyutuna orantılı dinamik bellek tahsisi yapar. Bulunan sonuçları tutan liste de yine en kötü ihtimalle $O(N)$ alan kaplar.