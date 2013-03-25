我以后写代码的时候要相当注意，所有的隐式转换都应该捕获到.
搞清楚到底发生了什么

这有些我还理解的不好, 以后必须注意.
摘自:
http://stackoverflow.com/questions/332030/when-should-static-cast-dynamic-cast-and-reinterpret-cast-be-used

static_cast is the first cast you should attempt to use. It does things like implicit conversions between types (such as int to float, or pointer to void*), and it can also call explicit conversion functions (or implicit ones). In many cases, explicitly stating static_cast isn't necessary, but it's important to note that the T(something) syntax is equivalent to (T)something and should be avoided (more on that later). A T(something, something_else) is safe, however, and guaranteed to call the constructor.

static_cast can also cast through inheritance hierarchies. It is unnecessary when casting upwards (towards a base class), but when casting downwards it can be used as long as it doesn't cast through virtual inheritance. It does not do checking, however, and it is undefined behavior to static_cast down a hierarchy to a type that isn't actually the type of the object.

const_cast can be used to remove or add const to a variable; no other C++ cast is capable of removing it (not even reinterpret_cast). It is important to note that using it is only undefined if the original variable is const; if you use it to take the const off a reference to something that wasn't declared with const, it is safe. This can be useful when overloading member functions based on const, for instance. It can also be used to add const to an object, such as to call a member function overload.

const_cast also works similarly on volatile, though that's less common.

dynamic_cast is almost exclusively used for handling polymorphism. You can cast a pointer or reference to any polymorphic type to any other class type (a polymorphic type has at least one virtual function, declared or inherited). You don't have to use it to cast downwards, you can cast sideways or even up another chain. The dynamic_cast will seek out the desired object and return it if possible. If it can't, it will return NULL in the case of a pointer, or throw std::bad_cast in the case of a reference.

dynamic_cast has some limitations, though. It doesn't work if there are multiple objects of the same type in the inheritance hierarchy (the so-called 'dreaded diamond') and you aren't using virtual inheritance. It also can only go through public inheritance - it will always fail to travel through protected or private inheritance. This is rarely an issue, however, as such forms of inheritance are rare.

reinterpret_cast is the most dangerous cast, and should be used very sparingly. It turns one type directly into another - such as casting the value from one pointer to another, or storing a pointer in an int, or all sorts of other nasty things. Largely, the only guarantee you get with reinterpret_cast is that if you cast the result back to the original type, you will get the exact same value. There are a number of conversions that reinterpret_cast cannot do, too. It's used primarily for particularly weird conversions and bit manipulations, like turning a raw data stream into actual data, or storing data in the low bits of an aligned pointer. Simply put, it is int32_t herp = 1337; float* derp = (float*)&herp; float magic = *derp; This is essentially how the fast inverse square root works.

C casts are casts using (type)object or type(object). A C-style cast is defined as the first of the following which succeeds:

    const_cast
    static_cast
    static_cast, then const_cast
    reinterpret_cast
    reinterpret_cast, then const_cast

It can therefore be used as a replacement for other casts in some instances, but can be extremely dangerous because of the ability to devolve into a reinterpret_cast, and the latter should be preferred when explicit casting is needed, unless you are sure static_cast will succeed or reinterpret_cast will fail. Even then, consider the longer, more explicit option.

C-style casts also ignore access control when performing a static_cast, which means that they have the ability to perform an operation that no other cast can. This is mostly a kludge, though, and in my mind is just another reason to avoid C-style casts.



implicit-cast
不了解, 先记下，以后回过头来看
http://stackoverflow.com/questions/868306/what-is-the-difference-between-static-cast-and-implicit-cast



类之间转换时候，怎么玩?

这篇文章总结了
Traditional explicit type-casting allows to convert any pointer into any other pointer type, independently of the types they point to. The subsequent call to member result will produce either a run-time error or a unexpected result.

In order to control these types of conversions between classes, we have four specific casting operators: dynamic_cast, reinterpret_cast, static_cast and const_cast. Their format is to follow the new type enclosed between angle-brackets (<>) and immediately after, the expression to be converted between parentheses.
http://www.cplusplus.com/doc/tutorial/typecasting/

为什么?
The second conversion in this piece of code would produce a compilation error since base-to-derived conversions are not allowed with dynamic_cast unless the base class is polymorphic.


reinterpret_cast converts any pointer type to any other pointer type, even of unrelated classes.


这篇文章做了概括:
http://tech.it168.com/a2011/0722/1221/000001221881_all.shtml

摘一些放在下面:
static_cast

类似于C风格的强制转换。无条件转换，静态类型转换。用于：
1. 基类和子类之间转换：其中子类指针转换成父类指针是安全的;但父类指针转换成子类指针是不安全的。(基类和子类之间的动态类型转换建议用dynamic_cast)
2. 基本数据类型转换。enum, struct, int, char, float等。static_cast不能进行无关类型(如非基类和子类)指针之间的转换。
3. 把空指针转换成目标类型的空指针。
4. 把任何类型的表达式转换成void类型。
5. static_cast不能去掉类型的const、volitale属性(用const_cast)。

const_cast
去掉类型的const或volatile属性。

dynamic_cast
有条件转换，动态类型转换，运行时类型安全检查(转换失败返回NULL)：

1. 安全的基类和子类之间转换。
2. 必须要有虚函数。
3. 相同基类不同子类之间的交叉转换。但结果是NULL。

reinterpreter_cast
仅仅重新解释类型，但没有进行二进制的转换; //没有进行二进制转换是什么意思?
1. 转换的类型必须是一个指针、引用、算术类型、函数指针或者成员指针。
2. 在比特位级别上进行转换。它可以把一个指针转换成一个整数，也可以把一个整数转换成一个指针(先把一个指针转换成一个整数，在把该整数转换成原类型的指针，还可以得到原先的指针值)。但不能将非32bit的实例转成指针。
3. 最普通的用途就是在函数指针类型之间进行转换。
4. 很难保证移植性。

总结:
去const属性用const_cast。
基本类型转换用static_cast。
多态类之间的类型转换用daynamic_cast。
不同类型的指针类型转换用reinterpreter_cast。
