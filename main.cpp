#include <iostream> // Input and Output Stream Library
#include <unordered_map> 
#include <unordered_set>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using vertex_t = int;

struct Graph{
	std :: unordered_map<vertex_t, std :: unordered_set<vertex_t>> graph;
	int d; // Number of vertices

	int number_vertices(){
		return(d);
	}

	std :: unordered_set<vertex_t> get_adj(vertex_t u){
		// Get adj list from vertex u
		std :: unordered_map<vertex_t,std :: unordered_set<vertex_t>> :: const_iterator got = graph.find(u);

		return (got->second);
	}

	bool is_in_adj(vertex_t u, std :: unordered_set<vertex_t> adj){
		if( std :: find(adj.begin(),adj.end(),u) != adj.end()){
			return(true);
		}
		else{
			return(false);
		}
	}

	vertex_t random_vertice(vertex_t u, vertex_t v, int d){
		// u and b are vertices and d = |V|
		int vertex;
		std :: srand(std :: time(NULL));
		do{
			vertex = std :: rand() % d;
		}while(vertex==u);

		return(vertex);
	}

};

struct Edge{
	std :: map<vertex_t,vertex_t> edges; // Pair of vertices
	std :: vector<int> edge_index; // Represent the number of paths through an edge
};

struct Path{
	std :: vector<vertex_t> path;

	void insert(vertex_t u){
		path.push_back(u);
	}

	bool is_in(vertex_t u){
		if( std :: find(path.begin(),path.end(),u) != path.end()){
			return(true);
		}
		else{
			return(false);
		}
	}

};

struct Routing{
	int d; // Size of vector number_vertices x number_vertices
	std :: vector<Path> routing;

	void initialize(){
		std :: vector<Path> r(d);
		routing = r;
	}

	void insert(Path c){
		routing.push_back(c);
	}
};

Path generate_path(Graph g, vertex_t u, vertex_t v){
	/*
		Generate a random path
		from vertex u to vertex v
	*/
	int d;
	Path c;
	vertex_t vertex;
	std :: unordered_set<vertex_t> adj;

	d = g.number_vertices();
	vertex = g.random_vertice(u,v,d);
	c.insert(u); // Insert vertex u as begin of the path
	while(vertex!=v){
		adj = g.get_adj(u);
		if(g.is_in_adj(vertex, adj) && !c.is_in(vertex)){
			c.insert(vertex);
			u = vertex;
		}
		vertex = g.random_vertice(vertex,u,d);
	}
	c.insert(vertex);
	return(c);
}

std :: vector<Routing> initial_population(Graph g, int mu){
	std :: vector<Routing> population(mu);
	/*
		TODO: Generate population
	*/
	return(population);
}


Routing genetic_algorithm(Graph g, int beta, int mu, float alpha){
	Routing r; // DELETE THIS

	std :: vector<Routing> population(mu);
	std :: vector<Routing> new_population(mu);
	std :: vector<Routing> parents(2);
	int generation = 0;

	population = initial_population(g, mu);
	while(generation<beta){
		generation++;
		/*
			TODO : Complete genetic algoritmhm
		*/
	}

	return(r);
}


int main(){
	Graph g;
	Routing r;
	int beta=50; // Number of generations
	int mu=30; // Number of population
	float alpha=0.1; // Probability of mutation
	int d = 4; // |V|

	g.d = d;

	std :: unordered_set<vertex_t> adj;
	std :: unordered_set<vertex_t> :: iterator itr_adj;
	std :: unordered_map<vertex_t, std :: unordered_set<vertex_t>> :: iterator itr_vertex;

	adj.insert(1); 
	adj.insert(2); 
	adj.insert(3);

	g.graph[0] = adj;

	adj.clear();
	adj.insert(0); 
	adj.insert(2); 
	adj.insert(3);

	g.graph[1] = adj;

	adj.clear();

	adj.insert(0); 
	adj.insert(1); 
	adj.insert(3);

	g.graph[2] = adj;

	adj.clear();

	adj.insert(0);
	adj.insert(1);
	adj.insert(2);

	g.graph[3] = adj;

	for(itr_vertex=g.graph.begin();itr_vertex!=g.graph.end();itr_vertex++){
		std :: cout << itr_vertex->first << " --> ";
		for(itr_adj=itr_vertex->second.begin();itr_adj!=itr_vertex->second.end();itr_adj++){
			std :: cout << (*itr_adj) << " ";
		}
		std :: cout << std :: endl;
	}

	adj = g.get_adj(0);

	for(itr_adj=adj.begin();itr_adj!=adj.end();itr_adj++){
		std :: cout << "0 -> " << (*itr_adj) << " ";
	}	

	std :: cout << std :: endl;

	Path c;
	c = generate_path(g, 2, 0);

	std :: cout << "Path to reach 2-0" << std :: endl;

	std :: vector<vertex_t> :: iterator itr_c;

	for(itr_c=c.path.begin();itr_c!=c.path.end();itr_c++){
		std :: cout << (*itr_c) << " ";
	}

	std :: cout << std :: endl;
	//r = genetic_algorithm(g,beta,mu,alpha);

	/*
	Routing routing;
	routing.d = d;
	routing.initialize();

	for(int i=0;i<d;i++){
		for(int j=0;j<d;j++){
			cout << routing.routing[i][j] << " ";
		}
		cout << endl;
	}*/

	/*
	std :: unordered_map<int, std :: unordered_set<int>> G;
	std :: unordered_map<int, std::unordered_set<int>> :: iterator itr;

	std :: unordered_set<int> adj;
	std :: unordered_set<int> :: iterator itr_adj;
	for(int i=0;i<5;i++){
		adj.insert(i);
	}
	G[0] = adj;

	for(itr = G.begin();itr!=G.end();itr++){
		std :: cout << itr->first << std :: endl;
		for(itr_adj = itr->second.begin(); itr_adj != itr->second.end(); itr_adj++){
			std :: cout << (*itr_adj) << " ";
		}
		std :: cout << std :: endl;
	}
	*/
	return(42);	
}