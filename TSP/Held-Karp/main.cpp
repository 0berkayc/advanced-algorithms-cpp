#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

const int INF = 1e9;

struct TSPResult {
    int min_cost;
    std::vector<int> path;
};

TSPResult TSPHeldKarp(const std::vector<std::vector<int>>& distance) {
    int n = distance.size();
    int num_states = 1 << n; // 2^N adet olası alt küme durumu

    // dp[mask][u]: mask kümesindeki şehirler gezildi, şu an u şehrindeyiz.
    std::vector<std::vector<int>> dp(num_states, std::vector<int>(n, INF));
    
    // Rota takibi için ebeveyn matrisi: parent[mask][u]
    std::vector<std::vector<int>> parent(num_states, std::vector<int>(n, -1));

    // Başlangıç Durumu: Sadece 0. şehir gezildi (mask = 1), konum = 0, maliyet = 0
    dp[1][0] = 0;

    // Tüm bitmask durumlarını küçükten büyüğe tara
    for (int mask = 1; mask < num_states; ++mask) {
        for (int u = 0; u < n; ++u) {
            // Eğer u şehri mevcut mask kümesinde yoksa veya u konumuna ulaşılamadıysa atla
            if (!(mask & (1 << u)) || dp[mask][u] == INF) continue;

            // Ziyaret edilmemiş bir v şehrine ilerlemeyi dene
            for (int v = 0; v < n; ++v) {
                // v şehri zaten gezildiyse atla
                if (mask & (1 << v)) continue;

                int next_mask = mask | (1 << v);
                int new_cost = dp[mask][u] + distance[u][v];

                // Daha kısa bir yol bulunduysa DP ve Parent tablosunu güncelle
                if (new_cost < dp[next_mask][v]) {
                    dp[next_mask][v] = new_cost;
                    parent[next_mask][v] = u; // v şehrine u şehrinden gelindi
                }
            }
        }
    }

    // Tüm şehirlerin gezildiği durum: full_mask = (1 << n) - 1
    int full_mask = num_states - 1;
    int min_cost = INF;
    int last_city = -1;

    // Son bulunulan v şehrinden tekrar başlangıç şehri olan 0'a dönüşü hesapla
    for (int v = 1; v < n; ++v) {
        if (dp[full_mask][v] != INF) {
            int total_cost = dp[full_mask][v] + distance[v][0];
            if (total_cost < min_cost) {
                min_cost = total_cost;
                last_city = v;
            }
        }
    }

    // Rotayı geriye doğru (Parent tablosundan) çıkar
    std::vector<int> path;
    int curr_mask = full_mask;
    int curr_city = last_city;

    path.push_back(0); // Bitiş şehri (Başlangıca dönüş)
    while (curr_city != -1) {
        path.push_back(curr_city);
        int prev_city = parent[curr_mask][curr_city];
        curr_mask = curr_mask ^ (1 << curr_city); // Bit'i temizle
        curr_city = prev_city;
    }

    std::reverse(path.begin(), path.end()); // Doğru sıraya çevir

    return {min_cost, path};
}

int main() {
    std::vector<std::vector<int>> distance = {
        {0,  10, 15, 20},
        {10, 0,  35, 25},
        {15, 35, 0,  30},
        {20, 25, 30, 0}
    };

    TSPResult result = TSPHeldKarp(distance);

    std::cout << "Mümkün Olan En Kısa Mesafe (Held-Karp): " << result.min_cost << "\n";
    std::cout << "Optimum Rota: ";
    for (size_t i = 0; i < result.path.size(); ++i) {
        std::cout << result.path[i] << (i == result.path.size() - 1 ? "" : " -> ");
    }
    std::cout << "\n";

    return 0;
}