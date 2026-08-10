#include <iostream>
#include <vector>
#include <cmath>

// Vezirin (row, col) konumuna emniyetle konulup konulamayacağını kontrol eder
bool isSafe(int row, int col, const std::vector<int>& board) {
    for (int r = 0; r < row; ++r) {
        int c = board[r];
        // 1. Aynı sütunda vezir var mı?
        // 2. Aynı çaprazda vezir var mı? (|r1 - r2| == |c1 - c2|)
        if (c == col || std::abs(r - row) == std::abs(c - col)) {
            return false;
        }
    }
    return true;
}

// N-Queens Backtracking fonksiyonu
void solveNQueens(int row, int n, std::vector<int>& board, int& solution_count) {
    // Taban Durum: Tüm satırlara vezir başarıyla yerleştirildi
    if (row == n) {
        solution_count++;
        std::cout << "Çözüm " << solution_count << ":\n";
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) {
                if (board[r] == c) {
                    std::cout << "Q ";
                } else {
                    std::cout << ". ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "\n";
        return;
    }

    // Mevcut satır (row) için tüm sütunları (col) dene
    for (int col = 0; col < n; ++col) {
        if (isSafe(row, col, board)) {
            board[row] = col;                                 // Veziri yerleştir
            solveNQueens(row + 1, n, board, solution_count);  // Bir sonraki satıra geç
            // Backtracking: 1D dizi kullandığımız için döngünün sonraki adımında 
            // 'board[row]' otomatik olarak yeni 'col' değeriyle ezilir.
        }
    }
}

int main() {
    int n = 8; // Tahta boyutu (N x N)
    std::vector<int> board(n, -1);
    int solution_count = 0;

    std::cout << n << "-Queens Çözümleri:\n\n";
    solveNQueens(0, n, board, solution_count);

    std::cout << "Toplam Çözüm Sayısı: " << solution_count << "\n";

    return 0;
}