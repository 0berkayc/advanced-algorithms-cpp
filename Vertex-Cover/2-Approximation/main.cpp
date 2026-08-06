#include <iostream>
#include <vector>
#include <utility>
#include <set>

std::vector<int> approxVertex(int numVertices, std::vector<std::pair<int, int>> edges) {
    
    std::vector<int> cover; //seçilen düğümler
    int numEdges = edges.size(); //kenar sayısı
    int uncoveredCount = numEdges; //kapsama alınmamış kenar sayısı
    std::vector<bool> visitedEdges(numEdges, false); //kenar kapsanıyor mu kontrolü

    while (uncoveredCount != 0)
    {
        int selectedEdgeIndex = -1; //seçilecek ilk kenar şu an boş

        for (int i = 0; i < numEdges; i++)
        {
            if (!visitedEdges[i]) //kenar kapsanmış mı?
            {
                selectedEdgeIndex = i; //ilk kenar olarak seç
                break;
            }
        }
        
        if (selectedEdgeIndex == -1)
        {
            break; //kapsanmamış kenar kalmadıysa döngüden çık
        }

        int u = edges[selectedEdgeIndex].first; //seçilen kenarın ilk düğümü
        int v = edges[selectedEdgeIndex].second; //seçilen kenarın ikinci düğümü

        cover.push_back(u); //u düğümünü listeye ekle
        cover.push_back(v); //v düğümünü listeye ekle

        for (int j = 0; j < numEdges; j++)
        {
            int node1 = edges[j].first; //ilk ucundaki düğüm
            int node2 = edges[j].second; //ikinci ucundaki düğüm

            //kenar henüz kapsanmamış ve uçlarından biri u veya v ise
            if (!visitedEdges[j] && (node1 == u || node1 == v || node2 == u || node2 == v))
            {
                visitedEdges[j] = true; //kapsama al
                uncoveredCount--;
            }
        }
    }
    return cover;
}

int main() {
    int numVertices = 7;
    std::vector<std::pair<int, int>> edges = {
        {0, 1}, {0, 2}, {1, 3}, {2, 3}, {3, 4}, {4, 5}, {4, 6}
    };

    std::vector<int> result = approxVertex(numVertices, edges);

    std::cout << "Seçilen Düğümler (Vertex Cover): ";
    for (int node : result) {
        std::cout << node << " ";
    }
    std::cout << "\nKullanılan Toplam Düğüm Sayısı: " << result.size() << "\n";

    return 0;
}