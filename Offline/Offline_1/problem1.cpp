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
const int maxn = 1e6 + 5;
const int MOD = 1e9 + 7;
const ll INF = 1e18;
vector <pair<int, int>> adjw[maxn];
int dist[maxn][2];
int n, m;

void dijkstra(int src) {
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> pq;
    for (int i = 1; i <= n; i++) {
        dist[i][0] = INF;
        dist[i][1] = INF;
    }

    dist[src][0] = 0;
    pq.push({0, {src, 0}});
    while (!pq.empty()) {
        int u = pq.top().second.first;
        int d = pq.top().first;
        int state = pq.top().second.second;
        pq.pop();
        if (d != dist[u][state]) continue;

        for (auto edge : adjw[u]) {
            int v = edge.first;
            int w = edge.second;
            if (d + w < dist[v][state]) {
                dist[v][state] = d + w;
                pq.push({dist[v][state], {v, state}});
            }
            if(state == 0) {
                if (d + w / 2 < dist[v][1]) {
                    dist[v][1] = d + w / 2;
                    pq.push({dist[v][1], {v, 1}});
                }
            }
        }
    }
}


void solve() {
    cin >> n >> m;
    forn(i, m) {
        int u, v, cost;
        cin >> u >> v >> cost;
        adjw[u].push_back({v, cost});
    }
    dijkstra(1);
    cout << dist[n][1] << endl;
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