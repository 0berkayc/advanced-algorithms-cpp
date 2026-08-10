#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

// dsatur (degree of saturation) graph coloring
std::vector<int> dsaturColoring(int num_vertices, const std::vector<std::vector<int>>& neighbors) {
    std::vector<int> color(num_vertices, -1); // düğüm renkleri (-1: boyanmamış)
    std::vector<std::set<int>> neighbor_colors(num_vertices); // komşulardaki farklı renklerin kümesi

    // tüm düğümler boyanana kadar dön
    for (int step = 0; step < num_vertices; ++step) {
        int best_node = -1;
        int max_sat = -1;
        int max_degree = -1;

        // en yüksek doygunluğa (sat) sahip boyanmamış düğümü seç
        for (int u = 0; u < num_vertices; ++u) {
            if (color[u] != -1) continue; // boyanmış düğümleri geç

            int sat = neighbor_colors[u].size(); // doygunluk derecesi (farklı renk sayısı)
            int deg = neighbors[u].size();       // orijinal komşu sayısı

            // 1. öncelik: yüksek doygunluk, 2. öncelik: yüksek derece
            if (sat > max_sat || (sat == max_sat && deg > max_degree)) {
                max_sat = sat;
                max_degree = deg;
                best_node = u;
            }
        }

        // seçilen düğümün komşularında kullanılan renkleri işaretle
        std::vector<bool> used(num_vertices, false);
        for (int v : neighbors[best_node]) {
            if (color[v] != -1) {
                used[color[v]] = true;
            }
        }

        // kullanılabilir en küçük boş rengi bul
        int assigned_color = 0;
        while (used[assigned_color]) {
            assigned_color++;
        }

        // rengi ata
        color[best_node] = assigned_color;

        // komşuların doygunluk kümelerine yeni atanan rengi ekle
        for (int v : neighbors[best_node]) {
            neighbor_colors[v].insert(assigned_color);
        }
    }

    return color;
}

int main() {
    int num_vertices = 5;
    std::vector<std::vector<int>> neighbors(num_vertices);

    // kenarlar: (0-1), (0-2), (1-2), (1-3), (2-3), (3-4)
    neighbors[0] = {1, 2};
    neighbors[1] = {0, 2, 3};
    neighbors[2] = {0, 1, 3};
    neighbors[3] = {1, 2, 4};
    neighbors[4] = {3};

    std::vector<int> colors = dsaturColoring(num_vertices, neighbors);

    std::cout << "düğüm renk atamaları:\n";
    int max_color = -1;
    for (int i = 0; i < num_vertices; ++i) {
        std::cout << "düğüm " << i << " -> renk " << colors[i] << "\n";
        if (colors[i] > max_color) max_color = colors[i];
    }
    std::cout << "toplam kullanılan renk sayısı: " << (max_color + 1) << "\n";

    return 0;
}