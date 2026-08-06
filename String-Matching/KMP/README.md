# Knuth-Morris-Pratt (KMP) Algoritması

## 1. Problemin Tanımı ve Zorluğu
* **Problem Tanımı:** Uzunluğu $N$ olan bir metin (text) içerisinde, uzunluğu $M$ olan bir kalıbın (pattern) geçtiği tüm başlangıç indekslerini bulma problemidir.
* **Zorluk Sınıfı:** **$P$ (Polynomial Time)** zorluk sınıfındadır. Polinom zamanda (doğrusal sürede) kesin çözümü elde edilir.

## 2. Algoritmanın Geliştirilme Amacı
* **Bu algoritma neden geliştirilmiştir?** Metin arama işlemlerinde, kaba kuvvet (Naive/Brute Force) yaklaşımın gereksiz yere metin indeksini geriye sarmasını (backtracking) önlemek ve doğrusal zaman karmaşıklığı ($O(N + M)$) elde etmek amacıyla geliştirilmiştir.
* **Hangi eksikliği gidermektedir?** Naive yaklaşımda bir karakter uyuşmazlığı (mismatch) yaşandığında metin indeksi bir adım ileri kaydırılıp arama en baştan başlatılır. KMP, önceden işlenmiş bilgiyi kullanarak metin indeksini **hiç geriye sarmadan** aramaya devam eder.
* **Önceki yöntemlere göre avantajı nedir?** 
  * Metin üzerinde geriye dönük okuma gerektirmez (*single-pass stream reading* için idealdir).
  * Kaba kuvvet yöntemin en kötü senaryodaki $O(N \cdot M)$ zaman maliyetini $O(N + M)$ seviyesine düşürür.
* **Hangi durumda tercih edilir?** 
  * Metin ve kalıp içerisinde tekrar eden karakter desenleri (örneğin DNA dizilimleri, ikili/binary veriler) fazla olduğunda.
  * Metnin boyutu çok büyük olduğunda veya geriye sarmaya izin vermeyen veri akışlarında (stream data).
* **Hangi durumda tercih edilmez?**
  * Kalıp çok kısa olduğunda veya karakter kümesi (alphabet) çok geniş ve tekrarsız olduğunda, LPS tablosu oluşturma ön hazırlığı ($O(M)$) sebebiyle Naive veya Boyer-Moore/Rabin-Karp algoritmaları pratik olarak daha hızlı çalışabilir.

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir
Bir uyuşmazlık gerçekleştiğinde, uyuşmayan karakterden hemen önce gelen alt dize (substring) zaten başarıyla eşleşmiştir. Eğer bu uyuşan kısmın başında ve sonunda aynı olan bir ek (prefix ve suffix) varsa, kalıp bu bilgiye göre doğrudan sağa kaydırılır ve eşleşmeye kalınan yerden devam edilir.

### Çalışma Prensibi
1. **LPS (Longest Proper Prefix which is also Suffix) Tablosunun Oluşturulması:**
   * Kalıp (pattern) kendi içinde analiz edilir.
   * Her $i$ indeksi için, $pattern[0..i]$ alt dizesinin hem öz ön eki hem de son eki olan en uzun çakışan parçanın uzunluğu hesaplanır ve `lps` dizisine kaydedilir.
2. **Eşleştirme Aşaması:**
   * $i$ metin indeksini, $j$ ise kalıp indeksini temsil eder.
   * $text[i] == pattern[j]$ ise $i$ ve $j$ birer artırılır.
   * $j == M$ olursa eşleşme bulunmuş demektir; indeks kaydedilir ve yeni eşleşme arayışı için $j = lps[j - 1]$ yapılır.
   * $text[i] \neq pattern[j]$ durumu oluşursa:
     * $j \neq 0$ ise $i$ **sabit tutulur**, $j = lps[j - 1]$ yapılarak kalıp kaydırılır.
     * $j == 0$ ise $i$ bir artırılır ($i++$).

### Veri Yapıları
* `std::string`: Metin ve aranan kalıbı temsil etmek için.
* `std::vector<int>` (veya `int[]`): Kalıbın ön ek/son ek bilgilerini saklayan $M$ boyutundaki LPS tablosu için.

### Sözde Kod (Pseudo-Code)
```text
KMP-SEARCH(Text, Pattern)
    n = Length(Text)
    m = Length(Pattern)
    LPS = COMPUTE-LPS(Pattern)

    i = 0  // Text üzerindeki ibre
    j = 0  // Pattern üzerindeki ibre

    DÖNGÜ i < n OLDUĞU SÜRECE:
        EĞER Pattern[j] == Text[i] İSE:
            i ve j değerlerini 1 artır
        
        EĞER j == m İSE:
            Eşleşme Bulundu -> İndeks: (i - j)
            j = LPS[j - 1]  // Sonraki olası eşleşme için kaydır
        AKSİ HÂLDE EĞER i < n VE Pattern[j] != Text[i] İSE:
            EĞER j != 0 İSE:
                j = LPS[j - 1]  // Text'i sarmadan Pattern'i kaydır
            AKSİ HÂLDE:
                i değerini 1 artır


COMPUTE-LPS(Pattern)
    m = Length(Pattern)
    LPS = m boyutlu 0 dizisi
    len = 0  // Çakışan en uzun ön ek/son ek uzunluğu
    i = 1

    DÖNGÜ i < m OLDUĞU SÜRECE:
        EĞER Pattern[i] == Pattern[len] İSE:
            len değerini 1 artır
            LPS[i] = len
            i değerini 1 artır
        AKSİ HÂLDE:
            EĞER len != 0 İSE:
                len = LPS[len - 1]
            AKSİ HÂLDE:
                LPS[i] = 0
                i değerini 1 artır
                
    DÖNDÜR LPS
```

## 4. Zaman Karmaşıklığı (Time Complexity)
$N$ metnin uzunluğu, $M$ ise aranan kalıbın uzunluğu olmak üzere:
* **Best Case — $O(N + M)$:** Kalıbın metnin en başında bulunması veya metin üzerinde doğrudan $O(N)$ adımda taranması durumu.
* **Average Case — $O(N + M)$:** Metin ve kalıp yapısından bağımsız olarak $O(M)$ sürede LPS tablosu oluşturulur, $O(N)$ sürede metin taranır.
* **Worst Case — $O(N + M)$:** Tekrarlayan karakterlerin çok olduğu senaryolarda bile $i$ indeksi hiç geriye gitmediği için toplam işlem sayısı $2N + M$ adımı geçmez.

## 5. Alan Karmaşıklığı (Space Complexity)
* **Ek Bellek Kullanımı (Auxiliary Space): $O(M)$**
  * Sadece aranan kalıbın ($pattern$) boyutuna bağlı olan $M$ elemanlı `lps` dizisi/vektörü hafızada tutulur. Metnin boyutu ($N$) ek alan kullanımını etkilemez.