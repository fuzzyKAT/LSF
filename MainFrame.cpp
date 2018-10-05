#include "MainFrame.h"
#include "TurtleGraph.h"
#include "WordIterator.h"

#include <wx/wfstream.h>
#include <wx/txtstrm.h>

void MainFrame::OnButIterate(wxCommandEvent &event)
{
    WordIterator *wit = new WordIterator();
    
    string axiom(axiomCtrl->GetLineText(0));
    
    for(int i = 0; i < ruleGrid->GetRows(); ++i)
    {
        string symbol(ruleGrid->GetCellValue(i, 0));
        string rule(ruleGrid->GetCellValue(i, 1));
        
        char sy = symbol.c_str()[0];
        
        wit->addRule(sy, rule);
    }
    
    string word(currWord);
    word = wit->iterateWord(word);
    currWord = word;
    
    double alpha = 0.0f, theta = M_PI/3;
    alphaCtrl->GetValue().ToDouble(&alpha);
    thetaCtrl->GetValue().ToDouble(&theta);
    alpha = M_PI*alpha/180;
    theta = M_PI*theta/180;
    
    turtleViever->CompileImage(word, theta, alpha);
    turtleViever->DrawImage();
    
    delete wit;
    
    SetIterNum(iterNum + 1);
}

void MainFrame::OnSaveWord(wxCommandEvent &event)
{
    // Диалог сохранения
    wxFileDialog saveFileDialog(
        this, _("Save TXT file"), "", "",
        "TXT files (*.txt)|*.txt", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
        
    if(saveFileDialog.ShowModal() == wxID_CANCEL)
    {
        // Отмена сохранения
        return;
    }
    
    wxFileOutputStream outputStream(saveFileDialog.GetPath());
    if(!outputStream.IsOk())
    {
        // Ошибка открытия файла
        wxLogError("Cannot save current contents in file '%s'.", 
            saveFileDialog.GetPath());
        return;
    }
    
    wxTextOutputStream outputText(outputStream);
    
    outputText<<currWord;
}

wxGrid *MainFrame::setupRuleGrid()
{
    wxGrid *ruleGrid = new wxGrid(this, ID_RULE_GRID);
    
    ruleGrid->CreateGrid(2, 2);
    ruleGrid->SetColLabelValue(0, "symbol");
    ruleGrid->SetColLabelValue(1, "rule");
    ruleGrid->SetCellValue(0, 0, "F");
    ruleGrid->SetCellValue(1, 0, "b");
    ruleGrid->SetCellValue(0, 1, "F");
    ruleGrid->SetCellValue(1, 1, "b");
    
    ruleGrid->DisableDragRowSize();
    ruleGrid->SetColSize(1, 200);
    
    return ruleGrid;
}

void MainFrame::SetupControlPanel()
{
    cmdsCtrl = new wxTextCtrl(
        this, ID_CMDS_CTRL, "F", wxDefaultPosition, 
        wxSize(200, 20), wxTE_LEFT);
    
    axiomCtrl = new wxTextCtrl(
        this, ID_AXIOM_CTRL, "F", wxDefaultPosition, 
        wxSize(200, 20), wxTE_LEFT);  

    alphaCtrl = new wxTextCtrl(
        this, -1, "0", wxDefaultPosition, 
        wxSize(50, 20), wxTE_LEFT);
        
    thetaCtrl = new wxTextCtrl(
        this, -1, "60", wxDefaultPosition, 
        wxSize(50, 20), wxTE_LEFT);  
        
    wxStaticText *axiomText = new wxStaticText(this, wxID_ANY, "axiom =");
    wxStaticText *alphaText = new wxStaticText(this, wxID_ANY, "alpha =");
    wxStaticText *thetaText = new wxStaticText(this, wxID_ANY, "theta =");
    wxStaticText *cmdText = new wxStaticText(this, wxID_ANY, "commands =");
    
    wxSize sz = axiomText->GetSize();
    alphaText->SetMinSize(sz);
    thetaText->SetMinSize(sz);
    
    wxSizer *angleSizer = new wxBoxSizer(wxHORIZONTAL);
    wxSizer *axiomSizer = new wxBoxSizer(wxHORIZONTAL);
    
    wxButton *butAddRule = new wxButton(this, ID_BUT_ADD_RULE, "Add Rule");
    wxButton *butRemRule = new wxButton(this, ID_BUT_REM_RULE, "Rem Rule");
    
    angleSizer->Add(alphaText, 0, wxRIGHT, 5);
    angleSizer->Add(alphaCtrl, 0, wxRIGHT, 5);
    angleSizer->Add(thetaText, 0, wxRIGHT, 5);
    angleSizer->Add(thetaCtrl, 0, wxRIGHT, 5);
    angleSizer->Add(cmdText, 0, wxRIGHT, 5);
    angleSizer->Add(cmdsCtrl, 1, wxEXPAND, 0);
    
    axiomSizer->Add(axiomText, 0, wxRIGHT, 5);
    axiomSizer->Add(axiomCtrl, 1, wxEXPAND, 0);
    
    wxButton *but1 = new wxButton(this, ID_BUT_ITERATE, "Iterate");
    wxButton *but2 = new wxButton(this, ID_BUT_CLEAR, "Clear");
    wxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
    wxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    wxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);
    wxSizer *textCtrlSizer = new wxBoxSizer(wxVERTICAL);
    
    ruleGrid = setupRuleGrid();
    textCtrlSizer->Add(angleSizer, 0, wxEXPAND | wxTOP, 10);
    textCtrlSizer->Add(axiomSizer, 0, wxEXPAND | wxTOP, 10);
    textCtrlSizer->Add(ruleGrid, 1, wxEXPAND | wxTOP, 10);
    
    buttonSizer->Add(but1, 1, wxTOP, 10);
    buttonSizer->Add(but2, 1, wxTOP, 10);
    buttonSizer->Add(butAddRule, 1, wxTOP, 10);
    buttonSizer->Add(butRemRule, 1, wxTOP, 10);
    
    bottomSizer->Add(buttonSizer, 0, wxLEFT, 10);
    bottomSizer->Add(textCtrlSizer, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);
    topSizer->Add(turtleViever, 1, wxEXPAND, 0);
    topSizer->Add(bottomSizer, 0, wxEXPAND, 0);
    
    SetSizerAndFit(topSizer);
}