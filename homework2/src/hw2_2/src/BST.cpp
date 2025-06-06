#include <iostream>
#include <cstdlib>  // for rand(), srand()
#include <ctime>    // for time()
#include <cmath>    // for log2()
#include <vector>
using namespace std;

// 節點定義
template <class K, class E>
struct TreeNode {
    pair<K, E> data;
    TreeNode* leftChild;
    TreeNode* rightChild;
    TreeNode(const pair<K, E>& elem) : data(elem), leftChild(nullptr), rightChild(nullptr) {}
};

// BST 類別
template <class K, class E>
class BST {
private:
    TreeNode<K, E>* root;

    // 遞迴刪除所有節點 (用於析構)
    void destroy(TreeNode<K, E>* node) {
        if (!node) return;
        destroy(node->leftChild);
        destroy(node->rightChild);
        delete node;
    }

public:
    // 建構與解構
    BST() : root(nullptr) {}
    ~BST() { destroy(root); }

    // 插入函式
    void Insert(const pair<K, E>& thePair) {
        TreeNode<K, E>* p = root, * pp = nullptr;

        while (p) {
            pp = p;
            if (thePair.first < p->data.first)
                p = p->leftChild;
            else if (thePair.first > p->data.first)
                p = p->rightChild;
            else {
                // key 重複，更新對應 value
                p->data.second = thePair.second;
                return;
            }
        }

        // 建立新節點並連接到父節點
        p = new TreeNode<K, E>(thePair);
        if (root) {
            if (thePair.first < pp->data.first)
                pp->leftChild = p;
            else
                pp->rightChild = p;
        }
        else {
            root = p; // 空樹的情況
        }
    }

    // 查詢函式 (體代版本)
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
        return 0; // 沒有找到
    }

    // 刪除函式
    void Delete(const K& k) {
        TreeNode<K, E>* p = root, * pp = nullptr;

        // 尋找要刪除的節點 p 及其父節點 pp
        while (p && p->data.first != k) {
            pp = p;
            if (k < p->data.first) p = p->leftChild;
            else p = p->rightChild;
        }

        if (!p) return; // 沒有找到

        // 若 p 有兩個子節點，找右子樹的最小節點替代
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

        // p 只有一個子節點或沒有子節點
        TreeNode<K, E>* child;
        if (p->leftChild) child = p->leftChild;
        else child = p->rightChild;

        if (!pp) root = child;
        else if (pp->leftChild == p) pp->leftChild = child;
        else pp->rightChild = child;

        delete p;
    }

    // 計算樹高
    int getHeight() { return getHeight(root); }
    int getHeight(TreeNode<K, E>* node) {
        if (!node) return 0;
        int leftH = getHeight(node->leftChild);
        int rightH = getHeight(node->rightChild);
        return 1 + max(leftH, rightH);
    }

    // 顯示 (中序遍歷)
    void inorder() { inorder(root); cout << endl; }
    void inorder(TreeNode<K, E>* node) {
        if (!node) return;
        inorder(node->leftChild);
        cout << "(" << node->data.first << ", " << node->data.second << ") ";
        inorder(node->rightChild);
    }
};




using namespace std;

int main() {
    srand(time(0));  // 設定隨機種子

    // 測試不同的 n
    vector<int> ns = { 100, 500, 1000, 2000, 3000, 10000 };

    cout << "n\tHeight\tlog2(n)\tRatio" << endl;
    for (int n : ns) {
        BST<int, int> tree;

        // 隨機插入 n 個節點
        for (int i = 0; i < n; ++i) {
            int key = rand();
            int value = rand();  // value 也隨機給
            tree.Insert({ key, value });
        }

        // 計算樹高
        int height = tree.getHeight();

        // 計算 log2(n)
        double logn = log2(n);

        // 計算比值
        double ratio = height / logn;

        // 印出結果
        cout << n << "\t" << height << "\t" << logn << "\t" << ratio << endl;
    }

    return 0;
}

