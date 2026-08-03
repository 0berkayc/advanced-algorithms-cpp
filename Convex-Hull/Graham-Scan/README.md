# Graham Scan Algoritması

## 1. Problemin Tanımı ve Zorluğu
* **Problem Tanımı:** 2 boyutlu düzlemde verilen $N$ adet noktayı tamamen içine alan, en dıştaki noktalardan oluşan en küçük dış bükey çokgeni (Convex Hull) bulma problemidir.
* **Zorluk Sınıfı:** **$P$ (Polynomial Time)** zorluk sınıfındadır. Kesin optimum çözüm polinom zamanda elde edilir.

## 2. Algoritmanın Geliştirilme Amacı
* **Bu algoritma neden geliştirilmiştir?** Convex Hull problemini, çıktı boyutuna ($H$) bağımlı kalmaksızın her durumda öngörülebilir ve hızlı bir şekilde $O(N \log N)$ zaman karmaşıklığı ile çözebilmek amacıyla geliştirilmiştir.
* **Hangi eksikliği gidermektedir?** Jarvis March (Gift Wrapping) gibi algoritmaların, tüm noktaların dış sınırda yer aldığı kötü durum senaryolarında (Worst Case) $O(N^2)$ zaman karmaşıklığına düşme eksikliğini giderir.
* **Önceki yöntemlere göre avantajı nedir?** 
  * En kötü senaryoda (Worst Case) bile performans garantisi sunar ($O(N \log N)$).
  * Sıralama aşamasından sonra noktaları Stack (Yığın) veri yapısı kullanarak tek bir taramada ($O(N)$) işler.
* **Hangi durumda tercih edilir?** 
  * Girdi boyutu ($N$) çok büyük olduğunda ve en kötü durum senaryosunda dahi sabit bir performans garantisi arandığında.
  * Noktaların çoğunun dış sınırda yer alma olasılığı yüksek olduğunda.
* **Hangi durumda tercih edilmez?**
  * Sınır üzerindeki köşe noktası sayısının ($H$) toplam nokta sayısına ($N$) kıyasla çok küçük olduğu kesin bilinen durumlarda ($H \ll N$), $O(N \cdot H)$ çalışan Jarvis March gibi çıktının boyutuna duyarlı (output-sensitive) algoritmalar daha hızlı sonuç verebilir.

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir
1. Y koordinatı en küçük olan (en alttaki) nokta pivot ($P_0$) olarak seçilir.
2. Diğer tüm noktalar, $P_0$ noktasına göre yaptıkları kutupsal açılara (polar angle) göre küçükten büyüğe sıralanır.
3. Sıralı noktalar tek tek taranır. Bir nokta eklendiğinde "sağa dönüş" (Clockwise - CW) veya "doğrusallık" oluşturuyorsa, konveksliği bozduğu için yığındaki son elemanlar çıkarılır (backtrack). Sadece "sola dönüş" (Counter-Clockwise - CCW) sağlayan noktalar kabukta tutulur.

### Çalışma Prensibi
1. En alt (y'si en küçük) nokta bulunur ve ilk sıraya taşınır ($P_0$).
2. Kalan $N-1$ nokta, $P_0$'a göre kutupsal açılarına göre sıralanır. Aynı açıya sahip noktalardan sadece en uzakta olanı tutulur.
3. İlk 3 nokta bir `Stack` (Yığın) veri yapısına eklenir.
4. $i = 3$'ten $N-1$'e kadar olan noktalar sırayla taranır:
   * Yığındaki son iki eleman ile sıradaki $i$. nokta arasındaki yönelim kontrol edilir.
   * Yönelim "sola dönüş" (CCW) olana kadar yığının tepesinden eleman çıkarılır (`pop`).
   * Sıradaki $i$. nokta yığına eklenir (`push`).
5. Yığında kalan noktalar Convex Hull köşelerini oluşturur.

### Veri Yapıları
* `struct Point`: Noktaların $(x, y)$ koordinatlarını tutmak için.
* `std::vector<Point>`: Noktaları saklamak ve açılara göre sıralamak için.
* `std::stack<Point>`: İçeride kalan (konveksliği bozan) noktaları geriye dönük elemek ve kabuk noktalarını tutmak için.

### Sözde Kod (Pseudo-Code)
```text
GRAHAM-SCAN(Noktalar):
    n = Noktalar dizisinin boyutu
    Eğer n < 3 ise geriye boş liste döndür

    1. En alt noktayı bul (p0) ve dizinin başına al
    p0 = Y koordinatı en küçük olan nokta
    
    2. Kalan noktaları p0'a göre kutupsal açı ile sırala
    SORT(Noktalar[1..n-1], p0'a göre kutupsal açı)

    3. Aynı açıya sahip noktaları temizle (en uzaktakini tut)

    4. İlk 3 noktayı yığına ekle
    Yığın st = boş yığın
    st.push(Noktalar[0])
    st.push(Noktalar[1])
    st.push(Noktalar[2])

    5. Kalan noktaları tara
    3'ten n-1'e kadar her i için:
        YÖNİLİM(st.nextToTop(), st.top(), Noktalar[i]) == CCW Olana Kadar:
            st.pop()
        st.push(Noktalar[i])

    Geriye st yığındaki noktaları döndür
```

## 4. Zaman Karmaşıklığı (Time Complexity)
$N$ toplam nokta sayısı olmak üzere:
* **Best Case — $O(N \log N)$:** Noktaların sıralanması aşaması en az $O(N \log N)$ sürer.
* **Average Case — $O(N \log N)$:** Noktaların sıralanması $O(N \log N)$, tarama (scan) aşaması $O(N)$ sürer. Toplam karmaşıklık sıralama tarafından domine edilir.
* **Worst Case — $O(N \log N)$:** Tüm noktalar dış sınır üzerinde olsa bile sıralama mantığı değişmediği için algoritma $O(N \log N)$ sürede tamamlanır.

## 5. Alan Karmaşıklığı (Space Complexity)
* **Ek Bellek Kullanımı (Auxiliary Space): $O(N)$**
  * Noktaların taranması ve elenmesi sırasında `std::stack` veri yapısı kullanılır. En kötü durumda yığın $N$ adet nokta tutabilir.
  * Noktaları sıralarken kullanılan ek bellek karmaşıklığı $O(N)$ seviyesindedir.