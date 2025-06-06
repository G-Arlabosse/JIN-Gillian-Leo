#include "Graph.h"
#include <iostream>


Graph::Graph():
    width {},
    height {},
    nodes {}
{}

const void Graph::setWidth(int new_width) {
    width = new_width;
}

const void Graph::setHeight(int new_height) {
    height = new_height;
}

const void Graph::addNode(int x, int y) {
    int id = y * width + x;
    struct node node{id, x, y};
    nodes[id] = std::make_unique<struct node>(node);
}

const void Graph::initGraphTransitions() {
    std::vector<b2Vec2> offsets = {
        {0, -1} , {1, 0}, {0, 1}, {-1, 0},
    //     Up      Right   Down    Left      
    };
    std::vector<bool> dirOk;
    std::vector<std::pair<int, int>> diags = {
        {0, 1},   {1, 2},    {2, 3},  {3, 0}
    //  UpRight, DownRight, DownLeft, UpLeft 
    };
    
    struct node adjacent_node;

    for (auto& [id, node] : nodes) {
        dirOk = { false, false, false, false };
        //Cardinal directions
        for (int i=0; i<4; i++) {
            b2Vec2 new_coords = node->coords + offsets[i];
            int new_id = getCoordsId(new_coords);
            if (new_coords.x >= 0 && new_coords.x <= width
                    && new_coords.y >= 0 && new_coords.y <= height
                    && nodes.contains(new_id)) {
                dirOk[i] = true;
                struct transition new_transition { new_id, 10 };
                node->transitions.push_back(new_transition);
            }
        }
        //Diagonal directions
        for (int i = 0; i < 4; i++) {
            auto diag = diags[i];
            if (dirOk[diag.first] && dirOk[diag.second]) {
                b2Vec2 new_coords = node->coords + offsets[diag.first] + offsets[diag.second];
                int new_id = getCoordsId(new_coords);
                if (new_coords.x >= 0 && new_coords.x <= width
                        && new_coords.y >= 0 && new_coords.y <= height
                        && nodes.contains(new_id)) {
                    struct transition new_transition { new_id, 14 };
                    node->transitions.push_back(new_transition);
                }
            }
        }
    }
}

const void Graph::dumpGraph() {
    /*for (const auto& [id, node] : nodes) {
        std::cout << "Node " << id << " (" << node->coords.x << ", " << node->coords.y << ") is empty and has " << node->transitions.size() << " empty adjacent tiles : \n";
        for (const auto& neighbour_id : node->transitions) {
            std::cout << "(" << nodes[neighbour_id]->coords.x << "," << nodes[neighbour_id]->coords.y << ") ";
        }
        std::cout << std::endl;
    }*/
}

struct NodeComparator {
    bool operator()(const std::pair<int, int>& a, const std::pair<int, int>& b) const {
        return a.second > b.second; // Min-heap basé sur fScore
    }
};

const int Graph::getCoordsId(b2Vec2 coords) {
    return coords.x + coords.y * width;
}

std::vector<b2Vec2> Graph::reconstructPath(const std::map<int, int>& cameFrom, int current_id) {
    std::vector<b2Vec2> path;
    path.push_back(nodes[current_id]->coords);
    while (cameFrom.find(current_id) != cameFrom.end()) {
        current_id = cameFrom.at(current_id);
        path.push_back(nodes[current_id]->coords);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<b2Vec2> Graph::getPath(b2Vec2& start, b2Vec2& goal) {
    int start_x = (int)((start.x + sizeMultiplier / 2) / sizeMultiplier);
    int start_y = (int)((start.y + sizeMultiplier / 2) / sizeMultiplier);
    int goal_x = (int)((goal.x + sizeMultiplier / 2) / sizeMultiplier);
    int goal_y = (int)((goal.y + sizeMultiplier / 2) / sizeMultiplier);

    b2Vec2 unscaled_start = { start_x,  start_y };
    b2Vec2 unscaled_goal = { goal_x, goal_y };

    int start_id = getCoordsId(unscaled_start);
    int goal_id = getCoordsId(unscaled_goal);

    // Checks if the given nodes exist
    if (!nodes.count(start_id) || !nodes.count(goal_id)) {
        std::cout << "Either Node Start or Goal doesnt exist\n";
        return {};
    }

    // Priority Queue for nodes to explore
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, NodeComparator> openSet;
    std::set<int> openSetTracker; // To quickly check if a node is in openSet

    std::map<int, int> cameFrom; // Used later to reconstruct the path

    std::map<int, int> gScore; //G score, the weight of the path
    std::map<int, int> fScore; //F score, the heuristic of the path
    for (const auto& pair : nodes) {
        gScore[pair.first] = std::numeric_limits<int>::max();
        fScore[pair.first] = std::numeric_limits<int>::max();
    }
    gScore[start_id] = 0;
    fScore[start_id] = 10 * b2Distance(unscaled_start, unscaled_goal);

    openSet.push({ start_id, fScore[start_id] });
    openSetTracker.insert(start_id);

    while (!openSet.empty()) {
        auto current_pair = openSet.top();
        int current_id = current_pair.first;
        openSet.pop();
        openSetTracker.erase(current_id);

        // Arrived to the goal
        if (current_id == goal_id) {
            return reconstructPath(cameFrom, current_id);
        }

        struct node* current_node = nodes[current_id].get();

        // Explore neighbors
        for (auto transition : current_node->transitions) {
            int neighbor_id = transition.neighbor_id;
            struct node* neighbor_node = nodes[neighbor_id].get();
            int tentative_gScore = gScore[current_id] + transition.weight;

            if (tentative_gScore < gScore[neighbor_id]) {
                cameFrom[neighbor_id] = current_id;
                gScore[neighbor_id] = tentative_gScore;
                fScore[neighbor_id] = tentative_gScore + 10 * b2Distance(neighbor_node->coords, goal);

                if (openSetTracker.find(neighbor_id) == openSetTracker.end()) {
                    openSet.push({ neighbor_id, fScore[neighbor_id] });
                    openSetTracker.insert(neighbor_id);
                }
            }
        }
    }

    return {}; // No path found
}

sf::VertexArray Graph::getPathRender(std::vector<b2Vec2>& path) {
    auto path_render = sf::VertexArray(sf::PrimitiveType::LineStrip, path.size());
    for (int i = 0; i < path.size(); i++) {
        path_render[i].position = sf::Vector2(path[i].x * sizeMultiplier, path[i].y * sizeMultiplier);
        path_render[i].color = sf::Color::Cyan;
    }
    return path_render;
}