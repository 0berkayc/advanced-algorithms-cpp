# Convex Hull (Konveks Örtü / Kapsayan Kabuk) Algoritmaları

## 1. Convex Hull Problemi Nedir?
2 boyutlu ($2D$) bir düzlemde rastgele dağılmış $N$ adet nokta verildiğinde, bu noktaların **hepsini kapsayan ve en dıştaki noktalardan oluşan en küçük dış bükey (konveks) çokgeni** bulma problemidir.

* **Fiziki Benzetme:** Düzleme çakılmış çivilerin (noktaların) en dışına gergin bir paket lastiği geçirildiğinde, lastiğin temas ettiği en dıştaki çiviler Convex Hull noktalarıdır.
* **Geometrik Kural:** İçeriye doğru "çökük" veya "girintili" (concave) hiçbir köşe bulunamaz. Çokgen içindeki herhangi iki noktayı birleştiren çizgi tamamen çokgenin içinde kalmalıdır.

---

## 2. Kullanım Alanları ve Uygulamalar
1. **Görüntü İşleme ve Nesne Tanıma:** El/parmak tespiti, sınır belirleme ve nesne şekil analizleri.
2. **Oyun Geliştirme ve Fizik Motorları:** Karmaşık nesnelerin çarpışma algılamasını (Collision Detection) basitleştirmek için nesne etrafında sınır kutusu oluşturma.
3. **Coğrafi Bilgi Sistemleri (GIS) ve Haritacılık:** Belirli tesislerin, olayların veya kapsama alanlarının harita üzerindeki dış sınırını çizme.
4. **Robotik:** Robotlar için engellerden kaçınma ve güvenli hareket alanı belirleme.

---

## 3. Uygulanan Algoritmalar

Bu bölümde Convex Hull problemini çözmek için geliştirilen iki temel algoritma uygulanmıştır:

| Algoritma | Zaman Karmaşıklığı (Worst) | Alan Karmaşıklığı | Yaklaşım / Öne Çıkan Özellik |
| :--- | :--- | :--- | :--- |
| **Jarvis's March** *(Gift Wrapping)* | $O(N \cdot H)$ | $O(H)$ | Çıktıya duyarlı (*output-sensitive*). En soldaki noktadan başlayıp dış noktaları sırayla paketler. |
| **Graham Scan** | $O(N \log N)$ | $O(N)$ | Kutupsal açılara göre sıralama yapar ve `Stack` kullanarak konveksliği bozan noktaları eler. |

> **Not:** $N$ toplam nokta sayısını, $H$ ise sadece kapsayan kabuk üzerinde yer alan dış köşe noktalarının sayısını temsil eder ($H \le N$).

---

## 4. Karşılaştırma ve Algoritma Seçim Rehberi

* **Jarvis's March Ne Zaman Seçilmeli?**
  * Sınır üzerindeki köşe noktası sayısının ($H$) toplam nokta sayısına ($N$) göre çok az olduğu bilindiğinde ($H \ll N$).
  * Kodlama kolaylığı ve düşük bellek kullanımı öncelikli olduğunda.
* **Graham Scan Ne Zaman Seçilmeli?**
  * En kötü senaryoda dahi garantili $O(N \log N)$ performansı istendiğinde.
  * Noktaların çoğunun dış sınır üzerinde yer alma ihtimali yüksek olduğunda.

---

## 5. Klasör Yapısı

```text
Convex-Hull/
├── README.md               # Genel Convex Hull bilgilendirme dokümanı
├── Jarvis-March/           # Jarvis's March (Gift Wrapping) uygulaması
│   ├── main.cpp
│   └── README.md
└── Graham-Scan/            # Graham Scan uygulaması
    ├── main.cpp
    └── README.md
```