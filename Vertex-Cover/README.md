# Vertex Cover (Düğüm Kaplama) Problemi ve Çözüm Yaklaşımları

Bu modül, NP-Hard sınıfında yer alan **Vertex Cover (Düğüm Kaplama)** probleminin iki farklı yaklaşım ile C++ dilindeki uygulamalarını ve karşılaştırmalı analizlerini içerir:

1. **2-Approximation Algoritması:** Polinomsal zamanda çalışan, teorik garantili sezgisel yaklaşım.
2. **Exact (Backtracking) Algoritması:** Küçük/orta ölçekli çizgelerde %100 mutlak optimum sonucu veren geri izleme yaklaşımı.

---

## 1. Problem Tanımı ve Zorluğu

Yönlendirilmemiş bir $G = (V, E)$ çizgesinde **Vertex Cover**, çizgedeki her bir kenarın ($e = (u, v) \in E$) en az bir ucunu kapsayacak şekilde seçilen minimum sayıda düğüm kümesidir ($V' \subseteq V$).

* **Zorluk Sınıfı:** NP-Hard (Optimizasyon) / NP-Complete (Karar).
* **Hesaplama Kısıtı:** Çizgedeki düğüm sayısı ($\vert{}V\vert{}$) arttıkça olası tüm alt kümelerin taranması üstel zaman ($O(2^{\vert{}V\vert{}})$) gerektirir. Bu nedenle büyük ölçekli çizgelerde hızlı sonuç almak için yaklaşım (approximation) algoritmalarına ihtiyaç duyulur.

---

## 2. Implente Edilen Algoritmalar

### A. 2-Approximation Algoritması (`/Approx-2`)

* **Mantık:** Açıkta kalan bir kenar seçilir ve bu kenarın **her iki ucu da** kaplamaya eklenir. Seçilen düğümlere bağlı tüm kenarlar kapatılarak işlem tekrarlanır.
* **Başarım Garanti Oranı:** Algoritmanın bulduğu çözüm boyutu, mutlak optimum çözümün (OPT) en fazla **2 katı** olabilir ($\vert{}C\vert{} \le 2 \cdot \text{OPT}$).
* **Avantajı:** Son derece hızlıdır ($O(\vert{}V\vert{} + \vert{}E\vert{})$). Devasa boyutlu çizgelerde saniyeler içinde kabul edilebilir bir çözüm üretir.

### B. Exact (Backtracking) Algoritması (`/Exact`)

* **Mantık:** Açıkta kalan herhangi bir $(u, v)$ kenarı için ya $u$'nun ya da $v$'nin seçilmesi gerektiği kuralına dayanarak ikili arama ağacı oluşturur. Budama (pruning) yöntemleri ile gereksiz dalları keserek tüm olasılıkları dener.
* **Başarım Garanti Oranı:** **%100 Mutlak Optimum ($\text{OPT}$)**.
* **Avantajı:** Hata payı yoktur; çizge üzerindeki en küçük düğüm kümesini bulur. Küçük çizgelerde ($\vert{}V\vert{} \le 30-40$) referans doğrulama aracı olarak kullanılır.

---

## 3. Algoritmaların Karşılaştırması

| Özellik / Metrik | 2-Approximation Algoritması | Exact (Backtracking) Algoritması |
| --- | --- | --- |
| **Çıktı Kalitesi** | Yaklaşık (En fazla $2 \cdot \text{OPT}$) | **%100 Mutlak Optimum ($\text{OPT}$)** |
| **Zaman Karmaşıklığı (Worst)** | $O(\vert{}V\vert{} + \vert{}E\vert{})$ (Polinomsal) | $O(2^{\vert{}V\vert{}} \cdot \vert{}E\vert{})$ (Üstel) |
| **Alan Karmaşıklığı** | $O(\vert{}V\vert{} + \vert{}E\vert{})$ | $O(\vert{}V\vert{} + \vert{}E\vert{})$ |
| **Çalışma Hızı** | Anlık (Saniyeler içinde) | Girdi büyüdükçe yavaşlar |
| **Kullanım Senaryosu** | Devasa ağlar, hızlı karar sistemleri | Küçük çizgeler, hassas optimizasyonlar |

---

## 4. Proje Dizin Yapısı

```text
Vertex-Cover/
├── README.md                 # Bu genel bilgilendirme dokümanı
├── 2-Approximation/
│   ├── main.cpp              # 2-Approximation C++ kaynak kodu
│   └── README.md             # 2-Approximation detaylı analizi ve sözde kodu
└── Exact/
    ├── main.cpp              # Exact (Backtracking) C++ kaynak kodu
    └── README.md             # Exact algoritma detaylı analizi ve sözde kodu

```

---

## 5. Örnek Test ve Sonuç Kıyaslaması

Aşağıdaki 7 düğüm ve 7 kenardan oluşan örnek çizge üzerinde her iki algoritma da test edilmiştir:

* **Düğümler:** `{0, 1, 2, 3, 4, 5, 6}`
* **Kenarlar:** `(0,1), (0,2), (1,3), (2,3), (3,4), (4,5), (4,6)`

### Test Sonuçları:

```text
[2-Approximation Sonucu]
Seçilen Düğümler: 0 1 2 3 4 5
Seçilen Düğüm Sayısı: 6

[Exact (Backtracking) Sonucu]
Seçilen Düğümler: 0 3 4
Seçilen Düğüm Sayısı: 3 (Optimum)

```

* **Analiz:** Exact algoritması mutlak optimum olan **3** değerini bulmuştur. 2-Approximation algoritması ise **6** değerini üreterek $6 \le 2 \times 3$ teorik üst sınır şartını ($2 \cdot \text{OPT}$) tam olarak doğrulamıştır.

---

## 6. Derleme ve Çalıştırma

Her iki algoritmayı ilgili klasörlerine giderek derleyip çalıştırabilirsiniz:

### 2-Approximation Çalıştırma

```bash
cd Vertex-Cover/2-Approximation
g++ -std=c++17 main.cpp -o approx2
./approx2

```

### Exact (Backtracking) Çalıştırma

```bash
cd Vertex-Cover/Exact
g++ -std=c++17 main.cpp -o exact
./exact

```
