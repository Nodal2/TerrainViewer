#ifndef MESHTOOL_DIJKSTRA
#define MESHTOOL_DIJKSTRA

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <limits>
#include <set>
#include <utility>
#include <algorithm>
#include <iterator>

typedef int DijkstraVertex;
typedef double DijkstraWeight;

const DijkstraWeight max_weight = std::numeric_limits<double>::infinity();

struct DijkstraNeighbor {
    DijkstraVertex target;
    DijkstraWeight weight;
    DijkstraNeighbor(DijkstraVertex arg_target, DijkstraWeight arg_weight) : target(arg_target), weight(arg_weight) {}
};

typedef std::vector<std::vector<DijkstraNeighbor>> DijkstraAdjacencyList;

inline void compute_paths(DijkstraVertex source, const DijkstraAdjacencyList &adjacency_list, std::vector<DijkstraWeight> &min_distance, std::vector<DijkstraVertex> &previous)
{
    int n = adjacency_list.size();
    min_distance.clear();
    min_distance.resize(n, max_weight);
    min_distance[source] = 0;
    previous.clear();
    previous.resize(n, -1);
    std::set<std::pair<DijkstraWeight, DijkstraVertex> > vertex_queue;
    vertex_queue.insert(std::make_pair(min_distance[source], source));

    while (!vertex_queue.empty()) 
    {
        DijkstraWeight dist = vertex_queue.begin()->first;
        DijkstraVertex u = vertex_queue.begin()->second;
        vertex_queue.erase(vertex_queue.begin());

        // Visit each edge exiting u
	    const std::vector<DijkstraNeighbor> &neighbors = adjacency_list[u];
        for (std::vector<DijkstraNeighbor>::const_iterator neighbor_iter = neighbors.begin(); neighbor_iter != neighbors.end(); neighbor_iter++)
        {
            DijkstraVertex v = neighbor_iter->target;
            DijkstraWeight weight = neighbor_iter->weight;
            DijkstraWeight distance_through_u = dist + weight;
	        if (distance_through_u < min_distance[v]) 
            {
	            vertex_queue.erase(std::make_pair(min_distance[v], v));
	            min_distance[v] = distance_through_u;
	            previous[v] = u;
	            vertex_queue.insert(std::make_pair(min_distance[v], v));
	        }
        }
    }
}

inline std::list<DijkstraVertex> get_shortest_path_to(DijkstraVertex vertex, const std::vector<DijkstraVertex> &previous)
{
    std::list<DijkstraVertex> path;
    for ( ; vertex != -1; vertex = previous[vertex])
        path.push_front(vertex);
    return path;
}

#endif