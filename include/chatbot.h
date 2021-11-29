#ifndef CHATBOT_H_
#define CHATBOT_H_

#include <wx/bitmap.h>
#include <string>

class GraphNode; //Forward declaration
class ChatLogic; //Forward declaration

class ChatBot
{
private:
  //Data handles (owned)
  wxBitmap *_image; //Avatar image

  //Data handles (not owned)
  GraphNode *_currentNode;
  GraphNode *_rootNode;
  ChatLogic *_chatLogic;

  //Proprietary functions
  int ComputeLevenshteinDistance(std::string s1, std::string s2);

public:
  //Constructor & Destructor
  ChatBot();                     //Constructor WITHOUT memory allocation
  ChatBot(std::string filename); //Constructor WITH memory allocation
  ~ChatBot();

  //Rule of Five
  ChatBot(const ChatBot &source);                 //Copy Constructor
  ChatBot &operator=(const ChatBot &source);      //Copy Assigment Operator
  ChatBot(ChatBot &&source) noexcept;             //Move Constructor
  ChatBot &operator=(ChatBot &&source) noexcept;  //Move Assigment Operator

  //Getters & Setters
  void SetCurrentNode(GraphNode *node);
  void SetRootNode(GraphNode *rootNode) { _rootNode = rootNode; }
  void SetChatLogicHandle(ChatLogic *chatLogic) { _chatLogic = chatLogic; }
  ChatLogic* GetChatLogicHandle() { return _chatLogic; }
  wxBitmap *GetImageHandle() { return _image; }

  //Communication
  void ReceiveMessageFromUser(std::string message);
};

#endif /* CHATBOT_H_ */
