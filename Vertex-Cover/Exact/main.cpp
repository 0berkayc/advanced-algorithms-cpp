#include <iostream>
#include <vector>
#include <utility>

// Exact (Backtracking) Vertex Cover Yardımcı Fonksiyonu
void exactVertexCoverHelper(
    int numVertices,
    const std::vector<std::pair<int, int>>& edges,
    std::vector<bool>& visitedEdges,
    int uncoveredCount,
    std::vector<int>& currentCover,
    std::vector<int>& bestCover
) {
    // 1. BUDAMA (Pruning): Mevcut yol zaten bulduğumuz en iyi çözümden kötü veya eşitse aramayı kes
    if (currentCover.size() >= bestCover.size()) {
        return;
    }

    // 2. TABAN KOŞULU (Base Case): Tüm kenarlar kapandıysa yeni optimum çözümü bulduk demektir
    if (uncoveredCount == 0) {
        bestCover = currentCover;
        return;
    }

    // 3. Kapsanmamış ilk kenarı bul
    int selectedEdgeIndex = -1;
    for (size_t i = 0; i < edges.size(); ++i) {
        if (!visitedEdges[i]) {
            selectedEdgeIndex = i;
            break;
        }
    }

    if (selectedEdgeIndex == -1) {
        if (currentCover.size() < bestCover.size()) {
            bestCover = currentCover;
        }
        return;
    }

    int u = edges[selectedEdgeIndex].first;
    int v = edges[selectedEdgeIndex].second;

    // ----------------------------------------------------
    // DAL 1: u Düğümünü Seç
    // ----------------------------------------------------
    currentCover.push_back(u);
    std::vector<int> newlyVisitedU;

    // u düğümüne bağlı ve henüz kapatılmamış kenarları işaretle
    for (size_t i = 0; i < edges.size(); ++i) {
        if (!visitedEdges[i] && (edges[i].first == u || edges[i].second == u)) {
            visitedEdges[i] = true;
            newlyVisitedU.push_back(i);
        }
    }

    // Özyinelemeli çağrı
    exactVertexCoverHelper(
        numVertices, edges, visitedEdges, 
        uncoveredCount - newlyVisitedU.size(), currentCover, bestCover
    );

    // GERİ ADIM (Backtrack U): Yapılan değişiklikleri geri al
    for (int edgeIdx : newlyVisitedU) {
        visitedEdges[edgeIdx] = false;
    }
    currentCover.pop_back();

    // ----------------------------------------------------
    // DAL 2: v Düğümünü Seç
    // ----------------------------------------------------
    currentCover.push_back(v);
    std::vector<int> newlyVisitedV;

    // v düğümüne bağlı ve henüz kapatılmamış kenarları işaretle
    for (size_t i = 0; i < edges.size(); ++i) {
        if (!visitedEdges[i] && (edges[i].first == v || edges[i].second == v)) {
            visitedEdges[i] = true;
            newlyVisitedV.push_back(i);
        }
    }

    // Özyinelemeli çağrı
    exactVertexCoverHelper(
        numVertices, edges, visitedEdges, 
        uncoveredCount - newlyVisitedV.size(), currentCover, bestCover
    );

    // GERİ ADIM (Backtrack V): Yapılan değişiklikleri geri al
    for (int edgeIdx : newlyVisitedV) {
        visitedEdges[edgeIdx] = false;
    }
    currentCover.pop_back();
}

// Ana Çağırıcı Fonksiyon
std::vector<int> exactVertexCover(int numVertices, const std::vector<std::pair<int, int>>& edges) {
    std::vector<bool> visitedEdges(edges.size(), false);
    std::vector<int> currentCover;
    std::vector<int> bestCover;

    // Başlangıçta bestCover'a en kötü durum olarak tüm düğümleri koyuyoruz
    for (int i = 0; i < numVertices; ++i) {
        bestCover.push_back(i);
    }

    exactVertexCoverHelper(numVertices, edges, visitedEdges, edges.size(), currentCover, bestCover);

    return bestCover;
}

int main() {
    int numVertices = 7;
    std::vector<std::pair<int, int>> edges = {
        {0, 1}, {0, 2}, {1, 3}, {2, 3}, {3, 4}, {4, 5}, {4, 6}
    };

    std::vector<int> result = exactVertexCover(numVertices, edges);

    std::cout << "Seçilen Düğümler (Exact Vertex Cover): ";
    for (int node : result) {
        std::cout << node << " ";
    }
    std::cout << "\nMinimum Düğüm Sayısı: " << result.size() << "\n";

    return 0;
}