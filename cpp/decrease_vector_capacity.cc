#include <vector>
#include <iostream>
using namespace std;


int main(int argc, const char *argv[])
{

    vector<int> vec(6);

    cout << vec.size() << endl;
    cout << vec.capacity() << endl;

    vec.push_back(4);

    cout << vec.size() << endl;
    cout << vec.capacity() << endl;

    {
        vector<int>  tmp;
        vec.swap(tmp);
    }

    cout << vec.size() << endl;
    cout << vec.capacity() << endl;

    return 0;
}
