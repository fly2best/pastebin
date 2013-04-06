
#include <vector>
#include <iostream>
using namespace std;

int main(int argc, const char *argv[])
{

    vector<int> vec;

    int i = 0;
    int cnt = 0;

    bool nextout = true;

    while (cnt < 20) {

        if (nextout)
            cout << vec.size() << " " << vec.capacity() << endl;

        nextout = false;
        if (vec.size() == vec.capacity()) {
            nextout = true;
            ++cnt;
        }

        vec.push_back(i);

    }

    return 0;
}

// g++ 4.7 的输出,
// capacity指数级别增长, 变为以前的2倍, 如果先前为0, 插入一个元素后, capacity变为1
// 0 0
// 1 1
// 2 2
// 3 4
// 5 8
// 9 16
// 17 32
// 33 64
// 65 128
// 129 256
// 257 512
// 513 1024
// 1025 2048
// 2049 4096
// 4097 8192
// 8193 16384
// 16385 32768
// 32769 65536
// 65537 131072
// 131073 262144
