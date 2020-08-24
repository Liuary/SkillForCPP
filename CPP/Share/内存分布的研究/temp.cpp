#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    int a = 0;

    int b[]{1, 2, 3, 4, 5};

    int *p = &a;
    // p += 0x61fe14 + 3;

    p += 16;

    *p = 255;
    // int *q = p;

    cout << /**q << " " <<*/ a <<endl;

    return 0;
}
