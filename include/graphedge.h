#ifndef GRAPHEDGE_H_
#define GRAPHEDGE_H_

#include <vector>
#include <string>

class GraphNode; //Forward declaration

class GraphEdge
{
private:
    //Data handles (not owned)
    GraphNode *_childNode;
    GraphNode *_parentNode;

    //Proprietary members
    int _id;
    std::vector<std::string> _keywords; //List of topics associated with this edge


public:
    //Constructor & Destructor
    GraphEdge(int id);

    //Getters & Setters
    int GetID() { return _id; }
    void SetChildNode(GraphNode *childNode);
    void SetParentNode(GraphNode *parentNode);
    GraphNode *GetChildNode() { return _childNode; }
    std::vector<std::string> GetKeywords() { return _keywords; }

    //Proprietary functions
    void AddToken(std::string token);
};

#endif /* GRAPHEDGE_H_ */
