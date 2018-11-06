#include "network.h"
#include <map>
#include "random.h"
#include <algorithm>
#include <gtest/gtest.h>


/*! Resizes the list of nodes (\ref values) */
void Network::resize(const size_t& n) {
	values.clear();
	RandomNumbers rng;
	for (size_t i = 0; i < n; ++i) { values.push_back(rng.normal());}
}

/*! Adds a bidirectional link between two nodes
  @param[in] a,b the indexes if the two nodes 
  @param[out] success the link was succesfully inserted
 */
bool Network::add_link(const size_t& a, const size_t& b) {
	if((a < values.size() and b < values.size()) and ( a != b)) {
		std::vector<size_t> voisins(neighbors(a));
		for (auto voisin : voisins) {
			if (voisin == b)return false;
		}
		links.insert(std::pair<size_t, size_t>(a,b) );
		links.insert(std::pair<size_t, size_t>(b,a) );
		return true;
	}
	return false;
}

/*! Creates random connections between nodes: each node *n* will be linked with *degree(n)* other nodes (randomly chosen) where *degree(n)* is Poisson-distributed.

  All previous links are cleared first.
  @param mean_deg the average of the Poisson distribution.
 */
 
 
size_t Network::random_connect(const double& mean_deg) {
	links.clear();
	for (size_t node(0); node < values.size(); ++node) {
		unsigned int nb_nodes_linked (RNG.poisson(mean_deg));
		if (nb_nodes_linked < 0) nb_nodes_linked = 0;
		if(nb_nodes_linked > values.size()-1) nb_nodes_linked = values.size()-1;
		for (size_t i(0); i < nb_nodes_linked; ++i) {
			double noeu(RNG.uniform_double(0, values.size()));
			while (not add_link(node,noeu)) {
				noeu = (RNG.uniform_double(0, values.size()));
			};
		}
	}
	return (links.size()/2);
}

/*! Resets all node values.
  @param[in] vector of new node values
  @param[out] number of nodes succesfully reset
 */
size_t Network::set_values(const std::vector<double>& valeurs) {
	values.clear();
	values = valeurs;
	return values.size();
}

/*! Number of nodes */
size_t Network::size() const {
	return values.size();
}

/*! Degree (number of links) of node no *n* */
size_t Network::degree(const size_t &_n) const {
	return links.count(_n);
}
    
/*! Value of node no *n* */
double Network::value(const size_t &_n) const {
	return values[_n];
}
    
/*! All node values in descending order */
std::vector<double> Network::sorted_values() const {
	std::vector<double> valeurs(values);
	std::sort(valeurs.begin(), valeurs.end());
	std::reverse(valeurs.begin(), valeurs.end());
	return valeurs;
}
    
/*! All neighbors (linked) nodes of node no *n* */
std::vector<size_t> Network::neighbors(const size_t& n) const {
	std::vector<size_t> voisins;
	for (auto it : links) {
		if (it.first == n) voisins.push_back(it.second);
	}
	return voisins;
}
    
    
    
    
