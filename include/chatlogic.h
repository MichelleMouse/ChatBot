#ifndef CHATLOGIC_H_
#define CHATLOGIC_H_

#include <memory>
#include <string>
#include <vector>
#include "chatgui.h"

//Forward declarations
class ChatBot;
class GraphEdge;
class GraphNode;

class ChatLogic
{
private:
  //Data handles (owned)
  std::vector< std::unique_ptr<GraphNode> > _nodes;

  //Data handles (not owned)
  GraphNode *_currentNode;
  ChatBot *_chatBot;
  ChatBotPanelDialog *_panelDialog;

  //Proprietary type definitions
  typedef std::vector<std::pair<std::string, std::string>> tokenlist;

  //Proprietary functions
  template <typename T>
  void AddAllTokensToElement(std::string tokenID, tokenlist &tokens, T &element);

public:
  //Constructor & Destructor
  ChatLogic();
  ~ChatLogic();

  //Getters & Setters
  void SetPanelDialogHandle(ChatBotPanelDialog *panelDialog);
  void SetChatbotHandle(ChatBot *chatbot);

  //Proprietary functions
  void LoadAnswerGraphFromFile(std::string filename);
  void SendMessageToChatbot(std::string message);
  void SendMessageToUser(std::string message);
  wxBitmap *GetImageFromChatbot();
};

#endif /* CHATLOGIC_H_ */
