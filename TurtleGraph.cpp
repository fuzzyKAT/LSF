#include "TurtleGraph.h"
#include <stack>

inline float min(float x, float y)
{
    return (x < y) ? x : y;
}

inline float max(float x, float y)
{
    return (x > y) ? x : y;
}

struct trt_state
{
    float x;
    float y;
    double alpha;
};

TurtleGraph::TurtleGraph(
    wxWindow *parent, wxWindowID id, float _gamma)
    : wxGLCanvas(parent, id, NULL), gamma(_gamma)
{
    Bind(wxEVT_SIZE, &TurtleGraph::OnResize, this);
}

void TurtleGraph::OnResize(wxSizeEvent &event)
{
    wxSize sz = event.GetSize();
    glViewport(0, 0, sz.GetWidth(), sz.GetHeight());
}

void TurtleGraph::CompileImage(
    const std::string &word, double theta, double alpha)
{
    if(image.size())
    {
        image.clear();
    }
    
    trt_state cst;
    
    cst.x = 0.0f;
    cst.y = 0.0f;
    cst.alpha = alpha;
    float xnext = 0.0f;
    float ynext = 0.0f;
    
    float xmin = 0.0f, xmax = 0.0f;
    float ymin = 0.0f, ymax = 0.0f;
    
    std::stack<trt_state> stk;
    std::string::const_iterator i;
    for(i = word.begin(); i != word.end(); ++i)
    {
        switch(*i)
        {
            case '\n':
            case '\r':
            case ' ':
                break;
                
            case '+': 
                cst.alpha += theta;
                break;
            
            case '-': 
                cst.alpha -= theta;
                break;
            
            case 'b': 
                cst.x += cos(cst.alpha);
                cst.y += sin(cst.alpha);
                
                break;
            
            case '[': 
            {
                stk.push(cst);
                break;
            }
            
            case ']': 
            {
                cst = stk.top();
                stk.pop();
                break;
            }
            
            default:
                if(drawcmd.find_first_of(*i) != std::string::npos)
                {
                    xnext = cst.x + cos(cst.alpha);
                    ynext = cst.y + sin(cst.alpha);
                
                    image.push_back(trt_point(cst.x, cst.y));
                    image.push_back(trt_point(xnext, ynext));
                
                    cst.x = xnext;
                    cst.y = ynext;
                
                    xmax = max(cst.x, xmax);
                    ymax = max(cst.y, ymax);
                    xmin = min(cst.x, xmin);
                    ymin = min(cst.y, ymin);
                }
                break;
        }
    }
    
    double ax = 2.0f*gamma/(xmax - xmin);
    double ay = 2.0f*gamma/(ymax - ymin);
    double bx = -gamma*(xmax + xmin)/(xmax - xmin);
    double by = -gamma*(ymax + ymin)/(ymax - ymin);
    
    if( (ymax == 0) && (ymin == 0) )
    {
        ay = by = 0;
    }
    
    if( (xmax == 0) && (xmin == 0) )
    {
        ax = bx = 0;
    }
    
    std::vector<trt_point>::iterator ip;
    for(ip = image.begin(); ip != image.end(); )
    {
        trt_point &p = *ip++;
        
        p.x = ax*p.x + bx;
        p.y = ay*p.y + by;
    }
}

void TurtleGraph::AddDrawCmd(char c)
{
    drawcmd.push_back(c);
}

void TurtleGraph::ClearImage()
{
    image.clear();
}

void TurtleGraph::DrawImage()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 0.0f);
    
    if(image.size() < 2)
    {
        SwapBuffers();
        return;
    }
    
    std::vector<trt_point>::const_iterator i;

    for(i = image.begin(); i != image.end(); )
    {
        trt_point p1 = *i++;
        trt_point p2 = *i++;
        
        glBegin(GL_LINES);
        glVertex2f(p1.x, p1.y);
        glVertex2f(p2.x, p2.y);
        glEnd();
    }
    
    glFinish();
    SwapBuffers();
}