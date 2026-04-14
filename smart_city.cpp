#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v, w;
};

class DSU {
    vector<int> parent, rank;
public:
    DSU(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for(int i=0;i<n;i++) parent[i]=i;
    }

    int find(int x) {
        if(parent[x]!=x)
            parent[x]=find(parent[x]);
        return parent[x];
    }

    void unite(int x,int y) {
        int px=find(x), py=find(y);
        if(px!=py){
            if(rank[px]<rank[py]) parent[px]=py;
            else if(rank[px]>rank[py]) parent[py]=px;
            else {
                parent[py]=px;
                rank[px]++;
            }
        }
    }
};

class SmartCity {
    int V;
    vector<vector<pair<int,int>>> adj;
    vector<Edge> edges;

public:
    void loadGraph(string filename) {
        ifstream file(filename);
        if(!file){
            cout<<"Error opening file!\n";
            return;
        }

        int E;
        file>>V>>E;
        adj.assign(V, {});

        for(int i=0;i<E;i++){
            int u,v,w;
            file>>u>>v>>w;
            adj[u].push_back({v,w});
            adj[v].push_back({u,w});
            edges.push_back({u,v,w});
        }

        cout<<"Graph loaded successfully!\n";
    }

    void dijkstra(int src) {
        vector<int> dist(V, INT_MAX), parent(V,-1);
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;

        dist[src]=0;
        pq.push({0,src});

        while(!pq.empty()){
            int u=pq.top().second;
            pq.pop();

            for(auto &e: adj[u]){
                int v=e.first, w=e.second;
                if(dist[v] > dist[u]+w){
                    dist[v]=dist[u]+w;
                    parent[v]=u;
                    pq.push({dist[v],v});
                }
            }
        }

        ofstream out("results.txt", ios::app);
        out<<"\nDijkstra from "<<src<<"\n";

        cout<<"\nShortest Paths:\n";
        for(int i=0;i<V;i++){
            cout<<"Node "<<i<<" Cost "<<dist[i]<<" Path: ";
            out<<"Node "<<i<<" Cost "<<dist[i]<<" Path: ";

            vector<int> path;
            for(int v=i; v!=-1; v=parent[v])
                path.push_back(v);

            reverse(path.begin(), path.end());

            for(int x: path){
                cout<<x<<" ";
                out<<x<<" ";
            }
            cout<<endl;
            out<<endl;
        }
    }
    void kruskal() {
        sort(edges.begin(), edges.end(), [](Edge a, Edge b){
            return a.w < b.w;
        });

        DSU dsu(V);
        int cost=0;

        cout<<"\nMST Edges:\n";
        ofstream out("results.txt", ios::app);
        out<<"\nMST Edges:\n";

        for(auto &e: edges){
            if(dsu.find(e.u)!=dsu.find(e.v)){
                dsu.unite(e.u,e.v);
                cost+=e.w;

                cout<<e.u<<" - "<<e.v<<" = "<<e.w<<endl;
                out<<e.u<<" - "<<e.v<<" = "<<e.w<<endl;
            }
        }

        cout<<"Total MST Cost: "<<cost<<endl;
        out<<"Total MST Cost: "<<cost<<endl;
    }

    void updateTraffic() {
        srand(time(0));
        for(int i=0;i<V;i++){
            for(auto &e: adj[i]){
                e.second += rand()%5;
            }
        }
        cout<<"Traffic updated dynamically!\n";
    }
};

void sortingAnalysis() {
    int n;
    cout<<"Enter number of elements: ";
    cin>>n;

    vector<int> arr(n);
    cout<<"Enter elements:\n";
    for(int i=0;i<n;i++) cin>>arr[i];

    vector<int> a=arr, b=arr;

    clock_t s,e;

    s=clock();
    sort(a.begin(), a.end());
    e=clock();
    double t1=(double)(e-s)/CLOCKS_PER_SEC;

    s=clock();
    stable_sort(b.begin(), b.end());
    e=clock();
    double t2=(double)(e-s)/CLOCKS_PER_SEC;

    cout<<"\nSorted (Quick/STL): ";
    for(int x:a) cout<<x<<" ";

    cout<<"\nSorted (Merge/Stable): ";
    for(int x:b) cout<<x<<" ";

    cout<<"\nTime Quick: "<<t1;
    cout<<"\nTime Merge: "<<t2<<endl;

    ofstream out("sorting.txt");
    out<<"Quick Sort Time: "<<t1<<"\n";
    out<<"Merge Sort Time: "<<t2<<"\n";
}

int main() {
    SmartCity city;
    city.loadGraph("graph.txt");

    int choice;

    do {
        cout<<"\nSMART CITY SYSTEM\n";
        cout<<"1. Shortest Path (Dijkstra)\n";
        cout<<"2. Minimum Spanning Tree (Kruskal)\n";
        cout<<"3. Update Traffic\n";
        cout<<"4. Sorting Analysis\n";
        cout<<"5. Exit\n";
        cout<<"Enter choice: ";
        cin>>choice;

        switch(choice) {
            case 1: {
                int src;
                cout<<"Enter source node: ";
                cin>>src;
                city.dijkstra(src);
                break;
            }
            case 2:
                city.kruskal();
                break;
            case 3:
                city.updateTraffic();
                break;
            case 4:
                sortingAnalysis();
                break;
        }

    } while(choice != 5);

    return 0;
}