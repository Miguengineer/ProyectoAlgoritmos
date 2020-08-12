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
    string color;
    City data;

public:
    Node(){
        position = Point();
        color = "WHITE";
    };
    Node(Point pos, City city, string col){
        position = pos;
        data = city;
        color = col;
    }
    City getData(){return data;}
    string getNodeType(){return color;}

};


class PRQuadTree {
private:
    float xMin, xMax;
    float yMin, yMax;
    PRQuadTree* NWNode;
    PRQuadTree* NENode;
    PRQuadTree* SWNode;
    PRQuadTree* SENode;
    Node* rootNode;
public:
    // Constructores
    PRQuadTree(float _xMin, float _xMax, float _yMin, float _yMax) {
        xMin = _xMin;
        xMax = _xMax;
        yMin = _yMin;
        yMax = _yMax;
        NWNode = nullptr;
        NENode = nullptr;
        SWNode = nullptr;
        SENode = nullptr;
        rootNode = nullptr;
    }

    // Destructor
    ~PRQuadTree();
    bool testInsert(float x, float y);
    void deletePoint();
    int quantityOfPoints();
    float populationAtPoint();
    float populationAtRegion();
    Node* getRootNode();
    string getQuadrant(Point point);

    bool insert(const City &city, PRQuadTree *R);

    bool testInsert(float x, float y, PRQuadTree *R);
};

PRQuadTree::~PRQuadTree() {
    delete(rootNode);
}

Node *PRQuadTree::getRootNode() {
    return rootNode;
}

bool PRQuadTree::insert(const City& city, PRQuadTree *R) {
    // Obtiene las coordenadas del punto a insertar
    float xCoord = stof(city.getLatitude());
    float yCoord = stof(city.getLongitude());
    // Revisa si el root de este árbol está vacío
    if (R->getRootNode() == nullptr){
        // RootNode está vacío, el punto lo inserta en la raíz
        rootNode = new Node(Point(xCoord, yCoord), city, "BLACK");
        // Punto ingresado correctamente
        cout << "Punto ingresado correctamente" << endl;
        return true;
    }
    // Nodo raíz está ocupado con datos, se debe reestructurar
    else if (R->getRootNode()->getNodeType() == "BLACK"){
        int x = 1;
    }
    // Nodo es un nodo interno
    else if (R->getRootNode()->getNodeType() == "GRAY"){

    }
    return true;

}


bool PRQuadTree::testInsert(float x, float y, PRQuadTree* R) {
    // Revisa si el root de este árbol está vacío
    if (R->getRootNode() == nullptr){
        // RootNode está vacío, el punto lo inserta en la raíz
        rootNode = new Node(Point(x, y), City(), "BLACK");
        // Punto ingresado correctamente
        cout << "Punto ingresado correctamente" << endl;
        return true;
    }
        // Nodo raíz está ocupado con datos, se debe reestructurar
    else if (R->getRootNode()->getNodeType() == "BLACK"){
        string test = getQuadrant(Point(x,y));
        cout << test << endl;
    }
        // Nodo es un nodo interno
    else if (R->getRootNode()->getNodeType() == "GRAY"){

    }
    return true;
}

/**
 * Obtiene el cuadrante al cual pertenece el punto. A la derecha y arriba son inclusivos (incluyen al punto)
 * @param point: Punto a preguntar
 * @return: String que indica el cuadrante al que pertenece en este quadtree
 */
string PRQuadTree::getQuadrant(Point point) {
    // Obtiene los halfsizes
    float xHalf = (xMax - xMin) / (float) 2.0;
    float yHalf = (yMax - yMin) / (float) 2.0;
    if (point.x < xHalf) {
        if (point.y < yHalf) return "SW";
        else return "NW";
    }
    else {
        if (point.y < yHalf) return "SE";
        else return "NE";
    }
}


#endif //PROYECTOALGORITMOS_PRQUADTREE_HPP
