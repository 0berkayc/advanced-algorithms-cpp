#include <iostream>
#include <vector>

// 2 boyutlu nokta yapısı
struct Point {
    int x, y;
};

// 3 noktanın yönelimini hesaplar
// 0: Doğrusal, 1: Saat Yönü (CW), 2: Saat Yönünün Tersi (CCW)
int orientation(Point p, Point q, Point r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

// Jarvis March Algoritması
std::vector<Point> jarvisMarch(const std::vector<Point>& points) {
    int n = points.size();
    std::vector<Point> hull;

    // En az 3 nokta gereklidir
    if (n < 3) return hull;

    // 1. ADIM: En soldaki noktayı bul
    int l = 0;
    for (int i = 1; i < n; i++) {
        if (points[i].x < points[l].x) {
            l = i;
        } else if (points[i].x == points[l].x && points[i].y < points[l].y) {
            l = i; // x'ler eşitse y'si küçük olanı seç
        }
    }

    // 2. ADIM: Etrafını paketleyerek dön
    int p = l;
    int q;
    do {
        // Mevcut köşeyi ekle
        hull.push_back(points[p]);

        // Geçici sonraki nokta adayı
        q = (p + 1) % n;

        // En dışta (en çok CCW) kalan noktayı ara
        for (int i = 0; i < n; i++) {
            if (orientation(points[p], points[i], points[q]) == 2) {
                q = i;
            }
        }

        p = q; // Sonraki köşeye geç
    } while (p != l); // Başlangıç noktasına dönünce bitir

    return hull;
}

int main() {
    std::vector<Point> points = {
        {0, 3}, {2, 2}, {1, 1}, {2, 1},
        {3, 0}, {0, 0}, {3, 3}
    };

    std::vector<Point> hull = jarvisMarch(points);

    std::cout << "Convex Hull köşe noktaları:\n";
    for (const auto& p : hull) {
        std::cout << "(" << p.x << ", " << p.y << ")\n";
    }

    return 0;
}