#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

struct Item {
    int id;
    int weight;
    int value;
    int scaled_value; // Ölçeklenmiş (küçültülmüş) değer
};

// Değer Tabanlı Dinamik Programlama (Value-Based DP)
// Zaman Karmaşıklığı: O(N * Sum(scaled_values))
int solveScaledDP(int W, const std::vector<Item>& items, int max_scaled_val) {
    int n = items.size();
    int max_possible_value = n * max_scaled_val;

    // dp[v]: v kadar değeri elde etmek için gereken minimum ağırlık
    // Başlangıçta tüm değerler sonsuz (INF) yapılır
    const int INF = 1e9;
    std::vector<int> dp(max_possible_value + 1, INF);
    dp[0] = 0; // 0 değer için 0 ağırlık gerekir

    for (int i = 0; i < n; ++i) {
        for (int v = max_possible_value; v >= items[i].scaled_value; --v) {
            dp[v] = std::min(dp[v], dp[v - items[i].scaled_value] + items[i].weight);
        }
    }

    // Kapasite W'yi aşmayan EN YÜKSEK ölçeklenmiş değeri bul
    int best_scaled_value = 0;
    for (int v = max_possible_value; v >= 0; --v) {
        if (dp[v] <= W) {
            best_scaled_value = v;
            break;
        }
    }

    return best_scaled_value;
}

double knapsackFPTAS(int W, std::vector<Item> items, double epsilon, int& approx_real_value) {
    int n = items.size();

    // 1. En büyük değere sahip eşyayı (V_max) bul
    int max_val = 0;
    for (const auto& item : items) {
        max_val = std::max(max_val, item.value);
    }

    // 2. Ölçekleme Faktörünü (K) Hesapla
    // K = (epsilon * V_max) / N
    double K = (epsilon * max_val) / n;

    // EĞER K < 1 ise ölçeklemeye gerek yok
    if (K < 1.0) K = 1.0;

    // 3. Eşya değerlerini K ile bölerek küçült (Floor işlemi)
    int max_scaled_val = 0;
    for (auto& item : items) {
        item.scaled_value = std::floor(item.value / K);
        max_scaled_val = std::max(max_scaled_val, item.scaled_value);
    }

    // 4. Değer Tabanlı DP'yi Küçük Değerlerle Çalıştır
    int best_scaled_val = solveScaledDP(W, items, max_scaled_val);

    // 5. Yaklaşık Gerçek Değeri Geri Hesapla
    approx_real_value = 0;
    // Ölçeklenmiş değeri K ile çarparak orijinale yaklaştır
    approx_real_value = std::round(best_scaled_val * K);

    return K;
}

int main() {
    int W = 100; // Çanta Kapasitesi
    std::vector<Item> items = {
        {1, 20, 6000},
        {2, 30, 10000},
        {3, 50, 12000},
        {4, 70, 13000}
    };

    double epsilon = 0.2; // %20 Hata Payı Toleransı
    int approx_value = 0;

    double K = knapsackFPTAS(W, items, epsilon, approx_value);

    std::cout << "--- FPTAS SONUÇLARI ---\n";
    std::cout << "Epsilon (Hata Payı) : " << epsilon << " (% " << epsilon * 100 << " hata toleransı)\n";
    std::cout << "Ölçekleme Faktörü (K): " << K << "\n";
    std::cout << "Yaklaşık Bulunan Değer: " << approx_value << "\n";

    return 0;
}