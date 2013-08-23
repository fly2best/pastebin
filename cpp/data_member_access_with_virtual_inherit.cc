
#include <stdio.h>

struct A {
    int x;
};

struct B :  virtual public A{
    int y;
};

struct C : virtual public B {
    int z;
};



int main(int argc, const char *argv[])
{
    C c;
    c.x = 0;
    c.y = 1;
    c.z = 2;

    C * pc = &c;

    pc->x = 0;
    pc->y = 1;
    pc->z = 2;

    return 0;
}

// 804851c:	55                   	push   %ebp
// 804851d:	89 e5                	mov    %esp,%ebp
// 804851f:	83 e4 f0             	and    $0xfffffff0,%esp
// 8048522:	83 ec 30             	sub    $0x30,%esp
// 8048525:	8d 44 24 1c          	lea    0x1c(%esp),%eax
// 8048529:	89 04 24             	mov    %eax,(%esp)
// 804852c:	e8 7b 00 00 00       	call   80485ac <_ZN1CC1Ev>
// 8048531:	c7 44 24 2c 00 00 00 	movl   $0x0,0x2c(%esp)
// 8048538:	00 
// 8048539:	c7 44 24 28 01 00 00 	movl   $0x1,0x28(%esp)
// 8048540:	00 
// 8048541:	c7 44 24 20 02 00 00 	movl   $0x2,0x20(%esp)
// 8048548:	00 
// 8048549:	8d 44 24 1c          	lea    0x1c(%esp),%eax
// 804854d:	89 44 24 18          	mov    %eax,0x18(%esp)
// 8048551:	8b 44 24 18          	mov    0x18(%esp),%eax
// 8048555:	8b 00                	mov    (%eax),%eax
// 8048557:	83 e8 10             	sub    $0x10,%eax
// 804855a:	8b 00                	mov    (%eax),%eax
// 804855c:	89 c2                	mov    %eax,%edx
// 804855e:	8b 44 24 18          	mov    0x18(%esp),%eax
// 8048562:	01 d0                	add    %edx,%eax
// 8048564:	c7 00 00 00 00 00    	movl   $0x0,(%eax)
// 804856a:	8b 44 24 18          	mov    0x18(%esp),%eax
// 804856e:	8b 00                	mov    (%eax),%eax
// 8048570:	83 e8 0c             	sub    $0xc,%eax
// 8048573:	8b 00                	mov    (%eax),%eax
// 8048575:	89 c2                	mov    %eax,%edx
// 8048577:	8b 44 24 18          	mov    0x18(%esp),%eax
// 804857b:	01 d0                	add    %edx,%eax
// 804857d:	c7 40 04 01 00 00 00 	movl   $0x1,0x4(%eax)
// 8048584:	8b 44 24 18          	mov    0x18(%esp),%eax
// 8048588:	c7 40 04 02 00 00 00 	movl   $0x2,0x4(%eax)
// 804858f:	b8 00 00 00 00       	mov    $0x0,%eax
// 8048594:	c9                   	leave  
// 8048595:	c3                   	ret    
