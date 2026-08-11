#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

struct Item {
    int id;
    int weight;
    int value;
};

struct Node {
    int level;    // İncelenen eşya seviyesi (0-indexed)
    int profit;   // O ana kadarki toplam değer
    int weight;   // O ana kadarki toplam ağırlık
    double bound; // Bu daldan ulaşılabilecek maksimum teorik değer (Upper Bound)

    // Priority Queue'nun en yüksek bound değerine sahip düğümü öne alması için
    bool operator<(const Node& other) const {
        return bound < other.bound;
    }
};

// Birim değere (value / weight) göre azalan sıralama
bool compare(const Item& a, const Item& b) {
    double r1 = (double)a.value / a.weight;
    double r2 = (double)b.value / b.weight;
    return r1 > r2;
}

// Upper Bound (Üst Sınır) Hesaplama Fonksiyonu (Fractional Knapsack Mantığı)
double calculateBound(Node u, int n, int W, const std::vector<Item>& items) {
    if (u.weight >= W) return 0; // Kapasite aşıldıysa üst sınır 0'dır

    double profit_bound = u.profit;
    int j = u.level + 1;
    int totweight = u.weight;

    // Tam sığabilen eşyaları al
    while ((j < n) && (totweight + items[j].weight <= W)) {
        totweight += items[j].weight;
        profit_bound += items[j].value;
        j++;
    }

    // Sığmayan ilk eşyayı parçalayarak üst sınırı tamamla
    if (j < n) {
        profit_bound += (W - totweight) * ((double)items[j].value / items[j].weight);
    }

    return profit_bound;
}

int knapsackBranchAndBound(int W, std::vector<Item> items) {
    int n = items.size();

    // 1. Eşyaları birim değerlerine (v/w) göre sırala
    std::sort(items.begin(), items.end(), compare);

    // En yüksek bound'a sahip düğümü üstte tutan Max-Heap
    std::priority_queue<Node> PQ;

    // Kök düğüm ilklendirmesi
    Node u, v;
    u.level = -1;
    u.profit = 0;
    u.weight = 0;
    u.bound = calculateBound(u, n, W, items);

    PQ.push(u);

    int maxProfit = 0; // Şu ana kadar bulunan en iyi gerçek değer

    while (!PQ.empty()) {
        u = PQ.top();
        PQ.pop();

        // BUDAMA 1: Eğer bu dalın vaat ettiği üst sınır, elimizdeki en iyi değerden küçükse ilerleme
        if (u.bound <= maxProfit) {
            continue;
        }

        // Sıradaki eşyaya geç
        v.level = u.level + 1;

        if (v.level >= n) continue;

        // --- SEÇENEK 1: Eşyayı AL (Take) ---
        v.weight = u.weight + items[v.level].weight;
        v.profit = u.profit + items[v.level].value;

        // Kapasite aşılmadıysa ve değer daha iyiyse güncelle
        if (v.weight <= W && v.profit > maxProfit) {
            maxProfit = v.profit;
        }

        v.bound = calculateBound(v, n, W, items);

        // BUDAMA 2: Üst sınır maxProfit'ten büyükse bu dalı kuyruğa ekle
        if (v.bound > maxProfit) {
            PQ.push(v);
        }

        // --- SEÇENEK 2: Eşyayı ALMA (Skip) ---
        v.weight = u.weight;
        v.profit = u.profit;
        v.bound = calculateBound(v, n, W, items);

        // BUDAMA 3: Üst sınır maxProfit'ten büyükse bu dalı kuyruğa ekle
        if (v.bound > maxProfit) {
            PQ.push(v);
        }
    }

    return maxProfit;
}

int main() {
    int W = 10;
    std::vector<Item> items = {
        {1, 2, 6},   // {id, weight, value}
        {2, 3, 10},
        {3, 5, 12},
        {4, 7, 13}
    };

    int max_val = knapsackBranchAndBound(W, items);
    std::cout << "Branch and Bound Maksimum Değer: " << max_val << "\n";

    return 0;
}