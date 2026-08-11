#include <iostream>
#include <vector>
#include <algorithm>

struct Item {
    int weight;
    int value;
};

// Yardımcı Özyinelemeli Fonksiyon (Memoization Takipli)
int knapsackMemoHelper(int i, int capacity, const std::vector<Item>& items, std::vector<std::vector<int>>& memo) {
    // 1. TABAN DURUM (Base Case)
    if (i == 0 || capacity == 0) {
        return 0;
    }

    // 2. Hafıza kontrolü: Bu durum daha önce hesaplandıysa doğrudan tablodan oku
    if (memo[i][capacity] != -1) {
        return memo[i][capacity];
    }

    // 3. Kısıt kontrolü: Mevcut eşya kalan kapasiteye sığmıyorsa mecburen atla
    if (items[i - 1].weight > capacity) {
        return memo[i][capacity] = knapsackMemoHelper(i - 1, capacity, items, memo);
    }

    // 4. Karar: Atla (skip) veya Al (take)
    int skip = knapsackMemoHelper(i - 1, capacity, items, memo);
    int take = items[i - 1].value + knapsackMemoHelper(i - 1, capacity - items[i - 1].weight, items, memo);

    // Sonucu matrise kaydet ve geriye döndür
    return memo[i][capacity] = std::max(skip, take);
}

// Ana Fonksiyon
int knapsackMemoization(int capacity, const std::vector<Item>& items) {
    int n = items.size();

    // (n + 1) x (capacity + 1) boyutunda bir matris oluşturup tüm hücreleri -1 ile doldurur.
    // -1: henüz hesaplanmadı
    std::vector<std::vector<int>> memo(n + 1, std::vector<int>(capacity + 1, -1));

    return knapsackMemoHelper(n, capacity, items, memo);
}

int main() {
    int W = 10; // Çanta Kapasitesi
    std::vector<Item> items = {
        {2, 6}, 
        {3, 10},
        {5, 12},
        {7, 13} 
    };

    int max_val = knapsackMemoization(W, items);
    std::cout << "Memoization Maksimum Değer: " << max_val << "\n";

    return 0;
}