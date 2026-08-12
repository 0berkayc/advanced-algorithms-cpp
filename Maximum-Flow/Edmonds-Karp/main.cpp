#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

class EdmondsKarp {
private:
    int V; // Düğüm sayısı
    std::vector<std::vector<int>> capacity; // Kalıntı kapasite matrisi

    // BFS ile S'den T'ye en az kenarlı en kısa yolu bulma
    bool bfs(int s, int t, std::vector<int>& parent) {
        std::fill(parent.begin(), parent.end(), -1);
        parent[s] = s;

        std::queue<std::pair<int, int>> q;
        q.push({s, INT_MAX}); // {düğüm, o ana kadarki minimum kapasite}

        while (!q.empty()) {
            int u = q.front().first;
            int flow = q.front().second;
            q.pop();

            for (int v = 0; v < V; ++v) {
                // Henüz ziyaret edilmediyse ve kalan kapasite > 0 ise
                if (parent[v] == -1 && capacity[u][v] > 0) {
                    parent[v] = u;
                    int new_flow = std::min(flow, capacity[u][v]);

                    if (v == t) return true; // Hedefe ulaşıldı

                    q.push({v, new_flow});
                }
            }
        }
        return false;
    }

public:
    EdmondsKarp(int nodes) : V(nodes) {
        capacity.assign(V, std::vector<int>(V, 0));
    }

    void addEdge(int u, int v, int cap) {
        capacity[u][v] += cap; // Yönlü kenar ekleme
    }

    int getMaxFlow(int s, int t) {
        int max_flow = 0;
        std::vector<int> parent(V);

        // Kalıntı grafta BFS ile yol bulunduğu sürece çalış
        while (bfs(s, t, parent)) {
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
    EdmondsKarp ek(6);

    ek.addEdge(0, 1, 16);
    ek.addEdge(0, 2, 13);
    ek.addEdge(1, 2, 10);
    ek.addEdge(1, 3, 12);
    ek.addEdge(2, 1, 4);
    ek.addEdge(2, 4, 14);
    ek.addEdge(3, 2, 9);
    ek.addEdge(3, 5, 20);
    ek.addEdge(4, 3, 7);
    ek.addEdge(4, 5, 4);

    std::cout << "Maksimum Akis (Edmonds-Karp): " << ek.getMaxFlow(0, 5) << "\n";

    return 0;
}