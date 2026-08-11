#include <iostream>
#include <vector>
#include <algorithm> //max için

struct Item {
    int weight;
    int value;
};

// Recursive Knapsack Fonksiyonu
int knapsackRecursive(int i, int capacity, const std::vector<Item>& items) {
    // 1. Base Case: Eşya kalmadıysa veya çanta kapasitesi 0 ise
    if (i == 0 || capacity == 0) {
        return 0;
    }

    // 2. Mevcut eşya kalan kapasiteye sığmıyorsa eşyayı alamaz
    if (items[i - 1].weight > capacity) {
        return knapsackRecursive(i - 1, capacity, items);
    }

    // 3. Karar: Eşyayı ALMA (skip) veya AL (take)
    int skip = knapsackRecursive(i - 1, capacity, items);
    int take = items[i - 1].value + knapsackRecursive(i - 1, capacity - items[i - 1].weight, items);

    // En yüksek değer veren kararı seç
    return std::max(take, skip);
}

int main() {
    int W = 10; // Çanta Kapasitesi
    std::vector<Item> items = { // {ağırlık, değer}
        {2, 6},   
        {3, 10},
        {5, 12},
        {7, 13}
    };

    int max_value = knapsackRecursive(items.size(), W, items);
    std::cout << "Maksimum Değer: " << max_value << "\n";

    return 0;
}