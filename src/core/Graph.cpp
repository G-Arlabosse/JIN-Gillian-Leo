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
    struct node node{id, x, y, 0, 0};
    nodes[id] = std::make_unique<struct node>(node);
}

const void Graph::initGraphTransitions() {
    std::vector<b2Vec2> offsets = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}
    };

    struct node adjacent_node;

    for (auto& [id, node] : nodes) {
        for (b2Vec2& offset : offsets) {
            b2Vec2 new_coords = node->coords + offset;
            int new_id = getCoordsId(new_coords);
            if (new_coords.x >= 0
                && new_coords.x <= width
                && new_coords.y >= 0
                && new_coords.y <= height
                && nodes.contains(new_id)) 
            {
                node->transitions.push_back(new_id);
            }
        }
    }
}

const void Graph::dumpGraph() {
    for (const auto& [id, node] : nodes) {
        std::cout << "Node " << id << " (" << node->coords.x << ", " << node->coords.y << ") is empty and has " << node->transitions.size() << " empty adjacent tiles : \n";
        for (const auto& neighbour_id : node->transitions) {
            std::cout << "(" << nodes[neighbour_id]->coords.x << "," << nodes[neighbour_id]->coords.y << ") ";
        }
        std::cout << std::endl;
    }
}

struct NodeComparator {
    bool operator()(const std::pair<int, int>& a, const std::pair<int, int>& b) const {
        return a.second > b.second; // Min-heap basé sur fScore
    }
};

const int Graph::getCoordsId(b2Vec2 coords) {
    return coords.x + coords.y * width;
}

// Fonction pour reconstruire le chemin
std::vector<b2Vec2> Graph::reconstructPath(const std::map<int, int>& cameFrom, int current_id) {
    std::vector<b2Vec2> path;
    path.push_back(nodes[current_id]->coords); // Ajouter le nœud final
    while (cameFrom.find(current_id) != cameFrom.end()) {
        current_id = cameFrom.at(current_id);
        path.push_back(nodes[current_id]->coords);
    }
    std::reverse(path.begin(), path.end()); // Inverser pour obtenir le chemin du départ à l'arrivée
    return path;
}

// Implémentation de l'algorithme A*
std::vector<b2Vec2> Graph::getPath(b2Vec2& start, b2Vec2& goal) {
    int start_x = (int)((start.x + sizeMultiplier / 2) / sizeMultiplier);
    int start_y = (int)((start.y + sizeMultiplier / 2) / sizeMultiplier);
    int goal_x = (int)((goal.x + sizeMultiplier / 2) / sizeMultiplier);
    int goal_y = (int)((goal.y + sizeMultiplier / 2) / sizeMultiplier);

    b2Vec2 unscaled_start = { start_x,  start_y };
    b2Vec2 unscaled_goal = { goal_x, goal_y };

    int start_id = getCoordsId(unscaled_start);
    int goal_id = getCoordsId(unscaled_goal);

    // Vérifier si les nœuds de départ et d'arrivée existent
    if (!nodes.count(start_id) || !nodes.count(goal_id)) {
        std::cout << "Pas de chemin possible\n";
        return {}; // Pas de chemin possible
    }

    // File de priorité pour l'ensemble ouvert
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, NodeComparator> openSet;
    std::set<int> openSetTracker; // Pour vérifier rapidement si un nœud est dans openSet

    std::map<int, int> cameFrom; // Pour reconstruire le chemin

    // Initialisation des scores
    std::map<int, int> gScore;
    std::map<int, int> fScore;
    for (const auto& pair : nodes) {
        gScore[pair.first] = std::numeric_limits<int>::max();
        fScore[pair.first] = std::numeric_limits<int>::max();
    }
    gScore[start_id] = 0;
    fScore[start_id] = b2Distance(unscaled_start, unscaled_goal);

    openSet.push({ start_id, fScore[start_id] });
    openSetTracker.insert(start_id);

    while (!openSet.empty()) {
        auto current_pair = openSet.top();
        int current_id = current_pair.first;
        openSet.pop();
        openSetTracker.erase(current_id);

        // Si on atteint l'objectif, reconstruire et retourner le chemin
        if (current_id == goal_id) {
            return reconstructPath(cameFrom, current_id);
        }

        struct node* current_node = nodes[current_id].get();

        // Explorer les voisins
        for (int neighbor_id : current_node->transitions) {
            struct node* neighbor_node = nodes[neighbor_id].get();
            int tentative_gScore = gScore[current_id] + 1; // Coût uniforme supposé

            if (tentative_gScore < gScore[neighbor_id]) {
                cameFrom[neighbor_id] = current_id;
                gScore[neighbor_id] = tentative_gScore;
                fScore[neighbor_id] = tentative_gScore + b2Distance(neighbor_node->coords, goal);

                if (openSetTracker.find(neighbor_id) == openSetTracker.end()) {
                    openSet.push({ neighbor_id, fScore[neighbor_id] });
                    openSetTracker.insert(neighbor_id);
                }
            }
        }
    }

    return {}; // Pas de chemin trouvé
}

sf::VertexArray Graph::getPathRender(std::vector<b2Vec2>& path) {
    auto path_render = sf::VertexArray(sf::PrimitiveType::LineStrip, path.size());
    for (int i = 0; i < path.size(); i++) {
        path_render[i].position = sf::Vector2(path[i].x * sizeMultiplier, path[i].y * sizeMultiplier);
        path_render[i].color = sf::Color::Cyan;
    }
    return path_render;
}