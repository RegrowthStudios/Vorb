#include <Vorb/types.h>

int main() {
    // Create an array of 32-bit integers
    ArrayBase a(sizeof(i32));

    // Array has now ten elements
    a.setData(10);
    {
        // You can have copies of the array
        ArrayBase aCopy = a;
    }

    // b and a both point to the same data, but b has an explicit type associated with it
    Array<i32> b = a;
    for (size_t i = 0; i < b.size(); i++) b[i] = (i32)i;

    // Reset a's data, but b still points to original data
    a.setData(0);

    return b[0];
}
