#include <iostream>

class noncopyable
{
    protected:
        noncopyable() {}
        ~noncopyable() {}
    private:  // emphasize the following members are private
        noncopyable( const noncopyable& );
        const noncopyable& operator=( const noncopyable& );
};

class DontTreadOnMe : private noncopyable
{
    public:
        DontTreadOnMe() { std::cout << "defanged!" << std::endl; }
};   // DontTreadOnMe

class DontTreadOnMe2
{
    friend int main();
    public:
        DontTreadOnMe2() { std::cout << "defanged!" << std::endl; }
    private:
         DontTreadOnMe2(const DontTreadOnMe2&);
         DontTreadOnMe2& operator=(const DontTreadOnMe2&);

};   // DontTreadOnMe


int main()
{
    // DontTreadOnMe object1;
    // DontTreadOnMe object2(object1);
    // object1 = object2;

    DontTreadOnMe2 object21;
    DontTreadOnMe2 object22(object21);
    object21 = object22;

    return 0;
}   // main

