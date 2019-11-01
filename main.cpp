#include <iostream> // Input and Output Stream Library
#include <unordered_map> 
#include <unordered_set>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using vertex_t = int;

struct Edge{
	std :: pair<vertex_t,vertex_t> edge; // Pair of vertices

	void create(vertex_t u, vertex_t v){
		edge = std  :: make_pair(u,v);
	}

	bool operator==(const Edge& e) const{
		if(edge.first == e.edge.first && edge.second == e.edge.second &&
			edge.first == e.edge.second && edge.second == e.edge.first){
			return(true);
		}
		return(false);
	}
};

struct Lambda{
	// Struct to store congestion of all edges in a routing
	// \phi(\Gamma.e) = # number of path in each edge
	std :: vector<int> lambda;

	void insert(int congestion){
		lambda.push_back(congestion);
	}
	// TODO: Build Lambda vector = \phi(\Gamme,e)
};

struct Graph{
	std :: unordered_map<vertex_t, std :: unordered_set<vertex_t>> graph;
	std :: vector<Edge> edges; // All unique edge
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

	friend std :: ostream& operator<<(std :: ostream& os, const Graph& g){
		std :: unordered_set<vertex_t> :: const_iterator itr_adj; // Iterator of adjacent list
		std :: unordered_map<vertex_t, std :: unordered_set<vertex_t>> :: const_iterator itr_vertex; // Iterator of vertex
		
		for(itr_vertex=g.graph.begin();itr_vertex!=g.graph.end();itr_vertex++){
			os << itr_vertex->first << " --> ";
			for(itr_adj=itr_vertex->second.begin();itr_adj!=itr_vertex->second.end();itr_adj++){
				os << (*itr_adj) << " ";
			}
			os << std :: endl;
		}
		return(os);
	}

	bool is_vextex_in_edges(vertex_t u, vertex_t v){
		Edge e;
		e.create(u,v);
		if(std :: find(edges.begin(),edges.end(),e) != edges.end()){
			return(true);
		}
		return(false);
	}

	Lambda get_congestion(){
		Edge e;
		Lambda congestion;
		std :: unordered_set<vertex_t> :: iterator itr_adj;
		std :: unordered_map<vertex_t, std :: unordered_set<vertex_t>> :: iterator itr_vertex;

		for(itr_vertex=graph.begin();itr_vertex!=graph.end();itr_vertex++){
			//std :: cout << itr_vertex->first << " --> ";
			for(itr_adj=itr_vertex->second.begin();itr_adj!=itr_vertex->second.end();itr_adj++){
				if(is_vextex_in_edges(itr_vertex->first, (*itr_adj))){ // Check if exisis and edge u,v==v,u
					break;
				}
				else{
					break;
				}
				//std :: cout << (*itr_adj) << " ";
			}
			//std :: cout << std :: endl;
		}

		return(congestion);
	}

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

struct Population{
	std :: vector<Routing> population;

	void insert(Routing r){
		population.push_back(r);
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

Routing generate_routing(Graph g){
	/*
		Generate random path between
		each pair of vertices (u,v)
		where u!=v
	*/
	Routing r;
	Path c;
	
	vertex_t u=0;
	r.d = g.d * g.d; // Total of all pair of vertices in a graph with g.d = |V|

	for(vertex_t v=0; v<r.d; v++){
		std :: cout << "AHHH" << std :: endl;
		std :: cout << u << " " << v << std :: endl;
		if(u!=v%g.d){
			c = generate_path(g,u,v%g.d);
			r.insert(c);
			if((v+1)%g.d==0) u++;
		}
	}
		/*
	std :: cout << "AHHH" << std :: endl;
	
	for(vertex_t u=0; u<g.d; u++){
		std :: cout << "AHHH" << std :: endl;
		for(vertex_t v=0; v<g.d; v++){
			if(u!=v){
				c = generate_path(g,u,v);
				r.insert(c);
			}
		}
	}
	*/
	return(r);
}

Population initial_population(Graph g, int mu){
	Population p;
	Routing r;

	for(int i=0;i<mu;i++){
		r = generate_routing(g);
		p.insert(r);
	}

	return(p);
};

Population psi(Population p){
	Population parents;
	// TODO: Complete selection process to get parents
	return(parents);
}

Routing genetic_algorithm(Graph g, int beta, int mu, float alpha){
	Routing r; // DELETE THIS

	Population p;
	Population new_p;
	Population parents;
	int generation = 0;

	p = initial_population(g, mu);
	while(generation<beta){
		parents = psi(p); // Parent Selection Function
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

	std :: unordered_set<vertex_t> adj; // Adjacent list
	std :: unordered_set<vertex_t> :: iterator itr_adj; // Iterator of adjacent list
	std :: unordered_map<vertex_t, std :: unordered_set<vertex_t>> :: iterator itr_vertex; // Iterator of vertex

	adj.insert(1); adj.insert(2); adj.insert(3);
	g.graph[0] = adj;
	adj.clear();

	adj.insert(0); adj.insert(2); adj.insert(3);
	g.graph[1] = adj;
	adj.clear();

	adj.insert(0); adj.insert(1); adj.insert(3);
	g.graph[2] = adj;
	adj.clear();

	adj.insert(0); adj.insert(1); adj.insert(2);
	g.graph[3] = adj;

    // Print Graph 
    /*
	for(itr_vertex=g.graph.begin();itr_vertex!=g.graph.end();itr_vertex++){
		std :: cout << itr_vertex->first << " --> ";
		for(itr_adj=itr_vertex->second.begin();itr_adj!=itr_vertex->second.end();itr_adj++){
			std :: cout << (*itr_adj) << " ";
		}
		std :: cout << std :: endl;
	}*/

	std :: cout << g << std :: endl;

	Population p = initial_population(g,5);

	Lambda congestion;

	congestion = g.get_congestion();


	/* Print initial population

	std :: vector<Routing> :: iterator itr_p;
	std :: vector<Path> :: iterator itr_r;
	std :: vector<int> :: iterator itr_c;
	int j = 0;

	for(itr_p=p.population.begin();itr_p!=p.population.end();itr_p++){
		std :: cout << "Population #" << j+1 << std :: endl;
		Routing r = (*itr_p);
		j++;
		int i=0;
		for(itr_r=r.routing.begin();itr_r!=r.routing.end();itr_r++){
			Path c = (*itr_r);
			std :: cout << "\tPath " << i+1 << " -> ";
			i++;
			for(itr_c=c.path.begin();itr_c!=c.path.end();itr_c++){
				std :: cout << (*itr_c) << " ";
			}
			std :: cout << std :: endl;
		}
	}

	*/

	

	/*	Generate path and routing
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

	r = generate_routing(g);

	std :: vector<Path> :: iterator itr_r;
	int i=0;

	for(itr_r=r.routing.begin();itr_r!=r.routing.end();itr_r++){
		Path c = (*itr_r);
		std :: cout << "Path " << i+1 << " -> ";
		i++;
		for(itr_c=c.path.begin();itr_c!=c.path.end();itr_c++){
			std :: cout << (*itr_c) << " ";
		}
		std :: cout << std :: endl;
	}
	*/

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