
面向对象的3个特征
-----------------
数据抽象, 继承， 动态绑定(多态).
有时也会加一个封装.

访问权限
--------
派生类只能通过派生类对象访问其基类的protected成员.
派生类对其基类型对象的protected成员没有特殊访问权限.


派生类的声明, 不能包含派生列表.
-----------------------------


虚函数使用需要注意的地方
------------------------
*   只有通过引用或指针调用，虚函数才在运行时确定。只有在这些情况下，知道运行时才知道对象的动态类型.
*   虚函数一经声明则始终是虚函数, 派生类在声明的时候可以使用virtual关键字，也可以不使用.
*   基类的析构函数应该定义为虚函数,
    so, 如果delete 基类指针时，如果基类指针指向派生类，则会调用派生类的析构函数.

*   如果在构造函数和析构函数的调用虚函数, 调用的是自个的版本.
    因为在构造函数中或者析构函数中,对象是不完整的, 派生类对象是不完整的，调用派生类的函数很可能出问题.

*   虚函数的默认参数是个坑.
    通过哪个类的指针调用，则用哪一个类的默认参数。
    如果使用了基类指针，则使用基类默认参数，调用子类函数, 可能会出问题的，亲...


名字查找与继承
-------------
*   确定进行函数调用的对象，引用或指针的静态类型.
*   在该类中查找函数, 如果找不到, 就在基类中查找, 如此循着类的继承链往上查找，知道找到该函数或者最后一个类.
    如果找不到，则调用是错误的, 报编译错误.
*   一旦找到了该名字, 就进行常规的类型检查, 查看如果给定找到的定义，改函数调用是否合法.
*   假定函数的调用是合法的, 编译器就生成代码. 如果函数是虚函数且通过引用或指针调用, 则编译器生成代码以确定
    根据对象的动态类型运行函数的哪个版本，否则编译器生成代码直接调用函数.

so   
派生类函数调用基类版本时，必须显示使用作用域操作符.  
如果派生类函数没有使用域操作符，则函数调用会在运行时确定并且将是一个自身的递归调用.

so   
派生类中虚函数的声明必须与基类中的定义方式完全匹配，//匹配是什么意思   
但有一个例外, 返回对基类型的引用(或指针)的虚函数, 派生类可以返回基类函数所返回类型的派生类对象.


访问权限与继承方式
------------------
公有继承，私有继承，protected 继承.

基类的private成员, 只有基类及其友元可以访问.
与派生类的继承方式没有关系.

基类的public 成员和 protected成员和继承方式的关系:

public 继承: 不变

protected继承: 全是protected

private继承: 全是为私有的

friendship 不被继承.
是老子的朋友，但不是儿子的朋友.

初始化顺序
----------
构造函数的初始化列表只提供了初始化数据, 不确定初始化次序。
基类的构造函数先被调用, 然后成员的初始化顺序是由成员声明的顺序决定的.

派生类的复制控制
----------------
应该调用基类的复制构造函数, 和赋值操作符.

for example:

    class Base { /* ... */ };
    class Derived: public Base {
        public:
            // Base::Base(const Base&) not invoked automatically
            Derived(const Derived& d):
                Base(d) /* other member initialization */ { /*... */ }
    };

    // Base::operator=(const Base&) not invoked automatically
    Derived &Derived::operator=(const Derived &rhs)
    {
        if (this != &rhs) {
            Base::operator=(rhs); // assigns the base part //注意其调用方式,挺少用的.
            // do whatever needed to clean up the old value in the derived part
            // assign the members from the derived
        }
        return *this;
    }





纯虚函数与抽象基类
----------
纯虚函数， 加 = 0;
防止类被实例化.

句柄类
----------
因为c++, 不支持通过对象进行动态绑定, 因此会一般会实现句柄类.

在句柄类保存一个基类的指针, 同过基类指针实现多态.

在具体实现的时候, 根据是否想用户屏蔽继承层次, 有两种不同句柄设计方法:

1.  重载句柄类的-> 和 * 操作符, 在句柄类上的调用，被转变为在基类指针上的调用, 从而实现多态.
2.  提供接口函数, 用户通过句柄接口函数简介使用背后的类.

这两种方式, 会影响构造函数的方法.

在1中的问题, 是怎么拷贝未知类型对象, 不知道是基类还是派生类.  
解决方法通过虚函数clone.

在2中用户不直接访问类层次结构, 所以其构造函数是私有的或protect的.  
为访问其构造函数, 须把句柄类设为友元.


不想细看...
----------------------------------------------
If the inheritance is public , then both user code and member functions of subsequently derived
classes may use the derived-to-base conversion. If a class is derived using private or protected
inheritance, then user code may not convert an object of derived type to a base type object. If
the inheritance is private , then classes derived from the private ly inherited class may not
convert to the base class. If the inheritance is protected , then the members of subsequently
derived classes may convert to the base type.
Regardless of the derivation access label, a public member of the base class is accessible to the
derived class itself. Therefore, the derived-to-base conversion is always accessible to the
members and friends of the derived class itself.
