# 0/1 Knapsack Problemi - Recursive Yaklaşım

Bu doküman, **Knapsack (Sırt Çantası)** probleminin en temel ve kaba kuvvet (brute-force) çözümü olan **Recursive (Özyinelemeli)** yaklaşımın teorik detaylarını ve karmaşıklık analizini içermektedir.

---

## 1. Problemin Tanımı, Zorluğu ve Geliştirilme Amacı

* **Problem Tanımı:** Elimizde belirli bir $W$ ağırlık kapasitesine sahip bir çanta ve her birinin kendine ait ağırlığı ($w_i$) ile değeri ($v_i$) olan $N$ adet eşya bulunmaktadır. Amaç, çantanın kapasitesini aşmadan, içine konulan eşyaların toplam değerini **maksimum** yapmaktır.
* **0/1 Kısıtı:** Eşyalar parçalanamaz. Bir eşya ya çantaya tamamen alınır ($1$) ya da tamamen dışarıda bırakılır ($0$).
* **Zorluk Sınıfı:** **NP-Complete** (Polinom zamanda kesin çözümü bilinmeyen zor problemler sınıfı).
* **Geliştirilme Amacı:** Recursive algoritma, problemin doğasındaki "Al ya da Alma" ikilemini matematiksel bir formüle (Recurrence Relation) dökerek, problemin kavramsal modelini kurmak için geliştirilmiştir. Amacı hız değil, **kesin doğruluğu (optimaliteyi)** ispatlamaktır.

---

## 2. Hangi Eksikliği Gidermektedir ve Avantajları Nelerdir?

* **Giderdiği Eksiklik (Greedy - Açgözlü Yöntemin Hatası):** 0/1 Knapsack probleminde, "Birim ağırlık başına en değerli olanı alayım" (Greedy) mantığı her zaman doğru sonucu **vermez** çünkü eşyalar bölünemediği için çantada ölü boşluklar kalabilir. Recursive yöntem, tüm kombinasyonları eksiksiz tarayarak Açgözlü yaklaşımın düştüğü bu hatayı (sub-optimal sonuç bulma riskini) tamamen ortadan kaldırır.
* **Avantajı:** Problemi alt problemlere bölen yapısı sayesinde, Dinamik Programlama (DP) çözümlerinin (Memoization ve Tabulation) inşa edilebilmesi için zorunlu olan **temel iskeleti** sağlar. Kesin (optimal) sonucu garanti eder.

---

## 3. Hangi Durumda Tercih Edilir / Edilmez?

* **Tercih Edildiği Durumlar:**
* Eşya sayısının çok küçük olduğu durumlarda ($N \le 20$).
* Dinamik programlama formülünü doğrulamak için bir referans (baseline) algoritma gerektiğinde.
* Eğitim amaçlı, problemin ikili karar ağacı (binary decision tree) mantığını anlamak için.


* **Tercih EDİLMEDİĞİ Durumlar:**
* Eşya sayısının büyük olduğu senaryolarda ($N > 25$). Algoritma **Overlapping Subproblems (Tekrar Eden Alt Problemler)** nedeniyle aynı hesaplamaları milyonlarca kez baştan yapar. Pratikte sistemin kilitlenmesine veya günler süren hesaplamalara yol açar.



---

## 4. Algoritmanın Çalışma Mantığı

### Temel Fikir

> *"Her eşya için önünde iki yol var: Ya çantaya atarsın ya da bırakırsın. İki yolu da sonuna kadar dene ve sana en büyük kazancı getiren yolu seç."*

### Çalışma Prensibi

Algoritma sondan başa doğru (veya baştan sona) her bir $i$. eşya için şu adımları izler:

1. **Taban Durum (Base Case):** Eğer incelenecek eşya kalmadıysa ($i = 0$) veya çantada yer kalmadıysa ($W = 0$), dönülecek değer $0$'dır.
2. **Kapasite Kontrolü:** Eğer $i$. eşyanın ağırlığı, çantanın o anki kalan kapasitesinden büyükse, bu eşya **mecburen** dışarıda bırakılır ve bir sonraki eşyaya geçilir.
3. **Karar Anı (Dallanma):** Eğer eşya çantaya sığıyorsa iki farklı senaryo hesaplanır:
* **ALMA (Skip):** Eşya dışarıda bırakılır, kapasite değişmez.
* **AL (Take):** Eşyanın değeri toplam değere eklenir, eşyanın ağırlığı çanta kapasitesinden düşülür.


4. **Maksimumu Seçme:** `ALMA` ve `AL` dallarından dönen değerler kıyaslanır (`std::max`) ve daha büyük olanı bir üst çağrıya iletilir.

---

## 5. Veri Yapıları

* **`Item` (Eşya) Yapısı:** Her eşyanın ağırlık (`weight`) ve değer (`value`) özelliklerini bir arada tutmak için kullanılan Struct veya Sınıf (Class).
* **Dizi/Vektör (`std::vector`):** Eşya listesini tutan tek boyutlu veri yapısı.
* **Çağrı Yığını (Call Stack):** Kodda doğrudan tanımlanmayan ancak işletim sistemi tarafından özyinelemeli fonksiyon çağrılarını (ağaç dallarını) takip etmek için kullanılan LIFO (Last-In-First-Out) bellek yapısı.

---

## 6. Sözde Kod (Pseudo-Code)

```text
knapsackRecursive(i, capacity, items):
    // Taban Durum: Eşya kalmadı veya kapasite bitti
    EĞER i == 0 VEYA capacity == 0 İSE:
        DÖNDÜR 0
    
    // İncelenen eşya çantaya fiziksel olarak sığmıyorsa
    EĞER items[i-1].weight > capacity İSE:
        DÖNDÜR knapsackRecursive(i - 1, capacity, items)
    
    // Eşya sığıyorsa iki alternatifi de dene
    skip ← knapsackRecursive(i - 1, capacity, items)
    
    take ← items[i-1].value + knapsackRecursive(i - 1, capacity - items[i-1].weight, items)
    
    En yüksek değeri veren kararı döndür
```

---

## 7. Zaman Karmaşıklığı

* **Best Case:** $O(N)$
* *Açıklama:* Taşıma kapasitesinin $0$ olması veya listedeki hiçbir eşyanın çantaya sığmaması durumunda, algoritma hiç sağ dala (AL dalına) giremez. Sadece tek bir koldan dümdüz aşağı inerek $N$ adımda işlemi bitirir.


* **Average / Worst Case:** $O(2^N)$
* *Açıklama:* $N$ adet eşya için her adımda iki kola (AL ve ALMA) dallanma yapılır. Bu durum bellekte tam dolu bir ikili ağaç (binary tree) yaratır ve yaprak düğüm sayısı üstel olarak artarak $2^N$ işlem yapılmasına neden olur.



---

## 8. Alan Karmaşıklığı

* **Ek Bellek Kullanımı (Auxiliary Space):** $O(N)$
* *Açıklama:* Özyineleme (recursion) işlemi sırasında işletim sisteminin Çağrı Yığını (Call Stack) en fazla eşya sayısı ($N$) kadar derine inebilir. Bu yüzden bellek tüketimi lineerdir.


* **Kullanılan Veri Yapılarının Etkisi:** $O(N)$
* *Açıklama:* Eşyaları hafızada tutmak için kullanılan vektör veya dizi $N$ elemanlıdır. Genel alan karmaşıklığı $O(N)$ olarak kalır.
