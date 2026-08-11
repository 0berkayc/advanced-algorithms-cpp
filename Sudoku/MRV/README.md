# Sudoku - MRV (Minimum Remaining Values) Heuristic

Bu doküman, Kısıt Sağlama Problemlerinin (Constraint Satisfaction Problem - CSP) çözümünde kullanılan en etkili sezgisellerden biri olan **MRV (Minimum Remaining Values - Minimum Kalan Değer)** yaklaşımının Sudoku Solver üzerindeki teorik temellerini, avantajlarını, çalışma prensibini ve karmaşıklık analizini içermektedir.

---

## 1. Problemin Tanımı, Zorluğu ve Geliştirilme Amacı

* **Problem Tanımı ve Zorluğu:** $9 \times 9$ boyutundaki bir Sudoku matrisini satır, sütun ve $3 \times 3$'lük blok kısıtlarına uyarak doldurma problemidir. Genel $N \times N$ formunda **NP-Complete** zorluk sınıfındadır.
* **Geliştirilme Amacı ve Giderdiği Eksiklik:** Klasik Backtracking algoritmaları tahtayı "kör" bir şekilde soldan sağa, yukarıdan aşağıya sırayla tarar. Bu yaklaşımda 10-15 adım sonra çıkmaz sokağa girecek yanlış bir rakam seçimi ilk adımlarda fark edilemez ve algoritma milyonlarca anlamsız dallanma yapar.
**MRV sezgisi**, *"Fail-First" (İlk Önce Başarısız Ol)* prensibine dayanarak geliştirilmiştir. Amacı; hata yapılacaksa bunu arama ağacının en başında tespit edip binlerce gereksiz olasılık kolunu anında budamaktır (pruning).

### Önceki Yöntemlere Göre Avantajları

* **Dramatik Arama Ağacı Daralması:** Her adımda yazılabilecek aday rakam sayısı en az olan (en kısıtlı) hücre seçildiği için arama ağacının dallanma faktörü (branching factor) minimuma iner.
* **Erken Tıkanma Tespiti:** Eğer bir hücrenin kurala uyan aday sayısı $0$'a düşmüşse, algoritma diğer hücreleri hiç denemeden o anki yolun geçersiz olduğunu anlar ve anında geri adım atar (backtrack).

### Hangi Durumda Tercih Edilir?

* Zor ve karmaşık Sudoku bulmacalarında (özellikle kaba kuvvetin ve klasik backtracking'in kilitlendiği bol boş hücreli durumlar).
* Arama uzayının çok geniş olduğu Kısıt Sağlama Problemlerinde (CSP), ders/sınav programı çizelgelemede ve harita boyama algoritmalarında.

### Hangi Durumda Tercih Etmezsiniz?

* Çok kolay veya zaten çözülmeye yakın (boş hücre sayısı çok az olan) Sudokularda. Çünkü her adımda en kısıtlı hücreyi arama maliyeti ($O(M)$ tarama döngüsü), kazanılacak hız avantajından daha fazla işlem yükü getirebilir.
* $100 \times 100$ gibi devasa boyutlu matrislerde. Bu ölçekte Knuth'un **Dancing Links (DLX)** algoritması tercih edilmelidir.

---

## 2. Algoritmanın Çalışma Mantığı

### Temel Fikir (Fail-First Principle)

> *"En az seçeneği kalan hücreyi en önce çöz. Eğer bir hücreye yazılabilecek sadece 1 rakam kaldıysa kararı erteleme; 0 rakam kaldıysa çıkmaz sokağı hemen fark et ve geri dön."*

### Çalışma Prensibi

1. **MRV Hücresi Arama:** Tahtadaki tüm boş hücreler taranır. Her boş hücre için `isValid` kontrolü yapılarak o hücreye yazılabilecek geçerli aday sayısı hesaplanır.
2. **En Kısıtlı Hücre Seçimi:** Aday sayısı en küçük olan hücre $(bestRow, bestCol)$ seçilir.
3. **Erken Çıkış (Çıkmaz Sokak Kontrolü):**
* Seçilen hücrenin aday sayısı $0$ ise, bu yol kesin olarak hatalıdır. Diğer adımlar denenmeden `false` dönülür.
* Eğer tahtada hiç boş hücre kalmadıysa bulmaca çözülmüştür (`true`).


4. **Dallanma ve Özyineleme:** Seçilen MRV hücresine geçerli aday rakamlar yerleştirilerek özyinelemeli `solveSudokuMRV()` çağrısı yapılır.
5. **Backtracking:** Tıkanma durumunda yapılan seçim temizlenir (`board[row][col] = 0`) ve sıradaki aday denenir.

---

## 3. Veri Yapıları

* **`std::vector<std::vector<int>> board` (2D Matris):** $9 \times 9$ boyutundaki Sudoku tahtasını temsil eder.
* **Tamsayı Değişkenler (`bestRow`, `bestCol`, `minCandidates`):** O anki tahta durumunda en kısıtlanmış hücrenin koordinatlarını ve minimum aday rakam sayısını tutar.

---

## 4. Sözde Kod (Pseudo-Code)

```text
FONKSİYON countValidCandidates(board, row, col):
    count ← 0
    HER num İÇİN 1'den 9'a KADAR:
        EĞER isValid(board, row, col, num) İSE:
            count ← count + 1
    
    DÖNDÜR count


FONKSİYON findMRVCell(board, bestRow, bestCol):
    minCandidates ← 10
    bestRow ← -1, bestCol ← -1

    HER r İÇİN 0'dan 8'e KADAR:
        HER c İÇİN 0'dan 8'e KADAR:
            EĞER board[r][c] == 0 İSE:
                candidates ← countValidCandidates(board, r, c)
                EĞER candidates < minCandidates İSE:
                    minCandidates ← candidates
                    bestRow ← r, bestCol ← c

                EĞER minCandidates == 0 İSE:
                    DÖNDÜR Doğru // Çıkmaz sokak tespit edildi
                
            
        
    
    DÖNDÜR (bestRow != -1)


FONKSİYON solveSudokuMRV(board):
    EĞER findMRVCell(board, row, col) == Yanlış İSE:
        DÖNDÜR Doğru // Boş hücre kalmadı, çözüldü
    

    EĞER countValidCandidates(board, row, col) == 0 İSE:
        DÖNDÜR Yanlış // Backtrack! (Çıkmaz sokak)
    

    HER num İÇİN 1'den 9'a KADAR:
        EĞER isValid(board, row, col, num) İSE:
            board[row][col] ← num
            EĞER solveSudokuMRV(board) == Doğru İSE:
                DÖNDÜR Doğru
            board[row][col] ← 0 // Backtrack

    DÖNDÜR Yanlış


```

---

## 5. Zaman Karmaşıklığı

* **Best Case:** $O(1)$ veya $O(M)$ — Tahtanın tek seçenekli yönlendirmelerle doğrudan çözüme ulaştığı durum.
* **Average Case:** $O(9^M)$ teorik üst sınıra sahip olmasına rağmen, MRV sezgisi sayesinde ortalama çalışma süresi klasik backtracking'e kıyasla **%90 ila %99 oranında azalır**.
* **Worst Case:** $O(9^M \cdot M)$
* **Açıklama:** $M$ boş hücre sayısı olmak üzere ($M \le 81$), en kötü durumda tüm dallanmalar taranabilir. Ayrıca her adımda MRV hücresini bulmak için $O(M \cdot 9)$ tutarında ek bir tarama maliyeti eklenir.



---

## 6. Alan Karmaşıklığı

* **Ek Bellek Kullanımı (Auxiliary Space):** $O(M)$
* **Kullanılan Veri Yapılarının Etkisi:**
* **Matris:** $9 \times 9 = 81$ elemanlı sabit matris $\rightarrow O(1)$ alan.
* **Özyineleme Yığını (Call Stack):** Çağrı derinliği en fazla boş hücre sayısı $M$ kadar uzayabilir ($M \le 81$) $\rightarrow O(M)$ ek bellek.



---