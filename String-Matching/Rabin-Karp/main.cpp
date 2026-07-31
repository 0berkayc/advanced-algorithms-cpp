#include <iostream>
#include <string>
#include <vector>

// d: Alfabe boyutu (ASCII karakter seti icin 256)
// q: Modulo hesabi ve overflow'u onlemek icin kullanilan buyuk bir asal sayi
std::vector<int> rabinKarpSearch(const std::string& text, const std::string& pattern, int d = 256, int q = 101) {
    std::vector<int> matches;
    int n = text.length();
    int m = pattern.length();

    // pattern metinden uzunsa arama yapma
    if (m > n || m == 0) {
        return matches;
    }

    int pHash = 0; // pattern'in hash değeri
    int tHash = 0; // metindeki mevcut pencerenin hash değeri
    int h = 1;     // h = d^(m-1) % q katsayısı

    // h değerini hesapla: d^(m-1) % q
    for (int i = 0; i < m - 1; i++) {
        h = (h * d) % q;
    }

    // ilk pencere ve pattern'in ilk hash degerlerini hesapla
    for (int i = 0; i < m; i++) {
        pHash = (d * pHash + pattern[i]) % q;
        tHash = (d * tHash + text[i]) % q;
    }

    // metin uzerinde kayan pencere ile ilerle
    for (int i = 0; i <= n - m; i++) {

        // 1. hash değerleri eşleşiyor mu
        if (pHash == tHash) {
            bool match = true;
            // spurious Hit durumuna karşı karakter karakter doğrula
            for (int j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            // karakterler de eşleşiyorsa indexi kaydet
            if (match) {
                matches.push_back(i);
            }
        }

        // 2. bir sonraki pencerenin hash değerini hesapla (Rolling Hash)
        if (i < n - m) {
            // eski karakterin (text[i]) etkisini çıkar, yeni karakteri (text[i + m]) ekle
            tHash = (d * (tHash - text[i] * h) + text[i + m]) % q;

            // negatif modulo sonucunu pozitif yapma kontrolü
            if (tHash < 0) {
                tHash = tHash + q;
            }
        }
    }

    return matches;
}

int main() {
    std::string text = "kocaeli universitesi bilisim sistemleri muhendisligi";
    std::string pattern = "abc";

    std::vector<int> results = rabinKarpSearch(text, pattern);

    if (results.empty()) {
        std::cout << "Aranan kalıp metin içinde bulunamadı." << std::endl;
    } else {
        std::cout << "Eşleşmenin bulunduğu indexler: ";
        for (int idx : results) {
            std::cout << idx << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}