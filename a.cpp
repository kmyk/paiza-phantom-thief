#include <cstdio>
#include <cmath>
#include <random>
#include <chrono>
#include <vector>
#include <algorithm>
#include <array>
#include <set>
#include <map>
#include <queue>
#include <tuple>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <cassert>
#define repeat(i, n) for (int i = 0; (i) < int(n); ++(i))
#define repeat_from(i, m, n) for (int i = (m); (i) < int(n); ++(i))
#define repeat_reverse(i, n) for (int i = (n)-1; (i) >= 0; --(i))
#define repeat_from_reverse(i, m, n) for (int i = (n)-1; (i) >= int(m); --(i))
#define whole(x) begin(x), end(x)
#define unittest_name_helper(counter) unittest_ ## counter
#define unittest_name(counter) unittest_name_helper(counter)
#define unittest __attribute__((constructor)) void unittest_name(__COUNTER__) ()
using ll = long long;
using namespace std;
template <class T> inline void setmax(T & a, T const & b) { a = max(a, b); }
template <class T> inline void setmin(T & a, T const & b) { a = min(a, b); }
template <typename X, typename T> auto vectors(X x, T a) { return vector<T>(x, a); }
template <typename X, typename Y, typename Z, typename... Zs> auto vectors(X x, Y y, Z z, Zs... zs) { auto cont = vectors(y, z, zs...); return vector<decltype(cont)>(x, cont); }


chrono::high_resolution_clock::time_point clock_begin;
bool check_tle() {
    chrono::high_resolution_clock::time_point clock_end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(clock_end - clock_begin).count() >= 2900;
}
default_random_engine gen;

constexpr double eps = 1e-6;
constexpr int MAX_N = 100;
int n;
double dist[MAX_N + 2][MAX_N + 2];
double calculate_length(vector<int> const & path); // for debug
vector<int> solve_greedy() {
    vector<int> path(n + 1);
    vector<bool> used(n + 1);
    constexpr int root = 0;
    path[0] = root;
    used[root] = true;
    repeat (len, n) {
        int i = path[len];
        int k = -1;
        repeat (j, n + 1) if (not used[j]) {
            if (k == -1) k = j;
            if (dist[i][j] < dist[i][k]) {
                k = j;
            }
        }
        path[len + 1] = k;
        used[k] = true;
    }
    return path;
}
vector<int> solve_swap() {
    // vector<int> path(n + 2);
    // iota(whole(path), 0);
    // shuffle(path.begin() + 1, path.begin() + (n + 1), gen);
    vector<int> path = solve_greedy();
    path.push_back(n + 1);
    for (int iteration = 0; ; ++ iteration) {
        bool modified = false;
        repeat_from (i, 1, n) {
            repeat_from (j, i + 1, n + 1) {
                double delta = 0;
                delta -= dist[path[i - 1]][path[i]] + dist[path[i]][path[i + 1]];
                delta -= dist[path[j - 1]][path[j]] + dist[path[j]][path[j + 1]];
                swap(path[i], path[j]);
                delta += dist[path[i - 1]][path[i]] + dist[path[i]][path[i + 1]];
                delta += dist[path[j - 1]][path[j]] + dist[path[j]][path[j + 1]];
                if (delta < - eps) {
                    modified = true;
                } else {
                    swap(path[i], path[j]);
                }
            }
        }
        if (not modified) break;
        if (iteration % 128 == 0 and check_tle()) break;
    }
    assert (path.back() == n + 1);
    path.pop_back();
    return path;
}

double calculate_length(vector<int> const & path) {
    double acc = 0;
    repeat (i, path.size() - 1) {
        acc += dist[path[i]][path[i + 1]];
    }
    return acc;
}

vector<int> solve(int a_n, vector<int> const & y, vector<int> const & x) {
    assert (a_n <= MAX_N);
    // prepare
    clock_begin = chrono::high_resolution_clock::now();
    n = a_n;
    constexpr int root = 0;
    dist[root][root] = 0;
    repeat (i, n) {
        dist[root][i + 1] = hypot(y[i], x[i]);
        dist[i + 1][root] = hypot(y[i], x[i]);
        repeat (j, n) {
            dist[i + 1][j + 1] = hypot(y[j] - y[i], x[j] - x[i]);
        }
        dist[n + 1][i + 1] = 0;
        dist[i + 1][n + 1] = 0;
    }
    // solve
    vector<int> path = solve_swap();
    assert (path.size() == n + 1);
    // result
    fprintf(stderr, "result: %lfm\n", calculate_length(path));
    assert (path[0] == 0);
    repeat (i, n) path[i] = path[i + 1] - 1;
    path.pop_back();
    return path;
}

int main() {
    // input
    int n; scanf("%d", &n);
    vector<int> y(n), x(n);
    repeat (i, n) scanf("%d%d", &y[i], &x[i]);
    // solve
    vector<int> path = solve(n, y, x);
    // output
    repeat (i, n) {
        int j = path[i];
        printf("%d %d\n", y[j], x[j]);
    }
    return 0;
}
