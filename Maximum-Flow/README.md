# Maximum Flow Algoritmaları Karşılaştırma ve Modül Rehberi

Bu modül, yönlü ve kapasiteli şebeke ağları üzerinde **Kaynaktan ($S$) Hedefe ($T$)** aktarılabilecek **Maksimum Akış (Maximum Flow)** miktarını hesaplayan üç temel algoritmanın (**Ford-Fulkerson**, **Edmonds-Karp** ve **Dinic**) karşılaştırmalı analizini, seçim kriterlerini ve teorik temellerini içermektedir.

---

## 1. Problem Özeti ve Temel İlkeler

Maksimum Akış problemi, $V$ adet düğüm ve $E$ adet yönlü/kapasiteli kenardan oluşan bir graf üzerinde iki temel kısıta uyarak $S$'den $T$'ye gönderilebilecek maksimum debiyi bulmayı hedefler:

1. **Kapasite Kısıtı (Capacity Constraint):** $0 \le f(u,v) \le c(u,v)$ *(Geçen akış, kalan kapasiteyi aşamaz).*
2. **Akışın Korunması (Flow Conservation):** $\sum_{u} f(u,v) = \sum_{w} f(v,w) \quad (\forall v \neq S, T)$ *(Ara düğümlerde giren akış çıkan akışa eşittir).*
3. **Max-Flow Min-Cut Teoremi:** Bir ağdaki maksimum akış değeri, kaynak ve hedefi iki ayrı kümeye bölen minimum kesitin (**Minimum Cut**) toplam kapasitesine eşittir.

---

## 2. Algoritmaların Özeti

### 1. Ford-Fulkerson Metodu

* **Arama Yöntemi:** Derinlemesine Arama (DFS).
* **Temel Fikir:** Kalıntı grafta $S$'den $T$'ye giden boş bir yol (Artırıcı Yol) arar. Her bulunan yolda şişe boynu (bottleneck) kadar akış gönderir ve geri kenarlar (backward edges) yardımıyla hatalı akışları düzeltir.
* **Kritik Zayıflık:** Akış büyüklüğüne ($\vert{}f_{max}\vert{}$) bağımlıdır. Kötü yol seçimlerinde milyonlarca gereksiz adım çalışabilir.

### 2. Edmonds-Karp Algoritması

* **Arama Yöntemi:** Enlemesine Arama (BFS).
* **Temel Fikir:** Ford-Fulkerson metodunun BFS ile somutlaştırılmış halidir. Her adımda en az kenar sayısına sahip **en kısa artırıcı yolu** seçer.
* **Kritik Avantaj:** Süresi akış değerinden ($\vert{}f_{max}\vert{}$) tamamen bağımsızdır ve güçlü polinom zamanda ($O(V \cdot E^2)$) çalışır.

### 3. Dinic Algoritması

* **Arama Yöntemi:** Seviye Grafı (BFS) + Engelleme Akışı (DFS).
* **Temel Fikir:** Her BFS adımında düğümlere seviyeler atayarak bir **Seviye Grafı (Level Graph)** kurar. Ardından DFS kullanarak ve `ptr` (dynamic pointer) optimizasyonuyla o seviyedeki tüm yolları tek bir fazda (**Engelleme Akışı / Blocking Flow**) doyurur.
* **Kritik Avantaj:** $O(V^2 \cdot E)$ zaman sınırı ile büyük graflarda rakipsizdir. Birim ağlarda (Bipartite Matching) $O(E \sqrt{V})$ hızına ulaşır.

---

## 3. Karşılaştırma Tablosu

| Kriter | Ford-Fulkerson | Edmonds-Karp | Dinic |
| --- | --- | --- | --- |
| **Arama Stratejisi** | DFS (Derinlemesine) | BFS (En Kısa Yol) | BFS (Seviye Grafı) + DFS (Engelleme Akışı) |
| **Zaman Karmaşıklığı (Worst Case)** | $O(E \cdot \Vert{}f_{max}\Vert{})$ | $O(V \cdot E^2)$ | $\mathbf{O(V^2 \cdot E)}$ |
| **Birim Ağ Karmaşıklığı (Cap = 1)** | $O(E \cdot \Vert{}f_{max}\Vert{})$ | $O(V \cdot E^2)$ | $\mathbf{O(E \sqrt{V})}$ |
| **Alan Karmaşıklığı** | $O(V^2)$ veya $O(V + E)$ | $O(V^2)$ veya $O(V + E)$ | $\mathbf{O(V + E)}$ |
| **Kapasite Değerine Bağımlı mı?** | **Evet** (Çok hassas) | **Hayır** | **Hayır** |
| **Kodlama Karmaşıklığı** | Çok Düşük (Kolay) | Orta (Sadece BFS) | Yüksek (BFS + DFS + `ptr` + `reverse`) |
| **En İyi Kullanım Senaryosu** | Küçük tamsayı kapasiteler | Orta ölçekli graflar ($V \le 1.000$) | Büyük ölçekli graflar ($V > 1.000$), CP, Bipartite Matching |

---

## 4. Algoritma Seçim Rehberi (Karar Akışı)

```text
Maksimum Akış Problemi
  │
  ├── Graflar çok küçük mü? (V < 100) ve Kapasiteler küçük tamsayılar mı?
  │     └── EVET ──> Ford-Fulkerson (Hızlı kodlama ve basit yapı)
  │
  ├── Kapasiteler çok yüksek veya irrasyonel sayılar içeriyor mu?
  │     │
  │     ├── Graflar orta ölçekli mi? (V ≤ 1.000, E ≤ 10.000)
  │     │     └── EVET ──> Edmonds-Karp (Garantili O(V * E^2) zamanı, güvenilir)
  │     │
  │     └── Graflar büyük, yoğun veya kısıtlı zaman limiti var mı? (V > 1.000)
  │           └── EVET ──> Dinic Algoritması (Sektör standardı, O(V^2 * E))
  │
  └── Problem İki Parçalı Eşleştirme (Bipartite Matching) mi?
        └── EVET ──> Dinic Algoritması (O(E * sqrt(V)) ile mükemmel performans)

```

---

## 5. Performans ve Algoritmik Gelişim Mantığı

1. **Ford-Fulkerson $\rightarrow$ Edmonds-Karp Geçişi:**
* DFS rastgele derin yollara saparak 1 birimlik akışlar için sürekli döngüye girebilir.
* BFS eklenerek her adımda kaynaktan hedefe giden **en kısa yol (en az kenarlı yol)** seçilmiş ve her kenarın en fazla $O(V)$ kez darboğaz olabileceği kanıtlanarak zaman sınırı $O(V \cdot E^2)$ olarak sabitleşmiştir.


2. **Edmonds-Karp $\rightarrow$ Dinic Geçişi:**
* Edmonds-Karp her BFS adımında sadece **1 adet artırıcı yol** bulup tüm grafı baştan tarar.
* Dinic ise tek bir BFS ile tüm düğümlere seviye atar (Seviye Grafı) ve tek bir fazda DFS çalıştırarak aynı seviyedeki **TÜM yolları doyurur (Blocking Flow)**.
* Tıkalı/doymuş kenarları `ptr` dizisiyle $O(1)$ sürede atlayarak işlem sayısını $O(V \cdot E^2)$ değerinden $O(V^2 \cdot E)$ seviyesine çeker.



---

## 6. Örnek Modül Dosya Yapısı

```text
max_flow_module/
├── include/
│   ├── FordFulkerson.hpp
│   ├── EdmondsKarp.hpp
│   └── Dinic.hpp
├── src/
│   ├── FordFulkerson.cpp
│   ├── EdmondsKarp.cpp
│   └── Dinic.cpp
├── tests/
│   └── main_benchmark.cpp    # Üç algoritmanın sürelerini ve doğru sonuçlarını doğrulayan test dosyası
└── README.md                 # Karşılaştırma ve dokümantasyon rehberi

```