#include<bits/stdc++.h>

#define Input                   freopen("in.txt","r",stdin)
#define Output                  freopen("out.txt","w",stdout)
#define ll                      long long int
#define ull                     unsigned long long int
#define pii                     pair<int,int>
#define pll                     pair<ll,ll>
#define sc                      scanf
#define scin(x)                 sc("%d",&(x))
#define scin2(x,y)              sc("%d %d",&(x),&(y))
#define scln(x)                 sc("%lld",&(x))
#define scln2(x,y)              sc("%lld %lld",&(x),&(y))
#define pf                      printf
#define all(a)                  (a.begin()),(a.end())
#define UNIQUE(X)               (X).erase(unique(all(X)),(X).end())
#define SORT_UNIQUE(c)          (sort(c.begin(),c.end()), c.resize(distance(c.begin(),unique(c.begin(),c.end()))))
#define ms(a,b)                 memset(a,b,sizeof(a))
#define pb(a)                   push_back(a)
#define mp                      make_pair
#define db                      double
#define EPS                     10E-10
#define ff                      first
#define ss                      second
#define sqr(x)                  (x)*(x)
#define vi                      vector<int>
#define vl                      vector<ll>
#define vii                     vector<vector<int> >
#define vll                     vector<vector<ll> >
#define DBG                     pf("HI\n")
#define MOD                     1000000007
#define CIN                     ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define RUN_CASE(t,T)           for(__typeof(t) t=1;t<=T;t++)
#define CASE(t)                 printf("Case %d: ",t)
#define CASEl(t)                printf("Case %d:\n",t)
#define intlimit                2147483690
#define longlimit               9223372036854775800
#define infinity                (1<<28)
#define gcd(a,b)                __gcd(a,b)
#define lcm(a,b)                (a)*(b)/gcd(a,b)
#define mxx                     123456789
#define PI                      2*acos(0.0)
#define rep(i,a,b)              for(__typeof(i) i=a;i<=b;i++)
#define rev(i,a,b)              for(__typeof(i) i=a;i>=b;i--)

using namespace std;

/**
Problem: You r given a graph with n nodes & m edges. For each edge[u,v] calculate
MST when taking that edge is must in that MST.

MST containing[u,v] = Exact MST - MAX edge cost in path(u to v) of exact MST Tree + cost[u,v]
Here, we can calculate max edge cost in path u to v of  exact MST tree by LCA.
**/

#define sz 100005
struct edge
{
    int u,v,cost;
    edge(){};
    edge(int _u,int _v,int _cost)
    {
        u = _u;
        v = _v;
        cost = _cost;
    }
    bool operator < (const edge &p)const{
        return cost < p.cost;
    }
};
vector<edge>ed;
int par[sz];
vi mstgraph[sz];
map<pii , int>cost;

///Calculate MST
void makeset(int n)
{
    for(int i=1 ; i<=n ; i++)   par[i] = i;
}

int Find(int x)
{
    if(par[x] == x)
        return x;
    return par[x] = Find(par[x]);
}

int MST(int n)
{
    sort(ed.begin() , ed.end());
    makeset(n);
    int cnt=0 , s=0;
    for(int i=0 ; i<(int)ed.size() ; i++){
        int u = Find(ed[i].u);
        int v = Find(ed[i].v);
        if(u != v){
            par[u] = v;
            s += ed[i].cost;
            cnt++;
            mstgraph[ed[i].u].pb(ed[i].v);
            mstgraph[ed[i].v].pb(ed[i].u);
            cost[make_pair(ed[i].u , ed[i].v)] = ed[i].cost;
            cost[make_pair(ed[i].v , ed[i].u)] = ed[i].cost;
            if(cnt == n-1)
                break;
        }
    }
    if(cnt == n-1)
        return s;   ///All nodes r connected
    else
        return -1;  ///All nodes r not connected
}

///LCA part
int vis[sz],level[sz],parent[sz];
void BFS(int start)
{
    ms(parent , -1);
    ms(vis , 0);
    ms(level , 0);
    queue<int>q;
    vis[start] = 1;
    level[start] = 0;
    q.push(start);
    while(!q.empty()){
        int u = q.front();
        q.pop();
        for(auto v : mstgraph[u]){
            if(!vis[v]){
                vis[v] = 1;
                level[v] = 1+level[u];
                parent[v] = u;
                q.push(v);
            }
        }
    }
}

struct info
{
    int par,mxcost;
    info(){}
    info(int a,int b)
    {
        par = a; mxcost = b;
    }
}sptable[sz][20];

void Init(int n)
{
    for(int i=0 ; i<=n ; i++){
        for(int j=0 ; j<=20 ; j++)
            sptable[i][j] = info(-1 , 0);
    }
}

void LCA_Init(int n)
{
    Init(n);
    for(int i=1 ; i<=n ; i++){
        sptable[i][0].par = parent[i];
        if(parent[i] != -1)
            sptable[i][0].mxcost = max(sptable[i][0].mxcost , cost[make_pair(i , parent[i])]);
    }
    for(int j=1 ; (1<<j)<=n ; j++){
        for(int i=1 ; i<=n ; i++){
            if(sptable[i][j-1].par != -1){
                sptable[i][j].par = sptable[sptable[i][j-1].par][j-1].par;
                sptable[i][j].mxcost = max(sptable[i][j].mxcost , max(sptable[i][j-1].mxcost , sptable[sptable[i][j-1].par][j-1].mxcost));
            }
        }
    }
}

int LCA_Query(int n,int p,int q)
{
    if(p == q)
        return 0;
    int ret = 0 , Log = 0;
    if(level[p] < level[q])
        swap(p , q);
    while(1){
        int nxt = Log+1;
        if((1<<nxt) > level[p])
            break;
        Log++;
    }
    for(int i=Log ; i>=0 ; i--){    ///Take them in equal level
        if(level[p]-(1<<i) >= level[q]){
            ret = max(ret , sptable[p][i].mxcost);
            p = sptable[p][i].par;
        }
    }
    if(p == q)
        return ret;
    for(int i=Log ; i>=0 ; i--){
        if(sptable[p][i].par != -1 && sptable[p][i].par != sptable[q][i].par){
            ret = max(ret , max(sptable[p][i].mxcost , sptable[q][i].mxcost));
            p = sptable[p][i].par;
            q = sptable[q][i].par;
        }
    }
    ret = max(ret , max(cost[make_pair(p , parent[p])] , cost[make_pair(q , parent[q])]));
    return ret;
}

void Solve(int t)
{
    int i,j,k,n,m,u,v,w,mst;
    scin2(n , m);
    for(i=1 ; i<=m ; i++){
        cin>>u>>v>>w;
        ed.pb(edge(u , v , w));
    }
    mst = MST(n);
    pf("Exact MST = %d\n",mst);
    BFS(1);
    LCA_Init(n);
    for(i=0 ; i<m ; i++){
        u = ed[i].u;
        v = ed[i].v;
        w = ed[i].cost;
        int mxedge = LCA_Query(n , u , v);
        int ans = mst-mxedge+w;
        pf("For edge: u=%d\tv=%d\tw=%d ,\t MST = %d\n",u,v,w,ans);
    }
}

int main()
{
    int t,T;
    T = 1;
//    scin(T);
    RUN_CASE(t,T)
    {
        Solve(t);
    }
    return 0;
}



