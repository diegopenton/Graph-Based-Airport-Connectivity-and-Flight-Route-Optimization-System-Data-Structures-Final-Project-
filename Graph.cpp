#include "Graph.h"
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <climits>

Graph::Graph() {}


//read the .csv file (changed to read .txt because onlinegbd doesn't read .csv)

void Graph::readCSV(const string& filename) {
    
    ifstream file(filename);
    
    if (!file.is_open()) {
    
        cerr << "Error opening the following file: " << filename << endl;
    
        return;
    
        
    } else {
        
        cout << "The following file has been successfully opened: " << filename << "\n\n" << endl; 
    
        
    }
    
    string line;
    
    getline(file, line); 

    while (getline(file, line)) {
    
        stringstream ss(line);
    
        string origin, dest, originCity, destCity, distStr, costStr;
        
    
        getline(ss, origin, ',');
    
        getline(ss, dest, ',');
    
        getline(ss, originCity, ',');
    
        getline(ss, destCity, ',');
    
        getline(ss, distStr, ',');
    
        getline(ss, costStr);

        int distance = stoi(distStr);
    
        int cost = stoi(costStr);
    
        addEdge(origin, dest, distance, cost);

        size_t pos1 = originCity.find_last_of(' ');
    
        size_t pos2 = destCity.find_last_of(' ');
        
        if (pos1 != string::npos) {
    
    
            airportToState[origin] = originCity.substr(pos1 + 1);
    
        }
        
        if (pos2 != string::npos) {
    
            airportToState[dest] = destCity.substr(pos2 + 1); 
    
        }
    
        
    }

    
    
}

void Graph::addEdge(const string& origin, const string& destination, int distance, int cost) {

    adjList[origin].push_back(make_pair(destination, make_pair(distance, cost)));

    airportConnections[origin].second++;  


    airportConnections[destination].first++;  

    
    
}

//For task 2 

void Graph::OriginToDes(const string& source, const string& destination) {


// Custom priority queue implementation for OriginToDes

    struct PriorityQueue {

            vector<pair<int, string>> elements;

        

    void push(const pair<int, string>& item) {

    elements.push_back(item);


// Simple insertion sort (not most efficient, but works for our needs)

    for (size_t i = elements.size()-1; i > 0; --i) {

        if (elements[i].first < elements[i-1].first) {

                swap(elements[i], elements[i-1]);


                    } else {


        break;


              }


        }

    }

        
        pair<int, string> top() const {

            return elements.front();


        }
        

        void pop() {

            if (!empty()) {

                elements.erase(elements.begin());



            }

        }

        

        bool empty() const {

            return elements.empty();


        }

    };
    

    PriorityQueue pq;


    unordered_map<string, int> dist;

    unordered_map<string, string> prev;

    unordered_set<string> visited;


    for (const auto& node : adjList) {

        dist[node.first] = INT_MAX;


    }

    dist[source] = 0;

    pq.push({0, source});


    while (!pq.empty()) {

        string u = pq.top().second;

        pq.pop();



        if (u == destination) {
            
            
            break;
            
            
    }
        if (visited.count(u)) {
            
            
            continue;
            
            
    }
    visited.insert(u);

    for (const auto& neighbor : adjList[u]) {
            string v = neighbor.first;
    
            int weight = neighbor.second.first;
    
            if (dist[v] > dist[u] + weight) {
    
            dist[v] = dist[u] + weight;
    
            prev[v] = u;
    
            pq.push({dist[v], v});
    
            }
    
    
        }
    
        
    }

    if (dist[destination] == INT_MAX) {
        
        
        cout << "No path from " << source << " to " << destination << endl;
        
        return;
    
        
        
    }

    vector<string> path;
    
    for (string at = destination; at != ""; at = prev[at]) {
    
        path.push_back(at);
    
        
        
    }
    
    
    reverse(path.begin(), path.end());

    cout << "Shortest path from " << source << " to " << destination << ":\n";
    
    
    
    for (size_t i = 0; i < path.size(); ++i) {
    
        cout << path[i];
    
        if (i < path.size() - 1) {
            
            
            cout << " -> ";
            
            
    }
    
    }
    
    
    cout << "\nTotal distance: " << dist[destination] << endl;

    
    
}

//for task 3

void Graph::ALLairports(const string& source, const string& state) {
   
    cout << "Airports in state " << state << " reachable from " << source << ":\n";
   
    for (const auto& entry : airportToState) {
   
        if (entry.second == state && entry.first != source) {
   
            cout << entry.first << endl;
   
   
        }
   
   
    }

    
    
}


//for task 4
void Graph::OriginToDesSTOPS(const string& source, const string& destination, int stops) {
    
//make sure that both the destination and the source exist within the data base    

    if (adjList.find(source) == adjList.end() || adjList.find(destination) == adjList.end()) {
        cout << "Either source or destination airport doesn't exist.\n";
        return;
    }

// Custom queue that is specific to the OriginToDesSTOPS function 

    struct QueueNode {
        
        string airport;
        
        vector<string> path;
        
        int stops_used;
        
        int total_distance;
    
        
        
    };

    class Queue {
    
    private:
    
        vector<QueueNode> elements;
    
        size_t frontIdx;
    
    public:
    
    
        Queue() : frontIdx(0) {}
    
        
        void push(const QueueNode& value) {
    
            elements.push_back(value);
    
    
        }
    
        
        void pop() {
    
            if (!empty()) {
    
    
                frontIdx++;
    

    
    
        if (frontIdx > 100 && frontIdx > elements.size()/2) {
    
            elements.erase(elements.begin(), elements.begin() + frontIdx);
    
        frontIdx = 0;
    
    
            }
    
            }
    
    
        }
        
        QueueNode front() const {
    
            if (!empty()) {
    
    return elements[frontIdx];
    
    
    }
    
    throw out_of_range("Queue is empty");
    
    
}
        
bool empty() const {
    
    return frontIdx >= elements.size();
    
    
        }
    
        
        
    };

//creates queue for database


    Queue q;
    
    q.push({source, {source}, 0, 0});


    
    
    vector<string> shortestPath;
    
    
    int minDistance = INT_MAX;
    
    
    bool pathFound = false;


    
    while (!q.empty()) {
    
    
        QueueNode current = q.front();
    
    
        q.pop();



    
    
// Check if we've reached destination with exact stop count
    
    //looks if we are in the destination, as well as the exact stop count
    
        if (current.airport == destination && current.stops_used == stops) {
    
            pathFound = true;
    
    
            if (current.total_distance < minDistance) {
    
    
                minDistance = current.total_distance;
    
    
                shortestPath = current.path;
    
    
    
            }
    
    
            continue;
    
    
        }


        if (current.stops_used > stops) {
    
    
            continue;
    
    
        }


        for (const auto& neighbor : adjList[current.airport]) {
    
    
            string nextAirport = neighbor.first;
    
            int edgeDistance = neighbor.second.first;


    
    
//makes sure that cycles aren't revisited
    
    
    bool alreadyVisited = false;
    
        for (const auto& airport : current.path) {
    
        if (airport == nextAirport) {
    
    
            alreadyVisited = true;
    
    
        break;
    
                }
    
    
    
            }

    
    if (!alreadyVisited) {
    
        vector<string> newPath = current.path;
    
        newPath.push_back(nextAirport);
    
    
    q.push({nextAirport, newPath, current.stops_used + 1, current.total_distance + edgeDistance});
    
    
            }
    
        }
    
        
    }
    
    
    

 //Output the findings of the code
    
    if (pathFound) {
    
    
        cout << "Shortest path from " << source << " to " << destination 
    
             << " with exactly " << stops << " stops:\n";
    
        for (size_t i = 0; i < shortestPath.size(); ++i) {
    
            cout << shortestPath[i];
    
    
            if (i < shortestPath.size() - 1) cout << " -> ";
    
    
        }
    
    
        cout << "\nTotal distance: " << minDistance << endl;
    
        
        
    } else {
        
        cout << "No path found from " << source << " to " << destination << " with exactly " << stops << " stops.\n";
   
   
   
   
    }

    
    
    
}


//Used for task 5 (how many connection does each have?)
void Graph::Connections() {
    
    
    cout << "Airport connection counts:\n";
    
    for (const auto& entry : airportConnections) {
    
int total = entry.second.first + entry.second.second;
    
    cout << entry.first << ": " << total << " connections\n";
    
        
        
    }
    
    
}

//Task 6

void Graph::UndirectedGraph() {
    
    
    undirectedGraph.clear();

    undirectedEdges.clear();

//uses U and V as asked for in the assingment

    unordered_map<string, unordered_map<string, int>> minCostEdges; 

//step A
    for (const auto& entry : adjList) {

        string u = entry.first;

        for (const auto& neighbor : entry.second) {

            string v = neighbor.first;

            int cost = neighbor.second.second;



            if (!minCostEdges[u].count(v) || cost < minCostEdges[u][v]) {

                minCostEdges[u][v] = cost;

                minCostEdges[v][u] = cost; 


            }

        }


    }


// step B
    for (const auto& u_entry : minCostEdges) {
        
        string u = u_entry.first;
        
        for (const auto& v_entry : u_entry.second) {
        
            string v = v_entry.first;
        
            int cost = v_entry.second;
        
        
            undirectedGraph[u].push_back({v, cost});
        
            undirectedEdges.push_back(Edge(u, v, cost));
        
            
        }
        
        
    }

//display the undirected graph in screen

    cout << "\nUndirected Graph:\n";
    for (const auto& entry : undirectedGraph) {
        
        
        cout << entry.first << " -> ";
        
        for (const auto& neighbor : entry.second) {
        
            cout << "(" << neighbor.first << ", " << neighbor.second << ") ";
        
            
            
        }
        
        
        cout << endl;
    
        
        
    }
    
    
}


//TASK 7 

void Graph::primAlMST() {


    if (undirectedGraph.empty()) {

        cout << "Undirected graph not created yet.\n";

        return;


    }

    
    unordered_map<string, bool> inMST;
    
// unique priority queue for that helps perform "Prim's algorithm"
    
    struct PrimPQ {
    
    vector<pair<int, pair<string, string>>> elements;
        
    void push(const pair<int, pair<string, string>>& item) {
    
    
    elements.push_back(item);
    
           
    
    for (size_t i = elements.size()-1; i > 0; --i) {
    
    
    if (elements[i].first < elements[i-1].first) {
    
            swap(elements[i], elements[i-1]);
    
    } else {
    
    break;
    
        }
    
    
    }
    
    
    }
        
    
    pair<int, pair<string, string>> top() const {
    
        return elements.front();
    
        }
        
    
        void pop() {
    
            if (!empty()) {
    
                elements.erase(elements.begin());
    
            }
    
        }
    
        
    
        bool empty() const {
    
            return elements.empty();
    
        }
    
      
    };
    
    
    
    PrimPQ pq;


    

    
    string start = undirectedGraph.begin()->first;
    
    inMST[start] = true;
    
    for (const auto& neighbor : undirectedGraph[start]) {
    
        pq.push({neighbor.second, {start, neighbor.first}});
    
        
    }

    
    int totalWeight = 0;
    
    cout << "\nPrim's MST Edges:\n";

    while (!pq.empty()) {
    
        auto edge = pq.top();
    
        pq.pop();


        string u = edge.second.first;
    
        string v = edge.second.second;
    
        int weight = edge.first;



    
        if (inMST[v]) continue;


        inMST[v] = true;
    
        totalWeight += weight;
    
        cout << u << " - " << v << " : " << weight << endl;

cout<< "\n"<<endl;
    

    
        for (const auto& neighbor : undirectedGraph[v]) {
    
            if (!inMST[neighbor.first]) {
    
                pq.push({neighbor.second, {v, neighbor.first}});
    
            }
    
        }
    }
    
    cout << "Total MST Weight: " << totalWeight << endl;

    
}

// TASK 8 (had several issues with this one, but after some fixes and bug fixes, we got it)

void Graph::kruskalMST(int BUGFIX) {

    if (BUGFIX == 1){

    if (undirectedEdges.empty()) {

        cout << "Undirected graph not created yet.\n";

        return;


    }


    sort(undirectedEdges.begin(), undirectedEdges.end(), 

        [](const Edge& a, const Edge& b) { return a.cost < b.cost; });



    unordered_map<string, string> parent;

    for (const auto& entry : undirectedGraph) {

        parent[entry.first] = entry.first;

    }



    int totalWeight = 0;


        
    cout << "\nKruskal's MST Edges:\n";


    for (const Edge& edge : undirectedEdges) {


        string rootU = findParent(parent, edge.from);

        string rootV = findParent(parent, edge.to);



        if (rootU != rootV) {

            cout << edge.from << " - " << edge.to << " : " << edge.cost << endl;


            totalWeight += edge.cost;


            unionSets(parent, edge.from, edge.to);


        }

    }


    cout << "Total MST Weight: " << totalWeight << endl;
    
    
}
    
}


string Graph::findParent(unordered_map<string, string>& parent, const string& node) {



    if (parent[node] != node) {

        parent[node] = findParent(parent, parent[node]);

    }
    
    
    return parent[node];

    
}



void Graph::unionSets(unordered_map<string, string>& parent, const string& u, const string& v) {

    string rootU = findParent(parent, u);

    string rootV = findParent(parent, v);


    if (rootU != rootV) {


        parent[rootV] = rootU;

    }

    
}






