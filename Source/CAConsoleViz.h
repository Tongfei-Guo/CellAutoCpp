#ifndef VACONSOLEVIZ_H
#define VACONSOLEVIZ_H

#include "CATypes.h"
#include "CAWorld.h"

class CAConsoleViz{

public:
    
CAConsoleViz(CAWorld *p)
    :m_world(p){}

void SetColorScheme(consoleviz_type f){ m_consoleviz_func=f; }

void Visualize(int steps, int fps);

private:
    CAWorld *m_world;
    consoleviz_type m_consoleviz_func;
};

#endif
