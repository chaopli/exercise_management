#include "priority_queue.h"
#include <iostream>
#include <list>
#include <unordered_map>
#include <utility>
#include <queue>
#include <limits>
#include <fstream>

template <class T>
class Edge
{
private:
    int v_;
    int w_;
    T weight_;

public:
    Edge()
        : v_(0),
          w_(0),
          weight_(T())
    {
    }
    Edge(int v, int w, T weight)
        : v_(v),
          w_(w),
          weight_(weight)
    {
    }
    Edge(const Edge<T> &other)
        : v_(other.v_),
          w_(other.w_),
          weight_(other.weight_)
    {
    }
    T weight() { return weight_; }
    int either() { return v_; }
    int other(int vertex) { return vertex == v_ ? w_ : v_; }
    bool operator==(const Edge &other) { return other.weight_ == weight_; }
    bool operator>(const Edge &other) { return other.weight_ > weight_; }
    bool operator<(const Edge &other) { return other.weight_ < weight_; }
};

template <class T>
class EdgeWeightedGraph
{
private:
    int V_;
    int E_;
    vector<vector<Edge<T>>> adj_;
    unordered_map<string, int> city_vertex_mapping;
    unordered_map<int, string> vertex_city_mapping;

public:
    EdgeWeightedGraph(string filename);
    EdgeWeightedGraph(int V)
        : V_(V),
          E_(0)
    {
        for (int i = 0; i < V_; i++)
        {
            adj_.push_back(vector<Edge<T>>());
        }
    }
    int V() { return V_; }
    int E() { return E_; }
    void add_edge(Edge<T> e)
    {
        int v = e.either();
        int w = e.other(v);
        adj_[v].push_back(e);
        adj_[w].push_back(e);
        E_++;
    }
    int degree(int vertex)
    {
        return adj_[vertex].size();
    }
    void set_E(int E) { E_ = E; }
    vector<Edge<T>> &adj(int i)
    {
        return adj_[i];
    }
    vector<string> shortest_paths(int v);
    vector<string> shortest_paths(string s)
    {
        return shortest_paths(city_vertex_mapping[s]);
    }
    string print_path(vector<int> parent, int j);

};

template <class T>
class PrimMST
{
private:
    vector<Edge<T>> edge_to_;
    vector<double> dist_to_;
    vector<bool> marked_;
    PriorityQueue<double> pq_;

public:
    void scan(EdgeWeightedGraph<T> &graph, int v)
    {
        marked_[v] = true;
        for (int i = 0; i < graph.adj(v).size(); i++)
        {
            Edge<T> e = graph.adj(v)[i];
            int w = e.other(v);
            if (marked_[w])
                continue;
            if (e.weight() < dist_to_[w])
            {
                dist_to_[w] = e.weight();
                edge_to_[w] = e;
                if (pq_.contains(w))
                    pq_.decrease_key(w, dist_to_[w]);
                else
                    pq_.push(w, dist_to_[w]);
            }
        }
    }
    void prim(EdgeWeightedGraph<T> &graph, int s)
    {
        dist_to_[s] = 0.0;
        pq_.push(s, dist_to_[s]);
        while (!pq_.empty())
        {
            int v = pq_.pop();
            scan(graph, v);
        }
    }
    PrimMST(EdgeWeightedGraph<T> &graph)
        : pq_(graph.V())
    {
        edge_to_.resize(graph.V());
        dist_to_.resize(graph.V());
        marked_.resize(graph.V());
        for (int i = 0; i < graph.V(); i++)
        {
            dist_to_[i] = numeric_limits<double>::max();
        }
        for (int i = 0; i < graph.V(); i++)
        {
            if (!marked_[i])
                prim(graph, i);
        }
    }
    vector<Edge<T>> edges()
    {
        vector<Edge<T>> queue;
        for (int v = 0; v < edge_to_.size(); v++)
        {
            Edge<T> e = edge_to_[v];
            if (e.weight() > 0)
                queue.push_back(e);
        }
        return queue;
    }
    double weight()
    {
        double weight = 0.0;
        vector<Edge<T>> es = edges();
        for (int i = 0; i < es.size(); i++)
        {
            weight += es[i].weight();
        }
        return weight;
    }
};

template <class T>
EdgeWeightedGraph<T>::EdgeWeightedGraph(string filename)
     : E_(0)
{
    ifstream fin(filename);
    fin >> V_;
    fin >> E_;
    int i = 0;
    fin.get();
    for (int i = 0; i < V_; i++)
    {
        adj_.push_back(vector<Edge<T>>());
    }
    while (fin.good())
    {
        string city1;
        getline(fin, city1, ',');
        string city2;
        getline(fin, city2, ',');
        string weight_str;
        getline(fin, weight_str, '\n');
        double weight = stod(weight_str);
        cout << city1 << '\t' << city2 << '\t' << weight << endl;
        int v, w;
        if (city_vertex_mapping.find(city1) == city_vertex_mapping.end()) {
            cout << city1 << " not found. Existing cities are" << endl;
            for (auto city : city_vertex_mapping)
            {
                cout << city.first << ", ";
            }
            cout << endl;

            city_vertex_mapping[city1] = i;
            vertex_city_mapping[i] = city1;
            v = i;
            i++;
            cout << i << " cities added" << endl;
        } else {
            v = city_vertex_mapping[city1];
        }
        if (city_vertex_mapping.find(city2) == city_vertex_mapping.end()) {
            cout << city2 << " not found. Existing cities are" << endl;
            for (auto city : city_vertex_mapping)
            {
                cout << city.first << ", ";
            }
            cout << endl;
            city_vertex_mapping[city2] = i;
            vertex_city_mapping[i] = city2;
            w = i;
            i++;
            cout << i << " cities added" << endl;
        } else {
            w = city_vertex_mapping[city2];
        }
        add_edge(Edge<T>(v, w, weight));
    }
}

template <class T>
vector<string> EdgeWeightedGraph<T>::shortest_paths(int src)
{
    priority_queue<pair<T, int>> pq;

    // Create a vector for distances and initialize all 
    // distances as infinite (INF) 
    vector<T> dist(V(), numeric_limits<T>::max()); 
    vector<int> parent(V(), -1);
  
    // Insert source itself in priority queue and initialize 
    // its distance as 0. 
    pq.push(make_pair(0, src)); 
    dist[src] = 0; 
  
    /* Looping till priority queue becomes empty (or all 
      distances are not finalized) */
    while (!pq.empty()) 
    { 
        // The first vertex in pair is the minimum distance 
        // vertex, extract it from priority queue. 
        // vertex label is stored in second of pair (it 
        // has to be done this way to keep the vertices 
        // sorted distance (distance must be first item 
        // in pair) 
        int u = pq.top().second; 
        pq.pop(); 
  
        // 'i' is used to get all adjacent vertices of a vertex 
        for (auto i = adj_[u].begin(); i != adj_[u].end(); ++i) 
        { 
            // Get vertex label and weight of current adjacent 
            // of u. 
            int v = (*i).other(u); 
            T weight = (*i).weight(); 
  
            //  If there is shorted path to v through u. 
            if (dist[v] > dist[u] + weight) 
            { 
                // Updating distance of v 
                dist[v] = dist[u] + weight; 
                parent[v] = u;
                pq.push(make_pair(dist[v], v));
            }
        }
    }
  
    vector<string> ret;
    // Print shortest distances stored in dist[]
    for (int i = 0; i < V(); ++i)
    {
        string s(vertex_city_mapping[src]);
        s += "-";
        s += print_path(parent, i) + ": " + to_string(dist[i]);
        ret.push_back(s);
    }
    return ret;

}

template <class T>
string EdgeWeightedGraph<T>::print_path(vector<int> parent, int j)
{
    if (parent[j] == -1)
    {
        return "";
    }
    return vertex_city_mapping[j] + "-" + print_path(parent, parent[j]);
}