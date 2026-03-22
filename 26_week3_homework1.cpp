#include <iostream>
using namespace std;

struct Num {
    int data;
    Num* next;
    Num();
    Num(int data);
    Num(int data, Num* front); // 숙제1
    void setNext(Num* node);
};

Num::Num(int data) {
    this->data = data;
    this->next = nullptr;
}

// 숙제1 노드 뒤에 새 노드를 삽입하는 생성자
Num::Num(int data, Num* front) {
    this->data = data;
    if (front != nullptr) { // 원래 front가 가리키던 다음 노드를 내가 가리키게 함
        this->next = front->next; // front가 나를 가리키게 함
        front->next = this;
    }
    else {
        this->next = nullptr;
    }
}

void Num::setNext(Num* node) {
    this->next = node;
}

// 숙제2 리스트의 노드 개수 세기
int countNext(Num* header) {
    int count = 0;
    Num* ptr = header;
    while (ptr != nullptr) { // 노드가 끝(NULL)일 때까지 반복
        count++;
        ptr = ptr->next;    // 다음 노드 이동
    }
    return count;
}

// 숙제3-1 마지막에 데이터 삽입
void insertLast(Num* header, int data) {
    Num* newNode = new Num(data);
    insertLast(header, newNode); // insertLast 호출
}

void insertLast(Num* header, Num* node) {
    if (header == nullptr) return;
    Num* ptr = header;
    while (ptr->next != NULL) {
        ptr = ptr->next;
    }
    ptr->next = node;
}

// 숙제3-2 맨 앞에 노드 삽입
void insertFirst(Num** header, Num* node) {
    if (node == nullptr) return; // 새 노드의 끝에 첫 번째 노드 연결
    node->next = *header; // 헤더가 가리키는 주소 자체를 새 노드로 변경
    *header = node;
}

int main() {
    Num* header = NULL;

    Num* ptr = new Num(10);
    header = ptr;
    Num* ptr1 = new Num(20);

    ptr->setNext(ptr1);
    Num* ptr2 = new Num(30);
    insertLast(header, ptr2);

    Num* ptr3 = new Num(40);// ptr3을 맨 앞으로
    insertFirst(&header, ptr3);

    cout << "Total nodes: " << countNext(header) << endl;
    Num* curr = header;
    while (curr != nullptr) {
        cout << curr->data << " -> ";
        curr = curr->next;
    }
    cout << "NULL" << endl;

    return 0;
}