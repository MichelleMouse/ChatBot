#ifndef CHATGUI_H_
#define CHATGUI_H_

#include <wx/wx.h>

class ChatLogic; //Forward declaration

//Middle part of the window containing the dialog between user and chatbot
class ChatBotPanelDialog : public wxScrolledWindow
{
private:
  //Control elements
  wxBoxSizer *_dialogSizer;
  wxBitmap _image;

  std::unique_ptr<ChatLogic> _chatLogic;

public:
  //Constructor & Destructor
  ChatBotPanelDialog(wxWindow *parent, wxWindowID id);
  ~ChatBotPanelDialog();

  //Getters & Setters
  ChatLogic *GetChatLogicHandle() { return _chatLogic.get(); }

  //Events
  void paintEvent(wxPaintEvent &evt);
  void paintNow();
  void render(wxDC &dc);

  //Proprietary functions
  void AddDialogItem(wxString text, bool isFromUser = true);
  void PrintChatbotResponse(std::string response);

  DECLARE_EVENT_TABLE()
};

//Dialog item shown in ChatBotPanelDialog
class ChatBotPanelDialogItem : public wxPanel
{
private:
  //Control elements
  wxStaticBitmap *_chatBotImg;
  wxStaticText *_chatBotTxt;

public:
  //Constructor & Destructor
  ChatBotPanelDialogItem(wxPanel *parent, wxString text, bool isFromUser);
};

//Frame containing all control elements
class ChatBotFrame : public wxFrame
{
private:
  //Control elements
  ChatBotPanelDialog *_panelDialog;
  wxTextCtrl *_userTextCtrl;

  //Events
  void OnEnter(wxCommandEvent &WXUNUSED(event));

public:
  //Constructor & Destructor
  ChatBotFrame(const wxString &title);
};

//Control panel for background image display
class ChatBotFrameImagePanel : public wxPanel
{
  //Control elements
  wxBitmap _image;

public:
  //Constructor & Destructor
  ChatBotFrameImagePanel(wxFrame *parent);

  //Events
  void paintEvent(wxPaintEvent &evt);
  void paintNow();
  void render(wxDC &dc);

  DECLARE_EVENT_TABLE()
};

//wxWidgets app that hides main()
class ChatBotApp : public wxApp
{
public:
  //Events
  virtual bool OnInit();
};

#endif /* CHATGUI_H_ */
