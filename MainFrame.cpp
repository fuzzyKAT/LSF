#include "MainFrame.h"
#include "TurtleGraph.h"
#include "WordIterator.h"

#include <wx/wfstream.h>
#include <wx/txtstrm.h>

void MainFrame::OnButIterate(wxCommandEvent &event)
{
    WordIterator *wit = new WordIterator();
    
    string axiom(axiomCtrl->GetLineText(0));
    string newrf(fRuleCtrl->GetLineText(0));
    string newrx(xRuleCtrl->GetLineText(0));
    string newry(yRuleCtrl->GetLineText(0));
    string newrb(bRuleCtrl->GetLineText(0));
    
    wit->addRule('F', newrf);
    wit->addRule('X', newrx);
    wit->addRule('Y', newry);
    wit->addRule('b', newrb);
    
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
    
    xRuleCtrl = new wxTextCtrl(
        this, -1, "X", wxDefaultPosition, 
        wxSize(200, 20), wxTE_LEFT);
    
    yRuleCtrl = new wxTextCtrl(
        this, -1, "Y", wxDefaultPosition, 
        wxSize(200, 20), wxTE_LEFT);
        
    fRuleCtrl = new wxTextCtrl(
        this, -1, "F", wxDefaultPosition, 
        wxSize(200, 20), wxTE_LEFT);
        
    bRuleCtrl = new wxTextCtrl(
        this, -1, "b", wxDefaultPosition, 
        wxSize(200, 20), wxTE_LEFT);
        
    //wordDisplay->SetEditable(false);
        
    wxStaticText *axiomText = new wxStaticText(this, wxID_ANY, "axiom =");
    wxStaticText *xRuleText = new wxStaticText(this, wxID_ANY, "newx =");
    wxStaticText *yRuleText = new wxStaticText(this, wxID_ANY, "newy =");
    wxStaticText *fRuleText = new wxStaticText(this, wxID_ANY, "newf =");
    wxStaticText *bRuleText = new wxStaticText(this, wxID_ANY, "newb =");
    wxStaticText *alphaText = new wxStaticText(this, wxID_ANY, "alpha =");
    wxStaticText *thetaText = new wxStaticText(this, wxID_ANY, "theta =");
    
    wxSize sz = axiomText->GetSize();
    xRuleText->SetMinSize(sz);
    yRuleText->SetMinSize(sz);
    fRuleText->SetMinSize(sz);
    bRuleText->SetMinSize(sz);
    alphaText->SetMinSize(sz);
    thetaText->SetMinSize(sz);
    
    wxSizer *angleSizer = new wxBoxSizer(wxHORIZONTAL);
    wxSizer *axiomSizer = new wxBoxSizer(wxHORIZONTAL);
    wxSizer *xRuleSizer = new wxBoxSizer(wxHORIZONTAL);
    wxSizer *yRuleSizer = new wxBoxSizer(wxHORIZONTAL);
    wxSizer *fRuleSizer = new wxBoxSizer(wxHORIZONTAL);
    wxSizer *bRuleSizer = new wxBoxSizer(wxHORIZONTAL);
    
    angleSizer->Add(alphaText, 0, wxRIGHT, 5);
    angleSizer->Add(alphaCtrl, 1, wxEXPAND, 0);
    angleSizer->Add(thetaText, 0, wxRIGHT | wxLEFT, 5);
    angleSizer->Add(thetaCtrl, 1, wxEXPAND, 0);
    axiomSizer->Add(axiomText, 0, wxRIGHT, 5);
    axiomSizer->Add(axiomCtrl, 1, wxEXPAND, 0);
    xRuleSizer->Add(xRuleText, 0, wxRIGHT, 5);
    xRuleSizer->Add(xRuleCtrl, 1, wxEXPAND, 0);
    yRuleSizer->Add(yRuleText, 0, wxRIGHT, 5);
    yRuleSizer->Add(yRuleCtrl, 1, wxEXPAND, 0);
    fRuleSizer->Add(fRuleText, 0, wxRIGHT, 5);
    fRuleSizer->Add(fRuleCtrl, 1, wxEXPAND, 0);
    bRuleSizer->Add(bRuleText, 0, wxRIGHT, 5);
    bRuleSizer->Add(bRuleCtrl, 1, wxEXPAND, 0);
    
    wxButton *but1 = new wxButton(this, ID_BUT_ITERATE, "Iterate");
    wxButton *but2 = new wxButton(this, ID_BUT_CLEAR, "Clear");
    wxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
    wxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    wxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);
    wxSizer *textCtrlSizer = new wxBoxSizer(wxVERTICAL);
    
    textCtrlSizer->Add(angleSizer, 1, wxEXPAND | wxTOP, 10);
    textCtrlSizer->Add(axiomSizer, 1, wxEXPAND | wxTOP, 10);
    textCtrlSizer->Add(xRuleSizer, 1, wxEXPAND | wxTOP, 10);
    textCtrlSizer->Add(yRuleSizer, 1, wxEXPAND | wxTOP, 10);
    textCtrlSizer->Add(fRuleSizer, 1, wxEXPAND | wxTOP, 10);
    textCtrlSizer->Add(bRuleSizer, 1, wxEXPAND | wxTOP, 10);
    
    buttonSizer->Add(but1, 1, wxTOP, 10);
    buttonSizer->Add(but2, 1, wxTOP, 10);
    
    bottomSizer->Add(buttonSizer, 0, wxLEFT, 10);
    bottomSizer->Add(textCtrlSizer, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);
    topSizer->Add(turtleViever, 1, wxEXPAND, 0);
    //topSizer->Add(wordDisplay, 0, wxEXPAND, 0);
    topSizer->Add(bottomSizer, 0, wxEXPAND, 0);
    
    SetSizerAndFit(topSizer);
}