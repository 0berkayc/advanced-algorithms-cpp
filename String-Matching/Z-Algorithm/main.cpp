#include <iostream>
#include <string>
#include <vector>

std::vector<int> calculateZ(const std::string& S) {
    int n = S.length();
    std::vector<int> Z(n, 0);
    int L = 0, R = 0; // z-box sol ve sağ sınırları

    for (int i = 1; i < n; i++) {
        if (i > R) {
            // i, z-box'ın dışında 
            L = R = i;
            while (R < n && S[R - L] == S[R]) {
                R++;
            }
            Z[i] = R - L;
            R--;
        } else {
            // i, z-box'ın içinde. önceki hesaplamaları tekrar kullan
            int k = i - L;
            if (Z[k] < R - i + 1) {
                Z[i] = Z[k];
            } else {
                L = i;
                while (R < n && S[R - L] == S[R]) {
                    R++;
                }
                Z[i] = R - L;
                R--;
            }
        }
    }
    return Z;
}

// Z-Algorithm ile metin arama fonksiyonu
std::vector<int> zAlgorithmSearch(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    int m = pattern.length();
    int n = text.length();

    if (m == 0 || m > n) {
        return matches;
    }

    std::string S = pattern + "$" + text; // $ karakteri text ve patternde bulunmmamalı
    
    // Z-Array'i hesapla
    std::vector<int> Z = calculateZ(S);

    // Z değerlerini kontrol et
    for (int i = 0; i < Z.size(); i++) {
        // Z değeri pattern'in uzunluğuna tam eşitse eşleşme var demektir
        if (Z[i] == m) {
            // S içindeki indeksi (i), orijinal text indeksine dönüştür
            // i'den pattern uzunluğu (m) ve ayırıcı karakter (1) çıkarılır
            matches.push_back(i - m - 1);
        }
    }

    return matches;
}

int main() {
    std::string text = "kocaeli universitesi bilisim sistemleri muhendisligi";
    std::string pattern = "bilisim";

    std::vector<int> results = zAlgorithmSearch(text, pattern);

    if (results.empty()) {
        std::cout << "Aranan kalip metin icinde bulunamadi." << std::endl;
    } else {
        std::cout << "Eslasmenin bulundugu indeksler: ";
        for (int idx : results) {
            std::cout << idx << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}