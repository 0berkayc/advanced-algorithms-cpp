#include <iostream>
#include <vector>

int bestFit(const std::vector<int>& items, int binCapacity) {
    std::vector<int> bins;
    for (int i = 0; i < items.size(); i++)
    {
        int bestBinIndex = -1; // boş dönmesi için -1
        int minSpace = binCapacity + 1; //başta ulaşılamaz

        for (int j = 0; j < bins.size(); j++)
        {
            int remainingSpace = bins[j] - items[i];

            if (remainingSpace >= 0)
            {
                if (remainingSpace == 0)
                {
                    bestBinIndex = j; //güncel index
                    minSpace = 0;
                    break; //kalan yer 0 ise yerleştir ve döngüden çık
                }
                
                else if (remainingSpace < minSpace)
                {
                    minSpace = remainingSpace;
                    bestBinIndex = j;
                }
            }
        }

        if (bestBinIndex != -1)
        {
            bins[bestBinIndex] = bins[bestBinIndex] - items[i];
        }
        else
        {
            bins.push_back(binCapacity - items[i]);
        }
    }
    return bins.size();
}

int main() {
    std::vector<int> items = {3, 8, 2, 9, 7, 4, 1};
    int binCapacity = 10;

    int totalBins = bestFit(items, binCapacity);

    std::cout << "Kullanılan kutu sayısı: " << totalBins << "\n";

    return 0;
}