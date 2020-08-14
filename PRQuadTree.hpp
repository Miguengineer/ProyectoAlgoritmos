#ifndef PROYECTOALGORITMOS_PRQUADTREE_HPP
#define PROYECTOALGORITMOS_PRQUADTREE_HPP
#include <bits/stdc++.h>

#include <utility>
#include "City.hpp"
using namespace std;

struct Point{
    double x;
    double y;
    Point(double x = 0, double y = 0): x(x), y(y){};
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
    Point getPoint(){return position;}

};


class PRQuadTree {
private:
    double xMin, xMax;
    double yMin, yMax;
    PRQuadTree* NWNode;
    PRQuadTree* NENode;
    PRQuadTree* SWNode;
    PRQuadTree* SENode;
    Node* rootNode;
public:
    // Constructores
    PRQuadTree(double _xMin, double _xMax, double _yMin, double _yMax) {
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
    void deletePoint();
    int quantityOfPoints();
    double populationAtPoint();
    double populationAtRegion();
    Node* getRootNode();
    string getQuadrant(Point point);

    bool insert(const City &city, PRQuadTree *R);

    bool testInsert(double x, double y, PRQuadTree *R);
};

PRQuadTree::~PRQuadTree() {
    delete(rootNode);
}

Node *PRQuadTree::getRootNode() {
    return rootNode;
}


double toDouble(std::string s){
    std::replace(s.begin(), s.end(), ',', '.');
    return std::atof(s.c_str());
}

bool PRQuadTree::insert(const City& city, PRQuadTree *R) {
    // Obtiene las coordenadas del punto a insertar
    double xCoord = toDouble(city.getLongitude());
    double yCoord = toDouble(city.getLatitude());
    cout << xCoord << endl;
    cout << yCoord << endl;
    // Revisa si el root de este árbol está vacío
    if (R->getRootNode() == nullptr){
        // RootNode está vacío, el punto lo inserta en la raíz
        R->rootNode = new Node(Point(xCoord, yCoord), city, "BLACK");
        // Se ingresó correctamente
        return true;
    }
    // Nodo raíz está ocupado con datos, se debe reestructurar
    else if (R->getRootNode()->getNodeType() == "BLACK") {
        Node *root = R->getRootNode();
        // Revisa si las coordenadas son las mismas
        if (root->getPoint().x == xCoord && root->getPoint().y == yCoord) {
            cout << "El punto ingresado contiene las mismas coordenadas que uno ya existente. Error al ingresar"
                 << endl;
            return false;
        }
        // Puntos no coinciden
        else {
            // Si el nodo era BLACK, todos su children tienen que estar desocupados. Busca el cuadrante del nuevo punto
            string quad = getQuadrant(Point(xCoord, yCoord));
            // Obtiene el nodo que estaba guardado
            Node currentNode = *(R->getRootNode());
            // Actualiza el nuevo nodo. City no contiene información relevante. Punto almacenado en nodo representa el centro
            R->rootNode = new Node(Point((R->xMax + R->xMin) / (double) 2.0, (R->yMax + R->yMin) / (double) 2.0), City(),
                                   "GRAY");
            // Determina dónde pertenece el nuevo nodo
            if (quad == "NW") {
                // Calcula las nuevas coordenadas del nuevo QuadTree a crear
                double newXMin = R->xMin;
                double newXMax = (R->xMax + R->xMin) / (double) 2.0;
                double newYMin = (R->yMax + R->yMin) / (double) 2.0;
                double newYMax = R->yMax;
                R->NWNode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);
                // Reinserta el nodo original
                R->insert(currentNode.getData(), R);
                // Inserta el nuevo nodo
                (R->NWNode)->insert(city, R->NWNode);
            } else if (quad == "NE") {
                // Calcula las nuevas coordenadas del nuevo QuadTree a crear
                double newXMin = (R->xMax + R->xMin) / (double) 2.0;
                double newXMax = R->xMax;
                double newYMin = (R->yMax + R->yMin) / (double) 2.0;
                double newYMax = R->yMax;
                // Crea el nuevo PR Tree donde corresponda
                R->NENode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);
                // Reinserta el nodo original
                R->insert(currentNode.getData(), R);
                // Inserta nuevo nodo
                (R->NENode)->insert(city, (R->NENode));
            } else if (quad == "SW") {
                // Calcula las nuevas coordenadas del nuevo QuadTree a crear
                double newXMin = R->xMin;
                double newXMax = (R->xMax + R->xMin) / (double) 2.0;
                double newYMin = R->yMin;
                double newYMax = (R->yMax + R->yMin) / (double) 2.0;
                // Crea el nuevo PR Tree donde corresponda
                R->SWNode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);
                // Reinserta el nodo original
                R->insert(currentNode.getData(), R);
                // Inserta nuevo nodo
                (R->SWNode)->insert(city, (R->SWNode));
            } else {
                // Calcula las nuevas coordenadas del nuevo QuadTree a crear
                double newXMin = (R->xMax + R->xMin) / (double) 2.0;
                double newXMax = R->xMax;
                double newYMin = R->yMin;
                double newYMax = (R->yMax + R->yMin) / (double) 2.0;
                // Crea el nuevo PR Tree donde corresponda
                R->SENode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);
                // Reinserta el nodo original
                R->insert(currentNode.getData(), R);
                // Inserta nuevo nodo
                (R->SENode)->insert(city, (R->SENode));
            }
        }
    }
   // Nodo es un nodo interno
    else if (R->getRootNode()->getNodeType() == "GRAY"){
        string quad = getQuadrant(Point(xCoord,yCoord));
        if (quad == "NW"){
            PRQuadTree* prToInsert = R->NWNode;
            if (prToInsert == nullptr){
                // Calcula las nuevas coordenadas del nuevo QuadTree a crear
                double newXMin = R->xMin;
                double newXMax = (R->xMax + R->xMin) / (double) 2.0;
                double newYMin = (R->yMax + R->yMin) / (double) 2.0;
                double newYMax = R->yMax;
                R->NWNode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);
            }
            prToInsert->insert(city,  R->NWNode);
        }
        else if (quad == "NE"){
            if (R->NENode == nullptr){
                double newXMin = (R->xMax + R->xMin) / (double) 2.0;
                double newXMax = R->xMax;
                double newYMin = (R->yMax + R->yMin) / (double) 2.0;
                double newYMax = R->yMax;
                R->NENode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);
            }
            (R->NENode)->insert(city, R->NENode);
        }
        else if (quad == "SW"){
            if (R->SWNode == nullptr){
                double newXMin = R->xMin;
                double newXMax = (R->xMax + R->xMin) / (double) 2.0;
                double newYMin = R->yMin;
                double newYMax = (R->yMax + R->yMin) / (double) 2.0;
                R->SWNode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);
            }
            (R->SWNode)->insert(city, R->SWNode);
        }
        else{
            if (R->SENode == nullptr){
                double newXMin = (R->xMax + R->xMin) / (double) 2.0;
                double newXMax = R->xMax;
                double newYMin = R->yMin;
                double newYMax = (R->yMax + R->yMin) / (double) 2.0;
                R->SENode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);
            }
            (R->SENode)->insert(city, R->SENode);
        }
    }
    return true;

}


bool PRQuadTree::testInsert(double x, double y, PRQuadTree* R) {
    // Revisa si el root de este árbol está vacío
    if (R->getRootNode() == nullptr){
        // RootNode está vacío, el punto lo inserta en la raíz
        R->rootNode = new Node(Point(x, y), City(), "BLACK");
        // Punto ingresado correctamente
        cout << "Punto ingresado correctamente" << endl;
        return true;
    }
        // Nodo raíz está ocupado con datos, se debe reestructurar
    else if (R->getRootNode()->getNodeType() == "BLACK"){
        Node* root = R->getRootNode();
        // Revisa si las coordenadas son las mismas
        if (root->getPoint().x == x && root->getPoint().y == y){
            cout << "El punto ingresado contiene las mismas coordenadas que uno ya existente. Error al ingresar" << endl;
            return false;
        }
        // Puntos no coinciden
        else {
            // Si el nodo era BLACK, todos su children tienen que estar desocupados. Busca el cuadrante del nuevo punto
            string quad = getQuadrant(Point(x,y));
            cout << quad << endl;
            // Obtiene el nodo que estaba guardado
            Node currentNode = *(R->getRootNode());
            // Actualiza el nuevo nodo. City no contiene información relevante
            R->rootNode = new Node(Point((R->xMax - R->xMin)/(double) 2.0, (R->yMax - R->yMin)/(double) 2.0),City(), "GRAY");

            // Determina dónde pertenece el nuevo nodo
            if (quad == "NW"){
                // Calcula las nuevas coordenadas del nuevo QuadTree a crear
                double newXMin = R->xMin;
                double newXMax = (R->xMax + R->xMin) / (double) 2.0;
                double newYMin = (R->yMax + R->yMin) / (double) 2.0;
                double newYMax = R->yMax;
                R->NWNode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);
                // Reinserta el nodo original
                R->testInsert(currentNode.getPoint().x, currentNode.getPoint().y, R);
                (R->NWNode)->testInsert(x, y, R->NWNode);
            }
            else if (quad == "NE"){
                // Calcula las nuevas coordenadas del nuevo QuadTree a crear
                double newXMin = (R->xMax + R->xMin) / (double) 2.0;
                double newXMax = R->xMax;
                double newYMin = (R->yMax + R->yMin) / (double) 2.0;
                double newYMax = R->yMax;
                // Crea el nuevo PR Tree donde corresponda
                R->NENode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);
                // Reinserta el nodo original
                R->testInsert(currentNode.getPoint().x, currentNode.getPoint().y, R);
                // Inserta nuevo nodo
                (R->NENode)->testInsert(x, y, (R->NENode));
            }
            else if (quad == "SW"){
                // Calcula las nuevas coordenadas del nuevo QuadTree a crear
                double newXMin = R->xMin;
                double newXMax = (R->xMax + R->xMin) / (double) 2.0;
                double newYMin = R->yMin;
                double newYMax = (R->yMax + R->yMin) / (double) 2.0;
                // Crea el nuevo PR Tree donde corresponda
                R->SWNode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);
                // Reinserta el nodo original
                R->testInsert(currentNode.getPoint().x, currentNode.getPoint().y, R);
                // Inserta nuevo nodo
                (R->SWNode)->testInsert(x, y, (R->SWNode));
            }
            else{
                // Calcula las nuevas coordenadas del nuevo QuadTree a crear
                double newXMin = (R->xMax + R->xMin) / (double) 2.0;
                double newXMax = R->xMax;
                double newYMin = R->yMin;
                double newYMax = (R->yMax + R->yMin) / (double) 2.0;
                // Crea el nuevo PR Tree donde corresponda
                R->SENode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);
                // Reinserta el nodo original
                R->testInsert(currentNode.getPoint().x, currentNode.getPoint().y, R);
                // Inserta nuevo nodo
                (R->SENode)->testInsert(x, y, (R->SENode));
            }
        }
    }
        // Nodo es un nodo interno
    else if (R->getRootNode()->getNodeType() == "GRAY"){
        string quad = getQuadrant(Point(x,y));
        if (quad == "NW"){
            PRQuadTree* prToInsert = R->NWNode;
            if (prToInsert == nullptr){
                // Calcula las nuevas coordenadas del nuevo QuadTree a crear
                double newXMin = R->xMin;
                double newXMax = (R->xMax + R->xMin) / (double) 2.0;
                double newYMin = (R->yMax + R->yMin) / (double) 2.0;
                double newYMax = R->yMax;
                R->NWNode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);
            }
            prToInsert->testInsert(x, y,  R->NWNode);
        }
        else if (quad == "NE"){
            if (R->NENode == nullptr){
                double newXMin = (R->xMax + R->xMin) / (double) 2.0;
                double newXMax = R->xMax;
                double newYMin = (R->yMax + R->yMin) / (double) 2.0;
                double newYMax = R->yMax;
                R->NENode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);
            }
            (R->NENode)->testInsert(x, y, R->NENode);
        }
        else if (quad == "SW"){
            if (R->SWNode == nullptr){
                double newXMin = R->xMin;
                double newXMax = (R->xMax + R->xMin) / (double) 2.0;
                double newYMin = R->yMin;
                double newYMax = (R->yMax + R->yMin) / (double) 2.0;
                R->SWNode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);
            }
            (R->SWNode)->testInsert(x, y, R->SWNode);
        }
        else{
            if (R->SENode == nullptr){
                double newXMin = (R->xMax + R->xMin) / (double) 2.0;
                double newXMax = R->xMax;
                double newYMin = R->yMin;
                double newYMax = (R->yMax + R->yMin) / (double) 2.0;
                R->SENode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);
            }
            (R->SENode)->testInsert(x, y, R->SENode);
        }
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
    double xHalf = (xMax + xMin) / (double) 2.0;
    double yHalf = (yMax + yMin) / (double) 2.0;
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
