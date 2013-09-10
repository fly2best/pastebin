
// stl中list的排序使用的归并排序.
// 很好的链式归并排序的例子
template <class T, class Alloc>
void list<T, Alloc>::sort() {
    if (node->next == node || link_type(node->next)->next == node) return;

    list<T, Alloc> carry;
    //最多可以对2^64-1个元素排序
    list<T, Alloc> counter[64];
    int fill = 0;

    //满足循环不变式
    //1. carry为空
    //2. counter[fill] 大小为空
    //3. 对于counter[i] 要么2^i个元素(排好序的)要么为空
    while (!empty()) {
        //取一个元素
        carry.splice(carry.begin(), *this, begin());
        int i = 0;
        while(i < fill && !counter[i].empty()) {
            //归并排序
            counter[i].merge(carry);
            carry.swap(counter[i++]);
        }
        carry.swap(counter[i]);         
        if (i == fill) ++fill;
    } 

    for (int i = 1; i < fill; ++i) 
        counter[i].merge(counter[i-1]);
    swap(counter[fill-1]);
}

// 时间复杂度分析:
// 结果肯定是O(nlogn), 但怎么算出来的呢.

// 对于每个counter, 从满变成空的时候, 会有一次merge.
// 现在要分析的就是
// 1. 从满变成空，多少次?
// 2. 每次merge的时候, 时间复杂的是多少?

// 问题2很容易回答, 对于第i个counter, 2^i个元素, 加上carray中的2^i个元素, merge的时间复杂度为O(2^(i+1))
// 问题1呢, 对于第i个couter, 从满变成空的次数为n/(2^(i+1))次(设n为2^k)

// 如此, 对于第i个couter的所花费的总时间为O(n/2^(i+1)) * O(2^(i+1))
// 总共有O(logn)个counter
// 因此时间复杂度为O(nlogn)
//
// 汇聚涓涓细流
