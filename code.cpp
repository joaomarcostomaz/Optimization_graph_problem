//--------------------------------------------------------------------------
//Trabalho prático 2                                                        |
//Objetivo: Problema de otimização de cidades em reino                      |
//Aluno: João Marcos Tomáz Silva Campos                                     |
//Matrícula: 2022043728                                                     |
//Disciplina: Algoritmos 1                                                  |
//Professor: GG                                                             |
//--------------------------------------------------------------------------|

#include <bits/stdc++.h>

using namespace std;

// Implementação da estrutura de dados aresta
struct edge{
    long long int u,v;
    vector<long long int> weight;

    edge(long long int _u, long long int _v, const vector<long long int>& _weight) : u(_u), v(_v), weight(_weight) {}
};

// Implementação da estrutura de dados Union-Find
struct dsu{
    vector<long long int> comp;
    vector<long long int> size;

    dsu(long long int n) {
        makeset(n);
    }

    void makeset(long long int n) {
        comp.resize(n);
        size.resize(n,1);

        for (int i = 0; i < n; ++i) {
            comp[i] = i;
        }
    }

    long long int find(long long int u) {
        if(comp[u] != u){
            comp[u] = find(comp[u]);
        }
        return comp[u];
    }

    void unite(long long int a, long long int b){
        a = find(a);
        b = find(b);

        if(a == b) return;

        if(size[a] < size[b]) swap(a,b);

        comp[b] = a;
        size[a] += size[b];
    }
};

// Função auxiliar para ordenar as arestas de forma crescente
struct Compare{
    long long int idx; 

    Compare(long long int weight) : idx(weight) {}

    bool operator()(const edge& a, const edge& b) const {
        return a.weight[idx] < b.weight[idx];
    }
};

// Qual o primeiro ano em que todo o reino alcancavel a partir do palacio real? - AGM
long long int Kruskal(vector<vector<edge> > graph, int u, int idx){
    int n = graph.size();
    dsu uf(n);
    long long int value = 0;
    long long int maxYear = 0;
    vector<edge> edges;

    for(int i = 0; i < n; i++){
        edges.insert(edges.end(), graph[i].begin(), graph[i].end());
    }

    // ordena de forma crescente
    sort(edges.begin(), edges.end(), Compare(idx));

    for(const edge& e : edges){
        long long int l = e.u;
        long long int m = e.v;
        long long int n = e.weight[idx];

        if(uf.find(l) != uf.find(m)){
            uf.unite(l,m);
            value += n;
            maxYear = max(maxYear, e.weight[0]);
        }
    }

    return (idx == 0)? maxYear : value;

}

//tempo minimo necessário para chegar de uma cidade a outra - Dijkstra
pair<vector<long long int>, long long int> Dijkstratime(vector<vector<edge> > graph, int u) {
    int n = graph.size();
    vector<long long int> time(n,LLONG_MAX);
    time[u] = 0;
    vector<long long int> firstMutual(n, 0);
    vector<int> visited(n, 0);
    
    // Cria uma fila de prioridade para armazenar os vértices a serem visitados
    priority_queue<tuple<long long int, long long int>, vector<tuple<long long int, long long int>>, greater<tuple<long long int, long long int>>> minHeap;
    minHeap.push(make_tuple(0, u));

    long long int year = 0;
    // Faz o algoritmo de dikstra adaptado para pegar o maior ano multiplo
    while(!minHeap.empty()){
        long long int cTime, cCity, pCity;
        tie(cTime, cCity) = minHeap.top();
        minHeap.pop();
        if(firstMutual[cCity] > year) year = firstMutual[cCity];

        if(visited[cCity]) continue;
        visited[cCity] = 1;

        for(auto e : graph[cCity]){
            long long int nCity = e.v;
            long long int nTime = cTime + e.weight[1];

            // Se o tempo necessário para chegar na cidade for menor que o tempo atual, atualiza o tempo
            if(nTime < time[nCity]){
                time[nCity] = nTime;
                minHeap.push(make_tuple(nTime, nCity));
                firstMutual[nCity] = max(firstMutual[nCity], e.weight[0]);
            }
            // Se o tempo necessário para chegar na cidade for igual ao tempo atual, atualiza, faz o desempate com o menor ano
            if(nTime == time[nCity]){
                long long int aux = min(firstMutual[nCity], e.weight[0]);
                firstMutual[nCity] = aux;
                minHeap.push(make_tuple(nTime, nCity));
            }
        }
    }

    return make_pair(time, year);
}

int main() {
    int N, M;
    scanf("%d %d", &N, &M);

    vector<vector<edge> > graph(N);

    // Armazena o grafo 
    for (int i = 0; i < M; ++i) {
        long long int u, v, a, l, c;
        scanf("%lld %lld %lld %lld %lld", &u, &v, &a, &l, &c);
        vector<long long int> weight = {a, l, c};

        graph[u - 1].push_back(edge(u-1, v-1, weight));
        graph[v-1].push_back(edge(v-1, u-1, weight));
    }

    int relm = 0;
    vector<long long int> time(N,LLONG_MAX);
    long long int A2;

    // Calcula o tempo mínimo necessário para sair do palácio real e chegar a cada vila
    pair<vector<long long int>, long long int> t = Dijkstratime(graph, 0);
    time = t.first;
    long long int firstMutual = t.second;

    A2 = Kruskal(graph, relm, 0);

    long long int cost = Kruskal(graph, relm, 2);

    //retorna o tempo mínimo necessário para chegar de uma cidade a outra
    for(int i = 0; i < N; i++){
        cout << time[i] << endl;
    }
    // retorna o primeiro ano em que as distancias listadas são mutuamente realizávies
    cout << firstMutual << endl;
    // retorna o ano em que o reino se torna alcançável a partir do palácio real
    cout << A2 << endl;
    // retorna o custo mínimo para construir estradas de forma que o reino se torne alcançável a partir do palácio real
    cout << cost << endl;

    return 0;
}

