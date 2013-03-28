
匿名命名空间
-----------

C++ 新的标准中提倡使用匿名命名空间,而不推荐使用static.

因为static用在不同的地方,涵义不同,容易造成混淆.

另外,static不能修饰class 的定义.

Static only applies to names of objects, functions, and anonymous unions, not to type declarations.

参考:
http://stackoverflow.com/questions/154469/unnamed-anonymous-namespaces-vs-static-functions
http://blog.csdn.net/fxpbupt/article/details/5600937

