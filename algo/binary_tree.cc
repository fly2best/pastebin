#include <iostream>
#include <stack>
using namespace std;

struct Node
{
    int data;
    Node *pLeft;
    Node *pRight;
};

int a[] = {1, 2, 4, 0, 0, 5, 6, 0, 0, 7, 0, 8, 0, 0, 3, 0, 0};
int ai = 0;

void build(Node* &pRoot)
{
    if (a[ai] == 0) {
        ai++;
        pRoot = NULL;
        return;
    }

    pRoot = new Node();
    pRoot->data = a[ai++];
    build(pRoot->pLeft);
    build(pRoot->pRight);
}

void pretraverse_r(Node *pRoot)
{
    if (pRoot == NULL)
        return ;

    cout << pRoot->data << " ";
    pretraverse_r(pRoot->pLeft);
    pretraverse_r(pRoot->pRight);
}


void pretraverse(Node *pRoot)
{
    if (pRoot == NULL)
        return;
    stack<Node*> stk;

    stk.push(pRoot);
    cout << pRoot->data << " ";

    while (!stk.empty()) {
        Node *p = stk.top();

        while (p->pLeft) {
            cout << p->pLeft->data << " ";
            stk.push(p->pLeft);
            p = p->pLeft;
        }

        stk.pop();
        while (!stk.empty() && !p->pRight) {
            p = stk.top();
            stk.pop();
        }

        if (p->pRight) {
            cout << p->pRight->data << " ";
            stk.push(p->pRight);
        }
    }

    cout << endl;
}

void midtraverse(Node *pRoot)
{
    if (pRoot == NULL)
        return;
    stack<Node*> stk;

    stk.push(pRoot);

    while (true) {
        Node *p = stk.top();
        while (p) {
            stk.push(p->pLeft);
            p = p->pLeft;
        }
        stk.pop();

        //why would it be empty?
        //从左子树返回的时候, 得到的父结点
        //从右子树返回的时候, 父结点已经不在了, 得到是一个祖先结点.
        //但是祖先结点不一定是存在的，比如在level 为1结点(level 从0开始).
        //这个祖先结点是谁? 就是按照中序遍历下一个要访问的结点.
        //所以要加判断是否为空
        //       1
        //      / \
        //     2   0   //0表示空
        //    / \
        //   0   0
        //当3的右子树空结点出栈的时候,栈已经空了,
        if (stk.empty())
            break;

        p = stk.top();
        cout << p->data << " ";
        stk.pop();
        stk.push(p->pRight);
    }

    cout << endl;
}

void posttraverser(Node *pRoot)
{
    if (pRoot == NULL)
        return;

    stack<Node*> stkNode;
    stack<bool>  stkIsRight;

    stkNode.push(pRoot);
    stkIsRight.push(false); //须初始化为false

    while (true) {

        Node *p = stkNode.top();

        while (p) {
            p = p->pLeft;
            stkNode.push(p);
            stkIsRight.push(false);
        }

        stkNode.pop();
        bool flag = stkIsRight.top();
        stkIsRight.pop();

        while (flag) {
            p = stkNode.top();

            cout << p -> data << " ";

            stkNode.pop();
            flag = stkIsRight.top(); //根结点出栈的时候，flag为false
            stkIsRight.pop();
        }

        //这是循环终止条件
        //上一个出栈的结点是其父结点的左儿子, 下一个要访问的结点就是其父节点
        //如果父节点不存在呢?比如根结点.终止呗.
        if (stkNode.empty()) {
            break;
        }

        //p是什么 ?
        //p刚出栈结点的父节点
        p = stkNode.top();

        stkNode.push(p->pRight);
        stkIsRight.push(true);
    }

    cout << endl;
}

int main(int argc, const char *argv[])
{
    Node *pRoot;
    build(pRoot);
    pretraverse_r(pRoot);
    cout << endl;
    pretraverse(pRoot);
    midtraverse(pRoot);

    posttraverser(pRoot);
    return 0;
}
