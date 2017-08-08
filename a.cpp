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
chrono::high_resolution_clock::time_point clock_end;
bool check_tle() {
    clock_end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(clock_end - clock_begin).count() >= 2950;
}
default_random_engine gen;

constexpr double eps = 1e-6;
constexpr int MAX_N = 100;
int n;
double dist[MAX_N + 10][MAX_N + 10] = {};
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
vector<int> solve_vertex_swap() {
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

vector<int> solve_2opt() {
    vector<int> path = solve_greedy();
    path.push_back(n + 1);
    auto d = [&](int i, int j) { return dist[path[i]][path[j]]; };
    for (int iteration = 0; ; ++ iteration) {
        bool modified = false;
        repeat_from (i, 1, n) {
            repeat_from (j, i + 1, n + 1) {
                if (path[i + 1] == path[j]) continue;
                double delta = 0;
                delta -= d(i - 1, i) + d(j, j + 1);
                delta += d(i - 1, j) + d(i, j + 1);
                if (delta < - eps) {
                    reverse(path.begin() + i, path.begin() + (j + 1));
                    modified = true;
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

vector<int> solve_bruteforce() {
    vector<int> result;
    double result_length = INFINITY;
    vector<int> path(n + 1);
    iota(whole(path), 0);
    do {
        if (path.front() != 0) break;
        double path_length = calculate_length(path);
        if (path_length < result_length) {
            result = path;
            result_length = path_length;
        }
    } while (next_permutation(whole(path)));
    return result;
}

vector<int> solve_2opt_sa() {
    vector<int> path = solve_greedy();
    path.push_back(n + 1);
    double acc = calculate_length(path);
    vector<int> best = path;
    double best_acc = acc;
    auto d = [&](int i, int j) { return dist[path[i]][path[j]]; };
    double temp = 0;
    for (int iteration = 0; ; ++ iteration) {
        if (check_tle()) break;
        temp = max<int>(1, 2600 - chrono::duration_cast<chrono::milliseconds>(clock_end - clock_begin).count());
        repeat_from (i, 1, n) {
            repeat_from (j, i + 1, n + 1) {
                double delta = 0;
                delta -= d(i - 1, i) + d(j, j + 1);
                delta += d(i - 1, j) + d(i, j + 1);
                if (delta < - eps or bernoulli_distribution(exp(- delta / temp))(gen)) {
                    reverse(path.begin() + i, path.begin() + (j + 1));
                    acc += delta;
                    if (acc < best_acc) {
                        best = path;
                        best_acc = acc;
                    }
                }
            }
        }
    }
    path = best;
    assert (path.back() == n + 1);
    path.pop_back();
    return path;
}

vector<int> solve_3opt() {
    vector<int> path = solve_greedy();
    path.push_back(n + 1);
    double acc = calculate_length(path);
    vector<int> best = path;
    double best_acc = acc;
    auto d = [&](int i, int j) { return dist[path[i]][path[j]]; };
    for (int iteration = 0; ; ++ iteration) {
        if (check_tle()) break;
        repeat_from (i, 1, n - 1) {
            repeat_from (j, i + 1, n) {
                repeat_from (k, j + 1, n + 1) {
                    double delta[5];
                    delta[0] = d(i, i + 1) + d(j, j + 1) + d(k, k+1); // the original distance
                    delta[1] = d(i, k) + d(j + 1, i + 1) + d(j, k + 1);
                    delta[2] = d(i, j + 1) + d(k, j) + d(i + 1, k + 1);
                    delta[3] = d(i, j + 1) + d(k, i + 1) + d(j, k + 1);
                    delta[4] = d(i, j) + d(i + 1, k) + d(j + 1, k + 1);
                    int l_min = 0;
                    repeat_from (l, 1, 5) {
                        if (delta[l] < delta[l_min]) l_min = l;
                    }
                    if (l_min == 0) continue;
                    vector<int> cache;
                    if (l_min == 1) {
                        reverse_copy(path.begin() + (j + 1), path.begin() + (k + 1), back_inserter(cache));
                        copy(path.begin() + (i + 1), path.begin() + (j + 1), back_inserter(cache));
                    } else if (l_min == 2) {
                        copy(path.begin() + (j + 1), path.begin() + (k + 1), back_inserter(cache));
                        reverse_copy(path.begin() + (i + 1), path.begin() + (j + 1), back_inserter(cache));
                    } else if (l_min == 3) {
                        copy(path.begin() + (j + 1), path.begin() + (k + 1), back_inserter(cache));
                        copy(path.begin() + (i + 1), path.begin() + (j + 1), back_inserter(cache));
                    } else if (l_min == 4) {
                        reverse_copy(path.begin() + (i + 1), path.begin() + (j + 1), back_inserter(cache));
                        reverse_copy(path.begin() + (j + 1), path.begin() + (k + 1), back_inserter(cache));
                    }
                    copy(whole(cache), path.begin() + (i + 1));
                    acc += delta[l_min] - delta[0];
                    if (acc < best_acc) {
                        best = path;
                        best_acc = acc;
                    }
                }
            }
        }
    }
    path = best;
    assert (path.back() == n + 1);
    path.pop_back();
    return path;
}

vector<int> solve_3opt_sa() {
    vector<int> path = solve_greedy();
    path.push_back(n + 1);
    double acc = calculate_length(path);
    vector<int> best = path;
    double best_acc = acc;
    auto d = [&](int i, int j) { return dist[path[i]][path[j]]; };
    double temp = 0;
    for (int iteration = 0; ; ++ iteration) {
        if (check_tle()) break;
        temp = max<int>(1, 2600 - chrono::duration_cast<chrono::milliseconds>(clock_end - clock_begin).count());
        repeat_from (i, 1, n - 1) {
            repeat_from (j, i + 1, n) {
                repeat_from (k, j + 1, n + 1) {
                    double prev = d(i, i + 1) + d(j, j + 1) + d(k, k+1);
                    double next[4];
                    next[0] = d(i, k) + d(j + 1, i + 1) + d(j, k + 1);
                    next[1] = d(i, j + 1) + d(k, j) + d(i + 1, k + 1);
                    next[2] = d(i, j + 1) + d(k, i + 1) + d(j, k + 1);
                    next[3] = d(i, j) + d(i + 1, k) + d(j + 1, k + 1);
                    int l_min = 3;
                    repeat (l, 3) {
                        if (next[l] < next[l_min]) l_min = l;
                    }
                    double delta = next[l_min] - prev;
                    if (delta < - eps or bernoulli_distribution(exp(- delta / temp))(gen)) {
                        vector<int> cache;
                        if (l_min == 0) {
                            reverse_copy(path.begin() + (j + 1), path.begin() + (k + 1), back_inserter(cache));
                            copy(path.begin() + (i + 1), path.begin() + (j + 1), back_inserter(cache));
                        } else if (l_min == 1) {
                            copy(path.begin() + (j + 1), path.begin() + (k + 1), back_inserter(cache));
                            reverse_copy(path.begin() + (i + 1), path.begin() + (j + 1), back_inserter(cache));
                        } else if (l_min == 2) {
                            copy(path.begin() + (j + 1), path.begin() + (k + 1), back_inserter(cache));
                            copy(path.begin() + (i + 1), path.begin() + (j + 1), back_inserter(cache));
                        } else if (l_min == 3) {
                            reverse_copy(path.begin() + (i + 1), path.begin() + (j + 1), back_inserter(cache));
                            reverse_copy(path.begin() + (j + 1), path.begin() + (k + 1), back_inserter(cache));
                        }
                        copy(whole(cache), path.begin() + (i + 1));
                        acc += delta;
                        if (acc < best_acc) {
                            best = path;
                            best_acc = acc;
                        }
                    }
                }
            }
        }
    }
    path = best;
    assert (path.back() == n + 1);
    path.pop_back();
    return path;
}

string vector_int_to_string(vector<int> const & xs) {
    string s;
    for (int x : xs) s += char(x);
    return s;
}
vector<int> string_to_vector_int(string const & s) {
    vector<int> xs;
    for (char c : s) xs.push_back(c);
    return xs;
}

vector<int> solve_both_sa() {
    vector<int> path = solve_greedy();
    path.push_back(n + 1);
    double acc = calculate_length(path);
    vector<int> best = path;
    double best_acc = acc;
    auto d = [&](int i, int j) { return dist[path[i]][path[j]]; };
    double temp = 0;
    vector<int> index_order(n + 1);
    iota(whole(index_order), 0);
    int iteration = 0;
    for (; ; ++ iteration) {
        if (check_tle()) break;
        temp = max<int>(1, 2600 - chrono::duration_cast<chrono::milliseconds>(clock_end - clock_begin).count()) * 7;
        shuffle(whole(index_order), gen);
        if (iteration % 2 == 0) {
            repeat_from (i1, 1, n) {
                repeat_from (j1, i1 + 1, n + 1) {
                    int i = index_order[i1];
                    int j = index_order[j1];
                    if (i > j) swap(i, j);
                    double delta = 0;
                    delta -= d(i - 1, i) + d(j, j + 1);
                    delta += d(i - 1, j) + d(i, j + 1);
                    if (delta < - eps or bernoulli_distribution(exp(- delta / temp))(gen)) {
                        reverse(path.begin() + i, path.begin() + (j + 1));
                        acc += delta;
                        if (acc < best_acc) {
                            best = path;
                            best_acc = acc;
                        }
                    }
                }
            }
        } else {
            repeat_from (i1, 1, n - 1) {
                repeat_from (j1, i1 + 1, n) {
                    repeat_from (k1, j1 + 1, n + 1) {
                        int i = index_order[i1];
                        int j = index_order[j1];
                        int k = index_order[k1];
                        if (i > j) swap(i, j);
                        if (j > k) swap(j, k);
                        if (i > j) swap(i, j);
                        double prev = d(i, i + 1) + d(j, j + 1) + d(k, k+1);
                        double next[4];
                        next[0] = d(i, k) + d(j + 1, i + 1) + d(j, k + 1);
                        next[1] = d(i, j + 1) + d(k, j) + d(i + 1, k + 1);
                        next[2] = d(i, j + 1) + d(k, i + 1) + d(j, k + 1);
                        next[3] = d(i, j) + d(i + 1, k) + d(j + 1, k + 1);
                        int l_min = 3;
                        repeat (l, 3) {
                            if (next[l] < next[l_min]) l_min = l;
                        }
                        double delta = next[l_min] - prev;
                        if (delta < - eps or bernoulli_distribution(exp(- delta / temp))(gen)) {
                            vector<int> cache;
                            if (l_min == 0) {
                                reverse_copy(path.begin() + (j + 1), path.begin() + (k + 1), back_inserter(cache));
                                copy(path.begin() + (i + 1), path.begin() + (j + 1), back_inserter(cache));
                            } else if (l_min == 1) {
                                copy(path.begin() + (j + 1), path.begin() + (k + 1), back_inserter(cache));
                                reverse_copy(path.begin() + (i + 1), path.begin() + (j + 1), back_inserter(cache));
                            } else if (l_min == 2) {
                                rotate(path.begin() + (i + 1), path.begin() + (j + 1), path.begin() + (k + 1));
                            } else if (l_min == 3) {
                                reverse(path.begin() + (i + 1), path.begin() + (j + 1));
                                reverse(path.begin() + (j + 1), path.begin() + (k + 1));
                            }
                            copy(whole(cache), path.begin() + (i + 1));
                            acc += delta;
                            if (acc < best_acc) {
                                best = path;
                                best_acc = acc;
                            }
                        }
                    }
                }
            }
        }
    }
    fprintf(stderr, "iteration: %d\n", iteration);
    path = best;
    assert (path.back() == n + 1);
    path.pop_back();
    return path;
}

vector<int> solve(int a_n, vector<int> const & y, vector<int> const & x) {
    assert (a_n <= MAX_N);
    // prepare
    clock_begin = clock_end = chrono::high_resolution_clock::now();
    gen = default_random_engine(random_device()());
    n = a_n;
    constexpr int root = 0;
    dist[root][root] = 0;
    repeat (i, n) {
        dist[root][i + 1] = hypot(y[i], x[i]);
        dist[i + 1][root] = hypot(y[i], x[i]);
        repeat (j, n) {
            dist[i + 1][j + 1] = hypot(y[j] - y[i], x[j] - x[i]);
        }
    }
    // solve
    vector<int> path = n <= 6 ? solve_bruteforce() : solve_both_sa();
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
    // check
    sort(whole(path));
    path.erase(unique(whole(path)), path.end());
    assert (path.size() == n);
    assert (path.front() == 0 and path.back() == n - 1);
    return 0;
}
