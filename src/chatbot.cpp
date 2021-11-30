#include <iostream>
#include <random>
#include <algorithm>
#include <ctime>

#include "chatlogic.h"
#include "graphnode.h"
#include "graphedge.h"
#include "chatbot.h"

//Constructor WITHOUT memory allocation
ChatBot::ChatBot()
{
  //Invalidate data handles
  _image = nullptr;
  _chatLogic = nullptr;
  _rootNode = nullptr;
}

//Constructor WITH memory allocation
ChatBot::ChatBot(std::string filename)
{
  std::cout << "ChatBot Constructor" << std::endl;

  //Invalidates data handles
  _chatLogic = nullptr;
  _rootNode = nullptr;

  //Loads image into heap memory
  _image = new wxBitmap(filename, wxBITMAP_TYPE_PNG);
}

ChatBot::~ChatBot()
{
  std::cout << "ChatBot Destructor" << std::endl;

  //Deallocates heap memory
  if(_image != NULL) //Attention: wxWidgets used NULL and not nullptr
  {
    delete _image;
    _image = NULL;
  }
}

//Copy Constructor
ChatBot::ChatBot(const ChatBot &source)
{
  std::cout << "Chatbot Copying Constructor" << std::endl;

  std::cout << "Size of _image is " << sizeof(*_image) << std::endl;

  _image = new wxBitmap();
  *_image = *source._image;
  _chatLogic = source._chatLogic;
  _rootNode = source._rootNode;

  _chatLogic->SetChatbotHandle(this);
}

//Copy Assigment Operator
ChatBot &ChatBot::operator=(const ChatBot &source)
{
  std::cout << "Chatbot Copy Assignment Operator" << std::endl;

  //Protectio against self-assignment
  if (this != &source)
  {
    _image = new wxBitmap();
    *_image = *source._image;
    _chatLogic = source._chatLogic;
    _rootNode = source._rootNode;

    _chatLogic->SetChatbotHandle(this);
  }

  return *this;
}

//Move Constructor
ChatBot::ChatBot(ChatBot &&source) noexcept
{
  std::cout << "Chatbot Move Constructor" << std::endl;

  _image = source._image;
  _chatLogic = source._chatLogic;
  _rootNode = source._rootNode;

  _chatLogic->SetChatbotHandle(this);

  source._image = NULL;
  source._chatLogic = nullptr;
  source._rootNode = nullptr;
}

//Move Assigment Operator
ChatBot &ChatBot::operator=(ChatBot &&source) noexcept
{
  std::cout << "Chatbot Move Assigment Operator" << std::endl;

  //Protection agains self-assigment
  if (this != &source)
  {
    _image = source._image;
    _chatLogic = source._chatLogic;
    _rootNode = source._rootNode;

    _chatLogic->SetChatbotHandle(this);

    source._image = NULL;
    source._chatLogic = nullptr;
    source._rootNode = nullptr;
  }

  return *this;
}

void ChatBot::ReceiveMessageFromUser(std::string message)
{
  // loop over all edges and keywords and compute Levenshtein distance to query
  typedef std::pair<GraphEdge *, int> EdgeDist;
  std::vector<EdgeDist> levDists; // format is <ptr,levDist>

  for (size_t i = 0; i < _currentNode->GetNumberOfChildEdges(); ++i)
  {
    GraphEdge *edge = _currentNode->GetChildEdgeAtIndex(i);
    for (auto keyword : edge->GetKeywords())
    {
      EdgeDist ed{edge, ComputeLevenshteinDistance(keyword, message)};
      levDists.push_back(ed);
    }
  }

  //Selects best fitting edge to proceed along
  GraphNode *newNode;
  if (levDists.size() > 0)
  {
    //Sorts in ascending order of Levenshtein distance (best fit is at the top)
    std::sort(levDists.begin(), levDists.end(), [](const EdgeDist &a, const EdgeDist &b) { return a.second < b.second; });
    newNode = levDists.at(0).first->GetChildNode(); // after sorting the best edge is at first position
  }
  else
  {
    //Goes back to root node
    newNode = _rootNode;
  }

  //Tells current node to move chatbot to new node
  _currentNode->MoveChatbotToNewNode(newNode);
}

void ChatBot::SetCurrentNode(GraphNode *node)
{
  //Updates pointer to current node
  _currentNode = node;

  //Selects a random node answer (if several answers should exist)
  std::vector<std::string> answers = _currentNode->GetAnswers();
  std::mt19937 generator(int(std::time(0)));
  std::uniform_int_distribution<int> dis(0, answers.size() - 1);
  std::string answer = answers.at(dis(generator));

  //Sends selected node answer to user
  _chatLogic->SendMessageToUser(answer);
}

int ChatBot::ComputeLevenshteinDistance(std::string s1, std::string s2)
{
  //Converts both strings to upper-case before comparing
  std::transform(s1.begin(), s1.end(), s1.begin(), ::toupper);
  std::transform(s2.begin(), s2.end(), s2.begin(), ::toupper);

  //Computes Levenshtein distance measure between both strings
  const size_t m(s1.size());
  const size_t n(s2.size());

  if (m == 0)
  return n;
  if (n == 0)
  return m;

  size_t *costs = new size_t[n + 1];

  for (size_t k = 0; k <= n; k++)
  costs[k] = k;

  size_t i = 0;
  for (std::string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i)
  {
    costs[0] = i + 1;
    size_t corner = i;

    size_t j = 0;
    for (std::string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j)
    {
      size_t upper = costs[j + 1];
      if (*it1 == *it2)
      {
        costs[j + 1] = corner;
      }
      else
      {
        size_t t(upper < corner ? upper : corner);
        costs[j + 1] = (costs[j] < t ? costs[j] : t) + 1;
      }

      corner = upper;
    }
  }

  int result = costs[n];
  delete[] costs;

  return result;
}
