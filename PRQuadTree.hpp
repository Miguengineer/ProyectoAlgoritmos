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
    City content;
public:
    Node(){
        position = Point();
        NWNode = nullptr;
        NENode = nullptr;
        SWNode = nullptr;
        SENode = nullptr;
        nodeType = "WHITE";
        content = City();
    };
    explicit Node (Point pos, string col = "GRAY", City data = City()){
        position = pos;
        NWNode = nullptr;
        NENode = nullptr;
        SWNode = nullptr;
        SENode = nullptr;
        nodeType = move(col);
        content = move(data);
    }
    explicit Node(Point pos, Node* NW = nullptr, Node* NE = nullptr, Node* SW = nullptr,
             Node* SE = nullptr, string col = "WHITE", City data = City()){
        position = pos;
        NWNode = NW;
        NENode = NE;
        SWNode = SW;
        SENode = SE;
        nodeType = move(col);
        content = move(data);
    }

    string nodeType;
};


class PRQuadTree {
private:
    float xMin, xMax;
    float yMin, yMax;
    Node* rootNode;
public:
    // Constructores
    PRQuadTree(float _xMin, float _xMax, float _yMin, float _yMax) {
        xMin = _xMin;
        xMax = _xMax;
        yMin = _yMin;
        yMax = _yMax;
        rootNode = nullptr;
    }
    explicit PRQuadTree(Node* _rootNode, float _xMin, float _xMax, float _yMin, float _yMax) {
        rootNode = _rootNode;
        xMin = _xMin;
        xMax = _xMax;
        yMin = _yMin;
        yMax = _yMax;
    }
    // Destructor
    ~PRQuadTree();
    bool insert(const City& city);
    bool testInsert(float x, float y);
    void deletePoint();
    int quantityOfPoints();
    float populationAtPoint();
    float populationAtRegion();
    Node* getRootNode();
    string getQuadrant(Node node);

};

PRQuadTree::~PRQuadTree() {
    delete(rootNode);
}

Node *PRQuadTree::getRootNode() {
    return rootNode;
}

bool PRQuadTree::insert(const City& city) {
    // Obtiene las coordenadas del punto a insertar
    float xCoord = stof(city.getLatitude());
    float yCoord = stof(city.getLongitude());
    // Revisa si el root de este árbol está vacío
    if (getRootNode() == nullptr){
        // RootNode está vacío, el punto lo inserta en la raíz
        rootNode = new Node(Point(xCoord, yCoord), "BLACK", city);
        // Punto ingresado correctamente
        cout << "Punto ingresado correctamente" << endl;
        return true;
    }
    // Nodo raíz está completamente ocupado, hay que crear un nuevo PR
    else if (getRootNode()->nodeType == "BLACK"){

    }
    // Nodo es un nodo interno
    else if (getRootNode()->nodeType == "GRAY"){

    }
    return true;

}


bool PRQuadTree::testInsert(float x, float y) {
    return false;
}

string PRQuadTree::getQuadrant(Node node) {

}


#endif //PROYECTOALGORITMOS_PRQUADTREE_HPP
