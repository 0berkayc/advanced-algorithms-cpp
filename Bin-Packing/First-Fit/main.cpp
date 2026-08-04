#include <iostream>
#include <vector>


int firstFit(const std::vector<int>& items, int binCapacity) {
    
    std::vector<int> bins; //açık kutuların kalan kapasiteleri

    for (int i = 0; i <= items.size() - 1; i++)
    {
        bool isPlaced = false; //nesne kutuya yerleştirildi mi kontrolü

        for (int j = 0; j < bins.size(); j++)
        {
            if (items[i] <= bins[j])
            {
                bins[j] = bins[j] - items[i]; //yerleştirilen kutudan nesne boyutunu eksilt
                isPlaced = true; 
                break;
            }
            
        }

        if (isPlaced == false)
        {
            bins.push_back(binCapacity - items[i]); //mevcut kutulara sığmadıysa yeni kutu oluştur
        }
    }
    return bins.size();
}

int main() {
    std::vector<int> items = {3, 8, 2, 9, 7, 4, 1};
    int binCapacity = 10;

    int totalBins = firstFit(items, binCapacity);

    std::cout << "Kullanılan kutu sayısı: " << totalBins << "\n";

    return 0;
}