#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

const int HASH_SIZE = 1000003;
struct HashNode {
    int key;
    bool valid;
} ht[HASH_SIZE];

void init_hash() {
    for (int i = 0; i < HASH_SIZE; ++i) {
        ht[i].key = -1;
        ht[i].valid = false;
    }
}

int hash_func(int x) {
    unsigned int h = x;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h % HASH_SIZE;
}

void set_valid(int x, bool v) {
    int idx = hash_func(x);
    while (ht[idx].key != -1 && ht[idx].key != x) {
        idx++;
        if (idx == HASH_SIZE) idx = 0;
    }
    ht[idx].key = x;
    ht[idx].valid = v;
}

bool is_valid(int x) {
    int idx = hash_func(x);
    while (ht[idx].key != -1) {
        if (ht[idx].key == x) return ht[idx].valid;
        idx++;
        if (idx == HASH_SIZE) idx = 0;
    }
    return false;
}

struct MinHeap {
    int arr[1000005];
    int cnt;
    MinHeap() { cnt = 0; }
    void push(int x) {
        arr[++cnt] = x;
        for (int i = cnt, j = cnt >> 1; j; i = j, j >>= 1) {
            if (arr[i] < arr[j]) {
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            } else break;
        }
    }
    void pop() {
        if (cnt == 0) return;
        arr[1] = arr[cnt--];
        for (int i = 1, j = 2; j <= cnt; i = j, j <<= 1) {
            if (j < cnt && arr[j + 1] < arr[j]) j++;
            if (arr[j] < arr[i]) {
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            } else break;
        }
    }
    int top() { return arr[1]; }
    bool empty() { return cnt == 0; }
};

struct MaxHeap {
    int arr[1000005];
    int cnt;
    MaxHeap() { cnt = 0; }
    void push(int x) {
        arr[++cnt] = x;
        for (int i = cnt, j = cnt >> 1; j; i = j, j >>= 1) {
            if (arr[i] > arr[j]) {
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            } else break;
        }
    }
    void pop() {
        if (cnt == 0) return;
        arr[1] = arr[cnt--];
        for (int i = 1, j = 2; j <= cnt; i = j, j <<= 1) {
            if (j < cnt && arr[j + 1] > arr[j]) j++;
            if (arr[j] > arr[i]) {
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            } else break;
        }
    }
    int top() { return arr[1]; }
    bool empty() { return cnt == 0; }
};

MinHeap up_heap;
MaxHeap down_heap;
int pos = 0;
int dir = 1;

void clean_up() {
    while (!up_heap.empty() && !is_valid(up_heap.top())) {
        up_heap.pop();
    }
    while (!down_heap.empty() && !is_valid(down_heap.top())) {
        down_heap.pop();
    }
}

void action() {
    clean_up();
    if (dir == 1) {
        if (!up_heap.empty()) {
            pos = up_heap.top();
            up_heap.pop();
            set_valid(pos, false);
        } else if (!down_heap.empty()) {
            dir = -1;
            pos = down_heap.top();
            down_heap.pop();
            set_valid(pos, false);
        }
    } else {
        if (!down_heap.empty()) {
            pos = down_heap.top();
            down_heap.pop();
            set_valid(pos, false);
        } else if (!up_heap.empty()) {
            dir = 1;
            pos = up_heap.top();
            up_heap.pop();
            set_valid(pos, false);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    init_hash();
    int n;
    if (!(cin >> n)) return 0;
    string op;
    int x;
    for (int i = 0; i < n; ++i) {
        cin >> op;
        if (op == "add") {
            cin >> x;
            set_valid(x, true);
            if (x > pos) up_heap.push(x);
            else if (x < pos) down_heap.push(x);
        } else if (op == "cancel") {
            cin >> x;
            set_valid(x, false);
        } else if (op == "action") {
            action();
        } else if (op == "locate") {
            cout << pos << "\n";
        }
    }
    return 0;
}
