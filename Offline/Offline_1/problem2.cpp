#include <bits/stdc++.h>
using namespace std;
#define int long long
typedef long long ll;
#define vll vector<ll>
#define vllp vector<pair<ll, ll>>
#define endl '\n'
#define fastIO() ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
#define vin(x) for (auto &i : x) cin >> i;
#define vout(x) for (auto &i : x) cout << i << " ";
#define all(x) x.begin(), x.end()
#define forn(i, n) for(int i = 0; i < (n); i++)
const int maxn = 1e5 + 43;
const int MOD = 1e9 + 7;
const ll INF = 1e13;
int arr[maxn];
vector <pair<int, int>> adjw[maxn];
vector <int> adj[maxn];
vector<int> dist(maxn, 0);
vector<int> parent(maxn, -1);
int n, m;

void bellman_ford(int src) {
    dist[src] = 0;
    int x;
    for (int i = 1; i <= n; i++) {
        x = -1;
        for (int u = 1; u <= n; u++) {
            for (auto edge : adjw[u]) {
                int v = edge.first;
                int w = edge.second;
                if (dist[u] != INF && dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    x = v;
                }
            }
        }
    }

    if(x == -1) {
        // cout << "NO" << endl;
        cout << -1 << endl;
        return;
    }

    for(int i = 0; i < n; i++) {
        x = parent[x];
    }

    vector<int> cycle;
    int v = x;
    while(true) {
        cycle.push_back(v);
        v = parent[v];
        if(v == x) break;
    }
    reverse(cycle.begin(), cycle.end());

    // cout << "YES" << endl;

    for(auto c : cycle) {
        cout << c << " ";
    }
    // cout << cycle.front() << " ";
    
    cout << endl;
}

void solve() {
    cin >> n >> m;

    forn(i, m) {
        int u, v, w;
        cin >> u >> v >> w;
        adjw[u].push_back({v, w});
    }
    bellman_ford(1);
}

signed main() {
    fastIO();
    int tt = 1;
    // cin >> tt;
    while (tt--) {
        solve();
    }
    return 0;
}