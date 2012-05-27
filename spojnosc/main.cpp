/**********************************************************
 * Autorzy: Marcin Gecow; Robert Krajewski
 * Program: wyznaczanie sp�jno�ci wierzcho�kowej
 **********************************************************/

#include <cstdio>
#include <algorithm>
#include <vector>

using namespace std;

const unsigned int MAXN = 1005; // maksymalna liczba wierzcho�k�w
const int INFTY = 1000000; // niesko�czono��

int flow[MAXN][MAXN];	// przep�yw
int cap[MAXN][MAXN];	// przeustowo��

int pv[MAXN];			// poprzedni wierzcho�ek

int q[MAXN];			// kolejka dla algorytmu bfs
int q_front,q_back;		// pocz�tek i koniec kolejki

vector<int> graph[MAXN];// graf reprezentowany przez list� s�siedztwa
int new_nodes[MAXN];		// nowe numery wierzcho�k�w 


/** 
 * Sprawdza czy istnieje �cie�ka z s do t a je�li istnieje to wyznacza drog� z s do t
 * Zastosowany jest tutaj klasyczny bfs (przeszukiwanie wszerz)
 */

bool bfs(int s, int t,const int& n){
	int u = s;
	// pocz�tkowo kolejka jest pusta
	q_front = q_back = 0;
	for(int i=0;i<=n;i++) pv[i] = -1;
	// oznacz wierzcho�ek �r�d�owy jako odwiedzony
	pv[u] = -2;
	// dodaj go do kolejki
	q[q_front++] = u;
	while(q_back < q_front && pv[t] == -1){ // dop�ki w kolejce jest jeszcze wierzcho�ek i nie by� wcze�niej odwiedzony
		// pobierz wierzcho�ek z kolejki
		u = q[q_back++]; 
		for(int v=1;v<=n;v++){
			// dla ka�dego wierzcho�ka
			if(pv[v] == -1 && flow[u][v] < cap[u][v]){ // je�li nie by� odwiedzony i co� jeszcze mo�e przep�yn��
				// wstaw v do kolejki
				q[q_front++] = v;
				// oznacz, �e poprzednikiem v jest wierzcho�ek u
				pv[v] = u;
			}
		}
	}
	// czy wierzcho�ek docelowy t zosta� osi�gni�ty (istnieje droga od s do t)
	return pv[t] != -1;
}

/**
 * za�o�enie: s != t
 * G��wna funkcja dla algorytm Forda-Fulkersona
 * Wykonuje iteracyjne powi�kszanie �cie�ek w grafie
 */
int maxflow(int s,int t, int n){
	int result = 0,u,v;
	// wyczy�� przep�ywy
	for(int i=0;i<=n;i++){
		for(int j=0;j<=n;j++){
			flow[i][j] = 0;
		}
	}


	int min_res_cap;
	while(bfs(s,t,n)){ // dop�ki istnieje droga od s do t
		u = pv[t]; v = t;
		// koniec przetwarzania je�li wierzcho�ek t nie jest osi�galny z s
		if(u<0) break;
		// wyznaczanie minimalnej przepustowo�ci w �cie�ce
		min_res_cap = INFTY;
		// wyznacz pozosta�y do zagospodarowania przep�y na �cie�ce z s do t
		// kt�ry jest r�wny minimalnemu wolnemu przep�ywowi na tej �cie�ce
		while(u>0){
			min_res_cap = min(min_res_cap, cap[u][v]-flow[u][v]);
			v = u;
			u = pv[u];
		}
		u = pv[t]; v = t;
		// uaktualnij przesustowo�ci na znalezionej �cie�ce
		while(u>0){
			flow[u][v] += min_res_cap;
			flow[v][u] -= min_res_cap;
			v = u;
			u = pv[u];
		}
		// powi�ksz wynik o znalezion� warto�� przep�ywu
		result += min_res_cap;

	}
	return result;
}

// zamienia �r�d�owy graf na jego rozszerzon� wersj�
// w ten spos�b, �e dla ka�dego wierzcho�ka v nie b�d�cego s ani t
// powstaj� nowe wierzcho�ki v' oraz v'' oraz �cie�ka od v' do v''
// Dla ka�dego wierzcho�ka v' pod��czone s� wszystkie kraw�dzie
// dochodz�ce pierwotnie do v, natomiast z v'' wychodz� wszystkie 
// kraw�dzie, kt�re wychodzi�y z v.
int tranform_flow(int s, int t, int n){
	int ns,nt,cnt=0;
	// wyczy�� dane
	for(int i=0;i<=2*(n+1);i++){
		for(int j=0;j<=2*(n+1);j++){
			cap[i][j] = 0;
		}
		new_nodes[i] = 0;
	}
	// nadaj nowe numery wierzcho�kom
	// w ten spos�b, �e:
	// dla wierzcho�ka o numerze v
	// numer wierzcho�ka v' oraz v'' znajduj� si�
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
			// przypisz jednostkow� przepustowo�� kraw�dzi od v' do v''
			cap[new_nodes[2*i]][new_nodes[2*i+1]] = 1;
		}
	}
	
	for(int i=0;i<=n;i++){
		// dodaj do grafu wszystkie kraw�dzie dochdz�ce do v'
		for(int j=0;j<graph[i].size();j++){
			cap[new_nodes[graph[i][j]*2+1]][new_nodes[i*2]] = 1;
		}
		// dodaj do grafu wszystkie kraw�dzie wychodz�ce z v''
		for(int j=0;j<graph[i].size();j++){
			cap[new_nodes[i*2+1]][new_nodes[graph[i][j]*2]] = 1;
		}
	}
	// oblicz maksymalny przep�yw w nowym grafie
	return maxflow(ns,nt,cnt);
}

// funkcja owyznaczaj�ca maksymalny przep�yw dla ka�dej pary wierzcho�k�w
int max_connectivity(int n){
	int minc = INFTY;
	for(int i=1;i<=n;i++){
		for(int j=1;j<=n;j++){
			// dla ka�dej pary r�nych wierzcho�k�w oblicz maksymalny przep�wy
			// wybierz minimaln� warto��
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