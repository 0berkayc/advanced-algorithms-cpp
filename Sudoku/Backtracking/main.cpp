#include <iostream>
#include <vector>

// (row, col) hücresine 'num' değerinin yerleştirilip yerleştirilemeyeceğini kontrol eder
bool isValid(const std::vector<std::vector<int>>& board, int row, int col, int num) {
    for (int i = 0; i < 9; ++i) {
        // 1. Satır Kontrolü: Aynı satırda aynı sayı var mı?
        if (board[row][i] == num) {
            return false;
        }

        // 2. Sütun Kontrolü: Aynı sütunda aynı sayı var mı?
        if (board[i][col] == num) {
            return false;
        }

        // 3. 3x3 Alt Blok Kontrolü: Hücrenin ait olduğu blokta aynı sayı var mı?
        int startRow = 3 * (row / 3);
        int startCol = 3 * (col / 3);
        if (board[startRow + i / 3][startCol + i % 3] == num) {
            return false;
        }
    }
    return true;
}

// Backtracking ile Sudoku Çözen Özyinelemeli Fonksiyon
bool solveSudoku(std::vector<std::vector<int>>& board) {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            // Boş hücre bulundu (0 = Boş)
            if (board[row][col] == 0) {
                
                // 1'den 9'a kadar tüm rakamları dene
                for (int num = 1; num <= 9; ++num) {
                    if (isValid(board, row, col, num)) {
                        
                        board[row][col] = num; // Rakamı yerleştir

                        // Bir sonraki boş hücre için özyinelemeli çağrı
                        if (solveSudoku(board)) {
                            return true; // Çözüm bulundu
                        }

                        // Backtracking: İleriki adımlarda tıkanırsa yapılan seçimi geri al
                        board[row][col] = 0;
                    }
                }

                // 1-9 arası hiçbir rakam uymuyorsa bu yol geçersizdir (Backtrack tetiklenir)
                return false;
            }
        }
    }

    // Hiç boş hücre kalmadıysa bulmaca çözülmüştür
    return true;
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
    // Örnek Sudoku Tahtası
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

    if (solveSudoku(board)) {
        std::cout << "\nÇözülmüş Sudoku Tahtası:\n\n";
        printBoard(board);
    } else {
        std::cout << "\nBu Sudoku için geçerli bir çözüm bulunamadı.\n";
    }

    return 0;
}