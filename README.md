# Advanced Algorithms

Klasik bilgisayar bilimleri, graf teorisi, kombinatöryel optimizasyon ve NP-Tam / NP-Zor (NP-Hard) problemlerin yüksek performanslı modern C++ uygulamalarını, zaman/alan karmaşıklığı analizlerini ve detaylı dokümantasyonlarını içeren algoritmik kütüphane projesi.

---

## Modül Özeti ve İçerik

Aşağıdaki tabloda depoda yer alan algoritmik modüller, kategorileri ve içerdikleri temel çözümler listelenmiştir:

| Modül / Klasör | Açıklama ve Kapsam | Kategori / Problem Sınıfı |
| --- | --- | --- |
| **`Bin-Packing`** | Nesneleri minimum sayıda konteynere/bin'e yerleştirme problemleri ve sezgisel yaklaşımlar (First-Fit, Best-Fit). | Optimizasyon / NP-Hard |
| **`Convex-Hull`** | 2D düzlemdeki nokta kümesini çevreleyen en küçük konveks çokgeni bulma algoritmaları (Graham Scan, Jarvis March). | Hesaplamalı Geometri |
| **`Graph-Coloring`** | Graf düğümlerini komşular çakışmayacak şekilde minimum renkle boyama (Greedy, Backtracking). | Graf Teorisi / NP-Complete |
| **`Knapsack`** | Sırt Çantası Problemi çözümleri (0/1 Knapsack - Dinamik Programlama, Kesirli Knapsack - Açgözlü Yaklaşım). | Dinamik Programlama / Optimizasyon |
| **`Maximum-Flow`** | Yönlü ağlarda kaynaktan hedefe maksimum akışı bulma (Ford-Fulkerson, Edmonds-Karp, Dinic). | Ağ Akışları / Graf Teorisi |
| **`N-Queens`** | $N \times N$ satranç tahtasında vezirleri birbirini tehdit etmeyecek şekilde yerleştirme problemleri. | Geri İzleme (Backtracking) |
| **`String-Matching`** | Metin içinde desen arama ve eşleştirme algoritmaları (Knuth-Morris-Pratt / KMP, Naive vb.). | Metin İşleme Algoritmaları |
| **`Sudoku`** | Sudoku tahtalarını çözen kısıt sağlama ve geri izleme algoritmaları. | Backtracking / NP-Complete |
| **`TSP`** | Gezgin Satıcı Problemi (Traveling Salesperson Problem) çözümleri (DP, 2-Opt Sezgisel, Brute-Force). | Optimizasyon / NP-Hard |
| **`Vertex-Cover`** | Graftaki tüm kenarları örten minimum düğüm kümesini bulma (Yaklaşım Algoritmaları, Tam Çözümler). | Graf Teorisi / NP-Complete |

---

## 📂 Proje Dizin Yapısı

```text
.
├── Bin-Packing/        # Bin Packing problemi ve sezgisel yöntemler
├── Convex-Hull/        # Konveks Örtü algoritmaları
├── Graph-Coloring/     # Graf boyama yöntemleri
├── Knapsack/           # 0/1 Knapsack ve türevleri
├── Maximum-Flow/       # Ford-Fulkerson, Edmonds-Karp, Dinic algoritmaları
├── N-Queens/           # N-Vezir problemi ve Backtracking
├── String-Matching/    # KMP ve metin arama algoritmaları
├── Sudoku/             # Sudoku çözücü algoritmalar
├── TSP/                # Gezgin Satıcı Problemi çözümleri
├── Vertex-Cover/       # Minimum Düğüm Örtüsü algoritmaları
├── .gitignore
└── README.md           # Proje genel dokümantasyon dosyası

```

---

## 🛠️ Gereksinimler ve Derleme

Projedeki kodlar modern **C++17 / C++20** standartları kullanılarak geliştirilmiştir.

### Ön Gereksinimler

* **Derleyici:** `g++` (GCC 9.0+) veya `clang++` (LLVM 10.0+) / MSVC
* **Build Sistemi (Opsiyonel):** `CMake` (3.15+) veya Doğrudan Terminal/Make

### Örnek Derleme ve Çalıştırma

Her modül kendi klasörü içerisinde bağımsız olarak derlenebilir ve çalıştırılabilir.

Örnek olarak **Maximum-Flow** modülünü derlemek için:

```bash
# Repoyu klonlayın
git clone https://github.com/kullanici_adi/repo_adi.git
cd repo_adi/Maximum-Flow

# C++ dosyasını derleyin
g++ -O3 -std=c++17 main.cpp -o maxflow

# Çalıştırın
./maxflow

```

---

## 📖 Dokümantasyon Standardı

Her alt klasör (modül) içerisinde şunları içeren özel bir `README.md` bulunur:

1. **Problemin Tanımı ve Zorluk Sınıfı:** Problemin matematiksel ve teorik altyapısı.
2. **Algoritma Mantığı ve Çalışma Prensibi:** Çözüm yaklaşımının detayları.
3. **Sözde Kod (Pseudo-code):** Adım adım algoritma akışı.
4. **Karmaşıklık Analizi:** Best, Average ve Worst Case Zaman/Alan karmaşıklıkları.