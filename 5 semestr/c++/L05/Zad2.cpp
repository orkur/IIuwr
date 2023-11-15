#include <iostream>
#include <map>
#include <unordered_map>
#include <utility>
#include <vector>
#include <stdexcept>
class Nodes{
    private:
	std::map<std::pair<std::string, std::string>, double> edges;
	std::map<std::string, std::vector<std::string> >  neighbors;
    public:
	Nodes(){}
	void push(std::string from, std::string to, double val){
	    if(edges.find({from, to}) != edges.end()){
		edges.insert({{from, to}, val});
	        if(neighbors.find(from) != neighbors.end())
	            neighbors[from].push_back(to);
		else{
		    //std::vector<std::string >v = {to};
		    neighbors.insert({from, std::vector<std::string>{to}});
		}
	    }else
	    	throw std::invalid_argument("podana sciezka juz istnieje!");

	}
	void change_weight(std::string from, std::string to, double new_val){
	    if(edges.find({from, to}) != edges.end())
		edges[{from, to}] = new_val;
	    else
	    	throw std::invalid_argument("podana sciezka nie istnieje!");
	}
	void delete_edge(std::string from, std::string to){
	    if(edges.find({from, to}) != edges.end()){
		edges.erase({from, to});
		for(int i = 0; i < neighbors[from].size(); i++)
			if(neighbors[from][i] == to) neighbors[from].erase(neighbors[from].begin() + i);
	    }else
	    	throw std::invalid_argument("podana sciezka do usuniecia nie istnieje!");
	}
	Nodes operator<<(){
	    std::cout << "lista sasiedstw:\n";
	    for(auto i = neighbors.begin(); it != neighbors.end(); it++)
	    {
		    std::cout << i->first << ": ";
		    for(auto j:i->second) std::cout<<j << " ";
		    std::cout << "\n";
	    }
	}


};
int main(){
    Nodes n = Nodes();
    n.push("piwo", "paliwo", 20.0);
    n.push("piwo2", "paliwo", 20.0);
    n.push("piwo", "paliwo2", 20.0);
    n.push("piwo", "paliwo5", 20.0);
    cout<< n;
    
}
