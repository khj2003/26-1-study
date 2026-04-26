#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <stack>
#include <string>
#include <limits>

using namespace std;

// =============================================================================
// 1. myVector: 동적 배열 기반 (임의 접근 $O(1)$)
// =============================================================================
template <typename T>
class myVector {
private:
    T* data;
    size_t cap;
    size_t sz;
    void resize(size_t new_cap) {
        T* new_data = new T[new_cap];
        for (size_t i = 0; i < sz; ++i) new_data[i] = data[i];
        delete[] data;
        data = new_data;
        cap = new_cap;
    }
public:
    class iterator {
        T* ptr;
    public:
        iterator(T* p) : ptr(p) {}
        T& operator*() const { return *ptr; }
        iterator& operator++() { ++ptr; return *this; }
        bool operator!=(const iterator& other) const { return ptr != other.ptr; }
    };
    myVector() : data(nullptr), cap(0), sz(0) {}
    ~myVector() { delete[] data; }
    void push_back(const T& val) {
        if (sz == cap) resize(cap == 0 ? 1 : cap * 2);
        data[sz++] = val;
    }
    void pop_back() {
        if (sz == 0) throw underflow_error("Vector empty");
        sz--;
    }
    T& operator[](size_t i) {
        if (i >= sz) throw out_of_range("Vector index error");
        return data[i];
    }
    size_t size() const { return sz; }
    bool empty() const { return sz == 0; }
    iterator begin() const { return iterator(data); }
    iterator end() const { return iterator(data + sz); }
};

// =============================================================================
// 2. myList: 이중 연결 리스트 (삽입/삭제 $O(1)$)
// =============================================================================
template <typename T>
class myList {
private:
    struct Node {
        T data; Node* prev, * next;
        Node(const T& v) : data(v), prev(nullptr), next(nullptr) {}
    };
    Node* head, * tail;
    size_t sz;
public:
    class iterator {
        Node* curr;
    public:
        iterator(Node* n) : curr(n) {}
        T& operator*() const { return curr->data; }
        iterator& operator++() { if (curr) curr = curr->next; return *this; }
        bool operator!=(const iterator& other) const { return curr != other.curr; }
    };
    myList() : head(nullptr), tail(nullptr), sz(0) {}
    ~myList() { while (!empty()) pop_front(); }
    void push_back(const T& v) {
        Node* newNode = new Node(v);
        if (!tail) head = tail = newNode;
        else { newNode->prev = tail; tail->next = newNode; tail = newNode; }
        sz++;
    }
    void pop_front() {
        if (!head) throw underflow_error("List empty");
        Node* temp = head;
        head = head->next;
        if (head) head->prev = nullptr; else tail = nullptr;
        delete temp; sz--;
    }
    T& front() { if (!head) throw underflow_error("List empty"); return head->data; }
    size_t size() const { return sz; }
    bool empty() const { return sz == 0; }
    iterator begin() const { return iterator(head); }
    iterator end() const { return iterator(nullptr); }
};

// =============================================================================
// 3. myStack & 4. myQueue: 컨테이너 어댑터
// =============================================================================
template <typename T>
class myStack {
    myVector<T> v;
public:
    void push(const T& val) { v.push_back(val); }
    void pop() { v.pop_back(); }
    T& top() { if (v.empty()) throw underflow_error("Stack empty"); return v[v.size() - 1]; }
    bool empty() { return v.empty(); }
};

template <typename T>
class myQueue {
    myList<T> l;
public:
    void push(const T& val) { l.push_back(val); }
    void pop() { l.pop_front(); }
    T& front() { return l.front(); }
    bool empty() { return l.empty(); }
};

// =============================================================================
// 5. myDeque: 원형 배열 기반 (양단 삽입/삭제 $O(1)$)
// =============================================================================
template <typename T>
class myDeque {
private:
    T* data;
    size_t cap, sz, head, tail;
    void resize(size_t nc) {
        T* nd = new T[nc];
        for (size_t i = 0; i < sz; ++i) nd[i] = data[(head + i) % cap];
        delete[] data; data = nd; head = 0; tail = sz; cap = nc;
    }
public:
    myDeque() : data(nullptr), cap(0), sz(0), head(0), tail(0) {}
    ~myDeque() { delete[] data; }
    void push_back(const T& v) {
        if (sz == cap) resize(cap == 0 ? 1 : cap * 2);
        data[tail] = v; tail = (tail + 1) % cap; sz++;
    }
    void push_front(const T& v) {
        if (sz == cap) resize(cap == 0 ? 1 : cap * 2);
        head = (head - 1 + cap) % cap; data[head] = v; sz++;
    }
    void pop_front() {
        if (sz == 0) throw underflow_error("Deque empty");
        head = (head + 1) % cap; sz--;
    }
    T& operator[](size_t i) {
        if (i >= sz) throw out_of_range("Deque index error");
        return data[(head + i) % cap];
    }
    size_t size() const { return sz; }
    bool empty() const { return sz == 0; }
};

// =============================================================================
// 6. myAVL: 자가 균형 이진 탐색 트리 (탐색 $O(\log N)$ 보장)
// =============================================================================
template <typename T>
class myAVL {
private:
    struct Node {
        T data; Node* left, * right; int height;
        Node(const T& v) : data(v), left(nullptr), right(nullptr), height(1) {}
    };
    Node* root;
    int h(Node* n) { return n ? n->height : 0; }
    int b(Node* n) { return n ? h(n->left) - h(n->right) : 0; }
    Node* rotR(Node* y) {
        Node* x = y->left; Node* T2 = x->right;
        x->right = y; y->left = T2;
        y->height = max(h(y->left), h(y->right)) + 1;
        x->height = max(h(x->left), h(x->right)) + 1;
        return x;
    }
    Node* rotL(Node* x) {
        Node* y = x->right; Node* T2 = y->left;
        y->left = x; x->right = T2;
        x->height = max(h(x->left), h(x->right)) + 1;
        y->height = max(h(y->left), h(y->right)) + 1;
        return y;
    }
    Node* ins(Node* node, const T& val) {
        if (!node) return new Node(val);
        if (val < node->data) node->left = ins(node->left, val);
        else if (val > node->data) node->right = ins(node->right, val);
        else return node;
        node->height = 1 + max(h(node->left), h(node->right));
        int bal = b(node);
        if (bal > 1 && val < node->left->data) return rotR(node);
        if (bal < -1 && val > node->right->data) return rotL(node);
        if (bal > 1 && val > node->left->data) { node->left = rotL(node->left); return rotR(node); }
        if (bal < -1 && val < node->right->data) { node->right = rotR(node->right); return rotL(node); }
        return node;
    }
    void clear(Node* n) { if (n) { clear(n->left); clear(n->right); delete n; } }
public:
    class iterator {
        stack<Node*> s;
        void pushL(Node* n) { while (n) { s.push(n); n = n->left; } }
    public:
        iterator(Node* r) { pushL(r); }
        T& operator*() const { return s.top()->data; }
        iterator& operator++() {
            Node* n = s.top(); s.pop();
            if (n->right) pushL(n->right);
            return *this;
        }
        bool operator!=(const iterator& other) const { return !s.empty(); }
    };
    myAVL() : root(nullptr) {}
    ~myAVL() { clear(root); }
    void insert(const T& v) { root = ins(root, v); }
    iterator begin() const { return iterator(root); }
    iterator end() const { return iterator(nullptr); }
};

// =============================================================================
// Main: 사용자 친화적 메뉴 (입력 오류 방지 적용)
// =============================================================================
int main() {
    myVector<int> v; myList<int> l; myStack<int> s;
    myQueue<int> q; myDeque<int> dq; myAVL<int> avl;

    while (true) {
        cout << "\n[1]Vector [2]List [3]Stack [4]Queue [5]Deque [6]AVL [0]Exit : ";
        int cmd;
        if (!(cin >> cmd)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "숫자만 입력해 주세요!\n"; continue;
        }
        if (cmd == 0) break;

        try {
            switch (cmd) {
            case 1: {
                cout << "Vector (1:추가 2:삭제 3:출력): "; int sub; cin >> sub;
                if (sub == 1) { int x; cin >> x; v.push_back(x); }
                else if (sub == 2) v.pop_back();
                else { cout << "Vector: "; for (auto x : v) cout << x << " "; cout << endl; }
                break;
            }
            case 2: {
                cout << "List (1:추가 2:삭제 3:출력): "; int sub; cin >> sub;
                if (sub == 1) { int x; cin >> x; l.push_back(x); }
                else if (sub == 2) l.pop_front();
                else { cout << "List: "; for (auto x : l) cout << x << " "; cout << endl; }
                break;
            }
            case 3: {
                cout << "Stack (1:Push 2:Pop 3:Top): "; int sub; cin >> sub;
                if (sub == 1) { int x; cin >> x; s.push(x); }
                else if (sub == 2) s.pop();
                else cout << "Top: " << s.top() << endl;
                break;
            }
            case 4: {
                cout << "Queue (1:Push 2:Pop 3:Front): "; int sub; cin >> sub;
                if (sub == 1) { int x; cin >> x; q.push(x); }
                else if (sub == 2) q.pop();
                else cout << "Front: " << q.front() << endl;
                break;
            }
            case 5: {
                cout << "Deque (1:뒤추가 2:앞추가 3:삭제 4:출력): "; int sub; cin >> sub;
                if (sub == 1) { int x; cin >> x; dq.push_back(x); }
                else if (sub == 2) { int x; cin >> x; dq.push_front(x); }
                else if (sub == 3) dq.pop_front();
                else { cout << "Deque: "; for (size_t i = 0; i < dq.size(); ++i) cout << dq[i] << " "; cout << endl; }
                break;
            }
            case 6: {
                cout << "AVL (1:삽입 2:중위순회출력): "; int sub; cin >> sub;
                if (sub == 1) { int x; cin >> x; avl.insert(x); }
                else { cout << "AVL: "; for (auto x : avl) cout << x << " "; cout << endl; }
                break;
            }
            }
        }
        catch (const exception& e) { cout << "Error: " << e.what() << endl; }
    }
    return 0;
}