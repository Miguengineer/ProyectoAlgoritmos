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
 * Estructura que representa un nodo. Contiene posición (Point) y contenido, que es un puntero a clase City.
 */
struct Node{
    Point pos;
    Node* NWNode;
    Node* NENode;
    Node* SWNode;
    Node* SENode;
    string color;
    City* content;
    Node(Point pos = Point(), Node* NWNode = nullptr, Node* NENode = nullptr, Node* SWNode = nullptr,
         Node* SENode = nullptr, string color = "WHITE", City* data = nullptr): pos(pos), content(data),
         NWNode(NWNode), NENode(NENode), SWNode(SWNode), SENode(SENode), color(std::move(color)) {};
};


class PRQuadTree {
private:
    // Cada uno de los 4 children (cuadrantes)
    Node rootNode;
    float xMin, xMax, yMin, yMax;
public:
    // Constructores
    PRQuadTree();
    // Destructor
    ~PRQuadTree();
    void subdivide();
    void deletePoint();
    int quantityOfPoints();
    float populationAtPoint();
    float populationAtRegion();

};


#endif //PROYECTOALGORITMOS_PRQUADTREE_HPP
