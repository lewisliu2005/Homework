# 41243221

## 解題說明

本報告基於提供的 C++ 程式碼和複雜度分析報告，詳細講解五種排序演算法（插入排序、快速排序、合併排序、堆排序、複合排序）的實現過程、隨機測資生成、執行時間測量以及記憶體使用量計算。

### 1.1 插入排序
插入排序（`insertionSort`）通過逐一將元素插入到已排序的子數組中完成排序：
- **過程**：從第二個元素開始，將當前元素（`key`）與前面的已排序部分比較，找到合適位置插入。
- **特點**：適合小型或近乎排序的數據，時間複雜度為 O(n²)，空間複雜度為 O(1)。

### 1.2 快速排序
快速排序（`quickSort`）採用分治法，選擇基準（pivot）將數組分為兩部分，遞迴排序：
- **過程**：
  - 使用 `medianOfThree` 選擇基準（首、中、末三數的中位數），減少最壞情況發生機率。
  - 將基準移到末尾，進行分區（partition），小於基準的元素移到左邊，大於的移到右邊。
  - 對子數組遞迴排序，當子數組小於 10 時改用插入排序優化。
- **特點**：平均時間複雜度為 O(n log n)，最壞情況 O(n²)，空間複雜度 O(log n)（遞迴棧）。

### 1.3 合併排序
合併排序（`mergeSort`）使用迭代方式實現分治法，將數組分成小塊後合併：
- **過程**：
  - 從大小為 1 的子數組開始，逐步合併成更大的有序數組。
  - `merge` 函數將兩個有序子數組合併到臨時數組 `temp`，再複製回原數組。
- **特點**：穩定排序，時間複雜度始終為 O(n log n)，空間複雜度 O(n)（臨時數組）。

### 1.4 堆排序
堆排序（`heapSort`）基於最大堆結構，將數組轉換為堆後逐步提取最大元素：
- **過程**：
  - 通過 `heapify` 構建最大堆，確保父節點大於子節點。
  - 將堆頂（最大值）與末尾元素交換，縮小堆大小並重新調整堆。
- **特點**：時間複雜度為 O(n log n)，空間複雜度 O(1)（原地排序）。

### 1.5 複合排序
複合排序（`compositeSort`）根據輸入數據特性和大小選擇適當的排序演算法：
- **過程**：
  - 如果 n ≤ 50，直接使用插入排序。
  - 計算數據的排序度（`sortedness`），若接近有序（<0.1 或 >0.9），使用插入排序。
  - 若 n ≤ 1000，使用快速排序；否則使用合併排序。
- **特點**：靈活適應不同場景，理論時間複雜度為 O(n log n)，但小數據或特定情況可能退化為 O(n²)。

### 1.6 隨機測資生成
- **平均情況**：使用 `permute` 函數生成隨機排列，通過 `mt19937` 隨機數生成器打亂數組，確保數據無特定模式。
- **最壞情況**：
  - 插入排序：逆序數組（`generateWorstCaseInsertion`）。
  - 快速排序：升序數組（`generateWorstCaseQuick`），因基準選擇影響分區。
  - 合併排序/堆排序：通過多次隨機排列測試，選取執行時間最長的數組（`generateWorstCaseMerge`/`generateWorstCaseHeap`）。
  - 複合排序：使用合併排序的最壞情況數據。

### 1.7 執行時間與記憶體測量
- **執行時間**：使用 `chrono::high_resolution_clock` 測量排序函數的運行時間（微秒），對每組數據重複 100 次取平均值，減少系統波動影響。
- **記憶體使用量**：通過 `getMemoryUsage` 函數獲取進程的 `WorkingSetSize`（單位：KB），記錄排序前後的最大值。

## 程式實作

以下為程式碼中各排序函數及相關功能的核心實現和功能講解。

### 2.1 插入排序實現
```cpp
void insertionSort(vector<int> &arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}
```
- **功能**：逐元素插入，通過右移操作騰出位置，適合小數據或近乎排序數據。
- **特點**：簡單高效，無需額外空間。

### 2.2 快速排序實現
```cpp
void quickSortHelper(vector<int> &arr, int low, int high, bool reverse) {
    if (low < high) {
        if (high - low < 10) {
            vector<int> subArr(arr.begin() + low, arr.begin() + high + 1);
            insertionSort(subArr);
            for (int i = low; i <= high; ++i) arr[i] = subArr[i - low];
            return;
        }
        int pivotIndex = medianOfThree(arr, low, high, reverse);
        swap(arr[pivotIndex], arr[high]);
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; ++j) {
            if (reverse ? arr[j] >= pivot : arr[j] <= pivot) {
                ++i;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        int pi = i + 1;
        quickSortHelper(arr, low, pi - 1, reverse);
        quickSortHelper(arr, pi + 1, high, reverse);
    }
}
```
- **功能**：使用中位數基準選擇和插入排序優化，減少遞迴深度和最壞情況影響。
- **特點**：支持逆序排序（`reverse`），高效處理大數據。

### 2.3 合併排序實現
```cpp
void merge(vector<int> &arr, vector<int> &temp, int left, int mid, int right) {
    int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (i = left; i <= right; ++i) arr[i] = temp[i];
}
void mergeSort(vector<int> &arr) {
    int n = arr.size();
    vector<int> temp(n);
    for (int size = 1; size < n; size *= 2) {
        for (int left = 0; left < n - size; left += 2 * size) {
            int mid = left + size - 1;
            int right = min(left + 2 * size - 1, n - 1);
            merge(arr, temp, left, mid, right);
        }
    }
}
```
- **功能**：迭代合併避免遞迴，穩定排序，適合大數據。
- **特點**：需要額外 O(n) 空間。

### 2.4 堆排序實現
```cpp
void heapify(vector<int> &arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}
void heapSort(vector<int> &arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; --i) heapify(arr, n, i);
    for (int i = n - 1; i > 0; --i) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}
```
- **功能**：構建最大堆後逐步提取最大值，原地排序。
- **特點**：無需額外空間，穩定性能。

### 2.5 複合排序實現
```cpp
void compositeSort(vector<int> &arr) {
    int n = arr.size();
    if (n <= 50) {
        insertionSort(arr);
        return;
    }
    double sorted_ratio = sortedness(arr);
    if (sorted_ratio < 0.1 || sorted_ratio > 0.9) {
        insertionSort(arr);
    } else if (n <= 1000) {
        quickSort(arr);
    } else {
        mergeSort(arr);
    }
}
```
- **功能**：根據數據大小和排序度動態選擇排序演算法。
- **特點**：靈活但可能因插入排序頻繁使用而退化。

### 2.6 隨機測資與測量實現
- **隨機測資**：
  ```cpp
  void permute(vector<int> &arr) {
      int n = arr.size();
      static mt19937 gen(static_cast<unsigned int>(time(nullptr)));
      for (int i = n - 1; i >= 1; --i) {
          uniform_int_distribution<> dis(0, i);
          swap(arr[i], arr[dis(gen)]);
      }
  }
  ```
  - 使用 Fisher-Yates 洗牌演算法生成隨機排列。
- **最壞情況測資**：針對各演算法生成特定數據（逆序、升序或高執行時間排列）。
- **執行時間測量**：
  ```cpp
  pair<double, size_t> measureSort(void (*sortFunc)(vector<int> &), vector<int> arr, int repeat = 1000, bool reverse = false) {
      double totalTime = 0;
      size_t memoryBefore = getMemoryUsage();
      for (int i = 0; i < repeat; ++i) {
          vector<int> temp = arr;
          auto start = chrono::high_resolution_clock::now();
          if (sortFunc == quickSort && reverse) {
              quickSortHelper(temp, 0, temp.size() - 1, true);
          } else {
              sortFunc(temp);
          }
          auto end = chrono::high_resolution_clock::now();
          totalTime += chrono::duration<double, micro>(end - start).count();
      }
      size_t memoryAfter = getMemoryUsage();
      return make_pair(totalTime / repeat, max(memoryAfter, memoryBefore));
  }
  ```
  - 重複 100 次取平均值，確保數據穩定。
- **記憶體測量**：
  ```cpp
  size_t getMemoryUsage() {
      PROCESS_MEMORY_COUNTERS memInfo;
      GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
      return memInfo.WorkingSetSize / 1024;
  }
  ```
  - 測量進程總記憶體使用量，無法精確反映演算法輔助空間。

## 效能分析


## 測試與驗證
### 3.1 原始數據
以下數據來自 `sorting_results.txt`，記錄了五種排序演算法在不同輸入大小（n = 500, 1000, 2000, 3000, 4000, 5000）下的平均情況和最壞情況執行時間（微秒）和記憶體使用量（KB）。

#### 3.1.1 Average-case 執行時間（微秒）
| n     | 插入排序   | 快速排序 | 合併排序 | 堆排序  | 複合排序  |
|-------|------------|----------|----------|---------|-----------|
| 500   | 2183.08    | 566.71   | 312.47   | 619.15  | 3739.31   |
| 1000  | 8109.56    | 1099.12  | 698.77   | 1335.43 | 13447.86  |
| 2000  | 32652.73   | 2319.84  | 1486.27  | 2929.27 | 48959.93  |
| 3000  | 74566.95   | 3780.51  | 2354.84  | 4643.57 | 108966.67 |
| 4000  | 128559.98  | 4964.71  | 3231.43  | 6407.42 | 193980.64 |
| 5000  | 210371.11  | 6444.70  | 4413.84  | 8424.55 | 308679.93 |

#### 3.1.2 Average-case 記憶體使用量（KB）
| n     | 插入排序 | 快速排序 | 合併排序 | 堆排序 | 複合排序 |
|-------|----------|----------|----------|--------|----------|
| 500   | 5920     | 5992     | 6028     | 6028   | 6028     |
| 1000  | 6052     | 6052     | 6112     | 6112   | 6112     |
| 2000  | 6168     | 6112     | 6188     | 6188   | 6188     |
| 3000  | 6240     | 6240     | 6332     | 6332   | 6364     |
| 4000  | 6424     | 6392     | 6512     | 6512   | 6544     |
| 5000  | 6592     | 6596     | 6596     | 6596   | 6596     |

#### 3.1.3 Worst-case 執行時間（微秒）
| n     | 插入排序   | 快速排序 | 合併排序 | 堆排序  | 複合排序  |
|-------|------------|----------|----------|---------|-----------|
| 500   | 4056.98    | 546.91   | 300.42   | 581.95  | 3552.58   |
| 1000  | 16186.64   | 1270.46  | 679.75   | 1308.31 | 12966.46  |
| 2000  | 65021.12   | 2778.81  | 1447.30  | 2925.33 | 49357.00  |
| 3000  | 145652.87  | 4463.33  | 2375.10  | 4762.01 | 110464.50 |
| 4000  | 259666.70  | 6252.46  | 3291.65  | 6480.12 | 199418.69 |
| 5000  | 415399.32  | 8506.74  | 4351.01  | 8392.84 | 314854.25 |

#### 3.1.4 Worst-case 記憶體使用量（KB）
| n     | 插入排序 | 快速排序 | 合併排序 | 堆排序 | 複合排序 |
|-------|----------|----------|----------|--------|----------|
| 500   | 6352     | 6352     | 6352     | 6352   | 6352     |
| 1000  | 6352     | 6352     | 6352     | 6352   | 6352     |
| 2000  | 6368     | 6368     | 6368     | 6368   | 6368     |
| 3000  | 6404     | 6404     | 6404     | 6404   | 6404     |
| 4000  | 6452     | 6452     | 6452     | 6452   | 6452     |
| 5000  | 6616     | 6616     | 6616     | 6580   | 6596     |

### 3.2 時間複雜度推算
時間複雜度通過運行時間隨輸入大小 n 的增長比率推算：
- O(n)：n 增 2 倍，時間增約 2 倍。
- O(n log n)：n 增 2 倍，時間增約 2.4 倍。
- O(n²)：n 增 2 倍，時間增約 4 倍。

#### 3.2.1 Average-case 時間複雜度
- **插入排序**：比率約 4（500 到 1000：3.72，1000 到 2000：4.03），推算為 O(n²)。
- **快速排序**：比率約 2（500 到 1000：1.94，1000 到 2000：2.11），推算為 O(n log n)。
- **合併排序**：比率約 2（500 到 1000：2.24，1000 到 2000：2.13），推算為 O(n log n)。
- **堆排序**：比率約 2（500 到 1000：2.16，1000 到 2000：2.19），推算為 O(n log n)。
- **複合排序**：比率約 4（500 到 1000：3.60，1000 到 2000：3.64），推算為 O(n²)。

#### 3.2.2 Worst-case 時間複雜度
- **插入排序**：比率約 4（500 到 1000：3.99，1000 到 2000：4.02），推算為 O(n²)。
- **快速排序**：比率約 2（500 到 1000：2.32，1000 到 2000：2.19），推算為 O(n log n)。
- **合併排序**：比率約 2（500 到 1000：2.26，1000 到 2000：2.13），推算為 O(n log n)。
- **堆排序**：比率約 2（500 到 1000：2.25，1000 到 2000：2.24），推算為 O(n log n)。
- **複合排序**：比率約 4（500 到 1000：3.65，1000 到 2000：3.81），推算為 O(n²)。

#### 3.2.3 時間複雜度結論
| 演算法    | 平均情況推算 | 最壞情況推算 | 理論值（參考）         |
|-----------|--------------|--------------|-----------------------|
| 插入排序  | O(n²)        | O(n²)        | O(n²)                 |
| 快速排序  | O(n log n)   | O(n log n)   | O(n log n) / O(n²)    |
| 合併排序  | O(n log n)   | O(n log n)   | O(n log n)            |
| 堆排序    | O(n log n)   | O(n log n)   | O(n log n)            |
| 複合排序  | O(n²)        | O(n²)        | O(n²) or O(n log n)   |

**備註**：
- 快速排序因使用中位數選擇，實際最壞情況接近 O(n log n)。
- 複合排序因測試數據觸發插入排序，推算為 O(n²)，理論上大 n 應為 O(n log n)。

### 3.3 空間複雜度推算
空間複雜度通過記憶體使用量隨 n 的變化趨勢推算：
- O(1)：記憶體幾乎不變。
- O(log n)：記憶體緩慢增加。
- O(n)：記憶體隨 n 線性增加。

#### 3.3.1 Average-case 空間複雜度
- **插入排序**：記憶體增約 11%（5920 到 6592 KB），推算為 O(1)。
- **快速排序**：記憶體增約 10%（5992 到 6596 KB），推算為 O(log n)。
- **合併排序**：記憶體增約 9%（6028 到 6596 KB），推算為 O(1)（理論 O(n)）。
- **堆排序**：記憶體增約 9%（6028 到 6596 KB），推算為 O(1)。
- **複合排序**：記憶體增約 9%（6028 到 6596 KB），推算為 O(1)（理論可能 O(n)）。

#### 3.3.2 Worst-case 空間複雜度
- **插入排序**：記憶體增約 4%（6352 到 6616 KB），推算為 O(1)。
- **快速排序**：記憶體增約 4%（6352 到 6616 KB），推算為 O(log n)。
- **合併 sorting**：記憶體增約 4%（6352 到 6616 KB），推算為 O(1)（理論 O(n)）。
- **堆排序**：記憶體增約 3%（6352 到 6580 KB），推算為 O(1)。
- **複合排序**：記憶體增約 4%（6352 到 6596 KB），推算為 O(1)（理論可能 O(n)）。

#### 3.3.3 空間複雜度結論
| 演算法    | 平均情況推算 | 最壞情況推算 | 理論值（參考）         |
|-----------|--------------|--------------|-----------------------|
| 插入排序  | O(1)         | O(1)         | O(1)                  |
| 快速排序  | O(log n)     | O(log n)     | O(log n)              |
| 合併排序  | O(1)         | O(1)         | O(n)                  |
| 堆排序    | O(1)         | O(1)         | O(1)                  |
| 複合排序  | O(1)         | O(1)         | O(1) or O(n)          |

**備註**：
- 合併排序推算為 O(1) 與理論 O(n) 不符，因 `getMemoryUsage` 測量整個進程記憶體，無法精確反映輔助數組。
- 複合排序推算為 O(1)，但使用合併排序時應為 O(n)。

### 3.4 注意事項
- 時間複雜度推算可能受硬體、系統負載影響，建議多次運行以穩定數據。
- 記憶體測量包括程式碼、堆棧等開銷，導致合併排序結果與理論不符。
- 複合排序因插入排序頻繁使用，性能退化為 O(n²)。

### 3.5 建議
- **時間複雜度**：添加計數器記錄比較/交換次數，驗證推算結果。
- **空間複雜度**：直接計算輔助數組大小（如合併排序的 `temp`）。
- **理論複雜度**：程式碼中添加理論值說明，方便對比。
- **複合排序**：調整閾值（如 `sortedness` 或 n），減少插入排序使用。

## 申論及開發報告

本報告通過程式實作和效能分析，全面評估了插入排序、快速排序、合併排序、堆排序和複合排序的性能：
- **插入排序**：適合小數據或近乎排序數據，時間複雜度 O(n²)，空間複雜度 O(1)。
- **快速排序**：高效且穩定，平均時間複雜度 O(n log n)，空間複雜度 O(log n)，因中位數選擇避免最壞情況。
- **合併排序**：穩定且時間複雜度始終 O(n log n)，但空間複雜度 O(n)，測量結果因方法限制偏低。
- **堆排序**：時間複雜度 O(n log n)，空間複雜度 O(1)，性能穩定但稍慢於快速排序。
- **複合排序**：理論上靈活適應場景，但因插入排序過多使用，實際性能退化為 O(n²)。

**總結**：
- 合併排序在執行時間上表現最佳，特別是平均和最壞情況。
- 快速排序和堆排序在大數據下表現良好，快速排序略勝。
- 插入排序和複合排序在大數據下效率較低，需優化複合排序的閾值。
- 記憶體測量需改進以精確反映輔助空間，特別是合併排序和複合排序。

未來可通過計數器、精確空間測量和閾值調整進一步優化程式，確保複合排序在大數據下達到 O(n log n) 的理論性能。
