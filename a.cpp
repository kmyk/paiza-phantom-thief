#include <cstdio>
#include <cmath>
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

double calculate_length(vector<int> const & path, vector<vector<double> > const & dist) {
    double acc = 0;
    repeat (i, path.size() - 1) {
        acc += dist[path[i]][path[i + 1]];
    }
    return acc;
}

constexpr int root = 0;
int main() {
    // input
    int n; scanf("%d", &n);
    vector<int> y(n), x(n);
    repeat (i, n) scanf("%d%d", &y[i], &x[i]);
    // solve
    auto dist = vectors(n + 1, n + 1, double());
    dist[root][root] = 0;
    repeat (i, n) {
        dist[root][i + 1] = hypotl(y[i], x[i]);
        dist[i + 1][root] = hypotl(y[i], x[i]);
        repeat (j, n) {
            dist[i + 1][j + 1] = hypotl(y[j] - y[i], x[j] - x[i]);
        }
    }
    vector<int> path(n + 1);
    vector<bool> used(n + 1);
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
    // output
    repeat (i, n) {
        int j = path[i + 1] - 1;
        printf("%d %d\n", y[j], x[j]);
    }
    fprintf(stderr, "result: %lfm\n", calculate_length(path, dist));
    return 0;
}
