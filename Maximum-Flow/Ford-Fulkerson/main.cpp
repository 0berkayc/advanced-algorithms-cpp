#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

class FordFulkerson {
private:
    int V; // Düğüm sayısı
    std::vector<std::vector<int>> capacity; // kalan kapasiteyi gösteren matris

    // DFS ile S'den T'ye giden artırıcı yol bulma
    bool dfs(int u, int t, int min_cap, std::vector<bool>& visited, std::vector<int>& parent) {
        if (u == t) return true; // Hedefe ulaşıldı
        visited[u] = true;

        for (int v = 0; v < V; ++v) {
            // Ziyaret edilmemiş ve kalan kapasitesi > 0 olan komşulara git
            if (!visited[v] && capacity[u][v] > 0) {
                parent[v] = u;
                if (dfs(v, t, min_cap, visited, parent)) {
                    return true;
                }
            }
        }
        return false;
    }

public:
    FordFulkerson(int nodes) : V(nodes) {
        capacity.assign(V, std::vector<int>(V, 0));
    }

    void addEdge(int u, int v, int cap) {
        capacity[u][v] += cap; // Doğrudan yönlü kenar ekleme
    }

    int getMaxFlow(int s, int t) {
        int max_flow = 0;
        std::vector<int> parent(V);

        // Kalıntı grafta S'den T'ye giden yol olduğu sürece döngüyü sürdür
        while (true) {
            std::vector<bool> visited(V, false);
            if (!dfs(s, t, INT_MAX, visited, parent)) {
                break; // Artık artırıcı yol kalmadı
            }

            // 1. Şişe boynu (bottleneck) kapasitesini bul
            int path_flow = INT_MAX;
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                path_flow = std::min(path_flow, capacity[u][v]);
            }

            // 2. Kalıntı grafı güncelle (İleri kenardan düş, geri kenara ekle)
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                capacity[u][v] -= path_flow;
                capacity[v][u] += path_flow;
            }

            max_flow += path_flow;
        }

        return max_flow;
    }
};

int main() {
    // 6 Düğümlü Çizge (0: Kaynak (S), 5: Hedef (T))
    FordFulkerson ff(6);

    ff.addEdge(0, 1, 16);
    ff.addEdge(0, 2, 13);
    ff.addEdge(1, 2, 10);
    ff.addEdge(1, 3, 12);
    ff.addEdge(2, 1, 4);
    ff.addEdge(2, 4, 14);
    ff.addEdge(3, 2, 9);
    ff.addEdge(3, 5, 20);
    ff.addEdge(4, 3, 7);
    ff.addEdge(4, 5, 4);

    std::cout << "Maksimum Akış (Ford-Fulkerson): " << ff.getMaxFlow(0, 5) << "\n";

    return 0;
}