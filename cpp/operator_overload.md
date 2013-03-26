
重载操作符的特性
---------------

重载操作符的个数是确定的。

不能重载的操作符   //想想为什么这么设计  
:: , *, ?:

重载操作符必须具有一个类类型参数。  
重载操作符必须有至少一个类类型或枚举类型的操作数。这条规则强制重载操作父不能重新定义用于内置类型对象的操作符定义.

优先级和结合性是固定的。

不在具备短路求值的特性。

类成员与非成员。

如果作为成员函数，则有一个默认的this指针作为第一个参数。

如果把操作符定义为非成员函数，通常必须将他们设置为所操作类的友元因为要访问类的私有成员。

重载操作符的使用:
*   像内置类型操作数上使用操作符的方式一样。
*   使用operator+(a, b) , 像函数调用一样并传递适当类型数目的形式参数。


重载操作符设计的准则
--------------------

*   不要重载内置含义的操作符， 如逗号，取地址，逻辑与，逻辑或.
    重载逻辑与或逻辑或，将失去短路求值的特性。

*   可以考虑重载的
    加: +,
    赋值: +=, =
    关系: == , !=, >, < , >=, <=
    输入输出: >> , <<
    下标: []
    调用: ()
    成员访问: ->
    解引用: *
    自增，自减: ++, --

*   成员or非成员

    *   =, [], (), -> 必须是成员;  
        //a-> , a =, a(), a[]. 
        //都是一元操作符, 使用的时候, 操作数在左, 操作符在右.
    *   复合赋值一般也定义为类成员;
    *   与类关系密切联系的其他操作符, ++, --, *(解引用), 一般定义类成员.

    *   对称的操作符一般定义为普通的非成员函数.
        二元


类操作符重载举例:

*   输出操作符, <<
    ostream& operator<<(ostream& out, const Sales_item& s)
    {
        out << s.isbn << "\t" << s.units_sold << "\t";
        return out;
    }

    特点: 尽量减少格式化, 返回类型, 必须为非成员函数.

    如果定义为成员函数, 使用起来就蛮怪。
        Sales item;
        item << cout;   //cout << item;

*   输入操作符, >>
    istream& operator>>(istream& in, Sales_item& s)
    {
        double price;
        in >> s.bin >> s.unites_sold >> price;
        if (in)
            // do something
         else
             s = Sales_item(); // input failed: reset object to default state
        return in;
    }

    特点, 必须处理错误和文件结束的可能性.

*   算术操作符和关系操作符
    一般而言，将算术和关系操作符定义为非成员成员函数。
    Sales_item operator+(const Sales_item& lhs, const Sales_item& rhs)
    {
        Sales_item ret(lhs);  
        ret += rhs;
        return ret;
    }
    attention: 为了与内置操作符保持一致,加法返回一个右值，而不是一个引用.

*   相等操作符
    *   == , 包含了相同的数据.
    *   !=, == 同时出现,  其中一个通过另外一个来实现。

*   关系操作符
    特点: ==， <, 一般定于为非成员函数.

*   赋值操作符

    class string {
    public:
        // 注意到没，可以有重载哈~~
        string& operator=(const string&);
        string& operator=(const char *);
        string& operator=(char *);
    }
    一般而言，赋值操作符和复合赋值操作符返回对*this的引用.
    a = b = c;

*   下标操作符
    提供读写访问, 一般定义两个版本，一个为非const成员并返回引用
    一个为const成员并返回const引用.
    const 对象使用const版本的，非const对象使用非const版本的.

*   成员访问操作符
    为了支持指针型类,如迭代器, C++允许重载解引用操作符*和箭头操作符->

    *, 必须声明为成员; ->一般声明为成员.

    Screen &operator*() { return *ptr->sp; }
    Screen *operator->() { return ptr->sp; }
    const Screen &operator*() const { return *ptr->sp; }   //const
    const Screen *operator->() const { return ptr->sp; }   //const

    p->action();
    (p->action)();
    (p.operator->()->action)();

*   自增操作符和自减操作符
    迭代器, 实现一般用到.
    一般作为类的成员, 因为和类的关系密切, 但不做强制要求.

    用一个int类型参数来区别前自增还是后自增.

    CheckedPtr operator++(int);     // postfix operators
    CheckedPtr operator--(int);

    CheckedPtr& operator++();       // prefix operators
    CheckedPtr& operator--();

*   调用操作符'()'和函数对象
    调用操作符必须声明为成员函数.
    定义了操作符的类，其对象常称为函数对象(function object).

    函数对象可以比函数更灵活.
    可以在定义的时候初始化一些内部状态，而不是作为参数传递.

    函数对象常用于覆盖算法使用的默认操作符.

*   转换与类类型

    构造函数进行隐式类型转化.

    转换函数必须是成员函数, 不能指定返回类型，并且形参必须为空.
    一般声明为const.

    //采用下面这种通用格式
    operator type();

    使用的时候，编译器自动转化，但一次只隐式类类型转化一次.

    标准转换可放在类类型转化以前.

    void calc(SmallInt);
    short obj;
    calc(obj); short->int->SmallInt


重载函数的匹配
-------------
如果重载集中的两个函数可以用通一转换函数匹配，
则使用在转换之后或之前的标准转换序列的等级来确定哪个函数具有最佳匹配.

否则，如果可以使用不同的转换操作，则认为这两个转换是一样好的匹配，不管需要或不需要的标准转换的等级如何.
