#include "Day23.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <limits>

// SMAStar.cpp : This file contains the 'main' function. Program execution begins and ends there.

/*
* Simplified Memory Bounded A* Search
*
* Search algorithm based on SMA*+ as presented by Lovinger and Zhang in GCAI 2017.
* Applied to AoC 2021 day 15 problem.
*
* From StackOverflow:
*
* I have solved problem, It should work in all IDEs I think, but it definitely works in Visual Studio.
* PROJECT->Properties->Configuration Properties->Linker->System->Stack Reserve Size=4194304 .
* This makes stack size 4 MB
*
*/

static std::map<char, short> grid_coding = { {'#',0}, {'.',1}, {'<',2}, {'>',3}, {'^',4}, {'v',5} };

// Details of a single location (state) in the path-finding algorithm.
struct State {
    int column;
    int row;
    short value;
    int bestg;

    State(int c, int r, char v) : column(c), row(r), value(v), bestg(INT_MAX) {}

    std::string to_string() {
        char buffer[80];
        sprintf_s(buffer, "(%d, %d, %d)", column, row, value);
        return buffer;
    }
};

typedef std::shared_ptr<State> StatePtr;

struct Grid
{
    int NumColumns;
    int NumRows;

    StatePtr* Buffer;

    Grid() {
        NumColumns = NumRows = 0;
        Buffer = nullptr;
    }

    StatePtr getState(int c, int r) const {
        return Buffer[r * NumColumns + c];
    }

    void setState(int c, int r, char value) {
        Buffer[r * NumColumns + c].reset(new State(c, r, value));
    }

    void dispose() {
        delete Buffer;
        NumColumns = NumRows = 0;
    }
};


// Forgotten successor states of a Node, with their backed up f-costs.
/*
struct ForgottenSuccessors {
    std::map<StatePtr, unsigned> items;
};
*/
/*
struct Node;
typedef std::shared_ptr<Node> NodePtr;
*/

struct Node;
typedef std::shared_ptr<Node> NodePtr;

// Search node.
struct Node {
    // This state and its current f-cost estimate.
    StatePtr state;
    unsigned fcost;
    unsigned gcost;
    // Forgotten successors of this node, with their backed up f-costs.
    std::map<StatePtr, unsigned> forgotten;
    // Connection to the parent node (or nullptr if there is none).
    NodePtr parent;

    Node(StatePtr s, NodePtr p, unsigned g) : state(s), parent(p), gcost(g), fcost(0) {}

    ~Node()
    {
        if (parent != nullptr) {
            parent.reset();
        }
        forgotten.clear();
    }
};

class SearchNode
{
public:

    SearchNode(StatePtr s, NodePtr p, unsigned g) : state(s), parent(p), gcost(g), fcost(0) {}
    SearchNode() {}
    ~SearchNode() { parent.reset(); }

    // Default size of a block of pre-allocated objects of this class.
    static int BatchSize;

    void* operator new(size_t size) {
        if (size != sizeof(SearchNode)) {
            // A derived type of this class?
            return malloc(size);
        }
        else if (newlist == nullptr) {
            // Allocate a new block of nodes.
            newlist = (SearchNode*)(new char[BatchSize * sizeof(SearchNode)]);
            for (int i = 0; i < BatchSize - 1; i++) {
                newlist[i].freepointer = &(newlist[i + 1]);
            }
            newlist[BatchSize - 1].freepointer = nullptr;
        }
        SearchNode* savenew = newlist;
        newlist = newlist->freepointer;
        return savenew;
    };

    void operator delete(void* p) {
        SearchNode* np = (SearchNode*)p;
        np->freepointer = newlist;
        newlist = np;
    }

    static SearchNode* newlist;
    union {
        SearchNode* freepointer;
        StatePtr state;
    };
    unsigned fcost;
    unsigned gcost;
    std::map<StatePtr, unsigned> forgotten;
    NodePtr parent;
};

// Required static initialisations.
int SearchNode::BatchSize = 1000;
SearchNode* SearchNode::newlist = nullptr;

// Node comparator for open list (using lambda).
struct NodeCompare
{
    bool operator()(const NodePtr n1, const NodePtr n2) const /* noexcept */
    {
        return (n1->fcost < n2->fcost);
    }
};

// Open queue must be ordered to provide node in order of smallest f-cost first.
static std::multiset<NodePtr, NodeCompare> OpenList;

// Shared record of the grid for the problem.
static Grid theGrid;


// Read the contents of the given input file and set up in a grid object, 
// representing the landscape to be explored.
static bool ReadInputFile(const std::string& fileName, Grid& grid)
{
    // Open the specified file.
    std::ifstream input(fileName);
    if (!input.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return false;
    }

    // Read the lines.
    size_t nr = 0, nc = 0;
    std::string line;
    std::vector<std::string> lines;
    while (!input.eof()) {
        input >> line;
        if (nc < line.length()) {
            nc = line.length();
        }
        lines.push_back(line);
    }
    input.close();

    // Set up the grid and associated buffer.
    nr = lines.size();
    size_t bufferSize = nc * nr;
    grid.NumColumns = nc;
    grid.NumRows = nr;
    grid.Buffer = new StatePtr[bufferSize];

    // Populate the grid buffer.
    for (auto i = 0; i < grid.NumColumns; i++) {
        for (auto j = 0; j < grid.NumRows; j++) {
            grid.setState(i, j, grid_coding[lines[j][i]]);
        }
    }

    return true;
}

static void ExpandMap(const Grid& fromGrid, Grid& toGrid)
{
    delete toGrid.Buffer;
    int nc = toGrid.NumColumns = fromGrid.NumColumns * 5;
    int nr = toGrid.NumRows = fromGrid.NumRows * 5;
    toGrid.Buffer = new StatePtr[nc * nr];
    // Populate the grid buffer.
    for (auto i = 0; i < toGrid.NumColumns; i++) {
        for (auto j = 0; j < toGrid.NumRows; j++) {
            StatePtr sp = fromGrid.getState(i % fromGrid.NumColumns, j % fromGrid.NumRows);
            int offset = (i / fromGrid.NumColumns) + (j / fromGrid.NumRows);
            char value = sp->value + offset;
            while (value > 9) {
                value -= 9;
            }
            toGrid.setState(i, j, value);
        }
    }
}

static bool isGoalNode(const NodePtr n)
{
    return ((n->state->row == theGrid.NumRows - 1) && (n->state->value == '.'));
}

static bool populateNeighbour(int col, int row, std::vector<StatePtr>& neighbours)
{
    if ((col >= 0) && (col < theGrid.NumColumns) && (row >= 0) && (row < theGrid.NumRows))
    {
        // Cannot go into forest (#).
        if (theGrid.getState(col, row)->value != 0) {
            neighbours.push_back(theGrid.getState(col, row));
            return true;
        }
    }
    return false;
}

static void neighbours(const StatePtr n, std::vector<StatePtr>& neighbours)
{
    auto c = n->column;
    auto r = n->row;
    // Where are we at present?
    switch (n->value) {
    case 0: // #
        // Shouldn't be here.
        return;
    case 1: // .
        // Simple path - can go in any direction.
        populateNeighbour(c, r - 1, neighbours);
        populateNeighbour(c + 1, r, neighbours);
        populateNeighbour(c, r + 1, neighbours);
        populateNeighbour(c - 1, r, neighbours);
        break;
    // Other options are "slippy" squares - must go in specified direction.
    case 2: // <
        populateNeighbour(c - 1, r, neighbours);
        break;
    case 3: // >
        populateNeighbour(c + 1, r, neighbours);
        break;
    case 4: // ^
        populateNeighbour(c, r - 1, neighbours);
        break;
    case 5: // v
        populateNeighbour(c, r + 1, neighbours);
        break;
    }
}

static bool isStateInPath(const StatePtr sp1, const NodePtr np1)
{
    for (auto node = np1; node != nullptr; node = node->parent) {
        if (node->state == sp1) {
            return true;
        }
    }
    return false;
}

static bool updateStateBestPathCost(StatePtr state, unsigned pathCost)
{
    unsigned bg = state->bestg;
    if (bg > pathCost) {
        state->bestg = pathCost;
        return true;
    }
    return false;
}

// Get successors of given node, eliminating potential cycles.
static void expand(const NodePtr np, std::vector<NodePtr>& successors)
{
    successors.clear();
    std::vector<StatePtr> neighbourStates;
    neighbours(np->state, neighbourStates);
    for (StatePtr neighbour : neighbourStates) {
        if (!isStateInPath(neighbour, np)) {
            NodePtr sp = std::make_shared<Node>(neighbour, np, np->gcost + neighbour->value);
            // Don't bother adding successor if better path to that state has been seen already.
            if (updateStateBestPathCost(neighbour, sp->gcost)) {
                successors.push_back(sp);
            }
        }
    }
}

static bool hasSuccessors(const NodePtr n)
{
    return true;
}

static size_t pathDepth(const NodePtr leaf)
{
    size_t count = 0;
    NodePtr n = leaf;
    while (n != nullptr) {
        count++;
        n = n->parent;
    }
    return count;
}

// Heuristic function for the given search node.
static unsigned h(const NodePtr n)
{
    // Manhattan distance from state of this node to the goal node.
    unsigned score = std::abs(theGrid.NumColumns - 1 - n->state->column);
    score += std::abs(theGrid.NumRows - 1 - n->state->row);
    return score;
}

static unsigned cullWorstLeaf()
{
    // Here we could use the culling heuristic to select the worst leaf node to cull.
    // However for the moment, just choose the node with the highest f-cost.
    unsigned result;
    // Note that OpenList must be a sorted container that supports access at both ends.
    auto ep = std::prev(OpenList.end());
    auto wp = *ep;
    result = wp->fcost;
    ep = OpenList.erase(ep);
    // Keep minimum f-cost, of forgotten worst leaf nodes, under parent node.
    auto pp = wp->parent;
    if (pp != nullptr) {
        auto f = pp->fcost;
        if (pp->forgotten.empty()) {
            f = INT_MAX;
        }
        // Record forgotten f-cost, backing up to parent if needed.
        pp->forgotten[wp->state] = wp->fcost;
        if (f > wp->fcost) {
            pp->fcost = wp->fcost;
        }
        // If parent is not in OpenList then add it (to revisit later if needed).
        if (OpenList.find(pp) == OpenList.end()) {
            OpenList.insert(pp);
        }
    }
    return result;
}

static bool smaStarSearch(NodePtr startNode, size_t memoryLimit, NodePtr& goalFound)
{
    unsigned lowTide = INT_MAX;
    unsigned highTide = 0;
    // Initialise empty open list (O).
    OpenList.clear();
    // Add initial node to O.
    OpenList.insert(startNode);
    // While O not empty.
    while (!OpenList.empty())
    {
        // b is min-cost node in O.
        auto bp = OpenList.begin();
        NodePtr b = *bp;
        lowTide = b->fcost;
        OpenList.erase(bp);
        if (isGoalNode(b)) {
            goalFound = b;
            return true;
        }
        else if (b->fcost == INT_MAX) {
            return false;
        }

        std::vector<NodePtr> N;
        // Has b been expanded before?
        if (b->forgotten.empty()) {
            // No - Expand b. N is list of successors of b.
            expand(b, N);
        }
        else {
            // N is forgotten successors of b.
            for (auto pair : b->forgotten) {
                NodePtr sp = std::make_shared<Node>(pair.first, b, b->gcost + pair.first->value);
                sp->fcost = pair.second;
                N.push_back(sp);
            }
        }

        for (auto n : N)
        {
            auto nstate = n->state;
            auto& items = b->forgotten;
            if (items.find(nstate) != items.end()) {
                n->fcost = items[nstate];
                items.erase(nstate);
            }
            else if (!isGoalNode(n) && (!hasSuccessors(n) || (pathDepth(n) + 1 > memoryLimit))) {
                n->fcost = INT_MAX;
            }
            else {
                n->fcost = std::max(b->fcost, n->gcost + h(n));
            }

            OpenList.insert(n);
        }

        N.clear();

        // Limit memory if needed.
        while (memoryLimit < OpenList.size()) {
            highTide = cullWorstLeaf();
        }
    }
    // Open list should be exhausted - failed search.
    return false;
}

static NodePtr startNode() {
    for (int c = 0; c < theGrid.NumColumns; c++) {
        if (theGrid.getState(c, 0)->value == 1) {
            NodePtr start(new Node(theGrid.getState(c, 0), nullptr, 0));
            start->fcost = h(start);
            return start;
        }
    }
    return nullptr;
}

static void displayPath(NodePtr goal)
{
    for (NodePtr np = goal; np != nullptr; np = np->parent) {
        std::cout << np->state->to_string() << ", " << np->gcost << "/" << np->fcost << std::endl;
    }
}

static int solve_part2()
{
    std::cout << "\nPart 2.\nLoading input file and expanding grid." << std::endl;

    Grid smallGrid;
    ReadInputFile("input/input15.txt", smallGrid);
    ExpandMap(smallGrid, theGrid);

    std::cout << "Searching for optimal path..." << std::endl;

    NodePtr start = startNode();
    NodePtr goal;
    try {
        if (smaStarSearch(start, 1000000, goal)) {
            std::cout << "Search succeeded." << std::endl;
        }
        else {
            std::cout << "Search failed." << std::endl;
        }
    }
    catch (std::exception e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    if (goal == nullptr) {
        std::cout << "Search failed - no path found." << std::endl;
        return -1;
    }
    else {
        std::cout << "Search completed - path details:" << std::endl;
        displayPath(goal);
        std::cout << "Total path risk = " << goal->gcost << std::endl;
    }
    return 0;
}

// Day23 implementations.

void Day23::execute()
{
    cout << "Part 1. Test Input..." << endl;
    part1("input/test23.txt");
}

void Day23::part1(const char* fileName)
{
    std::cout << "Part 1.\nLoading input file." << std::endl;
    if (ReadInputFile(fileName, theGrid)) {
        std::cout << "Searching for longest path..." << std::endl;
        NodePtr start = startNode();
        NodePtr goal;
        smaStarSearch(start, 1000000, goal);
        if (goal == nullptr) {
            std::cout << "Search failed - no path found." << std::endl;
        }
        else {
            std::cout << "Search completed - path details:" << std::endl;
            displayPath(goal);
            std::cout << "Total path risk = " << goal->gcost << std::endl;
        }
    }
}

void Day23::part2(const char* fileName)
{
}
