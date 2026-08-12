#include <iostream>
#include <vector>
#include <queue> // priority_queue, greater için
#include <climits> // INF için
#include <algorithm>

const int INF = 1e9;

struct Node {
    std::vector<std::vector<int>> reduced_matrix; // Düğümün o anki indirgenmiş matrisi
    int cost;         // Düğümün Alt Sınırı (Lower Bound)
    int city;         // Şimdiki şehir
    int level;        // Ziyaret edilen şehir sayısı
    std::vector<int> path; // İzlenen rota

    // Priority Queue'da en KÜÇÜK cost'a sahip düğümü üstte tutmak için (Min-Heap)
    bool operator>(const Node& other) const {
        return cost > other.cost;
    }
};

// Matrisi indirger ve toplam indirgeme maliyetini döndürür
int reduceMatrix(std::vector<std::vector<int>>& matrix) {
    int n = matrix.size();
    int reduction_cost = 0;

    // Satır İndirgeme
    for (int i = 0; i < n; ++i) {
        int row_min = INF;
        for (int j = 0; j < n; ++j) {
            if (matrix[i][j] < row_min) row_min = matrix[i][j];
        }
        if (row_min != INF && row_min != 0) {
            reduction_cost += row_min;
            for (int j = 0; j < n; ++j) {
                if (matrix[i][j] != INF) matrix[i][j] -= row_min;
            }
        }
    }

    // Sütun İndirgeme
    for (int j = 0; j < n; ++j) {
        int col_min = INF;
        for (int i = 0; i < n; ++i) {
            if (matrix[i][j] < col_min) col_min = matrix[i][j];
        }
        if (col_min != INF && col_min != 0) {
            reduction_cost += col_min;
            for (int i = 0; i < n; ++i) {
                if (matrix[i][j] != INF) matrix[i][j] -= col_min;
            }
        }
    }

    return reduction_cost;
}

// Branch and Bound ile TSP Çözümü
int TSPBranchAndBound(std::vector<std::vector<int>> distance) {
    int n = distance.size();

    // En küçük cost'a sahip düğümü öne alan Min-Priority Queue
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;

    // Kök düğüm oluşturma
    Node root;
    root.city = 0;
    root.level = 0;
    root.path.push_back(0);
    root.reduced_matrix = distance;
    root.cost = reduceMatrix(root.reduced_matrix);

    pq.push(root);

    while (!pq.empty()) {
        Node min_node = pq.top();
        pq.pop();

        int u = min_node.city;

        // Tüm şehirler ziyaret edildiyse en optimal çözüm bulunmuştur
        if (min_node.level == n - 1) {
            min_node.path.push_back(0); // Başlangıç noktasına dönüş
            std::cout << "Mümkün Olan En Kısa Mesafe: " << min_node.cost << "\n";
            std::cout << "Optimum Rota: ";
            for (size_t i = 0; i < min_node.path.size(); ++i) {
                std::cout << min_node.path[i] << (i == min_node.path.size() - 1 ? "" : " -> ");
            }
            std::cout << "\n";
            return min_node.cost;
        }

        // Ziyaret edilmeyen komşu şehirlere dallan
        for (int v = 0; v < n; ++v) {
            if (min_node.reduced_matrix[u][v] != INF) {
                Node child;
                child.path = min_node.path;
                child.path.push_back(v);
                child.city = v;
                child.level = min_node.level + 1;
                child.reduced_matrix = min_node.reduced_matrix;

                // Satır u, Sütun v ve Hücre [v][0] sonsuz yapılıyor
                for (int k = 0; k < n; ++k) {
                    child.reduced_matrix[u][k] = INF; // u satırı
                    child.reduced_matrix[k][v] = INF; // v sütunu
                }
                child.reduced_matrix[v][0] = INF; // Erken dönüş engeli

                // Yeni maliyet = Eski Cost + Geçiş Maliyeti + Yeni Matris İndirgeme Maliyeti
                int transition_cost = min_node.reduced_matrix[u][v];
                int new_reduction = reduceMatrix(child.reduced_matrix);
                child.cost = min_node.cost + transition_cost + new_reduction;

                pq.push(child);
            }
        }
    }

    return -1;
}

int main() {
    std::vector<std::vector<int>> distance = {
        {INF, 10,  15,  20},
        {10,  INF, 35,  25},
        {15,  35,  INF, 30},
        {20,  25,  30,  INF}
    };

    TSPBranchAndBound(distance);

    return 0;
}