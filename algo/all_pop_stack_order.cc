
#include <stdio.h>
#include <assert.h>

const int N = 10;
int stack[N];
int top = 0;
int pop_order[N];

int pop_order_total_cnt = 0;

int Cnk(int n, int k);
void generate_all_pop_stack_order(int x, int *p, int pi);
void print_and_check(int *p, int pi);
bool check_pop_order(int *p, int n);

int main(int argc, const char *argv[])
{
    pop_order_total_cnt = 0;
    generate_all_pop_stack_order(1, pop_order, 0);
    printf("N:%d, cnt:%d , %d\n", N, pop_order_total_cnt, Cnk(2*N,N) - Cnk(2*N, N -1));
    return 0;
}


//input
// x表示下一个要进栈的数字
// p 保存了出栈序列
// pi 下一个出栈的数应该保存的位置
//
// 递归+回溯
// generate_all_pop_stack_order,
// 回溯: 函数执行完后不应该修改stack.
void generate_all_pop_stack_order(int x, int *p, int pi)
{
    if (top == 0 && x != N +1) {
        //只能进栈
        stack[top++] = x;
        generate_all_pop_stack_order(x + 1, p, pi);
        top--; //回溯
    } else if (x == N + 1) {

        print_and_check(p, pi);

    } else {

        int backup = stack[--top]; //for回溯
        //出栈
        p[pi] = backup;
        generate_all_pop_stack_order(x, p, pi + 1);
        stack[top++] = backup;  //回溯

        //进栈
        stack[top++] = x;
        generate_all_pop_stack_order(x+1, p, pi);
        top--; //回溯
    }
}

void print_and_check(int *p, int pi)
{

    //print
    int i;
    //取出栈的内容,但是不改动栈顶指针
    //栈顶指针应该在回溯的时候修改
    i = 1;
    while (pi < N) {
        assert(pi < N);
        p[pi++] = stack[top - i];
        i++;
    }

    for (i = 0; i < N;++i)
        printf("%d ", p[i]);
    printf("\n");

    //统计次数
    pop_order_total_cnt++;

    //检查
    if (!check_pop_order(p,N)) {
        fprintf(stderr, "some thing wrong!\n");

        for (i = 0; i < N;++i)
            fprintf(stderr, "%d ", p[i]);
        fprintf(stderr,"\n");
    }

}

// 判断p是否是进栈顺序为1-N时的一个出栈序列.
bool check_pop_order(int *pop_order, int n)
{
    int i = 0;
    int stk[N];
    int stk_top = 0;
    int next_push = 1;

    while(i != n ) {

        //1.栈为空, 或者栈顶的元素不等于pop_order,
        //2.有余下的元素没有进栈
        while (((stk_top == 0) || (stk[stk_top - 1] != pop_order[i])) && next_push <= N ) {
            stk[stk_top++] = next_push++;
        }

        //栈顶的元素,应该等于p[i], 否则不是一个出栈序列
        if (stk_top == 0 || stk[stk_top - 1] != pop_order[i]) {
            return false;
        }

        --stk_top;
        ++i;
    }

    return true;
}

// Cn(k)= Cn(k-1)*(n-k+1)/k;
int Cnk(int n, int k)
{
    int ret = 1;

    for (int i = 1 ; i <= k; ++i) {
        ret = ret * (n - i + 1) / i;
    }
    return ret;
}
