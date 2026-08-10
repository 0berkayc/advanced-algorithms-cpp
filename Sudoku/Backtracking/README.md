# Sudoku - Backtracking 

Bu doküman, klasik bir Kısıt Sağlama Problemi (Constraint Satisfaction Problem - CSP) olan **Sudoku** probleminin **Standart Backtracking** yaklaşımı ile çözümünün teorik detaylarını, algoritma mantığını, sözde kodunu ve karmaşıklık analizini içermektedir.

---

## 1. Problemin Tanımı ve Zorluğu

* **Problem Tanımı:** $9 \times 9$ boyutundaki kısmen doldurulmuş bir matrisi, her satır, her sütun ve $9$ adet $3 \times 3$'lük alt blok içerisinde $1$'den $9$'a kadar olan rakamlar **tam olarak birer kez** yer alacak şekilde tamamlama problemidir.
* **Zorluk Sınıfı:** Genelleştirilmiş $N \times N$ Sudoku problemi **NP-Complete** zorluk sınıfındadır.
* **Arama Uzayı:** $M$ boş hücre sayısı olmak üzere, hiçbir kısıt kontrolü yapılmazsa kaba kuvvet (brute force) arama uzayı $9^M$ boyuttadır ($M \approx 60$ boş hücre için $9^{60} \approx 10^{57}$ olası durum).

---

## 2. Algoritmanın Geliştirilme Amacı ve Avantajları

### Geliştirilme Amacı ve Giderdiği Eksiklik

Klasik Kaba Kuvvet (Naive Brute Force) yaklaşımı, tüm boş hücrelere rakamlar yerleştirip tahta tamamen dolduktan sonra geçerliliği kontrol eder. Bu yöntem imkansız bir hesaplama maliyeti doğurur.

**Backtracking yaklaşımı**, arama ağacı üzerine **Erken Budama (Early Pruning)** mekanizması getirir. Bir hücreye yazılan rakam mevcut kısıtları (`isValid`) ihlal ediyorsa, o koldan aşağı doğru dallanmaya devam edilmez; o alt ağaç anında terk edilerek bir sonraki rakam denenir veya önceki karara geri dönülür (backtrack).

### Önceki / Kaba Kuvvet Yöntemlerine Göre Avantajları

* **Büyük Arama Alanı Tasarrufu:** Geçersiz seçimler henüz 1. veya 2. adımda elendiği için milyarlarca çıkmaz yol hiç taranmaz.
* **Kesin Çözüm Garantisi:** Çözümü olan her geçerli Sudoku bulmacasını er ya da geç sistematik olarak çözer.

### Hangi Durumda Tercih Edilir?

* Standart $9 \times 9$ boyutundaki Sudoku bulmacalarında ve orta ölçekli Kısıt Sağlama Problemlerinde.
* Kodlama karmaşıklığının düşük tutulması istendiğinde ve ek karmaşık sezgiseller (heuristics) kurmadan kesin çözüm arandığında.

### Hangi Durumda Tercih Edilmez?

* Çok büyük boyutlu bulmacalarda ($25 \times 25$ veya $100 \times 100$ genelleştirilmiş Sudoku). Bu tür durumlarda Knuth'un **Dancing Links (DLX / Exact Cover)** algoritması veya **SAT Solver** sistemleri tercih edilmelidir.

---

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir

> *"Boş bir hücre bul, $1$'den $9$'a kadar kurala uyan ilk rakamı yaz ve bir sonraki boş hücreye geç. İleride bir tıkanma (çıkmaz sokak) yaşarsan, geri dönüp (backtrack) önceki hücredeki seçimi değiştir."*

### Çalışma Prensibi

1. **Boş Hücre Tespiti:** Tahta taranarak değeri `0` olan ilk boş hücre $(row, col)$ bulunur. Hiç boş hücre kalmadıysa bulmaca çözülmüştür (`return true`).
2. **Dallanma ve Deneme:** Seçilen boş hücre için $num = 1 \dots 9$ rakamları sırayla denenir.
3. **Kısıt Kontrolü (`isValid`):**
* **Satır Kontrolü:** $row$. satırda $num$ var mı?
* **Sütun Kontrolü:** $col$. sütunda $num$ var mı?
* **$3 \times 3$ Blok Kontrolü:** $(row / 3) \times 3$ ve $(col / 3) \times 3$ ile hesaplanan $3 \times 3$'lük blok içinde $num$ var mı?


4. **Özyinelemeli Çağrı:** $num$ geçerliyse hücreye yazılır (`board[row][col] = num`) ve `solveSudoku()` özyinelemeli (recursive) olarak çağrılır.
5. **Geri Adım (Backtrack):** Alt çağrılardan `false` dönerse (ileride tıkanma olduysa), yapılan seçim sıfırlanır (`board[row][col] = 0`) ve döngüdeki bir sonraki rakam denenir.

---

## 4. Veri Yapıları

* **`std::vector<std::vector<int>> board` (2D Matris):** $9 \times 9$ boyutundaki tahtayı temsil eder.
* `0`: Boş hücreyi temsil eder.
* `1 - 9`: Hücreye yerleştirilmiş sabit veya geçici rakamları temsil eder.



---

## 5. Sözde Kod (Pseudo-Code)

```text
FONKSİYON isValid(board, row, col, num):
    HER i İÇİN 0'dan 8'e KADAR:
        // Satır ve Sütun Kontrolü
        EĞER board[row][i] == num VEYA board[i][col] == num İSE:
            DÖNDÜR Yanlış
        
        // 3x3 Alt Blok Kontrolü
        startRow ← 3 * (row / 3)
        startCol ← 3 * (col / 3)
        EĞER board[startRow + i / 3][startCol + i % 3] == num İSE:
            DÖNDÜR Yanlış
    DÖNDÜR Doğru

FONKSİYON solveSudoku(board):
    HER row İÇİN 0'dan 8'e KADAR:
        HER col İÇİN 0'dan 8'e KADAR:
            EĞER board[row][col] == 0 İSE: // Boş hücre bulundu
                
                HER num İÇİN 1'den 9'a KADAR:
                    EĞER isValid(board, row, col, num) == Doğru İSE:
                        board[row][col] ← num         // Rakamı dene
                        
                        EĞER solveSudoku(board) == Doğru İSE:
                            DÖNDÜR Doğru             // Çözüm bulundu
                        
                        board[row][col] ← 0           // Backtrack (Seçimi geri al)
                
                DÖNDÜR Yanlış // 1-9 arası hiçbir rakam uymadı, geri dön
    
    DÖNDÜR Doğru // Tüm hücreler başarıyla doldu

```

---

## 6. Zaman Karmaşıklığı

* **Best Case:** $O(1)$ veya $O(M)$ — Tahtanın zaten çözülmüş olduğu veya denenen ilk seçeneklerin doğrudan çözüme ulaştığı durum.
* **Average / Worst Case:** $O(9^M)$
* **Açıklama:** $M$ boş hücre sayısı olmak üzere ($M \le 81$), her boş hücre teorik olarak 9 farklı dallanma yapabilir. Ancak pratikte `isValid` budaması sayesinde arama ağacı çok büyük oranda daralır ve işlem milisaniyeler içinde tamamlanır.



---

## 7. Alan Karmaşıklığı

* **Ek Bellek Kullanımı (Auxiliary Space):** $O(M)$
* **Kullanılan Veri Yapılarının Etkisi:**
* **Matris:** $9 \times 9 = 81$ elemanlı sabit boyutlu 2D vektör $\rightarrow O(1)$ sabit alan.
* **Özyineleme Yığını (Call Stack):** Çağrı derinliği en fazla boş hücre sayısı $M$ kadar uzayabilir ($M \le 81$) $\rightarrow O(M)$ ek alan.



---

## 8. Örnek Derleme ve Çalıştırma

```bash
g++ -std=c++17 main.cpp -o app
./app

```