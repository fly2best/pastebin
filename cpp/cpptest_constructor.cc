
#include <iostream>
using namespace std;

class A
{
    public:
        A() {cout << "default constracuor" << endl;}
        A(const A& other) {cout << "copy constructor" << endl;}
        A& operator=(const A& other) {cout << "operator =" << endl; return *this; }
        explicit A(int a) {cout << a << ":" << "converted from int" << endl;}
};


int main(int argc, const char *argv[])
{
    A a= A();
    a = A();
    A a1 = a;
    A a2 = A(5);
    a = A(6);
    a = a1 = a2;
    return 0;
}
