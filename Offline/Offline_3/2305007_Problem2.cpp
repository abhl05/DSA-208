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

class DSU {
    vector<int> parent, rank;
public:

    DSU(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    int find(int i) {
        if (parent[i] != i)
            parent[i] = find(parent[i]);
        return parent[i];
    }

    void unite(int x, int y) {
        int parentX = find(x);
        int parentY = find(y);

        if (parentX != parentY) {
            if (rank[parentX] < rank[parentY])
                parent[parentX] = parentY;
            else if (rank[parentX] > rank[parentY])
                parent[parentY] = parentX;
            else {
                parent[parentY] = parentX;
                rank[parentX]++;
            }
        }
    }
};

void kruskal(int n, vector<tuple<int, int, int>> &edges) {
    sort(edges.begin(), edges.end());
    int mst_cost = 0;
    vector <pair<int, int>> selected_edges;

    DSU dsu(n);
    for(auto [w, u, v] : edges) {
        if(dsu.find(u) != dsu.find(v)) {
            dsu.unite(u, v);
            mst_cost += w;
            selected_edges.push_back({u, v});
        }
    }

    cout << "Total weight " << mst_cost << endl;
    for(auto [u, v] : selected_edges) {
        cout << u << " " << v << endl;
    }
}

signed main() {
    fastIO();
    int n, m;
    cin >> n >> m;
    vector <tuple<int, int, int>> edges;

    for(int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({w, u, v});
    }

    int root;
    cin >> root;
    kruskal(n, edges);

    return 0;
}