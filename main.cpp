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
#include <queue>

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

	void insert_edge(vertex_t u, vertex_t v){
		graph[u].insert(v);
		graph[v].insert(u);
	}

	int number_vertices(){
		return(graph.size());
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
		// Generate a random vertice from adj set
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  		std :: default_random_engine generator(seed);
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

	int check_edge(vertex_t u, vertex_t v){
		int congestion=0;

		for(int i=0;i<path_size-1;i++){
			if( (path[i]==u && path[i+1]==v) || (path[i]==v && path[i+1]==u))
				congestion++;
		}
		//std :: cout << u << " -- " << v << " --> " << congestion << std :: endl;
		return(congestion);
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
		routing.reserve(n*(n-1));
		size = n*(n-1);
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

	std :: vector<int> get_lambda(int n){
		int congestion=0;
		std :: vector<int> lambda;
		vertex_t u=0;

		for(vertex_t v=0; v<n*n; v++){
			if(u!=v%n){
				for(int r=0; r<size; r++){
					congestion += routing[r].check_edge(u,v%n);
				}
				if(congestion) lambda.push_back(congestion);
				congestion = 0;
				if((v+1)%n==0) u++;
			}
		}

		return(lambda);
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


Path generate_path(Graph g, vertex_t u, vertex_t v){
	// Generate a random path between vertex u,v

 	// TODO: Fix details about don't findind and edge
	
	vertex_t vertex = g.random_vertice();
	std :: queue<std :: pair<vertex_t, int>> q;
	std :: vector<int> path_lens(g.number_vertices(),99999999);
	int min_len;

	path_lens[u]=0;
	q.push(std :: pair<vertex_t, int>(u, 0));
	min_len = g.number_vertices();

	while(q.size()){
		std :: pair<vertex_t, int> front = q.front();
		q.pop();
		if(front.second > min_len) break;
		if(front.first == v){
			min_len = std :: min(min_len, front.second);
			break;
		}
		for(int node: g.get_adj(front.first)){
			if(path_lens[node] > front.second + 1){
				path_lens[node] = front.second + 1;
				q.push(std :: pair<vertex_t, int>(node, path_lens[node]));
			}
		}
	}

	Path c;
	std::random_device rd;
	std::mt19937 generator(rd());

	c.initialize(path_lens[v]+1);
	//c.path_size = path_lens[v];

	vertex_t act_node = v;
	c.insert(v);
	for(int i=path_lens[v]; i > 0; i--){
		std :: vector<vertex_t> possible_nodes;
		for(int node: g.get_adj(act_node)){
			if(path_lens[node] == i-1)
				possible_nodes.push_back(node);
		}
		std :: uniform_int_distribution<int> uniform_int_path(0, possible_nodes.size()-1);
		act_node = possible_nodes[uniform_int_path(generator)];
		c.insert(act_node);
	}
	std :: reverse(c.path.begin(),c.path.end());

	//std ::cout << c << std :: endl;
	return(c);
	/*
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
	*/
}

Routing generate_routing(Graph g){
	// Generate routing with n*(n-1) paths
	Routing r;
	Path c;

	vertex_t u=0;
	int n = g.number_vertices();
	r.initialize(n);
	for(vertex_t v=0; v<n*n; v++){
		if(u!=v%n){
			//std :: cout << u << " --> " << v%n << std :: endl;
			c = generate_path(g,u,v%n);
			r.insert(c);
			if((v+1)%n==0) u++;
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

int omega(Routing r, int n){
	// Cost function
	// Calculate norm of R = ||R||
	// ||R|| = for all c_i in R sqrt(sum_i^n |c_i|)
	//double cost=0.0;
	std :: vector<int> lambda; // Store \pi(\Gamma,e)

	lambda = r.get_lambda(n);
	/*
	for(auto it: lambda){
		std :: cout << it << " - ";
	}
	std :: cout << std :: endl;*/
	return(*(std :: max_element(lambda.begin(),lambda.end())));
	/*
	for(auto iterator: lambda){
		//std :: cout << iterator << " - ";
		cost += std :: pow(iterator,2);
		//std :: cout << iterator.size() << std :: endl;
	}
	//std :: cout << std :: endl;
	//std :: cout << sqrt(cost) << std :: endl;
	*/
	//return(sqrt(cost));
}

Population psi(Population p, int n){
	// Select the best parents based on his value
	Population parents;
	Routing r1=p.population[0], r2=p.population[1];
	int r1_value=omega(r1,n), r2_value=omega(r2,n);
	int aux;
	parents.initialize(2);

	for(int i=2; i<p.mu; i++){
		aux = omega(p.population[i],n);
		if(aux < r1_value){
			r1 = p.population[i];
			r1_value = aux;
		}
		else{
			if(aux <= r2_value){
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

Routing cross(Population parents, Graph g){
	// Crossing parents to obtain a child
	// Each path has a 0.5 chance to be selected
	Routing r;
	int d = g.number_vertices();

	r.initialize(d);
	for(int i=0;i<r.size;i++){
		if(random_probability()<0.5){
			r.insert(parents.population[0].routing[i]);
		}
		else{
			r.insert(parents.population[1].routing[i]);
		}
		//std :: cout << r << std :: endl;
	}
	return(r);
}

Population phi(Population parents, Graph g, int mu){
	// Generate a new population mu of childs
	Population new_p;

	new_p.initialize(mu);

	for(int i=0;i<mu;i++){
		new_p.insert(cross(parents,g));
	}

	return(new_p);
}

Population upsilon(Population new_p, Graph g, float alpha){
	//std :: cout << new_p.mu << std :: endl;
	for(int i=0;i<new_p.mu;i++){
		for(int j=0;j<new_p.population[i].routing.size();j++){
			if(random_probability() < alpha){
				new_p.population[i].routing[j] = generate_path(g,
					new_p.population[i].routing[j].path.front(),
					new_p.population[i].routing[j].path.back());
			}	
		}
	}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
	return(new_p);
}

Population theta(Population new_p, Population parents){
	// Set new population combining new_population and parents
	Population p;
	p.initialize(new_p.mu+parents.mu);

	p.population = new_p.population;
	p.insert(p.population[0]); p.insert(p.population[1]);
	return(p);
}

std :: pair<Routing,int> eta(Population p, int n){
	// Select the best routing : omega(r) is minimum
	Routing r1=p.population[0];
	int r1_value=omega(r1,n);
	int aux;
	// TODO --> Check cost between routings
	for(int i=1; i<p.mu; i++){
		aux = omega(p.population[i],n);
		if(aux < r1_value){
			r1 = p.population[i];
			r1_value = aux;
		}
	}
	return(std :: pair<Routing, int>(r1,r1_value));
}

std :: pair<Routing,int> genetic_algorithm(Graph g, int beta, int mu, float alpha){
	Population p, new_p, parents;

	p.initialize(mu);
	parents.initialize(2);
	new_p.initialize(mu-2);

	p = xi(g,mu); // Generate initial population
	//std :: cout << p << std :: endl;
	for(int generation=0; generation<beta; generation++){
		parents = psi(p, g.number_vertices()); // Select parents
		new_p = phi(parents, g, mu-2); // Generate new population with parents
		new_p = upsilon(new_p, g, alpha); // Apply mutation to new population if prob < alpha
		p = theta(new_p, parents); // Combine new population with parents to obtain current population
	}
	return(eta(p,g.number_vertices())); // Return the best routing : omega(r) is minimum
}

void store_graph(Graph &g){
	vertex_t u,v;

	while(std :: cin >> u >> v){
		g.insert_edge(u,v);
	}
}

using namespace std::chrono; 

int main(){
	Graph g; // Graph
	//Routing r; // Best routing
	int beta=30; // Number of generations
	int mu=30; // Number of population
	float alpha=0.02; // Probability of mutation

	store_graph(g); // Create a graph

	//std :: cout << g << std :: endl;
	
	
	std :: pair<Routing, int> r;

	std :: cout << "Vertices\tIndex\tTime(microseconds)" << std :: endl;

	std :: cout << g.number_vertices() << "\t";

	for(int i=0;i<3;i++){
		auto start = high_resolution_clock::now(); 

		r = genetic_algorithm(g,beta, mu, alpha);

		auto stop = high_resolution_clock::now();

		auto duration = duration_cast<microseconds>(stop - start); 

		std :: cout << r.second << "\t";
		std :: cout << duration.count() << "\t";
	}

	return(42);


	//std :: cout << "Best routing after " << beta << " generations with " << r.second << " index: " << std :: endl <<  r.first << std :: endl;
	
	//std :: cout << duration.count() << std :: endl;
	
	
	/* Store Graph

	std :: unordered_set<vertex_t> adj; // Adjacent list
	std :: unordered_set<vertex_t> :: iterator itr_adj; // Iterator of adjacent list
	std :: unordered_map<vertex_t, std :: unordered_set<vertex_t>> :: iterator itr_vertex; // Iterator of vertex

	
	
	g.d = 5;

	adj.insert(1); adj.insert(3); adj.insert(2);
	g.graph[0] = adj;
	adj.clear();

	adj.insert(0); adj.insert(4);
	g.graph[1] = adj;
	adj.clear();

	adj.insert(0); adj.insert(4);
	g.graph[2] = adj;
	adj.clear();

	adj.insert(4); adj.insert(0);
	g.graph[3] = adj;
	adj.clear();

	adj.insert(2); adj.insert(1); adj.insert(3);
	g.graph[4] = adj;
	adj.clear();
	*/
}