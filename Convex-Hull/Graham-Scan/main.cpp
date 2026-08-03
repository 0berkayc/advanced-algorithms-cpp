#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

// 2 boyutlu nokta yapısı
struct Point {
    int x, y;
};

// Referans pivot nokta
Point p0;

// İki nokta arasındaki mesafenin karesini hesaplar
int distSq(Point p1, Point p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

// 3 noktanın yönelimini hesaplar
// 0: Doğrusal, 1: Saat Yönü (CW), 2: Saat Yönünün Tersi (CCW)
int orientation(Point p, Point q, Point r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

// Noktaları kutupsal açıya göre sıralamak için karşılaştırma fonksiyonu
bool compare(Point p1, Point p2) {
    int o = orientation(p0, p1, p2);
    if (o == 0) {
        return distSq(p0, p2) >= distSq(p0, p1);
    }
    return (o == 2);
}

// Stack üzerindeki ikinci elemanı döndürür
Point nextToTop(std::stack<Point>& st) {
    Point p = st.top();
    st.pop();
    Point res = st.top();
    st.push(p);
    return res;
}

// Graham Scan Algoritması
std::vector<Point> grahamScan(std::vector<Point>& points) {
    int n = points.size();
    std::vector<Point> hull;

    if (n < 3) return hull;

    // En alt (y'si en küçük) noktayı bulur
    int ymin = points[0].y, min = 0;
    for (int i = 1; i < n; i++) {
        int y = points[i].y;
        if ((y < ymin) || (ymin == y && points[i].x < points[min].x)) {
            ymin = points[i].y;
            min = i;
        }
    }

    // En alt noktayı ilk sıraya taşır
    std::swap(points[0], points[min]);
    p0 = points[0];

    // Diğer noktaları p0 noktasına göre kutupsal açıyla sıralar
    std::sort(points.begin() + 1, points.end(), compare);

    // Aynı açıya sahip noktaları temizler, en uzaktakini tutar
    int m = 1;
    for (int i = 1; i < n; i++) {
        while (i < n - 1 && orientation(p0, points[i], points[i + 1]) == 0) {
            i++;
        }
        points[m] = points[i];
        m++;
    }

    if (m < 3) return hull;

    // İlk 3 noktayı yığına ekler
    std::stack<Point> st;
    st.push(points[0]);
    st.push(points[1]);
    st.push(points[2]);

    // Kalan noktaları tarar, sağa dönüşleri çıkarır
    for (int i = 3; i < m; i++) {
        while (st.size() > 1 && orientation(nextToTop(st), st.top(), points[i]) != 2) {
            st.pop();
        }
        st.push(points[i]);
    }

    // Yığındaki noktaları vektöre aktarır
    while (!st.empty()) {
        hull.push_back(st.top());
        st.pop();
    }

    // Noktaları doğru sırada almak için ters çevirir
    std::reverse(hull.begin(), hull.end());

    return hull;
}

int main() {
    std::vector<Point> points = {
        {0, 3}, {1, 1}, {2, 2}, {4, 4},
        {0, 0}, {1, 2}, {3, 1}, {3, 3}
    };

    std::vector<Point> hull = grahamScan(points);

    std::cout << "Convex Hull Köşe Noktaları:\n";
    for (const auto& p : hull) {
        std::cout << "(" << p.x << ", " << p.y << ")\n";
    }

    return 0;
}