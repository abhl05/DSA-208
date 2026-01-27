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
    int n, m, q;
    cin >> n >> m >> q;

    vector <vector<int>> dist(n + 1, vector<int>(n + 1, INF));
    for(int i = 1; i <= n; i++) {
        dist[i][i] = 0;
    }

    forn(i, m) {
        int u, v, w;
        cin >> u >> v >> w;
        dist[u][v] = min(dist[u][v], w);
        dist[v][u] = min(dist[v][u], w);
    }

    for(int k = 1; k <= n; k++) {
        for(int i = 1; i <= n; i++) {
            for(int j = 1; j <= n; j++) {
                if(dist[i][k] != INF && dist[k][j] != INF) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    while(q--) {
        int u, v;
        cin >> u >> v;
        if(dist[u][v] == INF) {
            cout << -1 << endl;
        } else {
            cout << dist[u][v] << endl;
        }
    }
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