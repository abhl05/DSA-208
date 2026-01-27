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
const int maxn = 1e7 + 43;
const int MOD = 1e9 + 7;
const ll INF = 1e18;
int arr[maxn];
vector<pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

bool bfs(vector<vector<int>> &adj, int s, int t, vector<int> &parent, int n) {
    vector<bool> visited(n, false);

    queue<int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; v++) {
            if (!visited[v] && adj[u][v] > 0) {
                if (v == t) {
                    parent[v] = u;
                    return true;
                }
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }

    return false;
}

void edmondsKarp(vector<vector<int>> &adj, vector<tuple<int, int, int>> &edges, int s, int t, int n) {
    int u, v;
    vector<vector<int>> rGraph(n, vector<int>(n));
    for (u = 0; u < n; u++) {
        for (v = 0; v < n; v++) {
            rGraph[u][v] = adj[u][v];
        }
    }

    vector<vector<int>> flow(n, vector<int>(n, 0)); 

    vector<int> parent(n);  
    int max_flow = 0;  

    while (bfs(rGraph, s, t, parent, n)) {
        int path_flow = INF;
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }
        
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            if(adj[u][v] > 0) {
                flow[u][v] += path_flow; 
            } else {
                flow[v][u] -= path_flow; 
            }
            rGraph[u][v] -= path_flow; 
            rGraph[v][u] += path_flow; 
        }
        
        max_flow += path_flow;
    }

    cout << max_flow << endl;
    for(auto &[u, v, cap] : edges) {
        cout << u << " " << v << " " << flow[u][v] << "/" << cap << endl;
    }
}

void solve() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> capacity(n + 1, vector<int>(n + 1, 0));
    vector<tuple<int, int, int>> edges;

    forn(i, m) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
        capacity[u][v] += w; 
    }
    int s; 
    int t; 
    cin >> s >> t;
    edmondsKarp(capacity, edges, s, t, n + 1);
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