#ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include <memory>
#include <string>
#include <vector>
#include "chatbot.h"

// forward declarations
class GraphEdge;

class GraphNode
{
private:
    //// STUDENT CODE
    ////

    //Data handles (owned)
    std::vector< std::unique_ptr<GraphEdge> > _childEdges;

    //Data handles (not owned)
    std::vector<GraphEdge *> _parentEdges; // edges to preceding nodes
    ChatBot *_chatBot;

    ////
    //// EOF STUDENT CODE

    //Proprietary members
    int _id;
    std::vector<std::string> _answers;

public:
    //Constructor & Destructor
    GraphNode(int id);
    ~GraphNode();

    //Getters & Setters
    int GetID() { return _id; }
    int GetNumberOfChildEdges() { return _childEdges.size(); }
    GraphEdge *GetChildEdgeAtIndex(int index);
    std::vector<std::string> GetAnswers() { return _answers; }
    int GetNumberOfParents() { return _parentEdges.size(); }

    //Proprietary functions
    void AddToken(std::string token); // add answers to list
    void AddEdgeToParentNode(GraphEdge *edge);
    void AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge);

    //// STUDENT CODE
    ////

    void MoveChatbotHere(ChatBot *chatbot);

    ////
    //// EOF STUDENT CODE

    void MoveChatbotToNewNode(GraphNode *newNode);
};

#endif /* GRAPHNODE_H_ */
