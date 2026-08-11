# 0/1 Knapsack Problemi - Memoization (Top-Down DP) Yaklaşımı

Bu doküman, **0/1 Knapsack (Sırt Çantası)** probleminin **Memoization (Top-Down Dynamic Programming / Not Etme)** yöntemi ile çözümünün teorik detaylarını, çalışma prensibini, sözde kodunu ve karmaşıklık analizini içermektedir.

---

## 1. Problemin Tanımı, Zorluğu ve Geliştirilme Amacı

* **Problem Tanımı:** Elimizde belirli bir $W$ ağırlık kapasitesine sahip bir sırt çantası ve her birinin ağırlığı ($w_i$) ile değeri ($v_i$) olan $N$ adet eşya bulunmaktadır. Amaç, çanta kapasitesini aşmadan toplam değeri **maksimum** yapmaktır. Eşyalar bölünemez ($0/1$ kısıtı).
* **Zorluk Sınıfı:** **NP-Complete** zorluk sınıfındadır.
* **Geliştirilme Amacı:** Saf Özyinelemeli (Pure Recursive) yöntemin ürettiği $O(2^N)$ üstel karmaşıklığı ortadan kaldırmak için geliştirilmiştir. Arama ağacında tekrar eden alt problemleri (**Overlapping Subproblems**) tespit edip sonuçlarını belleğe kaydederek arama uzayını drastik biçimde küçültmeyi amaçlar.

---

## 2. Hangi Eksikliği Gidermektedir ve Avantajları Nelerdir?

* **Giderdiği Eksiklik:** Saf Özyinelemeli yaklaşım, aynı $(i, capacity)$ durumlarını (örneğin *"3. eşyadayım ve çantamda 5 kg yer kalındı"*) arama ağacının farklı dallarında milyonlarca kez baştan hesaplar. Memoization yöntemi, daha önce çözülmüş alt problemlerin sonuçlarını $O(1)$ sürede hatırlayarak bu işlem israfını tamamen çözer.
* **Önceki Yönteme (Pure Recursive) Göre Avantajı:** Zaman karmaşıklığını üstel $O(2^N)$ seviyesinden **Pseudo-polynomial** olan $O(N \cdot W)$ seviyesine düşürür. $N=40$ için saf özyinelemede trilyonlarca işlem yapılırken, Memoization ile bu sayı birkaç yüz adıma iner.

### Hangi Durumda Tercih Edilir?

* Problemin mantıksal tanımının özyinelemeli (Top-Down) yapısının korunması istendiğinde.
* Matristeki durumların hepsinin ziyaret edilmesinin gerekmediği (bazı $(i, capacity)$ kombinasyonlarının hiç oluşmadığı) senaryolarda. Bottom-Up DP'ye göre sadece ihtiyaç duyulan durumları hesapladığı için daha verimli olabilir.

### Hangi Durumda Tercih Edilmez?

* Çanta kapasitesi $W$ ve eşya sayısı $N$ çok büyük olduğunda ($W = 10^9$ gibi). Bu durumda $O(N \cdot W)$ boyutundaki bellek matrisi bellek taşmasına (out of memory) neden olur.
* Çok derin özyinelemelerde (çağrı yığınının binlerce seviyeye ulaştığı durumlar) "Stack Overflow" riski oluşabileceğinden Bottom-Up (Tabulation) yaklaşımı tercih edilir.

---

## 3. Algoritmanın Çalışma Mantığı

### Temel Fikir

> *"Geleceği etkileyen iki parametreyi ($i$. eşya ve $capacity$ kalan kapasite) bir durum olarak kabul et. Bu durumu ilk kez çözdüğünde sonucu Not Matrisine yaz. Aynı durumla tekrar karşılaştığında hesabı tekrarlama, not matrisinden okuyup geç."*

### Çalışma Prensibi

1. **İlklendirme:** $(N + 1) \times (W + 1)$ boyutunda bir `memo` matrisi oluşturulur ve tüm hücreleri `-1` (henüz hesaplanmadı) olarak işaretlenir.
2. **Durum Kontrolü:** Özyinelemeli fonksiyon çağrıldığında ilk olarak taban durumlar ($i=0$ veya $capacity=0$) kontrol edilir.
3. **Not Defteri (Memo) Sorgusu:** `memo[i][capacity] != -1` ise, bu durum daha önce çözülmüştür. $O(1)$ sürede matristeki değer geriye döndürülür.
4. **Hesaplama ve Kayıt:** Durum ilk kez görülüyorsa:
* Eşyayı **ALMA** (`skip`) ve sığıyorsa **AL** (`take`) seçenekleri özyinelemeli çağrılır.
* `max(skip, take)` sonucu hesaplanır.
* Elde edilen bu değer geriye dönülmeden önce `memo[i][capacity]` hücresine kaydedilir.



---

## 4. Veri Yapıları

* **`Item` Yapısı:** Eşyaların `weight` (ağırlık) ve `value` (değer) verilerini tutar.
* **`std::vector<std::vector<int>> memo` (2D Matris):** Hesaplanmış alt problemlerin sonuçlarını saklayan matristir.
* **Satır ($i$):** İncelenen eşya sayısı ($0 \dots N$).
* **Sütun ($capacity$):** Çantada kalan kullanılabilir kapasite ($0 \dots W$).


* **Çağrı Yığını (Call Stack):** Özyineleme derinliğini tutan sistem belleği.

---

## 5. Sözde Kod (Pseudo-Code)

```text
knapsackMemoHelper(i, capacity, items, memo):
    // 1. Taban Durum
    EĞER i == 0 VEYA capacity == 0 İSE:
        DÖNDÜR 0
    
    // 2. Not Defteri Kontrolü (Hafıza)
    EĞER memo[i][capacity] != -1 İSE:
        DÖNDÜR memo[i][capacity]
    
    // 3. Eşya Kapasiteye Sığmıyorsa
    EĞER items[i-1].weight > capacity İSE:
        memo[i][capacity] ← knapsackMemoHelper(i - 1, capacity, items, memo)
        DÖNDÜR memo[i][capacity]
    
    // 4. Karar Anı (Dallanma)
    skip ← knapsackMemoHelper(i - 1, capacity, items, memo)
    take ← items[i-1].value + knapsackMemoHelper(i - 1, capacity - items[i-1].weight, items, memo)
    
    // Sonucu matrise kaydet ve döndür
    memo[i][capacity] ← MAX(skip, take)
    DÖNDÜR memo[i][capacity]

knapsackMemoization(capacity, items):
    N ← items.size()
    memo ← (N + 1) x (capacity + 1) boyutunda -1 ile dolu matris
    DÖNDÜR knapsackMemoHelper(N, capacity, items, memo)


```

---

## 6. Zaman Karmaşıklığı

* **Best Case:** $O(1)$ — Çağrı anında taban durumların tetiklendiği veya hesaplanmış durumun sorgulandığı anlar.
* **Average / Worst Case:** $O(N \cdot W)$
* **Açıklama:** Matriste en fazla $(N + 1) \times (W + 1)$ adet benzersiz alt durum vardır. Her durum yalnızca **1 kez** hesaplanır ve sonrasında $O(1)$ sürede matristen çekilir. Bu nedenle zaman karmaşıklığı durum sayısı ile doğru orantılıdır ($O(N \cdot W)$).



---

## 7. Alan Karmaşıklığı

* **Ek Bellek Kullanımı (Auxiliary Space):** $O(N \cdot W) + O(N)$
* **Kullanılan Veri Yapılarının Etkisi:**
* **`memo` Matrisi:** $(N + 1) \times (W + 1)$ boyutundaki matris hafızada $O(N \cdot W)$ kadar yer kaplar.
* **Çağrı Yığını (Call Stack):** Özyineleme derinliği en fazla eşya sayısı $N$ kadar derine inebilir $\rightarrow O(N)$.
* **Toplam Alan Karmaşıklığı:** Dominant olan $O(N \cdot W)$'dir.



---