#include <iostream>
#include <vector>
#include <algorithm> //next_permutation ve sort için
#include <climits> // INT_MAX için

struct TSPResult {
    int min_cost;
    std::vector<int> best_path; //optimum rotanın sırası
};

TSPResult TSPBruteForce(const std::vector<std::vector<int>>& distance) {
    int n = distance.size();

    // 0. hariç şehirleri permütasyona sokar (0 başlangıç)
    std::vector<int> cities;
    for (int i = 1; i < n; ++i) {
        cities.push_back(i);
    }

    int min_cost = INT_MAX;
    std::vector<int> best_path;

    // std::next_permutation'ın çalışması için sıralama
    std::sort(cities.begin(), cities.end());

    do {
        int current_cost = 0; // denenen permütasyonun toplam mesafesi
        int current_city = 0; // Başlangıç şehri (0)

        // Rota üzerindeki mesafeleri topla: 0 -> c1 -> c2 -> ... -> c(n-1)
        for (int next_city : cities) {
            current_cost += distance[current_city][next_city];
            current_city = next_city;
        }
        
        // Son şehirden tekrar başlangıç şehri olan 0'a dönüş mesafesini ekle
        current_cost += distance[current_city][0];

        // Daha kısa bir yol bulunduysa rekoru güncelle
        if (current_cost < min_cost) {
            min_cost = current_cost;

            best_path.clear();
            best_path.push_back(0);
            best_path.insert(best_path.end(), cities.begin(), cities.end());
            best_path.push_back(0);
        }

    } while (std::next_permutation(cities.begin(), cities.end()));

    return {min_cost, best_path};
}

int main() {
    // distance[i][j] = i şehri ile j şehri arasındaki mesafe
    std::vector<std::vector<int>> distance = {
        {0,  10, 15, 20}, // 0. Şehirden diğerlerine olan mesafeler
        {10, 0,  35, 25},
        {15, 35, 0,  30},
        {20, 25, 30, 0}
    };

    TSPResult result = TSPBruteForce(distance);

    std::cout << "Mümkün Olan En Kısa Mesafe: " << result.min_cost << "\n";
    std::cout << "Optimum Rota: ";
    for (size_t i = 0; i < result.best_path.size(); ++i) {
        std::cout << result.best_path[i];
        if (i != result.best_path.size() - 1) std::cout << " -> ";
    }
    std::cout << "\n";

    return 0;
}