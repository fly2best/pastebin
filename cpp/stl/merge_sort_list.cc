
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
