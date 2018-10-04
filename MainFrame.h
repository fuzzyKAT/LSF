#ifndef MAIN_FRAME_H_
#define MAIN_FRAME_H_
#include "TurtleGraph.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/grid.h>

enum EventID
{
    // menu File
    ID_SAVE_WORD = 1,
    
    ID_CREATE_FRACTAL,
    
    ID_AXIOM_CTRL,
    ID_RULE_GRID,
    ID_CMDS_CTRL,
    
    ID_BUT_ITERATE,
    ID_BUT_CLEAR,
    ID_BUT_ADD_RULE,
    ID_BUT_REM_RULE
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
        turtleViever->SetDrawCmds(std::string("F"));
        glContext = new wxGLContext(turtleViever);
        turtleViever->SetMinSize(wxSize(600, 300));
        
        SetupControlPanel();
        
        currWord = "F";
    
        Bind(wxEVT_MENU, &MainFrame::OnSaveWord, this, ID_SAVE_WORD);
        Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
        Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
        
        Bind(wxEVT_BUTTON, &MainFrame::OnButIterate, this, ID_BUT_ITERATE);
        Bind(wxEVT_BUTTON, &MainFrame::OnButClear, this, ID_BUT_CLEAR);
        Bind(wxEVT_BUTTON, &MainFrame::OnButAddRule, this, ID_BUT_ADD_RULE);
        Bind(wxEVT_BUTTON, &MainFrame::OnButRemRule, this, ID_BUT_REM_RULE);
        
        Bind(wxEVT_TEXT, &MainFrame::OnAxiomChanged, this, ID_AXIOM_CTRL);
        Bind(wxEVT_TEXT, &MainFrame::OnCmdsChanged, this, ID_CMDS_CTRL);
        
        Bind(wxEVT_PAINT, &MainFrame::OnPaint, this);
    }
    
private:

    int iterNum;

    wxString currWord;

    wxTextCtrl *cmdsCtrl;
    wxTextCtrl *axiomCtrl;
    wxTextCtrl *alphaCtrl; 
    wxTextCtrl *thetaCtrl;
    
    wxGrid *ruleGrid;
    
    TurtleGraph *turtleViever;
    wxGLContext *glContext;
    
    wxGrid *setupRulePanel();
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
        currWord =  axiomCtrl->GetLineText(0);
        turtleViever->ClearImage();
    }
    
    void OnCmdsChanged(wxCommandEvent &event)
    {
        std::string cmds(cmdsCtrl->GetLineText(0));
        turtleViever->SetDrawCmds(cmds);
    }
    
    void OnButIterate(wxCommandEvent &event);
    
    void OnButClear(wxCommandEvent &event)
    {
        SetIterNum(0);
        currWord =  axiomCtrl->GetLineText(0);
        turtleViever->ClearImage();
    }
    
    void OnButAddRule(wxCommandEvent &event)
    {
        ruleGrid->AppendRows(1);
    }
    
    void OnButRemRule(wxCommandEvent &event)
    {
        wxArrayInt srows = ruleGrid->GetSelectedRows();
        
        for(size_t i = 0; i < srows.GetCount(); ++i)
        {
            ruleGrid->DeleteRows(srows[i]);
        }
    }
};

#endif