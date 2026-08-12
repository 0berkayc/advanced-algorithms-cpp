#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

struct Edge {
    int to;         // Gidilen düğüm
    int flow;       // Mevcut geçen akış
    int capacity;   // Toplam kapasite
    int reverse;        // Tersi yöndeki kenarın neighbor[to] içindeki indeksi
};

class Dinic {
private:
    int V;
    std::vector<std::vector<Edge>> neighbor;
    std::vector<int> level; // Seviye grafı için düğüm uzaklıkları
    std::vector<int> ptr;   // Doymuş kenarları atlamak için dynamic pointer

    // 1. BFS: Seviye Grafı Oluşturma
    bool bfs(int s, int t) {
        std::fill(level.begin(), level.end(), -1);
        level[s] = 0;

        std::queue<int> q;
        q.push(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (const auto& edge : neighbor[u]) {
                // Kalan kapasite > 0 VE düğüm henüz seviyelendirilmediyse
                if (edge.capacity - edge.flow > 0 && level[edge.to] == -1) {
                    level[edge.to] = level[u] + 1;
                    q.push(edge.to);
                }
            }
        }
        return level[t] != -1; // Hedefe ulaşılabildi mi?
    }

    // 2. DFS: Seviye Grafı Üzerinde Engelleme Akışı (Blocking Flow) Gönderme
    int dfs(int u, int t, int pushed) {
        if (pushed == 0) return 0;
        if (u == t) return pushed;

        // ptr[u] sayesinde tıkalı/doymuş kenarları tekrar incelemeyiz
        for (int& cid = ptr[u]; cid < neighbor[u].size(); ++cid) {
            auto& edge = neighbor[u][cid];
            int tr = edge.to;

            // Yalnızca 1 üst seviyedeki düğümlere ve kalan kapasitesi olanlara git
            if (level[u] + 1 != level[tr] || edge.capacity - edge.flow == 0) {
                continue;
            }

            int tr_pushed = dfs(tr, t, std::min(pushed, edge.capacity - edge.flow));
            if (tr_pushed == 0) continue;

            // İleri kenar akışını artır, geri kenar akışını azalt
            edge.flow += tr_pushed;
            neighbor[tr][edge.reverse].flow -= tr_pushed;

            return tr_pushed;
        }
        return 0;
    }

public:
    Dinic(int nodes) : V(nodes) {
        neighbor.resize(V);
        level.resize(V);
        ptr.resize(V);
    }

    // Yönlü Kenar Ekleme (İleri ve Sanal Geri Kenar Birlikte Oluşturulur)
    void addEdge(int u, int v, int capacity) {
        Edge a = {v, 0, capacity, static_cast<int>(neighbor[v].size())};
        Edge b = {u, 0, 0, static_cast<int>(neighbor[u].size())}; // Geri kenar kapasitesi 0
        neighbor[u].push_back(a);
        neighbor[v].push_back(b);
    }

    int getMaxFlow(int s, int t) {
        int max_flow = 0;

        // Seviye grafı oluşturulabildiği sürece devam et
        while (bfs(s, t)) {
            std::fill(ptr.begin(), ptr.end(), 0); // Pointer dizisini sıfırla

            // O anki seviye grafındaki tüm akışı (Blocking Flow) çek
            while (int pushed = dfs(s, t, INT_MAX)) {
                max_flow += pushed;
            }
        }
        return max_flow;
    }
};

int main() {
    // 6 Düğümlü Çizge (0: Kaynak (S), 5: Hedef (T))
    Dinic dinic(6);

    dinic.addEdge(0, 1, 16);
    dinic.addEdge(0, 2, 13);
    dinic.addEdge(1, 2, 10);
    dinic.addEdge(1, 3, 12);
    dinic.addEdge(2, 1, 4);
    dinic.addEdge(2, 4, 14);
    dinic.addEdge(3, 2, 9);
    dinic.addEdge(3, 5, 20);
    dinic.addEdge(4, 3, 7);
    dinic.addEdge(4, 5, 4);

    std::cout << "Maksimum Akis (Dinic): " << dinic.getMaxFlow(0, 5) << "\n";

    return 0;
}