// 2023/3/13
// zhanzhong

#include "tabular/rod_cutting.h"
#include <assert.h>
#include <stdlib.h>

void test_rod_cutting() {
    int prices[] = {
        0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30,
    };
    const int size = sizeof(prices) / sizeof(int);

    assert(rod_cutting(prices, size, 1) == 1);
    assert(rod_cutting(prices, size, 2) == 5);
    assert(rod_cutting(prices, size, 3) == 8);
    assert(rod_cutting(prices, size, 4) == 10);
    assert(rod_cutting(prices, size, 5) == 13);
    assert(rod_cutting(prices, size, 6) == 17);
    assert(rod_cutting(prices, size, 7) == 18);
    assert(rod_cutting(prices, size, 8) == 22);
    assert(rod_cutting(prices, size, 9) == 25);
    assert(rod_cutting(prices, size, 10) == 30);
}

int main() {
    test_rod_cutting();
    return 0;
}
