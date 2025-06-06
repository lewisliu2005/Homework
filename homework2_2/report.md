# 41243221
# 41243250

## 解題說明

本題要求實作一個二元搜尋樹（BST）類別，並分析隨機插入 $n$ 個節點後的樹高，與理論對數高度 $\log_2 n$ 進行比較，觀察高度與對數的比值趨勢。

---

## 解題策略

1. **BST 節點定義**
   節點包含 `data`（鍵值對）、左子節點與右子節點指標。
2. **BST 類別設計**

   * `Insert`：隨機插入節點，若鍵值已存在則更新對應的值。
   * `Get`：查詢節點。
   * `Delete`：刪除節點，含完整情境（0/1/2 個子節點）。
   * `getHeight`：遞迴計算樹高。
   * `inorder`：中序遍歷印出節點。
   * `destroy`：釋放所有節點，避免記憶體外洩。
3. **主程式**

   * 產生不同 $n$ 的隨機節點，插入 BST。
   * 計算樹高 $h$、理論對數高度 $\log_2 n$，並計算比值 $r = h / \log_2 n$。
   * 印出結果，觀察趨勢。

---

## 程式實作

### 標頭檔與命名空間

```cpp
#include <iostream>
#include <cstdlib>  // for rand(), srand()
#include <ctime>    // for time()
#include <cmath>    // for log2()
#include <vector>
using namespace std;
```

* **功能**：

  * 引入必要的函式庫（輸入輸出、隨機數、時間、對數計算、向量容器）。
  * 使用 `std` 命名空間，簡化程式中呼叫標準函式與容器。

---

### 節點結構定義

```cpp
template <class K, class E>
struct TreeNode {
    pair<K, E> data;
    TreeNode* leftChild;
    TreeNode* rightChild;
    TreeNode(const pair<K, E>& elem) : data(elem), leftChild(nullptr), rightChild(nullptr) {}
};
```

* **功能**：

  * 定義 BST 的節點 `TreeNode` 結構。
  * 包含：

    * `data`：存放鍵值對 $(K, E)$。
    * `leftChild`、`rightChild`：指向左右子節點的指標。
  * 建構子：接受一個 `pair<K, E>`，初始化節點並將子節點設為 `nullptr`。

---
### BST 類別定義

```cpp
template <class K, class E>
class BST {
private:
    TreeNode<K, E>* root;
};
```

* **功能**：

  * 建立 BST 類別模板，支援不同型態的鍵與值。
  * `root`：指向整棵樹的根節點。

---

### 3.1 節點釋放（遞迴刪除）

```cpp
    void destroy(TreeNode<K, E>* node) {
        if (!node) return;
        destroy(node->leftChild);
        destroy(node->rightChild);
        delete node;
    }
```

* **功能**：

  * 遞迴刪除 `node` 及其左右子節點，避免記憶體洩漏。

---

### 3.2 建構與解構函式

```cpp
public:
    BST() : root(nullptr) {}
    ~BST() { destroy(root); }
```

* **功能**：

  * 建構子：初始化根節點為 `nullptr`（空樹）。
  * 解構子：呼叫 `destroy` 釋放整棵樹。

---
### 3.3 插入新節點

```cpp
    void Insert(const pair<K, E>& thePair) {
        TreeNode<K, E>* p = root, * pp = nullptr;

        while (p) {
            pp = p;
            if (thePair.first < p->data.first)
                p = p->leftChild;
            else if (thePair.first > p->data.first)
                p = p->rightChild;
            else {
                p->data.second = thePair.second;
                return;
            }
        }

        p = new TreeNode<K, E>(thePair);
        if (root) {
            if (thePair.first < pp->data.first)
                pp->leftChild = p;
            else
                pp->rightChild = p;
        }
        else {
            root = p;
        }
    }
```

* **功能**：

  * 透過迴圈找到適當插入位置。
  * 如果找到相同鍵值，更新對應值並結束。
  * 否則建立新節點並插入父節點的左或右子節點。
  * 若原樹為空，直接指定根節點。

---

### 3.4 查詢節點

```cpp
    pair<K, E>* Get(const K& k) {
        TreeNode<K, E>* currentNode = root;
        while (currentNode) {
            if (k < currentNode->data.first)
                currentNode = currentNode->leftChild;
            else if (k > currentNode->data.first)
                currentNode = currentNode->rightChild;
            else
                return &currentNode->data;
        }
        return 0;
    }
```

* **功能**：

  * 以迴圈方式尋找節點，找到後回傳其資料指標；找不到則回傳 `0`（`nullptr`）。

---

### 3.5 刪除節點

```cpp
    void Delete(const K& k) {
        TreeNode<K, E>* p = root, * pp = nullptr;

        while (p && p->data.first != k) {
            pp = p;
            if (k < p->data.first) p = p->leftChild;
            else p = p->rightChild;
        }

        if (!p) return;

        if (p->leftChild && p->rightChild) {
            TreeNode<K, E>* minP = p->rightChild, * minPP = p;
            while (minP->leftChild) {
                minPP = minP;
                minP = minP->leftChild;
            }
            p->data = minP->data;
            p = minP;
            pp = minPP;
        }

        TreeNode<K, E>* child;
        if (p->leftChild) child = p->leftChild;
        else child = p->rightChild;

        if (!pp) root = child;
        else if (pp->leftChild == p) pp->leftChild = child;
        else pp->rightChild = child;

        delete p;
    }
```

* **功能**：

  * 先找到要刪除的節點 `p` 及其父節點 `pp`。
  * 若 `p` 有兩個子節點，找到右子樹最小值節點 `minP` 替代 `p`。
  * 最終將 `p` 的子節點接到父節點，並刪除 `p`。

---

### 3.6 樹高計算

```cpp
    int getHeight() { return getHeight(root); }
    int getHeight(TreeNode<K, E>* node) {
        if (!node) return 0;
        int leftH = getHeight(node->leftChild);
        int rightH = getHeight(node->rightChild);
        return 1 + max(leftH, rightH);
    }
```

* **功能**：

  * 遞迴取得子樹最大高度，回傳目前節點高度。
  * 樹高是「邊數 + 1」，根節點高度為 1。

---

### 3.7 中序遍歷印出節點

```cpp
    void inorder() { inorder(root); cout << endl; }
    void inorder(TreeNode<K, E>* node) {
        if (!node) return;
        inorder(node->leftChild);
        cout << "(" << node->data.first << ", " << node->data.second << ") ";
        inorder(node->rightChild);
    }
```

* **功能**：

  * 中序遍歷印出節點（從小到大順序），方便檢查樹形結構。

---

## 主程式測試區塊

```cpp
int main() {
    srand(time(0));  // 設定隨機種子

    vector<int> ns = { 100, 500, 1000, 2000, 3000, 10000 };
    cout << "n\tHeight\tlog2(n)\tRatio" << endl;

    for (int n : ns) {
        BST<int, int> tree;

        for (int i = 0; i < n; ++i) {
            int key = rand();
            int value = rand();
            tree.Insert({ key, value });
        }

        int height = tree.getHeight();
        double logn = log2(n);
        double ratio = height / logn;

        cout << n << "\t" << height << "\t" << logn << "\t" << ratio << endl;
    }

    return 0;
}
```

* **功能**：

  * 產生不同 `n` 節點數，隨機產生鍵值對並插入 BST。
  * 計算並印出：

    * 實際高度 `height`
    * 理論對數高度 `log2(n)`
    * 比值 `ratio = height / log2(n)`
  * 觀察趨勢，驗證隨機 BST 高度接近 $\log_2 n$。

---

## 效能分析

* **時間複雜度**：

  * **平均情況**：BST 插入與搜尋複雜度為 $O(\log n)$。
  * **最壞情況**：當插入序列完全遞增或遞減時，BST 退化為鏈狀結構，複雜度為 $O(n)$。
* **空間複雜度**：

  * 主要來自節點儲存空間與遞迴堆疊深度，空間複雜度約為 $O(n)$。

---

## 測試與驗證

| 測試案例 | 節點數 $n$ | 實際高度 $h$ | $\log_2 n$ | 比值 $r = h / \log_2 n$ |
| ---- | ------- | -------- | ---------- | --------------------- |
| 測試一  | 100     | 12       | 6.64       | 1.81                  |
| 測試二  | 500     | 18       | 8.96       | 2.01                  |
| 測試三  | 1000    | 27       | 9.97       | 2.70                  |
| 測試四  | 2000    | 25       | 10.97      | 2.28                  |
| 測試五  | 3000    | 24       | 11.55      | 2.08                  |
| 測試六  | 10000   | 29       | 13.29      | 2.18                  |

---

## 申論及開發報告

### 開發過程與思路

本次作業的目標是設計一個二元搜尋樹 (BST) 類別，並進行隨機插入節點後的高度分析。首先，我們先定義了樹的節點結構，包括儲存鍵值對（`pair<K,E>`）以及左右子節點指標。接著，在 BST 類別中，設計插入、查詢、刪除等基本操作，並特別加入計算樹高的遞迴函式。為了避免記憶體外洩，也在解構時使用遞迴釋放所有節點。

在主程式中，我們透過亂數產生器（`rand()`）生成節點，並觀察不同節點數量下，BST 實際高度與對數高度 $\log_2 n$ 的比值。經由多次實驗，可清楚觀察到實際高度會隨著節點數增加而增長，但其增長趨勢接近 $\log_2 n$，顯示隨機插入能避免退化成鏈狀結構的問題。

---

### 使用遞迴計算高度的原因

在本作業中，使用遞迴計算 BST 的樹高是非常自然且方便的作法。由於樹高定義為從根節點到最遠葉節點的邊數，使用遞迴能直接表達「從左右子樹分別找出最大高度，再加上目前節點自身」的邏輯。程式碼簡潔易懂，也方便後續維護。

---

### 隨機性的影響

從多次實驗結果可發現，隨機插入節點的 BST 雖然高度不是最小化（不像自動平衡樹），但高度仍相對接近 $\log_2 n$。這顯示隨機插入有助於避免最壞情境（完全鏈狀），適合中小型資料規模的應用。

---

### 實驗結果與分析

根據測試結果，實際高度與 $\log_2 n$ 的比值大約落在 1.8–2.7 之間，說明隨機插入已經有效降低高度增長速度，讓 BST 結構不易退化。此結果也與理論「隨機 BST 高度期望值約為 $2 \log_2 n$」相符。

---

### 結論與心得

本題練習BST基本操作，實驗結果證實，BST並未自動平衡，但在隨機插入的情況下，樹高人接近 $2 \log_2 n$ ，證明BST具有良好的搜尋效率。透過本次作業，了解平衡資料結構對效能的影響，以及圖表與數據分析在程式驗證的重要性。

------
