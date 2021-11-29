#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <tuple>
#include <algorithm>

#include "graphedge.h"
#include "graphnode.h"
#include "chatbot.h"
#include "chatlogic.h"

//Constructor
ChatLogic::ChatLogic() { }

//Destructor
ChatLogic::~ChatLogic() { }

//Template
template <typename T>
void ChatLogic::AddAllTokensToElement(std::string tokenID, tokenlist &tokens, T &element)
{
  //Finds all occurences for current node
  auto token = tokens.begin();
  while (true)
  {
    token = std::find_if(token, tokens.end(), [&tokenID](const std::pair<std::string, std::string> &pair) { return pair.first == tokenID;; });
    if (token != tokens.end())
    {
      element.AddToken(token->second); //Adds new keyword to edge
      token++;                         //Increments iterator to next element
    }
    else
    {
      break; //Quits infinite while-loop
    }
  }
} //End Template

void ChatLogic::LoadAnswerGraphFromFile(std::string filename)
{
  //Loads file with answer graph elements
  std::ifstream file(filename);

  //Checks for file availability and process it line by line
  if (file)
  {
    //Loops over all lines in the file
    std::string lineStr;
    while (getline(file, lineStr))
    {
      //Extracts all tokens from current line
      tokenlist tokens;
      while (lineStr.size() > 0)
      {
        //Extracts next token
        int posTokenFront = lineStr.find("<");
        int posTokenBack = lineStr.find(">");
        if (posTokenFront < 0 || posTokenBack < 0)
        {
          break; //Quits loop if no complete token has been found
        }
        std::string tokenStr = lineStr.substr(posTokenFront + 1, posTokenBack - 1);

        //Extracts token type and info
        int posTokenInfo = tokenStr.find(":");
        if (posTokenInfo != std::string::npos)
        {
          std::string tokenType = tokenStr.substr(0, posTokenInfo);
          std::string tokenInfo = tokenStr.substr(posTokenInfo + 1, tokenStr.size() - 1);

          //Adds token to vector
          tokens.push_back(std::make_pair(tokenType, tokenInfo));
        }

        //Removes token from current line
        lineStr = lineStr.substr(posTokenBack + 1, lineStr.size());
      }

      //Processes tokens for current line
      auto type = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "TYPE"; });
      if (type != tokens.end())
      {
        //Checks for id
        auto idToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "ID"; });
        if (idToken != tokens.end())
        {
          //Extracts id from token
          int id = std::stoi(idToken->second);

          //Node-based processing
          if (type->second == "NODE")
          {
            //Checks if node with this ID exists already
            auto newNode = std::find_if(_nodes.begin(), _nodes.end(), [&id](std::unique_ptr<GraphNode>& node) { return node->GetID() == id; });

            //Creates new element if ID does not yet exist
            if (newNode == _nodes.end())
            {
              _nodes.emplace_back(std::make_unique<GraphNode>(id));
              newNode = _nodes.end() - 1; //Gets iterator to last element

              //Adds all answers to current node
              AddAllTokensToElement("ANSWER", tokens, **newNode);
            }
          }

          //Edge-based processing
          if (type->second == "EDGE")
          {
            //Finds tokens for incoming (parent) and outgoing (child) node
            auto parentToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "PARENT"; });
            auto childToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "CHILD"; });

            if (parentToken != tokens.end() && childToken != tokens.end())
            {
              //Gets iterator on incoming and outgoing node via ID search
              auto parentNode = std::find_if(_nodes.begin(), _nodes.end(), [&parentToken](std::unique_ptr<GraphNode> const &node) { return node->GetID() == std::stoi(parentToken->second); });
              auto childNode = std::find_if(_nodes.begin(), _nodes.end(), [&childToken](std::unique_ptr<GraphNode> const &node) { return node->GetID() == std::stoi(childToken->second); });

              //Creates new edge
              std::unique_ptr<GraphEdge> edge = std::make_unique<GraphEdge>(id);
              edge->SetChildNode((*childNode).get());
              edge->SetParentNode((*parentNode).get());

              //Finds all keywords for current node
              AddAllTokensToElement("KEYWORD", tokens, *edge);

              //Stores reference in child node and parent node
              (*childNode)->AddEdgeToParentNode(edge.get());
              (*parentNode)->AddEdgeToChildNode(std::move(edge));
            }
          }
        }
        else
        {
          std::cout << "Error: ID missing. Line is ignored!" << std::endl;
        }
      }
    } //EOF loop over all lines in the file

    file.close();

  } //EOF check for file availability
  else
  {
    std::cout << "File could not be opened!" << std::endl;
    return;
  }

  //Identifies root node
  GraphNode *rootNode = nullptr;
  for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it)
  {
    //Searches for nodes which have no incoming edges
    if ((*it)->GetNumberOfParents() == 0)
    {

      if (rootNode == nullptr)
      {
        rootNode = (*it).get(); //Assigns current node to root
      }
      else
      {
        std::cout << "ERROR : Multiple root nodes detected" << std::endl;
      }
    }
  }

  //Moves chatbot to graph root node
  ChatBot _localChatBot("../images/chatbot.png"); //Creates a local instance of chatbot
  _localChatBot.SetChatLogicHandle(this);
  _localChatBot.SetRootNode(rootNode);
  rootNode->MoveChatbotHere(std::move(_localChatBot));
}

void ChatLogic::SetPanelDialogHandle(ChatBotPanelDialog *panelDialog)
{
  _panelDialog = panelDialog;
}

void ChatLogic::SetChatbotHandle(ChatBot *chatbot)
{
  _chatBot = chatbot;
}

void ChatLogic::SendMessageToChatbot(std::string message)
{
  _chatBot->ReceiveMessageFromUser(message);
}

void ChatLogic::SendMessageToUser(std::string message)
{
  _panelDialog->PrintChatbotResponse(message);
}

wxBitmap *ChatLogic::GetImageFromChatbot()
{
  return _chatBot->GetImageHandle();
}
