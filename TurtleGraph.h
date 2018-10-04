#ifndef TURTLE_GRAPH_H_
#define TURTLE_GRAPH_H_
#include <wx/glcanvas.h>
#include <vector>
#include <string>

struct trt_point
{
    trt_point(float _x, float _y)
        : x(_x), y(_y) {}
    
    float x;
    float y;
};

typedef std::vector<trt_point> trt_image;

class TurtleGraph : public wxGLCanvas
{
public:

    TurtleGraph(wxWindow *parent, wxWindowID id, float _gamma = 0.95f);
    
    void CompileImage(const std::string &word, double theta, double alpha);
    
    void ClearImage();
    
    void DrawImage();
    
    void AddDrawCmd(char c);

private:
    
    void OnResize(wxSizeEvent &event);
    
    float gamma;
    trt_image image;
    std::string drawcmd;
};

#endif