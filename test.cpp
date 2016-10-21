#include <stdlib.h>
#include <cmath>
#include <iostream>

void a(int &i) {
    for (int j=0; j<3; ++j) {
        i+=1;
    }
}

void b(int &i) {
    if (i / 9 % 5 == 0) i+=1;
    i-=2;
}

void c(int &i) {
    if ((i+9) % 16 == 0) i-=1;
    i+=2;
    for (int j=0; j<3; ++j) {
        i -= 1;
    }
}

int main() {
    int i = 0;
    int j = 0;
    int k = 0;
    for (; i<5000000000; ++i, ++j, ++k) {
        a(i);
        b(j);
        c(k);
        a(j);
        if (i%2000==0) c(i);
    }
    
    std::cout << i;
}
