#ifndef MAIN_FRAME_H_
#define MAIN_FRAME_H_
#include "TurtleGraph.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

enum EventID
{
    // menu File
    ID_SAVE_WORD = 1,
    
    ID_CREATE_FRACTAL,
    
    ID_BUT_ITERATE,
    ID_BUT_CLEAR,
    
    ID_AXIOM_CTRL
};

class MainFrame : public wxFrame
{
public:

    MainFrame(wxString title = "LSF") :
        wxFrame(NULL, wxID_ANY, title)
    {
        wxMenu *menuFile = new wxMenu();
        menuFile->Append(ID_SAVE_WORD, "Save word");
        menuFile->AppendSeparator();
        menuFile->Append(wxID_EXIT);
    
        wxMenu *menuHelp = new wxMenu();
        menuHelp->Append(wxID_ABOUT);
        
        wxMenu *menuFrac = new wxMenu();
        menuFrac->Append(ID_CREATE_FRACTAL, "&Create Fractal");
    
        wxMenuBar *menuBar = new wxMenuBar();
        menuBar->Append(menuFile, "&File");
        menuBar->Append(menuHelp, "&Help");
        menuBar->Append(menuFrac, "&Fractals");
        SetMenuBar(menuBar);
    
        CreateStatusBar();
        SetIterNum(0);
        
        turtleViever = new TurtleGraph(this, wxID_ANY);
        glContext = new wxGLContext(turtleViever);
        //glContext->SetCurrent(*turtleViever);
        turtleViever->SetMinSize(wxSize(600, 300));
        
        SetupControlPanel();
        
        currWord = "F";
    
        Bind(wxEVT_MENU, &MainFrame::OnSaveWord, this, ID_SAVE_WORD);
        Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
        Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
        
        Bind(wxEVT_BUTTON, &MainFrame::OnButIterate, this, ID_BUT_ITERATE);
        Bind(wxEVT_BUTTON, &MainFrame::OnButClear, this, ID_BUT_CLEAR);
        
        Bind(wxEVT_TEXT, &MainFrame::OnAxiomChanged, this, ID_AXIOM_CTRL);
        
        Bind(wxEVT_PAINT, &MainFrame::OnPaint, this);
    }
    
private:

    int iterNum;

    wxString currWord;
    //wxTextCtrl *wordDisplay;
    wxTextCtrl *axiomCtrl;
    wxTextCtrl *alphaCtrl; 
    wxTextCtrl *thetaCtrl;
    wxTextCtrl *xRuleCtrl;
    wxTextCtrl *yRuleCtrl;
    wxTextCtrl *fRuleCtrl;
    wxTextCtrl *bRuleCtrl;
    
    TurtleGraph *turtleViever;
    wxGLContext *glContext;
    
    void SetupControlPanel();
    
    void SetIterNum(int val)
    {
        iterNum = val;
        wxString status; 
        status.Printf("Iteration %d", iterNum);
        SetStatusText(status);
    }
    
    void OnPaint(wxPaintEvent &event)
    {
        glContext->SetCurrent(*turtleViever);
        turtleViever->DrawImage();
    }
    
    void OnSaveWord(wxCommandEvent &event);
    
    void OnExit(wxCommandEvent &event)
    {
        Close(true);
    }
    
    void OnAbout(wxCommandEvent &event)
    {
        wxMessageBox("This is a wxWidgets Hello World example",
            "About Hello World", wxOK | wxICON_INFORMATION);
    }
    
    void OnAxiomChanged(wxCommandEvent &event)
    {
        SetIterNum(0);
        //wordDisplay->Clear();
        //wordDisplay->AppendText(axiomCtrl->GetLineText(0));
        currWord =  axiomCtrl->GetLineText(0);
        turtleViever->ClearImage();
    }
    
    void OnButIterate(wxCommandEvent &event);
    
    void OnButClear(wxCommandEvent &event)
    {
        SetIterNum(0);
        //wordDisplay->Clear();
        //wordDisplay->AppendText(axiomCtrl->GetLineText(0));
        currWord =  axiomCtrl->GetLineText(0);
        turtleViever->ClearImage();
    }
};

#endif