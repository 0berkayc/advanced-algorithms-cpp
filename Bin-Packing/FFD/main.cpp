#include <iostream>
#include <vector>
#include <algorithm> // std::sort için

int firstFitDecreasing(const std::vector<int>& items, int binCapacity) {
    // 1. Orijinal veriyi korumak için kopyasını al
    std::vector<int> sortedItems = items;

    // 2. Nesneleri büyükten küçüğe sırala (Azalan Sıra)
    std::sort(sortedItems.begin(), sortedItems.end(), std::greater<int>());

    // 3. Sıralanmış liste üzerinde Standart First-Fit uygula
    std::vector<int> bins; // Açık kutuların kalan kapasiteleri

    for (int i = 0; i < sortedItems.size(); i++) {
        bool isPlaced = false;

        // Mevcut kutuları soldan sağa tara
        for (int j = 0; j < bins.size(); j++) {
            if (sortedItems[i] <= bins[j]) {
                bins[j] = bins[j] - sortedItems[i];
                isPlaced = true;
                break; // Sığan ilk kutuya kondu
            }
        }

        // Hiçbir kutuya sığmadıysa yeni kutu aç
        if (!isPlaced) {
            bins.push_back(binCapacity - sortedItems[i]);
        }
    }

    return bins.size();
}

int main() {
    //first-fit sonucu 4, ffd beklenen sonuç 3 
    std::vector<int> items = {2, 5, 4, 7, 1, 3, 8};
    int binCapacity = 10;

    int totalBins = firstFitDecreasing(items, binCapacity);

    std::cout << "Kullanılan Toplam Kutu Sayısı (FFD): " << totalBins << "\n";

    return 0;
}