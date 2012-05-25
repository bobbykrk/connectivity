#include <cstdio>
#include <algorithm>
#include <vector>

using namespace std;

const unsigned int MAXN = 1005;

int flow[MAXN][MAXN];	// przep�yw
int cap[MAXN][MAXN];	// przeustowo��

int pv[MAXN];			// poprzedni wierzcho�ek

int q[MAXN];
int q_front,q_back;

vector<int> graph[MAXN], graph_in[MAXN], graph_out[MAXN];	// graf reprezentowany przez list� s�siedztwa, grafy kraw�dzi wchodz�cych i wychodz�cych
int node_nums[MAXN], new_nodes[MAXN];		// nowe numery wierzcho�k�w 


// wylicza nowy numer wierzcho�ka
inline int new_num(int u, int s, int t, int outv = 0){
	return u == s ? (2*s+1) : (u==t ? (2*t) : (2*u+outv));
}

// w nowym grafie wierzcho�ki wchodz�ce otrzymuj� numer 2*v natomiast wychodz�ce 2*v+1
// gdzie v to numer wierzcho�ka
// z tego wynika, �e wierzcho�ek startowy otzyma numer 2*s+1 a ko�cowy 2*t
int transform_graph(int s,int t,int n){
	int node_num = 1;
	for(int i=0;i<=n*2;i++){
		for(int j=0;j<=2*n;j++){
			cap[i][j] = 0;
		}
	}
	for(int i=1;i<=n;i++){
		// w grafie nieskierowanym wirzcho�ki wychodz�ce s� jednocze�nie wchodz�ce
		for(int j=0;j<graph[i].size();j++){
			//wchodz�ce
			cap[new_num(graph[i][j],s,t,1)][new_num(i,s,t,0)] = 1;
			//wychodz�ce
			cap[new_num(i,s,t,1)][new_num(graph[i][j],s,t,0)] = 1;
		}
		cap[new_num(i,s,t,0)][new_num(i,s,t,1)] = 1;
	}
	return node_num;
}



// sprawdza czy istnieje �cie�ka z s do t a je�li istnieje to wyznacza drog� z s do t
bool bfs(int s, int t,const int& n){
	int u = s;
	q_front = q_back = 0;
	for(int i=0;i<=n;i++) pv[i] = -1;
	pv[u] = -2;
	q[q_front++] = u;
	while(q_back < q_front && pv[t] == -1){
		u = q[q_back++];
		for(int v=1;v<=n;v++){
			if(pv[v] == -1 && flow[u][v] < cap[u][v]){
				q[q_front++] = v;
				pv[v] = u;
			}
		}
	}

	return pv[t] != -1;
}

/**
 * za�o�enie: s != t
 */
int maxflow(int s,int t, int n){
	int result = 0,u,v;
	for(int i=0;i<=n;i++){
		for(int j=0;j<=n;j++){
			flow[i][j] = 0;
		}
	}


	int min_res_cap;
	while(bfs(s,t,n)){
		// tworzenie sieci residualnej
		u = pv[t]; v = t;
		// koniec przetwarzania je�li wierzcho�ek t nie jest osi�galny z s w sieci residualnej
		if(u<0) break;
		// wyznaczanie minimalnej przepustowo�ci w �cie�ce
		min_res_cap = 10000000; // niesko�czono��
		while(u>0){
			min_res_cap = min(min_res_cap, cap[u][v]-flow[u][v]);
			v = u;
			u = pv[u];
		}
		u = pv[t]; v = t;
		// uaktualnianie przepustowo�ci na �cie�ce
		while(u>0){
			flow[u][v] += min_res_cap;
			flow[v][u] -= min_res_cap;
			v = u;
			u = pv[u];
		}
		result += min_res_cap;

	}
	return result;
}

void test_print(int n){
	printf("\n");
	for(int i=1;i<n;i++){
		for(int j=1;j<n;j++){
			printf("%d ", cap[i][j]);
		}
		printf("\n");
	}
}

int tranform_flow(int s, int t, int n){
	int ns,nt,cnt=0;
	for(int i=0;i<=2*(n+1);i++){
		for(int j=0;j<=2*(n+1);j++){
			cap[i][j] = 0;
		}
		new_nodes[i] = 0;
	}
	for(int i=0;i<=n;i++){
		if(i==s) {
			ns = cnt;
			new_nodes[2*i] = cnt;
			new_nodes[2*i+1] = cnt++;
		} else if(i==t){
			nt = cnt;
			new_nodes[2*i] = cnt;
			new_nodes[2*i+1] = cnt++;
		} else {
			new_nodes[2*i] = cnt++;
			new_nodes[2*i+1] = cnt++;
			cap[new_nodes[2*i]][new_nodes[2*i+1]] = 1;
		}
	}
	//test_print(cnt);
	for(int i=0;i<=n;i++){
		for(int j=0;j<graph_in[i].size();j++){
			// do wierzcho�ka i dochodzi wierzcho�ek graph[i][j] (graph[i][j] ~> i)
			cap[new_nodes[graph_in[i][j]*2+1]][new_nodes[i*2]] = 1;
			//printf("in: %d -> %d\n",new_nodes[graph_in[i][j]*2+1], new_nodes[i*2]);
		}
		for(int j=0;j<graph_out[i].size();j++){
			// do wierzcho�ka graph[i][j] dochodzi wierzcho�ek i (i ~> graph[i][j] )
			cap[new_nodes[i*2+1]][new_nodes[graph_out[i][j]*2]] = 1;
			//printf("out: %d -> %d\n",new_nodes[i*2+1], new_nodes[graph_out[i][j]*2]);
		}
	}
	//test_print(cnt);
	return maxflow(ns,nt,cnt);
}

int max_connectivity(int n){
	int maxc = 0;
	for(int i=1;i<=n;i++){
		for(int j=i+1;j<=n;j++){
			//transform_graph(i,j,n);
			//maxc = max(maxc, maxflow(i*2+1,j*2,n*2));
			maxc = max(maxc, tranform_flow(i,j,n));
		}
	}
	return maxc;
}

int main(){

	int n,m,u,v,res;
	scanf("%d%d",&n, &m);
	for(int i=0;i<m;i++){
		scanf("%d%d", &u, &v);
		graph[u].push_back(v);
		graph[v].push_back(u);
		graph_in[v].push_back(u);
		graph_out[u].push_back(v);
		cap[u][v] = 1;
		cap[v][u] = 1;
	}
	// maksymalny przep�yw od wierzcho�ka 1 do wierzcho�ka n
	//int res = maxflow(1,n,n);
	res = max_connectivity(n);
	printf("%d", res);

	scanf(" ");
	return 0;
}