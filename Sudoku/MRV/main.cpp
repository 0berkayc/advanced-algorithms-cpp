#include <iostream>
#include <vector>

// (row, col) hücresine 'num' değerinin yerleştirilip yerleştirilemeyeceğini kontrol eder
bool isValid(const std::vector<std::vector<int>>& board, int row, int col, int num) {
    for (int i = 0; i < 9; ++i) {
        if (board[row][i] == num || board[i][col] == num) {
            return false;
        }
        int startRow = 3 * (row / 3);
        int startCol = 3 * (col / 3);
        if (board[startRow + i / 3][startCol + i % 3] == num) {
            return false;
        }
    }
    return true;
}

// Belirli bir boş hücreye yazılabilecek geçerli rakamların sayısını hesaplar
int countValidCandidates(const std::vector<std::vector<int>>& board, int row, int col) {
    int count = 0;
    for (int num = 1; num <= 9; ++num) {
        if (isValid(board, row, col, num)) {
            count++;
        }
    }
    return count;
}

// MRV Sezgisi: Tahtadaki en az seçeneği kalan boş hücreyi bulur
bool findMRVCell(const std::vector<std::vector<int>>& board, int& bestRow, int& bestCol) {
    int minCandidates = 10; // 9'dan büyük herhangi bir değer
    bestRow = -1;
    bestCol = -1;

    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (board[r][c] == 0) {
                int candidates = countValidCandidates(board, r, c);

                // En az seçeneği kalan hücreyi güncelle
                if (candidates < minCandidates) {
                    minCandidates = candidates;
                    bestRow = r;
                    bestCol = c;
                }

                // Hiç seçeneği kalmayan hücre bulunduysa aramayı anında kesip bu hücreyi seç (Backtracking hemen tetiklensin)
                if (minCandidates == 0) {
                    return true;
                }
            }
        }
    }

    return (bestRow != -1); // Boş hücre bulundu mu?
}

// MRV Sezgisi ile Sudoku Çözen Backtracking Fonksiyonu
bool solveSudokuMRV(std::vector<std::vector<int>>& board) {
    int row, col;

    // 1. En kısıtlı hücreyi seç
    if (!findMRVCell(board, row, col)) {
        return true; // Boş hücre kalmadıysa Sudoku çözülmüştür
    }

    // 2. Seçilen hücrenin 0 adayı varsa bu dal çıkmaz sokaktır
    if (countValidCandidates(board, row, col) == 0) {
        return false; // Backtrack
    }

    // 3. Seçilen kısıtlı hücre için 1-9 arası geçerli rakamları dene
    for (int num = 1; num <= 9; ++num) {
        if (isValid(board, row, col, num)) {
            board[row][col] = num;

            // Özyinelemeli çağrı
            if (solveSudokuMRV(board)) {
                return true;
            }

            // Backtracking
            board[row][col] = 0;
        }
    }

    return false;
}

void printBoard(const std::vector<std::vector<int>>& board) {
    for (int row = 0; row < 9; ++row) {
        if (row > 0 && row % 3 == 0) {
            std::cout << "---------------------\n";
        }
        for (int col = 0; col < 9; ++col) {
            if (col > 0 && col % 3 == 0) {
                std::cout << "| ";
            }
            std::cout << board[row][col] << " ";
        }
        std::cout << "\n";
    }
}

int main() {
    std::vector<std::vector<int>> board = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    std::cout << "Başlangıç Sudoku Tahtası:\n\n";
    printBoard(board);

    if (solveSudokuMRV(board)) {
        std::cout << "\nMRV Sezgisi ile Çözülmüş Sudoku Tahtası:\n\n";
        printBoard(board);
    } else {
        std::cout << "\nBu Sudoku için geçerli bir çözüm bulunamadı.\n";
    }

    return 0;
}