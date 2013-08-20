
struct big_thing
{
    char buf[128];
};

struct big_thing return_test()
{
    struct big_thing b;
    b.buf[0] = 0;
    return b;
}


int main(int argc, const char *argv[])
{
    struct big_thing n;
    n.buf[0] = 1;
    n = return_test();

    return_test();
    return 0;
}

/* 结论: */
/* 如果函数的返回值被保存了, 如上面的第一个调用, 会直接把结果拷贝到n的地址上; */
/* 如果没有保存, 如上面的第二个调用, 会生成一个临时变量, 把结果保存到临时变量上. */

/* 那个书上讲的和我实验的结果不一样. */
/* 不知道是不是编译环境原因. */

/* 我是这么编译的 */
/* gcc -O0 return_test.c */
/* gcc version 4.7.2 (Ubuntu/Linaro 4.7.2-2ubuntu1)  */

/* 反汇编 */
/* objdump  -d a.out  */
/* 附main函数和return_test的反汇编输出 */

/* 080484ed <main>: */
 /* 80484ed:	8d 4c 24 04          	lea    0x4(%esp),%ecx */
 /* 80484f1:	83 e4 f0             	and    $0xfffffff0,%esp */
 /* 80484f4:	ff 71 fc             	pushl  -0x4(%ecx) */
 /* 80484f7:	55                   	push   %ebp */
 /* 80484f8:	89 e5                	mov    %esp,%ebp */
 /* 80484fa:	51                   	push   %ecx */
 /* 80484fb:	81 ec 34 01 00 00    	sub    $0x134,%esp */
 /* 8048501:	89 c8                	mov    %ecx,%eax */
 /* 8048503:	8b 40 04             	mov    0x4(%eax),%eax */
 /* 8048506:	89 85 64 ff ff ff    	mov    %eax,-0x9c(%ebp) */
 /* 804850c:	65 a1 14 00 00 00    	mov    %gs:0x14,%eax */
 /* 8048512:	89 45 f4             	mov    %eax,-0xc(%ebp) */
 /* 8048515:	31 c0                	xor    %eax,%eax */
 /* 8048517:	c6 85 74 ff ff ff 01 	movb   $0x1,-0x8c(%ebp) */
 /* 804851e:	8d 85 74 ff ff ff    	lea    -0x8c(%ebp),%eax    */
 /* 8048524:	89 04 24             	mov    %eax,(%esp)    //把变量n的地址放到栈里传给 return_test了 */
 /* 8048527:	e8 00 ff ff ff       	call   804842c <return_test> */
 /* 804852c:	83 ec 04             	sub    $0x4,%esp */
 /* 804852f:	8d 85 d8 fe ff ff    	lea    -0x128(%ebp),%eax */
 /* 8048535:	89 04 24             	mov    %eax,(%esp)    //这个地方有一个临时变量 */
 /* 8048538:	e8 ef fe ff ff       	call   804842c <return_test> */
 /* 804853d:	83 ec 04             	sub    $0x4,%esp */
 /* 8048540:	b8 00 00 00 00       	mov    $0x0,%eax */
 /* 8048545:	8b 55 f4             	mov    -0xc(%ebp),%edx */
 /* 8048548:	65 33 15 14 00 00 00 	xor    %gs:0x14,%edx */
 /* 804854f:	74 05                	je     8048556 <main+0x69> */
 /* 8048551:	e8 ba fd ff ff       	call   8048310 <__stack_chk_fail@plt> */
 /* 8048556:	8b 4d fc             	mov    -0x4(%ebp),%ecx */
 /* 8048559:	c9                   	leave   */
 /* 804855a:	8d 61 fc             	lea    -0x4(%ecx),%esp */
 /* 804855d:	c3                   	ret     */

/* 0804842c <return_test>: */
 /* 804842c:	55                   	push   %ebp */
 /* 804842d:	89 e5                	mov    %esp,%ebp */
 /* 804842f:	57                   	push   %edi */
 /* 8048430:	56                   	push   %esi */
 /* 8048431:	53                   	push   %ebx */
 /* 8048432:	81 ec ac 00 00 00    	sub    $0xac,%esp */
 /* 8048438:	8b 45 08             	mov    0x8(%ebp),%eax */
 /* 804843b:	89 85 54 ff ff ff    	mov    %eax,-0xac(%ebp) */
 /* 8048441:	65 a1 14 00 00 00    	mov    %gs:0x14,%eax */
 /* 8048447:	89 45 e4             	mov    %eax,-0x1c(%ebp) */
 /* 804844a:	31 c0                	xor    %eax,%eax */
 /* 804844c:	c6 85 64 ff ff ff 00 	movb   $0x0,-0x9c(%ebp) */
 /* 8048453:	8b 85 54 ff ff ff    	mov    -0xac(%ebp),%eax */
 /* 8048459:	8d 95 64 ff ff ff    	lea    -0x9c(%ebp),%edx */
 /* 804845f:	bb 80 00 00 00       	mov    $0x80,%ebx */
 /* 8048464:	89 c1                	mov    %eax,%ecx */
 /* 8048466:	83 e1 01             	and    $0x1,%ecx */
 /* 8048469:	85 c9                	test   %ecx,%ecx */
 /* 804846b:	74 0e                	je     804847b <return_test+0x4f> */
 /* 804846d:	0f b6 0a             	movzbl (%edx),%ecx */
 /* 8048470:	88 08                	mov    %cl,(%eax) */
 /* 8048472:	83 c0 01             	add    $0x1,%eax */
 /* 8048475:	83 c2 01             	add    $0x1,%edx */
 /* 8048478:	83 eb 01             	sub    $0x1,%ebx */
 /* 804847b:	89 c1                	mov    %eax,%ecx */
 /* 804847d:	83 e1 02             	and    $0x2,%ecx */
 /* 8048480:	85 c9                	test   %ecx,%ecx */
 /* 8048482:	74 0f                	je     8048493 <return_test+0x67> */
 /* 8048484:	0f b7 0a             	movzwl (%edx),%ecx */
 /* 8048487:	66 89 08             	mov    %cx,(%eax) */
 /* 804848a:	83 c0 02             	add    $0x2,%eax */
 /* 804848d:	83 c2 02             	add    $0x2,%edx */
 /* 8048490:	83 eb 02             	sub    $0x2,%ebx */
 /* 8048493:	89 d9                	mov    %ebx,%ecx */
 /* 8048495:	c1 e9 02             	shr    $0x2,%ecx */
 /* 8048498:	89 c7                	mov    %eax,%edi */
 /* 804849a:	89 d6                	mov    %edx,%esi */
 /* 804849c:	f3 a5                	rep movsl %ds:(%esi),%es:(%edi) */
 /* 804849e:	89 f2                	mov    %esi,%edx */
 /* 80484a0:	89 f8                	mov    %edi,%eax */
 /* 80484a2:	b9 00 00 00 00       	mov    $0x0,%ecx */
 /* 80484a7:	89 de                	mov    %ebx,%esi */
 /* 80484a9:	83 e6 02             	and    $0x2,%esi */
 /* 80484ac:	85 f6                	test   %esi,%esi */
 /* 80484ae:	74 0b                	je     80484bb <return_test+0x8f> */
 /* 80484b0:	0f b7 34 0a          	movzwl (%edx,%ecx,1),%esi */
 /* 80484b4:	66 89 34 08          	mov    %si,(%eax,%ecx,1) */
 /* 80484b8:	83 c1 02             	add    $0x2,%ecx */
 /* 80484bb:	83 e3 01             	and    $0x1,%ebx */
 /* 80484be:	85 db                	test   %ebx,%ebx */
 /* 80484c0:	74 07                	je     80484c9 <return_test+0x9d> */
 /* 80484c2:	0f b6 14 0a          	movzbl (%edx,%ecx,1),%edx */
 /* 80484c6:	88 14 08             	mov    %dl,(%eax,%ecx,1) */
 /* 80484c9:	8b 85 54 ff ff ff    	mov    -0xac(%ebp),%eax */
 /* 80484cf:	8b 55 e4             	mov    -0x1c(%ebp),%edx */
 /* 80484d2:	65 33 15 14 00 00 00 	xor    %gs:0x14,%edx */
 /* 80484d9:	74 05                	je     80484e0 <return_test+0xb4> */
 /* 80484db:	e8 30 fe ff ff       	call   8048310 <__stack_chk_fail@plt> */
 /* 80484e0:	81 c4 ac 00 00 00    	add    $0xac,%esp */
 /* 80484e6:	5b                   	pop    %ebx */
 /* 80484e7:	5e                   	pop    %esi */
 /* 80484e8:	5f                   	pop    %edi */
 /* 80484e9:	5d                   	pop    %ebp */
 /* 80484ea:	c2 04 00             	ret    $0x4 */
