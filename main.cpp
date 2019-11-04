#include <iostream> // Input and Output Stream Library
#include <unordered_map> 
#include <unordered_set>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <set>
#include <array>
#include <chrono>
#include <random>
#include <numeric>
#include <cmath>

using vertex_t = int;

struct Graph{
	/*
		Graph Datastruct
		
		Dic of list adj, based on hash
		{
			'u' : [vi_...vk] 
		}
	*/

	std :: unordered_map<vertex_t, std :: unordered_set<vertex_t>> graph;
	int d; // d = |V|

	int number_vertices(){
		return(d);
	}

	std :: unordered_set<vertex_t> get_adj(vertex_t u){
		// Get adj list of vertex u
		return(graph.find(u)->second);
	}

	void print_adj(std :: unordered_set<vertex_t> adj){
		for(auto iterator: adj){
			std :: cout << iterator << " ";
		}
		std :: cout << std :: endl;
	}

	vertex_t random_vertice(){
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  		std::default_random_engine generator (seed);
  		std :: uniform_int_distribution<vertex_t> distribution(0,d-1);

  		return(distribution(generator));
	}

	friend std :: ostream& operator<<(std :: ostream& os, const Graph& g){
		// Overload for operator << to print graph
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
};

struct Path{
	std :: vector<vertex_t> path; // Set of vertex to reach from u to v
	int path_size=0; // Number of vertices in path

	void initialize(int n){
		path.reserve(n); // Reserve memory for len path, max path is |V|
	}

	void insert(vertex_t u){
		path.push_back(u);
		path_size++;
	}

	bool find(vertex_t u){
		if(std :: find(path.begin(),
			path.end(),u) != path.end()) return(true);
		return(false);
	}

	int size() const{
		return(path_size);
	}

	friend std :: ostream& operator<<(std :: ostream& os,const Path& c){
		for(auto iterator: c.path){
			os << iterator << " ";
		}
		return(os);
	}
};

struct Routing{
	int size;
	std :: vector<Path> routing;

	void initialize(int n){
		routing.reserve(n);
		size = n;
	}

	void insert(Path c){
		routing.push_back(c);
	}

	friend std :: ostream& operator<<(std :: ostream& os, const Routing& r){
		int i=0;
		for(auto iterator: r.routing){
			os << "\tPath #" << i+1 << " --> " << iterator << std :: endl;
			i++;
		}
		return(os);
	}
};

struct Population{
	std :: vector<Routing> population;
	int mu; // Number of population

	void initialize(int n){
		population.reserve(n);
		mu = n;
	}

	void insert(Routing r){
		population.push_back(r);
	}

	friend std :: ostream& operator<<(std :: ostream& os, const Population& p){
		int i=0;
		os << "Population" << std :: endl;
		for(auto iterator: p.population){
			os << "Routing #" << i+1 << std :: endl << iterator << std :: endl;
			i++;
		}
		return(os);
	}
};

float random_probability(){
	std::random_device rd;
	std::mt19937 engine(rd());
	std::uniform_real_distribution<float>generator(0, 1);

	return(generator(engine)); 
}

void store_graph(Graph &g){
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
}

Path generate_path(Graph g, vertex_t u, vertex_t v){
	// Generate a random path between vertex u,v

	Path c;
	vertex_t vertex;
	std :: unordered_set<vertex_t> adj;
	
	c.initialize(g.d);
	c.insert(u);
	vertex = g.random_vertice();

	while(vertex!=v){
		if(!c.find(vertex)){
			adj = g.get_adj(vertex);
			if(adj.find(v)!=adj.end()) c.insert(vertex);
		}
		vertex = g.random_vertice();
	}
	c.insert(vertex);
	return(c);
}

Routing generate_routing(Graph g){
	// Generate routing with n*(n-1) paths
	Routing r;
	Path c;

	vertex_t u=0;
	r.initialize(g.d*g.d);

	for(vertex_t v=0; v<r.size; v++){
		if(u!=v%g.d){
			c = generate_path(g,u,v%g.d);
			r.insert(c);
			if((v+1)%g.d==0) u++;
		}
	}
	return(r);
}

Population xi(Graph g, int mu){
	// Generate initial population of routing with mu individuals
	Population p;
	Routing r;

	p.initialize(mu);

	for(int i=0; i<mu; i++){
		r = generate_routing(g);
		p.insert(r);
	}

	return(p);
}

double omega(Routing r){
	// Cost function
	// Calculate norm of R = ||R||
	// ||R|| = for all c_i in R sqrt(sum_i^n |c_i|)
	double cost=0.0;

	for(auto iterator: r.routing){
		cost += std :: pow(iterator.size(),2);
		//std :: cout << iterator.size() << std :: endl;
	}
	//std :: cout << sqrt(cost) << std :: endl;
	return(sqrt(cost));
}

Population psi(Population p){
	// Select the best parents based on his value
	Population parents;
	Routing r1=p.population[0], r2=p.population[1];
	double r1_value=omega(r1), r2_value=omega(r2);
	double aux;
	parents.initialize(2);

	for(int i=2; i<p.mu; i++){
		aux = omega(p.population[i]);
		if(aux < r1_value){
			r1 = p.population[i];
			r1_value = aux;
		}
		else{
			if(aux < r2_value){
				r2 = p.population[i];
				r2_value = aux;
			}
		}
		
		//std :: cout << omega(iterator) << std :: endl;
	}
	parents.insert(r1); parents.insert(r2);
	//std :: cout << r1 << " --> " << r1_value << std :: endl;
	//std :: cout << r2 << " --> " << r2_value << std :: endl;
	return(parents);
}

Routing cross(Population parents){
	// Crossing parents to obtain a child
	// Each path has a 0.5 chance to be selected
	Routing r;

	r.initialize(parents.population[0].routing.size());

	for(int i=0;i<r.size;i++){
		if(random_probability()<0.5){
			r.insert(parents.population[0].routing[i]);
		}
		else{
			r.insert(parents.population[1].routing[i]);
		}
	}
	return(r);
}

Population phi(Population parents, int mu){
	// Generate a new population mu of childs
	Population new_p;

	new_p.initialize(mu);

	for(int i=0;i<mu;i++){
		new_p.insert(cross(parents));
	}

	return(new_p);
}

Population upsilon(Population new_p, float alpha){

	return(new_p);
}

Routing genetic_algorithm(Graph g, int beta, int mu, float alpha){
	Population p, new_p, parents;
	Routing r;

	p = xi(g,mu); // Generate initial population
	for(int generation=0; generation<mu; generation++){
		parents = psi(p); // Select parents
		new_p = phi(parents, mu-2); // Generate new population with parents
		new_p = upsilon(new_p, alpha); // Apply mutation to new population if prob < alpha
	}
	
	return(r);
}

int main(){
	Graph g; // Graph
	int beta=50; // Number of generations
	int mu=50; // Number of population
	float alpha=0.1; // Probability of mutation
	int d=4; // |V|

	g.d = d;

	store_graph(g); // Create a graph

	std :: cout << g << std :: endl;

	genetic_algorithm(g,beta, mu, alpha);

	//std :: cout << random_probability() << std :: endl;
	/*
	Population p;

	p = xi(g,80);

	std :: cout << p;*/
	/*
	Routing r;

	r = generate_routing(g);

	std :: cout << r << std :: endl;

	omega(r);
	*/
	return(42);
}