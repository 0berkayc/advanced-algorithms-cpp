#include <iostream>

// bitmask kullanarak n-queens çözümü
void solveNQueensBitmask(int cols, int left_diags, int right_diags, int all_on_mask, int& solution_count) {
    // taban durum: tüm sütunlara vezir yerleştirildi (tüm bitler 1 oldu)
    if (cols == all_on_mask) {
        solution_count++;
        return;
    }

    // tehdit altında olmayan (boşta olan) kullanılabilir sütunları bul
    int available_positions = all_on_mask & ~(cols | left_diags | right_diags);

    // boş pozisyon kaldığı sürece döngüyü sürdür
    while (available_positions > 0) {
        // en sağdaki boş biti (ilk 1 olan biti) izole et
        int p = available_positions & -available_positions;

        // seçilen biti listeden düş
        available_positions -= p;

        // bir sonraki satıra geç ve çapraz maskelerini kaydırarak güncelle
        solveNQueensBitmask(
            cols | p,
            (left_diags | p) << 1,
            (right_diags | p) >> 1,
            all_on_mask,
            solution_count
        );
    }
}

int main() {
    int n = 8; // tahta boyutu (n x n)
    int solution_count = 0;

    // n adet biti 1 yapan maske (örneğin n=8 için 11111111 = 255)
    int all_on_mask = (1 << n) - 1;

    solveNQueensBitmask(0, 0, 0, all_on_mask, solution_count);

    std::cout << "Toplam çözüm sayısı: " << solution_count << "\n";
    
    return 0;
}