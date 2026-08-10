# N-Queens - Bitmask

Bu doküman, **N-Queens (N-Vezir)** probleminin klasik Backtracking çözümündeki kısıt kontrollerini ve durum yönetimini **CPU seviyesinde bitwise (bitsel) işlemler** ile $O(1)$ zaman karmaşıklığına indirgeyen **Bitmasking Optimization** yaklaşımını detaylandırmaktadır.

---

## 1. Problemin Tanımı ve Zorluğu

* **Problem Tanımı:** $N \times N$ boyutundaki bir satranç tahtasına $N$ adet veziri, hiçbir vezir bir diğerini tehdit etmeyecek (aynı satır, sütun veya çaprazda bulunmayacak) şekilde yerleştirme ve **toplam geçerli çözüm sayısını bulma** problemidir.
* **Zorluk Sınıfı:** Combinatorial Search / Constraint Satisfaction Problem ($O(N!)$ arama uzayı).
* **Geliştirilme Amacı:** Klasik Backtracking algoritmasında her yeni vezir yerleşiminde yapılan $O(N)$ süreli `isSafe` kontrollerini ve bellek erişim maliyetlerini **bitsel operatörler (`&`, `|`, `~`, `<<`, `>>`)** kullanarak $O(1)$ seviyesine çekmek ve devasa arama ağaçlarını milisaniyeler içinde taramaktır.

---

## 2. Hangi Eksikliği Gidermektedir?

Klasik Backtracking yönteminde:

1. Her vezir yerleşiminde önceki satırlar üzerinde döngü kurulup (`isSafe`) sütun ve çapraz çakışmaları taranır ($O(N)$ işlem).
2. Hafızada `board` dizileri tutulur ve özyineleme (recursion) adımlarında bu bellek alanları güncellenir.

**Bitmask yaklaşımı**, tahtanın sütunlarını ve sağ/sol çaprazlarını 32-bit veya 64-bit bir tamsayının **bitleri** olarak saklar. Böylece tüm çakışma kontrolleri ve boş yer tespiti tek bir CPU çevriminde (bitwise operation) gerçekleştirilerek bu eksiklikler tamamen giderilir.

---

## 3. Avantajları ve Kullanım Durumları

### Önceki Yöntemlere Göre Avantajları

* **Işık Hızında Kontrol ($O(1)$):** Çakışma kontrolü için döngü yazılmaz; `cols | left_diags | right_diags` ifadesiyle tüm tehdit altındaki kareler anında maskelenir.
* **Sıfır Dizi/Dinamik Bellek Maliyeti:** Diziler yerine tam sayılar (primitives) kullanılır. Hafızada ekstra `vector` veya matris saklanmaz.
* **Mükemmel Performans Artışı:** Klasik Backtracking'e kıyasla uygulamada **10 ila 50 kat daha hızlı** çalışır.

### Hangi Durumda Tercih Edilir?

* Özellikle $N \ge 10$ gibi büyük tahta boyutlarında **sadece toplam çözüm sayısının (counting problem)** yüksek performansla hesaplanması istendiğinde.
* Donanım/yazmaç seviyesinde (CPU registers) maksimum performans hedefleyen sistemlerde.

### Hangi Durumda Tercih Edilmez?

* $N > 64$ durumlarında (standart 64-bit integer sınırını aştığı için özel `BigInt` bitmask yazılması gerekir).
* Amacın sadece sayı bulmak değil, her adımda tahta durumunun matris olarak saklanması ve I/O süreçlerine aktarılması olduğu durumlarda (tahta çizdirmek bitsel mantığın hız avantajını maskeler).

---

## 4. Algoritmanın Çalışma Mantığı

### Temel Fikir

Tahta üzerindeki kısıtlar 3 ana bitmask (tamsayı) değişkeni ile yönetilir:

1. **`cols`:** Hangi sütunların dolu olduğunu gösteren maske.
2. **`left_diags`:** Sol çaprazların (sola yatık) tehdit durumunu gösteren maske.
3. **`right_diags`:** Sağ çaprazların (sağa yatık) tehdit durumunu gösteren maske.

Bir alt satıra geçildiğinde ($row \to row + 1$):

* Sol çapraz tehditleri sola kayar: `(left_diags | p) << 1`
* Sağ çapraz tehditleri sağa kayar: `(right_diags | p) >> 1`

### Çalışma Prensibi

1. `all_on_mask = (1 << n) - 1` tanımlanarak $N$ adet biti `1` olan hedef maske oluşturulur (örneğin $N=4$ için `1111_2 = 15`).
2. **Boş Konumları Bulma:** `available_positions = all_on_mask & ~(cols | left_diags | right_diags)` işlemiyle tehdit altında olmayan tüm güvenli kareler tek hamlede `1` yapılır.
3. **En Sağdaki Biti Seçme (En Sağdaki Boş Kare):** `p = available_positions & -available_positions` (İkinin tümleyeni matematiği ile ilk `1` biti izole edilir).
4. **Seçilen Biti Temizleme:** `available_positions -= p`
5. **Özyinelemeli Çağrı:** `cols | p`, `(left_diags | p) << 1` ve `(right_diags | p) >> 1` parametreleriyle bir sonraki satıra geçilir.
6. **Taban Durum:** `cols == all_on_mask` olduğunda tüm sütunlar dolmuş demektir, çözüm sayısı 1 artırılır.

---

## 5. Veri Yapıları

* **Tamsayı Değişkenler (`int` / `uint64_t`):**
* `cols`: Sütun maskesi.
* `left_diags`: Sol çapraz maskesi.
* `right_diags`: Sağ çapraz maskesi.
* `all_on_mask`: $N$ boyutunu belirleyen bit sınırı maskesi.
* `available_positions`: O anki satırda vezir konulabilecek boş sütunların maskesi.
* `p`: Seçilen tek bir vezir pozisyonunun biti.



*Hiçbir dizi (`array`/`vector`) kullanılmaz.*

---

## 6. Sözde Kod (Pseudo-Code)

```text
solveNQueensBitmask(cols, left_diags, right_diags, all_on_mask, solution_count):

cols, left_diags, right_diags (Bitmaskler), all_on_mask (Hedef Maske)
solution_count (Toplam çözüm sayısı referansı)


    // Taban Durum: Tüm sütunlar doldu
    EĞER cols == all_on_mask İSE:
        solution_count ← solution_count + 1
        DÖNDÜR
    

    // Tehdit altında olmayan kullanılabilir sütunları bul
    kullanılabilir ← all_on_mask VE DEĞİL(cols VEYA left_diags VEYA right_diags)

    // Boş pozisyon kaldığı sürece dön
    DÖNGÜ kullanılabilir > 0 OLDUĞU SÜRECE:
        // En sağdaki 1 bitini izole et
        p ← kullanılabilir VE (-kullanılabilir)

        // Seçilen biti listeden düş
        kullanılabilir ← kullanılabilir - p

        // Bir sonraki satıra geç ve çaprazları kaydır
        solveNQueensBitmask(
            cols VEYA p,
            (left_diags VEYA p) SOLA_KAYDIR 1,
            (right_diags VEYA p) SAĞA_KAYDIR 1,
            all_on_mask,
            solution_count
        )



```

---

## 7. Zaman Karmaşıklığı

| Durum | Karmaşıklık | Açıklama |
| --- | --- | --- |
| **Best Case** | $O(N)$ | Çözümün hemen bulunduğu teorik durum. |
| **Average Case** | $O(N!)$ | Budama ile geçersiz dallar elenir, kısıt kontrolleri $O(1)$'dir. |
| **Worst Case** | $O(N!)$ | Tüm durum ağacı taranır, sabit çarpan (constant factor) çok küçüktür. |

---

## 8. Alan Karmaşıklığı

* **Ek Bellek Kullanımı (Auxiliary Space):** $O(N)$
* **Kullanılan Veri Yapılarının Etkisi:**
* Dizi veya Matris Kullanımı: **$O(0)$** (Hafızada hiçbir nesne oluşturulmaz).
* Özyineleme Yığını (Call Stack): Maksimum $N$ derinliğe ulaşır $\rightarrow O(N)$.
* **Toplam Ek Bellek:** Yalnızca özyineleme yığını kadar $O(N)$.



---

## 9. Örnek Derleme ve Çalıştırma

```bash
g++ -std=c++17 main.cpp -o app
./app

```

### Örnek Çıktı ($N = 8$)

```text
Toplam çözüm sayısı: 92

```

---