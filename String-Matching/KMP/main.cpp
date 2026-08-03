#include <iostream>
#include <vector>
#include <string>

// KMP algoritması için en uzun ön ek - son ek (LPS) tablosunu oluşturur
std::vector<int> computeLPSArray(const std::string& pattern) {
    int m = pattern.length();
    std::vector<int> lps(m, 0);
    int len = 0; // Önceki en uzun ön ek son ek uzunluğu
    int i = 1;

    // Pattern boyunca ilerleyerek lps değerlerini hesaplar
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                // Eşleşme bozulduğunda önceki en uzun ön eke geri döner
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

// Knuth-Morris-Pratt (KMP) metin arama algoritması
void KMPMatch(const std::string& text, const std::string& pattern) {
    int n = text.length();
    int m = pattern.length();

    if (m == 0 || n < m) return;

    // LPS tablosunu hazırlar
    std::vector<int> lps = computeLPSArray(pattern);

    int i = 0; // Metin (text) indeksi
    int j = 0; // Kalıp (pattern) indeksi

    // Metni tek bir geçişte tarar
    while (i < n) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        if (j == m) {
            // Eşleşme bulur ve indeksi ekrana yazdırır
            std::cout << "Pattern " << (i - j) << ". indekste bulundu.\n";
            j = lps[j - 1];
        } else if (i < n && pattern[j] != text[i]) {
            // Eşleşmeyen durumda metin indeksini geri sarmadan j değerini kaydırır
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
}

int main() {
    std::string text = "ABABDABACDABABCABAB";
    std::string pattern = "ABABCABAB";

    std::cout << "Metin: " << text << "\n";
    std::cout << "Pattern: " << pattern << "\n\n";

    KMPMatch(text, pattern);

    return 0;
}