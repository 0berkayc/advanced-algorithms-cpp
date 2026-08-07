#include <iostream>
#include <vector>
#include <algorithm>

// Welsh-Powell Greedy Graph Coloring
std::vector<int> greedyColoring(int numVertices, const std::vector<std::vector<int>>& neighbors) {
    std::vector<int> color(numVertices, -1); // Düğüm renkleri (başlangıçta -1)
    
    std::vector<int> nodes(numVertices); // düğüm dizisi
    for (int i = 0; i < numVertices; i++) // düğüm indexlerini tut
    {
        nodes[i] = i;
    }
    
    std::sort(nodes.begin(), nodes.end(), [&](int a, int b) {
        return neighbors[a].size() > neighbors[b].size(); // Derecesi büyük olan öne geçer
    });

    // Düğümleri Sırayla Boyama
    for (int u : nodes) {
        // available[c] = true: c rengi kullanılabilir demektir
        std::vector<bool> available(numVertices, true);

        // u'nun boyanmış komşularının renklerini yasakla
        for (int v : neighbors[u]) {
            if (color[v] != -1) {
                available[color[v]] = false;
            }
        }

        // Kullanılabilecek en küçük boş rengi bul
        int assignedColor = 0;
        for (int cr = 0; cr < numVertices; ++cr) {
            if (available[cr]) {
                assignedColor = cr;
                break;
            }
        }

        // Rengi düğüme ata
        color[u] = assignedColor;
    }
    return color;
}

int main() {
    int numVertices = 5;
    std::vector<std::vector<int>> neighbors(numVertices);

    // Kenarlar: (0-1), (0-2), (1-2), (1-3), (2-3), (3-4)
 neighbors[0] = {1, 2};
 neighbors[1] = {0, 2, 3};
 neighbors[2] = {0, 1, 3};
 neighbors[3] = {1, 2, 4};
 neighbors[4] = {3};

    std::vector<int> colors = greedyColoring(numVertices, neighbors);

    std::cout << "Düğüm Renk Atamaları:\n";
    int maxColor = -1;
    for (int i = 0; i < numVertices; ++i) {
        std::cout << "Düğüm " << i << " -> Renk " << colors[i] << "\n";
        if (colors[i] > maxColor) maxColor = colors[i];
    }
    std::cout << "Toplam Kullanılan Renk Sayısı: " << (maxColor + 1) << "\n";

    return 0;
}