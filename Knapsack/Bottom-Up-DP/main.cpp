#include <iostream>
#include <vector>
#include <algorithm>

struct Item {
    int weight;
    int value;
};

int knapsackBottomUp(int capacity, const std::vector<Item>& items) {
    int n = items.size();

    // (n + 1) x (capacity + 1) boyutunda matris oluşturup tüm elemanları 0 yap
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));

    // Tabloyu döngülerle doldurma (Aşağıdan Yukarıya)
    for (int i = 1; i <= n; ++i) {
        for (int w = 0; w <= capacity; ++w) {
            // Eğer mevcut eşya bakılan w kapasitesine sığıyorsa
            if (items[i - 1].weight <= w) {
                int skip = dp[i - 1][w];
                int take = items[i - 1].value + dp[i - 1][w - items[i - 1].weight];
                
                dp[i][w] = std::max(skip, take);
            } else {
                // Sığmıyorsa üst satırdaki değeri aynen koru
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    // Sağ alt hücre (en optimal çözüm) döndür
    return dp[n][capacity];
}

int main() {
    int W = 10;
    std::vector<Item> items = {
        {2, 6}, 
        {3, 10},
        {5, 12},
        {7, 13} 
    };

    int max_val = knapsackBottomUp(W, items);
    std::cout << "Bottom-Up DP Maksimum Değer: " << max_val << "\n";

    return 0;
}