# N-Queens (N-Vezir) Problemi ve Backtracking Çözümü

Bu doküman, bilgisayar bilimlerinin klasikleri arasında yer alan **N-Queens (N-Vezir)** probleminin tanımını, zorluğunu, **Backtracking (Geri Adım Atma)** yaklaşımıyla elde edilen çözümünün mantığını, zaman ve alan karmaşıklığını içermektedir.

---

## 1. Problemin Tanımı ve Zorluğu

* **Problem Tanımı:** $N \times N$ boyutundaki bir satranç tahtasına $N$ adet veziri, hiçbir vezir bir diğerini tehdit etmeyecek (aynı satır, sütun veya çaprazda bulunmayacak) şekilde yerleştirme problemidir.
* **Zorluk Sınıfı:** Combinatorial Search / Constraint Satisfaction Problem (Kısıt Sağlama Problemi).
* **Arama Uzayının Büyüklüğü:** $N \times N$ kareden $N$ kare seçmek için kaba kuvvet (brute force) arama uzayı $\binom{N^2}{N}$ boyuttadır (örneğin $N=8$ için yaklaşık 4.4 milyar olasılık).

---

## 2. Algoritmanın Geliştirilme Amacı ve Avantajları

### Geliştirilme Amacı ve Giderdiği Eksiklik

Klasik Kaba Kuvvet (Naive Brute Force) yaklaşımı, tahtadaki tüm olası vezir dizilimlerini oluşturur ve ardından bu dizilimlerin kısıtları ihlal edip etmediğini kontrol eder. Bu yöntem $N$ biraz büyüdüğünde dahi üstel patlama (exponential explosion) nedeniyle kilitlenir.

**Backtracking yaklaşımı**, arama ağacı üzerine **Budama (Pruning)** mekanizmasını getirerek bu eksikliği giderir. Bir satıra konulan vezir çakışma yaratıyorsa, o koldan aşağı doğru dallanmaya devam edilmez; o alt ağaç anında terk edilerek (backtrack) bir sonraki olasılığa geçilir.

### Önceki / Kaba Kuvvet Yöntemlerine Göre Avantajları

* **Devasa Arama Alanı Tasarrufu:** Geçersiz çözümleri daha ilk adımlarda fark edip elediği için milyarlarca gereksiz kontrolü atlar.
* **Tüm Çözümleri Bulabilme:** Sadece tek bir geçerli konumu değil, tahta üzerindeki simetrik ve benzersiz tüm çözümleri sistematik olarak bulabilir.

### Hangi Durumda Tercih Edilir?

* Tüm geçerli dizilimlerin (veya toplam çözüm sayısının) eksiksiz şekilde listelenmesi istendiğinde.
* $N$ değerinin makul sınırlar içinde olduğu ($N \le 20$) ve kesin matematiksel çözüme ihtiyaç duyulan durumlarda.

### Hangi Durumda Tercih Edilmez?

* Çok büyük tahta boyutlarında ($N = 1000+$ gibi). Bu tür ölçeklerde tek bir geçerli dizilimi $O(N)$ sürede bulan Min-Conflicts / Local Search gibi sezgisel (heuristic) algoritmalar tercih edilmelidir.

---

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir

> *"Vezirleri satır satır ($row = 0, 1, \dots, N-1$) yerleştirerek ilerle. Bir satırda güvenli bir sütun bulamazsan, bir önceki satırdaki vezirin konumunu değiştir (Backtrack)."*

### Çalışma Prensibi

1. Her satıra **en fazla 1 vezir** gelebileceği mantığıyla, $0$. satırdan başlanır.
2. Mevcut satır ($row$) için $0$'dan $N-1$'e kadar olan sütunlar ($col$) sırayla denenir.
3. Seçilen $(row, col)$ konumunun önceki satırlara yerleştirilmiş vezirlerle çakışıp çakışmadığı kontrol edilir (`isSafe`):
* **Sütun Kontrolü:** Önceki satırlarda aynı sütunda vezir var mı?
* **Çapraz Kontrolü:** Önceki satırlardaki vezirlerle $\lvert row_1 - row_2 \rvert = \lvert col_1 - col_2 \rvert$ eşitliği sağlanıyor mu?


4. **Güvenliyse:** Vezir o konuma konur ve bir sonraki satır için fonksiyon özyinelemeli (recursive) olarak çağrılır (`solveNQueens(row + 1)`).
5. **Güvenli Değilse veya Alt Daldan Çözüm Çıkmadıysa:** Döngü bir sonraki sütuna ($col + 1$) geçer ve önceki seçimi otomatik olarak geçersiz kılar (Backtrack).
6. **Taban Durum (Base Case):** $row == N$ olduğunda $N$ vezir de başarıyla yerleştirilmiş demektir. Çözüm kaydedilir/yazdırılır.

---

## 4. Veri Yapıları

* **`std::vector<int> board(N, -1)` (1D Dizi):** $N \times N$ boyutunda 2D matris tutmak yerine tek boyutlu bir dizi kullanılır.
* **Mantık:** `board[row] = col` ifadesi $row$. satırdaki vezirin $col$. sütunda olduğunu belirtir.
* **Avantajı:** Alan karmaşıklığını $O(N^2)$ seviyesinden $O(N)$ seviyesine indirir ve iki vezirin aynı satırda olma ihtimalini veri yapısının doğası gereği imkansız kılar.



---

## 5. Sözde Kod (Pseudo-Code)

```text
FONKSİYON isSafe(row, col, board):
    HER r İÇİN 0'dan (row - 1)'e KADAR:
        c ← board[r]
        EĞER c == col VEYA ABS(r - row) == ABS(c - col) İSE:
            DÖNDÜR Yanlış
    DÖNDÜR Doğru


FONKSİYON solveNQueens(row, n, board, solution_count):
    // Taban Durum: Tüm vezirler yerleşti
    EĞER row == n İSE:
        solution_count ← solution_count + 1
        Tahtayı Yazdır(board)
        DÖNDÜR
    

    // Mevcut satır için tüm sütunları dene
    HER col İÇİN 0'dan (n - 1)'e KADAR:
        EĞER isSafe(row, col, board) == Doğru İSE:
            board[row] ← col                        // Veziri koy
            solveNQueens(row + 1, n, board, ...)    // Bir sonraki satıra geç
            // Backtracking: 'col' döngüsü ilerledikçe board[row] değeri otomatik ezilir.
        
    


```

---

## 6. Zaman Karmaşıklığı

* **Best Case:** $O(N)$ — İlk denenen yolların doğrudan geçerli çözüme ulaştığı teorik durum.
* **Average / Worst Case:** $O(N!)$
* **Açıklama:** İlk satırda $N$ seçenek, ikinci satırda en fazla $N-1$ seçenek, üçüncüde $N-2$ seçenek kalır. Budama (pruning) sayesinde arama uzayı $\approx 0.14 \times N!$ seviyelerine düşse de asemptotik üst sınır $O(N!)$ olarak kabul edilir.



---

## 7. Alan Karmaşıklığı

* **Ek Bellek Kullanımı (Auxiliary Space):** $O(N)$
* **Kullanılan Veri Yapılarının Etkisi:**
* `board` Dizisi: $N$ elemanlı 1D dizi $\rightarrow O(N)$
* Özyineleme Yığını (Call Stack): En fazla $N$ derinliğe ulaşır $\rightarrow O(N)$
* Toplam Ek Alan: $O(N)$



---

## 8. Örnek Derleme ve Çalıştırma

```bash
g++ -std=c++17 main.cpp -o app
./app

```