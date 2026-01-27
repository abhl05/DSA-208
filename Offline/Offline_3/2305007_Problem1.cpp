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


void prim(int root, vector <vector<pair<int, int>>> &adj, int n) {
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq;
    vector <bool> in_mst(n + 1, false);
    vector <pair<int, int>> selected_edges;
    int mst_cost = 0;
    int edge_cnt = 0;
    
    in_mst[root] = true; 
    
    for(auto [weight, neighbor] : adj[root]) {
        pq.push({weight, root, neighbor});
    }
    
    while(!pq.empty() && edge_cnt < n - 1) {
        auto [cost, u, v] = pq.top();
        pq.pop();
        
        if(in_mst[v]) continue;  
        
        in_mst[v] = true;
        mst_cost += cost;
        selected_edges.push_back({u, v});
        edge_cnt++;
        
        for(auto [w, neighbor] : adj[v]) {
            if(!in_mst[neighbor]) {
                pq.push({w, v, neighbor});
            }
        }
    }
    cout << "Total weight " << mst_cost << endl;
    cout << "Root node " << root << endl;
    for(auto [u, v] : selected_edges) {
        cout << u << " " << v << endl;
    }
}

signed main() {
    fastIO();
    int n, m;
    cin >> n >> m;
    vector <vector<pair<int, int>>> adj(n + 1);

    for(int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({w, v});
        adj[v].push_back({w, u});
    }

    int root;
    cin >> root;
    prim(root, adj, n);

    return 0;
}