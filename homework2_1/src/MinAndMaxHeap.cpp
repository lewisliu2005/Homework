#include <iostream>
#include <string>

using namespace std;

template <class T>
void ChangeSize1D(T*& a, const int oldSize, const int newSize) {
    if (newSize < 0) throw "New length must be >= 0";
    T* temp = new T[newSize];
    int number = min(oldSize, newSize);
    for (int i = 0; i < number; i++)
        temp[i] = a[i];
    delete[] a;
    a = temp;
}

// 抽象基類 MaxPQ
template <class T>
class MaxPQ {
public:
    MaxPQ(int theCapacity = 10) {}
    virtual ~MaxPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
protected:
    T* heap = nullptr;
    int heapSize = 0;
    int capacity = 0;
};

// 最大堆實現
template <class T>
class MaxHeap : public MaxPQ<T> {
public:
    MaxHeap(int theCapacity = 10) {
        if (theCapacity < 1) throw "Capacity must be >= 1";
        this->capacity = theCapacity;
        this->heapSize = 0;
        this->heap = new T[this->capacity + 1];
    }
    ~MaxHeap() override {
        delete[] this->heap;
    }
    bool IsEmpty() const override {
        return this->heapSize == 0;
    }
    const T& Top() const override {
        if (IsEmpty()) throw "Heap is empty. Cannot get top.";
        return this->heap[1];
    }
    void Push(const T& e) override {
        if (this->heapSize == this->capacity) {
            ChangeSize1D(this->heap, this->capacity, 2 * this->capacity);
            this->capacity *= 2;
        }
        int currentNode = ++this->heapSize;
        while (currentNode != 1 && this->heap[currentNode / 2] < e) {
            this->heap[currentNode] = this->heap[currentNode / 2];
            currentNode /= 2;
        }
        this->heap[currentNode] = e;
    }
    void Pop() override {
        if (IsEmpty()) throw "Heap is empty. Cannot delete.";
        this->heap[1] = this->heap[this->heapSize--];
        int currentNode = 1;
        int child = 2;
        T lastE = this->heap[this->heapSize + 1];
        while (child <= this->heapSize) {
            if (child < this->heapSize && this->heap[child] < this->heap[child + 1]) child++;
            if (lastE >= this->heap[child]) break;
            this->heap[currentNode] = this->heap[child];
            currentNode = child;
            child *= 2;
        }
        this->heap[currentNode] = lastE;
    }
    void visualizeTree(size_t index = 1, size_t level = 0) const { // 使用 size_t 避免溢出警告
        if (index > static_cast<size_t>(this->heapSize)) return;
        visualizeTree(2 * index + 1, level + 1);
        for (size_t i = 0; i < level; i++) cout << "    ";
        cout << this->heap[index] << endl;
        visualizeTree(2 * index, level + 1);
    }
};

// 抽象基類 MinPQ
template <class T>
class MinPQ {
public:
    MinPQ(int theCapacity = 10) {}
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
protected:
    T* heap = nullptr;
    int heapSize = 0;
    int capacity = 0;
};

// 最小堆實現
template <class T>
class MinHeap : public MinPQ<T> {
public:
    MinHeap(int theCapacity = 10) {
        if (theCapacity < 1) throw "Capacity must be >= 1";
        this->capacity = theCapacity;
        this->heapSize = 0;
        this->heap = new T[this->capacity + 1];
    }
    ~MinHeap() override {
        delete[] this->heap;
    }
    bool IsEmpty() const override {
        return this->heapSize == 0;
    }
    const T& Top() const override {
        if (IsEmpty()) throw "Heap is empty. Cannot get top.";
        return this->heap[1];
    }
    void Push(const T& e) override {
        if (this->heapSize == this->capacity) {
            ChangeSize1D(this->heap, this->capacity, 2 * this->capacity);
            this->capacity *= 2;
        }
        int currentNode = ++this->heapSize;
        while (currentNode != 1 && this->heap[currentNode / 2] > e) {
            this->heap[currentNode] = this->heap[currentNode / 2];
            currentNode /= 2;
        }
        this->heap[currentNode] = e;
    }
    void Pop() override {
        if (IsEmpty()) throw "Heap is empty. Cannot delete.";
        this->heap[1] = this->heap[this->heapSize--];
        int currentNode = 1;
        int child = 2;
        T lastE = this->heap[this->heapSize + 1];
        while (child <= this->heapSize) {
            if (child < this->heapSize && this->heap[child] > this->heap[child + 1]) child++;
            if (lastE <= this->heap[child]) break;
            this->heap[currentNode] = this->heap[child];
            currentNode = child;
            child *= 2;
        }
        this->heap[currentNode] = lastE;
    }
    void visualizeTree(size_t index = 1, size_t level = 0) const { // 使用 size_t 避免溢出警告
        if (index > static_cast<size_t>(this->heapSize)) return;
        visualizeTree(2 * index + 1, level + 1);
        for (size_t i = 0; i < level; i++) cout << "    ";
        cout << this->heap[index] << endl;
        visualizeTree(2 * index, level + 1);
    }
};

// 單一 main 函數，用於測試 MaxHeap 和 MinHeap
int main() {    
    int num = 0;
    cout << "請輸入你要輸入幾個數字: ";
    cin >> num;
    
    // 測試 MaxHeap
    MaxHeap<int> maxHeap(num);
    cout << "=== 測試 MaxHeap ===" << endl;
    int N = 0;
    for (int i = 0; i < num; i++) {
        cout << "請輸入你要放入最大堆積中的數字 第(" << i + 1 << "): ";
        cin >> N;
        maxHeap.Push(N);
    }
    cout << "初始最大堆(樹根是最左邊)" << endl;
    cout << endl;
    maxHeap.visualizeTree();
    for (int j = 1; j < num; j++) { 
        cout << "Top: " << maxHeap.Top() << endl;  
        maxHeap.Pop();
        cout << "after Pop Graph: " << endl;
        maxHeap.visualizeTree(); 
        cout << endl;  
    }
    
    cout << "All numbers have been Pop, Program over" << endl;

    // 測試 MinHeap
    MinHeap<int> minHeap(num);
    cout << "=== 測試 MinHeap ===" << endl;
    for (int i = 0; i < num; i++) {
        cout << "請輸入你要放入最小堆積中的數字 第(" << i + 1 << "): ";
        cin >> N;
        minHeap.Push(N);
    }
    cout << "初始最小堆(樹根是最左邊)" << endl;
    cout << endl;
    minHeap.visualizeTree();
    for (int j = 1; j < num; j++) { 
        cout << "Top: " << minHeap.Top() << endl;  
        minHeap.Pop();
        cout << "after Pop Graph: " << endl;
        minHeap.visualizeTree(); 
        cout << endl;  
    }
    
    cout << "All numbers have been Pop, Program over" << endl;
    return 0;
}