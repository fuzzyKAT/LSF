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
    //glContext->SetCurrent(*turtleViever);
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
    
    //outputText<<wordDisplay->GetValue();
    outputText<<currWord;
}

wxGrid *MainFrame::setupRulePanel()
{
    wxGrid *ruleGrid = new wxGrid(this, wxID_ANY);
    
    ruleGrid->CreateGrid(4, 2);
    ruleGrid->SetColLabelValue(0, "symbol");
    ruleGrid->SetColLabelValue(1, "rule");
    ruleGrid->SetCellValue(0, 0, "F");
    ruleGrid->SetCellValue(1, 0, "b");
    ruleGrid->SetCellValue(2, 0, "X");
    ruleGrid->SetCellValue(3, 0, "Y");
    ruleGrid->SetCellValue(0, 1, "F");
    ruleGrid->SetCellValue(1, 1, "b");
    ruleGrid->SetCellValue(2, 1, "X");
    ruleGrid->SetCellValue(3, 1, "Y");
    
    return ruleGrid;
}

void MainFrame::SetupControlPanel()
{
    //wordDisplay = new wxTextCtrl(
    //    this, -1, "F", wxDefaultPosition, 
    //    wxSize(100,60), wxTE_MULTILINE);
    
    axiomCtrl = new wxTextCtrl(
        this, ID_AXIOM_CTRL, "F", wxDefaultPosition, 
        wxSize(200, 20), wxTE_LEFT);  

    alphaCtrl = new wxTextCtrl(
        this, -1, "0", wxDefaultPosition, 
        wxSize(200, 20), wxTE_LEFT);
        
    thetaCtrl = new wxTextCtrl(
        this, -1, "60", wxDefaultPosition, 
        wxSize(200, 20), wxTE_LEFT);  
        
    //wordDisplay->SetEditable(false);
        
    wxStaticText *axiomText = new wxStaticText(this, wxID_ANY, "axiom =");
    wxStaticText *alphaText = new wxStaticText(this, wxID_ANY, "alpha =");
    wxStaticText *thetaText = new wxStaticText(this, wxID_ANY, "theta =");
    
    wxSize sz = axiomText->GetSize();
    alphaText->SetMinSize(sz);
    thetaText->SetMinSize(sz);
    
    wxSizer *angleSizer = new wxBoxSizer(wxHORIZONTAL);
    wxSizer *axiomSizer = new wxBoxSizer(wxHORIZONTAL);
    
    angleSizer->Add(alphaText, 0, wxRIGHT, 5);
    angleSizer->Add(alphaCtrl, 1, wxEXPAND, 0);
    angleSizer->Add(thetaText, 0, wxRIGHT | wxLEFT, 5);
    angleSizer->Add(thetaCtrl, 1, wxEXPAND, 0);
    axiomSizer->Add(axiomText, 0, wxRIGHT, 5);
    axiomSizer->Add(axiomCtrl, 1, wxEXPAND, 0);
    
    wxButton *but1 = new wxButton(this, ID_BUT_ITERATE, "Iterate");
    wxButton *but2 = new wxButton(this, ID_BUT_CLEAR, "Clear");
    wxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
    wxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    wxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);
    wxSizer *textCtrlSizer = new wxBoxSizer(wxVERTICAL);
    
    ruleGrid = setupRulePanel();
    textCtrlSizer->Add(angleSizer, 0, wxEXPAND | wxTOP, 10);
    textCtrlSizer->Add(axiomSizer, 0, wxEXPAND | wxTOP, 10);
    textCtrlSizer->Add(ruleGrid, 1, wxEXPAND | wxTOP, 10);
    
    buttonSizer->Add(but1, 1, wxTOP, 10);
    buttonSizer->Add(but2, 1, wxTOP, 10);
    
    bottomSizer->Add(buttonSizer, 0, wxLEFT, 10);
    bottomSizer->Add(textCtrlSizer, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);
    topSizer->Add(turtleViever, 1, wxEXPAND, 0);
    //topSizer->Add(wordDisplay, 0, wxEXPAND, 0);
    topSizer->Add(bottomSizer, 0, wxEXPAND, 0);
    
    SetSizerAndFit(topSizer);
}