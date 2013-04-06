#include <vector>
#include <iostream>
using namespace std;

typedef vector<int>::iterator vecintiter;

int main(int argc, const char *argv[])
{

    vector<int> vec;

    int i = 0;
    for (i = 0; i < 4; ++i)
        vec.push_back(i);

    vecintiter it1 = vec.begin() +  2;
    int *p1 = &vec[2];

    cout << "p1 befor insert:" << p1  << " " << *p1 << endl;

    for (; i < (1 << 25); ++i)
        vec.push_back(i);


    cout << *it1 << endl; // it's should not be safe, i should see stl source code...

    cout << "p1 after insert:" << p1  << " " << *p1 << endl;

    vecintiter it2 = vec.begin() + 2;
    int *p2 = &vec[2];

    cout << "p2 before erase:" << p2  << " " << *p2 << endl;

    cout << *it2 << endl;

    vec.erase(it2);

    cout << *it2 << endl; //返回3, but, still not safe, 要看iterator是怎么实现的,
                          //如果知识一个下标的话, 还是有效的吧，我想.

    cout << "p2 after erase:" << p2  << " " << *p2 << endl;

    for (; i < (1 << 25); ++i)
        vec.push_back(i);

    cout << "p2 after insert:" << p2  << " " << *p2 << endl;

    return 0;
}


// linux g++ 4.6
// output:
// p1 befor insert:0x98d7030 2
// 2
// p1 after insert:0x98d7030 2
// p2 before erase:0xa74a5010 2
// 2
// 3
// p2 after erase:0xa74a5010 3
// p2 after insert:0xa74a5010 3
//

// windows vc6.0
//
// p1 befor insert:00272600 2
// -572662307
// p1 after insert:00272600 -572662307  //这个变了
// p2 before erase:08500048 2
// 2
// 3
// p2 after erase:08500048 3
// p2 after insert:08500048 3  //这个没变
// Press any key to continue
//
// 不安全的终究是不安全哈...
