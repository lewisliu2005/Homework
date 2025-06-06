#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm> // 為 min_element
#include <iterator>  // 為 distance

using namespace std;

int main() {
    // 基本參數
    double t_s = 0.08;  // 尋道時間
    double t_l = 0.02;  // 延遲時間
    double t_s_plus_t_l = t_s + t_l;  // t_s + t_l = 0.1 秒
    int m = 64;  // 第一輪產生的 run 數量
    int n = 200000;  // 總任務數
    int S = 2000;  // 系統並行度
    double t_t = 0.001;  // 每個任務的傳輸時間，1ms = 0.001秒

    // K 值範圍
    vector<int> k_values;
    for (int k = 2; k <= 600; k++) {
        k_values.push_back(k);
    }

    // 計算對應的總 input 時間
    vector<double> tinput_total_values;
    for (int k : k_values) {
        // 計算合併輪數: log_k(m) = log(m)/log(k)
        double merge_rounds = log(static_cast<double>(m)) / log(static_cast<double>(k));

        // 計算公式: t_input = ((ceil((n/m)/(S/(k+1))) * (t_s + t_l) + (n * t_t)) * log_k(m)
        double batch_size = static_cast<double>(S) / (k + 1);
        double batches = ceil((static_cast<double>(n) / m) / batch_size);
        double tinput_total = ((batches * t_s_plus_t_l) + (n * t_t)) * merge_rounds;
        tinput_total_values.push_back(tinput_total);
    }

    // 打印關鍵數值
    vector<int> key_points = { 2, 4, 8, 16, 32, 50, 100, 200, 300, 400, 500, 600 };
    cout << "關鍵K值的計算結果:" << endl;
    cout << "----------------------------------------" << endl;
    for (int k : key_points) {
        auto it = find(k_values.begin(), k_values.end(), k);
        if (it != k_values.end()) {
            size_t idx = distance(k_values.begin(), it); // 使用 distance 計算索引
            double merge_rounds = log(static_cast<double>(m)) / log(static_cast<double>(k));
            double batch_size = static_cast<double>(S) / (k + 1);
            double batches = ceil((static_cast<double>(n) / m) / batch_size);
            double tinput_total = ((batches * t_s_plus_t_l) + (n * t_t)) * merge_rounds;
            cout << "K=" << k << ": 合併輪數=" << merge_rounds << ", 批次數="
                << static_cast<int>(batches) << ", 總Input時間=" << tinput_total << "秒" << endl;
        }
    }

    // 找出最優 K 值（總時間最短）
    auto min_it = min_element(tinput_total_values.begin(), tinput_total_values.end());
    size_t min_idx = distance(tinput_total_values.begin(), min_it); // 使用 distance 計算索引
    int optimal_k = k_values[min_idx];
    double min_time = tinput_total_values[min_idx];
    cout << "\n最優K值: K=" << optimal_k << ", 總Input時間=" << min_time << "秒" << endl;

    return 0;
}