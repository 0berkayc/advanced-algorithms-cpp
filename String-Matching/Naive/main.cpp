#include <iostream>
#include <string>
#include <vector>

std::vector<int> naiveSearch(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    int n = text.length();
    int m = pattern.length();

    for (int i = 0; i <= n - m; i++) {
        int j = 0;

        while (j < m && text[i + j] == pattern[j]) {
            j++;
        }

        if (j == m) {
            matches.push_back(i);
        }
    }
    return matches;
}

int main() {
    std::string text = "kocaeli universitesi bilisim sistemleri mühendisligi";
    std::string pattern = "bilisim";

    std::cout << "=== NAIVE STRING MATCHING ===" << std::endl;
    std::cout << "Metin: " << text << std::endl;
    std::cout << "Aranan Pattern: " << pattern << std::endl;

    std::vector<int> results = naiveSearch(text, pattern);

    std::cout << "Eşleşmenin bulunduğu indexler: ";
    for (int idx : results) {
        std::cout << idx << " ";
    }
    std::cout << std::endl;

    return 0;
}