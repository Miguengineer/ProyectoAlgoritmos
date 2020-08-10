#ifndef PROYECTOALGORITMOS_PRQUADTREE_HPP
#define PROYECTOALGORITMOS_PRQUADTREE_HPP
#include <bits/stdc++.h>

#include <utility>
#include "City.hpp"
using namespace std;


struct Point{
    float x;
    float y;
    Point(float x = 0, float y = 0): x(x), y(y){};
};




/**
 * Modela un cuadrante y contiene métodos para saber si se intersecta con otro Quadrant y
 * para saber si un punto pertenece a este cuadrante
 */
struct Quadrant{
    Point centre;
    Point halfSize;
    bool contains(Point a)
    {
        if(a.x < centre.x + halfSize.x && a.x > centre.x - halfSize.x)
        {
            if(a.y < centre.y + halfSize.y && a.y > centre.y - halfSize.y)
            {
                return true;
            }
        }
        return false;
    }

    bool intersects(Quadrant other)
    {
        //this right > that left                                          this left <s that right
        if(centre.x + halfSize.x > other.centre.x - other.halfSize.x || centre.x - halfSize.x < other.centre.x + other.halfSize.x)
        {
            // This bottom > that top
            if(centre.y + halfSize.y > other.centre.y - other.halfSize.y || centre.y - halfSize.y < other.centre.y + other.halfSize.y)
            {
                return true;
            }
        }
        return false;
    }
};

struct Data{
    Point pos;
    City* content;
    Data(Point pos = Point(), City* data = NULL): pos(pos), content(data){};
};


class PRQuadTree {




};


#endif //PROYECTOALGORITMOS_PRQUADTREE_HPP
