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
const int MOD = 1e9 + 7;
const ll INF = 1e18;

void solve() {
    int n, m;
    cin >> n;

    unordered_map<string, int> currency_idx;
    forn(i, n) {
        string currency;
        cin >> currency;
        currency_idx[currency] = i + 1;
    }

    cin >> m;
    vector <tuple<int, int, double>> edges;

    vector <vector<double>> dist(n + 1, vector<double>(n + 1, INF));
    for(int i = 1; i <= n; i++) {
        dist[i][i] = 0;
    }

    forn(i, m) {
        string u, v;
        double rate;
        cin >> u >> rate >> v;
        int u_node = currency_idx[u];
        int v_node = currency_idx[v];
        dist[u_node][v_node] = min(dist[u_node][v_node], -log(rate));
    }

    for(int k = 1; k <= n; k++) {
        for(int i = 1; i <= n; i++) {
            if(dist[i][k] >= INF) continue;
            for(int j = 1; j <= n; j++) {
                if(dist[k][j] >= INF) continue;
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    for(int i = 1; i <= n; i++) {
        if(dist[i][i] < 0) {
            cout << "Yes" << endl;
            return;
        }
    }
    cout << "No" << endl;
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








    // forn(i, m) {
    //     string u, v;
    //     double rate;
    //     cin >> u >> rate >> v;
    //     int u_node = currency_idx[u];
    //     int v_node = currency_idx[v];
    //     edges.push_back({u_node, v_node, -log(rate)});
    // }

    // vector <double> dist(n + 1, 0.0);
    
    // for(int i = 0; i < n - 1; i++) {
    //     for(auto [u, v, w] : edges) {
    //         if(dist[v] > dist[u] + w) {
    //             dist[v] = dist[u] + w;
    //         }
    //     }
    // }
    // for (auto [u, v, w] : edges) {
    //     if (dist[v] > dist[u] + w) {
    //         cout << "Yes" << endl;
    //         return;
    //     }
    // }
    // cout << "No" << endl;