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

class Node{
private:
    Point position;
    Node* NWNode;
    Node* NENode;
    Node* SWNode;
    Node* SENode;
    string color;
    City content;
public:
    Node(){
        position = Point();
        NWNode = nullptr;
        NENode = nullptr;
        SWNode = nullptr;
        SENode = nullptr;
        color = "WHITE";
        content = City();
    };
    explicit Node(Point pos, Node* NW = nullptr, Node* NE = nullptr, Node* SW = nullptr,
             Node* SE = nullptr, string col = "WHITE", City data = City()){
        position = pos;
        NWNode = NW;
        NENode = NE;
        SWNode = SW;
        SENode = SE;
        color = move(col);
        content = move(data);
    }
};


class PRQuadTree {
private:
    int CAPACITY;
public:
    // Constructores
    PRQuadTree() = default;
    explicit PRQuadTree(int capacity):CAPACITY(capacity){};
    // Destructor
    ~PRQuadTree();
    void deletePoint();
    int quantityOfPoints();
    float populationAtPoint();
    float populationAtRegion();

};


#endif //PROYECTOALGORITMOS_PRQUADTREE_HPP
