#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
using std::swap;

struct Node{
    int data;
    Node * next;

    Node(int data = -1): data(data), next(NULL) {}
};


void init_list(Node * &pHead, int n);
void print_list(Node *pHead);
void merge(Node *pA, Node *pB, Node * pC);
void list_sort(Node *pHead);
int main(int argc, const char *argv[])
{
    int n = 10;
    Node *pHead1;
    Node *pHead2;

    init_list(pHead1, n);
    print_list(pHead1);

    // init_list(pHead2, n);
    // print_list(pHead2);

    // Node *pMerged;

    // merge(pHead1, pHead2, pMerged);
    // print_list(pMerged);
    //
    list_sort(pHead1);
    print_list(pHead1);

    return 0;
}

//链表的归并排序
//这个时间复杂度的证明
void list_sort(Node *pHead)
{
    Node* buffer[64];
    for(int i = 0; i < sizeof(buffer) / sizeof(buffer[0]); ++i) {
        buffer[i] = new Node();
    }

    Node *carry = new Node();
    Node* tmp = new Node();

    Node *next = pHead->next;
    pHead->next = NULL;

    while ( next != NULL) {

        carry->next = next;
        next = next->next;
        carry->next->next = NULL;

        int i = 0;

        while (i < 63 && buffer[i]->next != NULL) {

            merge(buffer[i], carry, tmp);
            swap(carry, tmp);
            ++i;
        }
        swap(buffer[i], carry);
    }

    for(int i = 0; i < 64; ++i) {
        merge(buffer[i], pHead, tmp);
        swap(pHead->next, tmp->next);
    }
}


/**
 * merge pA, pB to pC
 */
void merge(Node *pA, Node *pB, Node * pC)
{

    Node *pNext1 = pA->next;
    Node *pNext2 = pB->next;

    pA->next = NULL;
    pB->next = NULL;

    Node *pTail = pC;

    while (pNext1 != NULL && pNext2 != NULL) {

        if ( pNext1->data < pNext2->data) {
            pTail->next = pNext1;
            pNext1 = pNext1->next;
        } else {
            pTail->next = pNext2;
            pNext2 = pNext2->next;
        }

        pTail = pTail->next;
        pTail->next = NULL;
    }

    if (pNext1) {
        pTail->next = pNext1;
    }

    if (pNext2) {
        pTail->next = pNext2;
    }
}



void init_list(Node * &pHead, int n)
{
    pHead = new Node;

    for(int i = 0; i < n; ++i) {
        int data = rand()%n;
        // data = n - i;
        Node *pNew = new Node(data);
        pNew->next = pHead->next;
        pHead->next = pNew;
    }
}


void print_list(Node *pHead)
{
    Node *pNext = pHead->next;
    while (pNext != NULL) {
        printf("%d " , pNext->data);
        pNext = pNext->next;
    }
    printf("\n");
}
