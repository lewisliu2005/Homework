#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
#include <windows.h>
#include <psapi.h>

using namespace std;

// 測量記憶體使用量（單位：KB）
size_t getMemoryUsage() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    return memInfo.WorkingSetSize / 1024;
}

// 產生隨機排列
void permute(vector<int>& arr) {
    int n = arr.size();
    static mt19937 gen(static_cast<unsigned int>(time(nullptr)));
    for (int i = n - 1; i >= 1; --i) {
        uniform_int_distribution<> dis(0, i);
        swap(arr[i], arr[dis(gen)]);
    }
}

// 插入排序
void insertionSort(vector<int>& arr) {
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

// 快速排序
int medianOfThree(vector<int>& arr, int low, int high, bool reverse = false) {
    int mid = low + (high - low) / 2;
    if (reverse) {
        if (arr[low] < arr[mid]) swap(arr[low], arr[mid]);
        if (arr[mid] < arr[high]) swap(arr[mid], arr[high]);
        if (arr[low] < arr[mid]) swap(arr[low], arr[mid]);
    }
    else {
        if (arr[low] > arr[mid]) swap(arr[low], arr[mid]);
        if (arr[mid] > arr[high]) swap(arr[mid], arr[high]);
        if (arr[low] > arr[mid]) swap(arr[low], arr[mid]);
    }
    return mid;
}

void quickSortHelper(vector<int>& arr, int low, int high, bool reverse = false) {
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

void quickSort(vector<int>& arr) {
    quickSortHelper(arr, 0, arr.size() - 1, false);
}

// 迭代合併排序
void merge(vector<int>& arr, vector<int>& temp, int left, int mid, int right) {
    int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (i = left; i <= right; ++i) arr[i] = temp[i];
}

void mergeSort(vector<int>& arr) {
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

// 堆排序
void heapify(vector<int>& arr, int n, int i) {
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

void heapSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; --i) heapify(arr, n, i);
    for (int i = n - 1; i > 0; --i) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// 檢查數據排序度
double sortedness(const vector<int>& arr) {
    int n = arr.size();
    int inversions = 0;
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (arr[i] > arr[j]) ++inversions;
        }
    }
    return static_cast<double>(inversions) / (n * (n - 1) / 2);
}

// 新複合排序
void compositeSort(vector<int>& arr) {
    int n = arr.size();
    if (n <= 50) {
        insertionSort(arr);
        return;
    }
    double sorted_ratio = sortedness(arr);
    if (sorted_ratio < 0.1 || sorted_ratio > 0.9) {
        insertionSort(arr);
    }
    else if (n <= 1000) {
        quickSort(arr);
    }
    else {
        mergeSort(arr);
    }
}

// 生成 worst-case 數據
vector<int> generateWorstCaseInsertion(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; ++i) arr[i] = n - i;
    return arr;
}

vector<int> generateWorstCaseQuick(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; ++i) arr[i] = i + 1;
    return arr;
}

vector<int> generateWorstCaseMerge(int n) {
    vector<int> arr(n);
    mt19937 gen(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < n; ++i) arr[i] = i + 1;
    vector<int> worst;
    double max_time = 0;
    for (int trial = 0; trial < 10; ++trial) {
        permute(arr);
        vector<int> temp = arr;
        auto start = chrono::high_resolution_clock::now();
        mergeSort(temp);
        auto end = chrono::high_resolution_clock::now();
        double time = chrono::duration<double, micro>(end - start).count();
        if (time > max_time) {
            max_time = time;
            worst = arr;
        }
    }
    return worst;
}

vector<int> generateWorstCaseHeap(int n) {
    vector<int> arr(n);
    mt19937 gen(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < n; ++i) arr[i] = i + 1;
    vector<int> worst;
    double max_time = 0;
    for (int trial = 0; trial < 10; ++trial) {
        permute(arr);
        vector<int> temp = arr;
        auto start = chrono::high_resolution_clock::now();
        heapSort(temp);
        auto end = chrono::high_resolution_clock::now();
        double time = chrono::duration<double, micro>(end - start).count();
        if (time > max_time) {
            max_time = time;
            worst = arr;
        }
    }
    return worst;
}

// 測量排序性能
pair<double, size_t> measureSort(void (*sortFunc)(vector<int>&), vector<int> arr,
    int repeat = 1000, bool reverse = false) {
    double totalTime = 0;
    size_t memoryBefore = getMemoryUsage();
    for (int i = 0; i < repeat; ++i) {
        vector<int> temp = arr;
        auto start = chrono::high_resolution_clock::now();
        if (sortFunc == quickSort && reverse) {
            quickSortHelper(temp, 0, temp.size() - 1, true);
        }
        else {
            sortFunc(temp);
        }
        auto end = chrono::high_resolution_clock::now();
        totalTime += chrono::duration<double, micro>(end - start).count();
    }
    size_t memoryAfter = getMemoryUsage();
    return make_pair(totalTime / repeat, max(memoryAfter, memoryBefore));
}

int main() {
    vector<int> sizes = { 500, 1000, 2000, 3000, 4000, 5000 };
    vector<string> sortNames = { "插入", "快速", "合併", "堆", "複合" };
    vector<void (*)(vector<int>&)> sortFuncs = { insertionSort, quickSort, mergeSort,
                                                heapSort, compositeSort };

    ofstream outFile("sorting_results.txt");
    outFile << fixed << setprecision(2);

    outFile << "排序演算法性能分析\n\n";

    // Average-case 測試
    outFile << "Average-case 執行時間（微秒）:\n";
    outFile << setw(10) << "n" << setw(15) << "插入" << setw(15) << "快速" << setw(15)
        << "合併" << setw(15) << "堆" << setw(15) << "複合" << "\n";
    vector<vector<double>> avgTimes(sizes.size(), vector<double>(sortFuncs.size()));
    vector<vector<size_t>> avgMemories(sizes.size(), vector<size_t>(sortFuncs.size()));

    for (size_t i = 0; i < sizes.size(); ++i) {
        int n = sizes[i];
        vector<vector<int>> testData(10, vector<int>(n));
        for (int j = 0; j < 10; ++j) {
            for (int k = 0; k < n; ++k) testData[j][k] = k;
            permute(testData[j]);
        }

        outFile << setw(10) << n;
        for (size_t j = 0; j < sortFuncs.size(); ++j) {
            double totalTime = 0;
            size_t maxMemory = 0;
            for (const auto& data : testData) {
                auto result = measureSort(sortFuncs[j], data, 100);
                totalTime += result.first;
                maxMemory = max(maxMemory, result.second);
            }
            avgTimes[i][j] = totalTime / 10;
            avgMemories[i][j] = maxMemory;
            outFile << setw(15) << avgTimes[i][j];
        }
        outFile << "\n";
    }

    outFile << "\nAverage-case 記憶體使用量（KB）:\n";
    outFile << setw(10) << "n" << setw(15) << "插入" << setw(15) << "快速" << setw(15)
        << "合併" << setw(15) << "堆" << setw(15) << "複合" << "\n";
    for (size_t i = 0; i < sizes.size(); ++i) {
        outFile << setw(10) << sizes[i];
        for (size_t j = 0; j < sortFuncs.size(); ++j) {
            outFile << setw(15) << avgMemories[i][j];
        }
        outFile << "\n";
    }

    // Worst-case 測試
    outFile << "\nWorst-case 執行時間（微秒）:\n";
    outFile << setw(10) << "n" << setw(15) << "插入" << setw(15) << "快速" << setw(15)
        << "合併" << setw(15) << "堆" << setw(15) << "複合" << "\n";
    vector<vector<double>> worstTimes(sizes.size(), vector<double>(sortFuncs.size()));
    vector<vector<size_t>> worstMemories(sizes.size(), vector<size_t>(sortFuncs.size()));

    for (size_t i = 0; i < sizes.size(); ++i) {
        int n = sizes[i];
        vector<vector<int>> worstData = {
            generateWorstCaseInsertion(n), generateWorstCaseQuick(n),
            generateWorstCaseMerge(n), generateWorstCaseHeap(n), generateWorstCaseMerge(n) };

        outFile << setw(10) << n;
        for (size_t j = 0; j < sortFuncs.size(); ++j) {
            auto result = measureSort(sortFuncs[j], worstData[j], 100, j == 1);
            worstTimes[i][j] = result.first;
            worstMemories[i][j] = result.second;
            outFile << setw(15) << worstTimes[i][j];
        }
        outFile << "\n";
    }

    outFile << "\nWorst-case 記憶體使用量（KB）:\n";
    outFile << setw(10) << "n" << setw(15) << "插入" << setw(15) << "快速" << setw(15)
        << "合併" << setw(15) << "堆" << setw(15) << "複合" << "\n";
    for (size_t i = 0; i < sizes.size(); ++i) {
        outFile << setw(10) << sizes[i];
        for (size_t j = 0; j < sortFuncs.size(); ++j) {
            outFile << setw(15) << worstMemories[i][j];
        }
        outFile << "\n";
    }

    // 最快演算法
    outFile << "\nAverage-case 最快演算法:\n";
    outFile << setw(10) << "n" << setw(20) << "最快演算法" << setw(15) << "時間（微秒）"
        << "\n";
    for (size_t i = 0; i < sizes.size(); ++i) {
        double minTime = avgTimes[i][0];
        size_t minIndex = 0;
        for (size_t j = 1; j < sortFuncs.size(); ++j) {
            if (avgTimes[i][j] < minTime) {
                minTime = avgTimes[i][j];
                minIndex = j;
            }
        }
        outFile << setw(10) << sizes[i] << setw(20) << sortNames[minIndex] << setw(15)
            << minTime << "\n";
    }

    outFile << "\nWorst-case 最快演算法:\n";
    outFile << setw(10) << "n" << setw(20) << "最快演算法" << setw(15) << "時間（微秒）"
        << "\n";
    for (size_t i = 0; i < sizes.size(); ++i) {
        double minTime = worstTimes[i][0];
        size_t minIndex = 0;
        for (size_t j = 1; j < sortFuncs.size(); ++j) {
            if (worstTimes[i][j] < minTime) {
                minTime = worstTimes[i][j];
                minIndex = j;
            }
        }
        outFile << setw(10) << sizes[i] << setw(20) << sortNames[minIndex] << setw(15)
            << minTime << "\n";
    }

    outFile.close();
    cout << "結果已寫入 sorting_results.txt\n";

    return 0;
}