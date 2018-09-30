#include "MainFrame.h"

class lsfApp : public wxApp
{
public:
    virtual bool OnInit()
    {
        MainFrame *frame = new MainFrame();
        frame->Show(true);
        return true;
    }
    
    virtual int OnExit()
    {
        return 0;
    }
};

wxIMPLEMENT_APP(lsfApp);
