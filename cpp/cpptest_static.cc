#include <iostream>
#include <vector>
using namespace std;
struct test_t
{
    int a;
    int b;
    static int static_c; // = 8, connot initalizeon of non-const static member
    static const int static_d = 4;

    static vector<int> vec; //vec() this a function

};

int test_t::static_c = 3;  // no static

vector<int> test_t::vec(static_d);

const int test_t::static_d;  // no need to intilize any more

int main(int argc, const char *argv[])
{
    cout << sizeof(test_t) << endl;

    test_t t;

    cout << test_t::static_c << "  " << t.static_d << endl;
    return 0;
}
