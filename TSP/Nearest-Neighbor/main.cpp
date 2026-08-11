#include <iostream>
#include <vector>
#include <climits> // INT_MAX için

struct TSPResult {
    int total_cost; // toplam katediden mesafe
    std::vector<int> path; // rota sırası
};

TSPResult TSPNearestNeighbor(const std::vector<std::vector<int>>& distance, int start_city = 0) {
    int n = distance.size();
    
    std::vector<bool> visited(n, false); // şehir gezildi mi kontrolü
    std::vector<int> path;
    int total_cost = 0; // toplam mesafeyi ölçer

    int current_city = start_city; // başlangıç şehrinden başlatır
    path.push_back(current_city); // başlangıç şehri rotaya eklenir
    visited[current_city] = true; // başlangıç şehri ziyaret edildi olarak işaretlenir

    // N - 1 adet kalan şehri gez
    for (int step = 1; step < n; ++step) {
        int nearest_city = -1;
        int min_distance = INT_MAX;

        // Bulunulan şehre en yakın ziyaret edilmemiş şehri bul
        for (int next_city = 0; next_city < n; ++next_city) {
            if (!visited[next_city] && distance[current_city][next_city] < min_distance) {
                min_distance = distance[current_city][next_city];
                nearest_city = next_city;
            }
        }

        // Bulunan en yakın şehre git
        visited[nearest_city] = true;
        path.push_back(nearest_city);
        total_cost += min_distance;
        current_city = nearest_city;
    }

    // Başlangıç şehrine dönüş mesafesini ekle
    total_cost += distance[current_city][start_city];
    path.push_back(start_city);

    return {total_cost, path};
}

int main() {
    std::vector<std::vector<int>> distance = {
        {0,  10, 15, 20},
        {10, 0,  35, 25},
        {15, 35, 0,  30},
        {20, 25, 30, 0}
    };

    TSPResult result = TSPNearestNeighbor(distance, 0);

    std::cout << "Nearest Neighbor Hesaplanan Mesafe: " << result.total_cost << "\n";
    std::cout << "Hesaplanan Rota: ";
    for (size_t i = 0; i < result.path.size(); ++i) {
        std::cout << result.path[i];
        if (i != result.path.size() - 1) std::cout << " -> ";
    }
    std::cout << "\n";

    return 0;
}