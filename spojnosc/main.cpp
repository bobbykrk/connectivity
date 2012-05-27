/**********************************************************
 * Autorzy: Marcin Gecow; Robert Krajewski
 * Program: wyznaczanie spójnoœci wierzcho³kowej
 **********************************************************/

#include <cstdio>
#include <algorithm>
#include <vector>

using namespace std;

const unsigned int MAXN = 1005; // maksymalna liczba wierzcho³ków
const int INFTY = 1000000; // nieskoñczonoœæ

int flow[MAXN][MAXN];	// przep³yw
int cap[MAXN][MAXN];	// przeustowoœæ

int pv[MAXN];			// poprzedni wierzcho³ek

int q[MAXN];			// kolejka dla algorytmu bfs
int q_front,q_back;		// pocz¹tek i koniec kolejki

vector<int> graph[MAXN];// graf reprezentowany przez listê s¹siedztwa
int new_nodes[MAXN];		// nowe numery wierzcho³ków 


/** 
 * Sprawdza czy istnieje œcie¿ka z s do t a jeœli istnieje to wyznacza drogê z s do t
 * Zastosowany jest tutaj klasyczny bfs (przeszukiwanie wszerz)
 */

bool bfs(int s, int t,const int& n){
	int u = s;
	// pocz¹tkowo kolejka jest pusta
	q_front = q_back = 0;
	for(int i=0;i<=n;i++) pv[i] = -1;
	// oznacz wierzcho³ek ¿ród³owy jako odwiedzony
	pv[u] = -2;
	// dodaj go do kolejki
	q[q_front++] = u;
	while(q_back < q_front && pv[t] == -1){ // dopóki w kolejce jest jeszcze wierzcho³ek i nie by³ wczeœniej odwiedzony
		// pobierz wierzcho³ek z kolejki
		u = q[q_back++]; 
		for(int v=1;v<=n;v++){
			// dla ka¿dego wierzcho³ka
			if(pv[v] == -1 && flow[u][v] < cap[u][v]){ // jeœli nie byæ odwiedzony i coœ jeszcze mo¿e przep³yn¹æ
				// wstaw v do kolejki
				q[q_front++] = v;
				// oznacz, ¿e poprzednikiem v jest wierzcho³ek u
				pv[v] = u;
			}
		}
	}
	// czy wierzcho³ek docelowy t zosta³ osi¹gniêty (istnieje droga od s do t)
	return pv[t] != -1;
}

/**
 * za³o¿enie: s != t
 * G³ówna funkcja dla algorytm Forda-Fulkersona
 * Wykonuje iteracyjne powiêkszanie œcie¿ek w grafie
 */
int maxflow(int s,int t, int n){
	int result = 0,u,v;
	// wyczyœæ przep³ywy
	for(int i=0;i<=n;i++){
		for(int j=0;j<=n;j++){
			flow[i][j] = 0;
		}
	}


	int min_res_cap;
	while(bfs(s,t,n)){ // dopóki istnieje droga od s do t
		u = pv[t]; v = t;
		// koniec przetwarzania jeœli wierzcho³ek t nie jest osi¹galny z s
		if(u<0) break;
		// wyznaczanie minimalnej przepustowoœci w œcie¿ce
		min_res_cap = INFTY;
		// wyznacz pozosta³y do zagospodarowania przep³y na œcie¿ce z s do t
		// który jest równy minimalnemu wolnemu przep³ywowi na tej œcie¿ce
		while(u>0){
			min_res_cap = min(min_res_cap, cap[u][v]-flow[u][v]);
			v = u;
			u = pv[u];
		}
		u = pv[t]; v = t;
		// uaktualnij przesustowoœci na znalezionej œcie¿ce
		while(u>0){
			flow[u][v] += min_res_cap;
			flow[v][u] -= min_res_cap;
			v = u;
			u = pv[u];
		}
		// powiêksz wynik o znalezion¹ wartoœæ przep³ywu
		result += min_res_cap;

	}
	return result;
}

// zamienia Ÿród³owy graf na jego rozszerzon¹ wersjê
// w ten sposób, ¿e dla ka¿dego wierzcho³ka v nie bêd¹cego s ani t
// powstaj¹ nowe wierzcho³ki v' oraz v'' oraz œcie¿ka od v' do v''
// Dla ka¿dego wierzcho³ka v' pod³¹czone s¹ wszystkie krawêdzie
// dochodz¹ce pierwotnie do v, natomiast z v'' wychodz¹ wszystkie 
// krawêdzie, które wychodzi³y z v.
int tranform_flow(int s, int t, int n){
	int ns,nt,cnt=0;
	// wyczyœæ dane
	for(int i=0;i<=2*(n+1);i++){
		for(int j=0;j<=2*(n+1);j++){
			cap[i][j] = 0;
		}
		new_nodes[i] = 0;
	}
	// nadaj nowe numery wierzcho³kom
	// w ten sposób, ¿e:
	// dla wierzcho³ka o numerze v
	// numer wierzcho³ka v' oraz v'' znajduj¹ siê
	// na pozycjach odpowiednio 2*v oraz 2*v+1 
	// w tablicy new_nodes
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
			// przypisz jednostkow¹ przepustowoœæ krawêdzi od v' do v''
			cap[new_nodes[2*i]][new_nodes[2*i+1]] = 1;
		}
	}
	
	for(int i=0;i<=n;i++){
		// dodaj do grafu wszystkie krawêdzie dochdz¹ce do v'
		for(int j=0;j<graph[i].size();j++){
			cap[new_nodes[graph[i][j]*2+1]][new_nodes[i*2]] = 1;
		}
		// dodaj do grafu wszystkie krawêdzie wychodz¹ce z v''
		for(int j=0;j<graph[i].size();j++){
			cap[new_nodes[i*2+1]][new_nodes[graph[i][j]*2]] = 1;
		}
	}
	// oblicz maksymalny przep³yw w nowym grafie
	return maxflow(ns,nt,cnt);
}

// funkcja owyznaczaj¹ca maksymalny przep³yw dla ka¿dej pary wierzcho³ków
int max_connectivity(int n){
	int minc = INFTY;
	for(int i=1;i<=n;i++){
		for(int j=1;j<=n;j++){
			// dla ka¿dej pary ró¿nych wierzcho³ków oblicz maksymalny przep³wy
			// wybierz minimaln¹ wartoœæ
			if(i!=j){
				minc = min(minc, tranform_flow(i,j,n));
			}
		}
	}
	return minc;
}

int main(){

	int n,m,u,v,res;
	scanf("%d%d",&n, &m);
	for(int i=0;i<m;i++){
		scanf("%d%d", &u, &v);
		graph[u].push_back(v);
		graph[v].push_back(u);
	}
	res = max_connectivity(n);
	printf("%d\n", res);

	return 0;
}