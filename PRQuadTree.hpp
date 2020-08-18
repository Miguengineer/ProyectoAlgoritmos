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
    double populationAtRegion();
    Node* getRootNode();
    string getQuadrant(Point point);
    bool insert(const City &city, PRQuadTree *R);
    double populationAtPoint(double x, double y, PRQuadTree* pr);
    bool testInsert(double x, double y, PRQuadTree *R);

    string getQuadrant(Point point, PRQuadTree *pr);
};

PRQuadTree::~PRQuadTree() {
    delete(rootNode);
}

Node *PRQuadTree::getRootNode() {
    return rootNode;
}


void getCoordinate (std::string s, double *x, double *y){
    stringstream linestream(s);
    string currentString;
    getline(linestream, currentString, ',');
    *y = stod(currentString);
    getline(linestream, currentString, ',');
    *x = stod(currentString);
}

bool PRQuadTree::insert(const City& city, PRQuadTree *R) {
    // Obtiene las coordenadas del punto a insertar
    double xCoord;
    double yCoord;
    getCoordinate(city.getGeopoint(), &xCoord, &yCoord);
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
        double xDiff = abs(root->getPoint().x  - xCoord);
        double yDiff = abs(root->getPoint().y - yCoord);
        if (xDiff < std::numeric_limits<double>::epsilon()  && yDiff < std::numeric_limits<double>::epsilon()) {
//            cout << "El punto ingresado contiene las mismas coordenadas que uno ya existente. Error al ingresar"
//                 << endl;
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

/**
 * OBtiene el cuadrante al que pertenece el punto. A la derecha y arriba son inclusivos (incluyen al punto). Variación
 * de la función en que considera el Quadtree entregado
 * @param point: Punto a preguntar
 * @param pr: Quadtree en el cual hacer la consulta
 * @return:  String que indica el cuadrante al que pertenece en el quadtree entregado
 */
string PRQuadTree::getQuadrant(Point point, PRQuadTree* pr) {
    // Obtiene los halfsizes
    double xHalf = (pr->xMax + pr->xMin) / (double) 2.0;
    double yHalf = (pr->yMax + pr->yMin) / (double) 2.0;
    if (point.x < xHalf) {
        if (point.y < yHalf) return "SW";
        else return "NW";
    }
    else {
        if (point.y < yHalf) return "SE";
        else return "NE";
    }
}


/**
 * Obtiene la población estimada registrada en el dataset, basada en coordenadas geográficas.
 * @param x: Coordenada x a buscar (longitud)
 * @param y: Coordenada y a buscar (latitud)
 * @param pr: Puntero al Quadtree. Inicialmente se debe llamar con el Quadtree creado. El resto son llamadas recursivas
 * manejadas por la función
 * @return: Retorna la población estimada en el dataset. Si el punto consultado no se encuentra en el dataset, retorna
 * -1.
 */
double PRQuadTree::populationAtPoint(double x, double y, PRQuadTree* pr) {
    // En caso de que el PRQUadtree consultado todavía no haya sido creado, no puede estar el punto requerido
    if (pr == nullptr){
        cout << "El punto consultado no se encuentra ingresado" << endl;
        return -1;
    }
    Node* root = pr->getRootNode();
    // Rootnode puede ser null (en cuyo caso paramos), black (llegamos a leaf node, comprobamos si es el punto
    // que estamos buscando) o gray (seguimos buscando recursivamente)
    if (root == nullptr){
        cout << "El punto consultado no se encuentra ingresado" << endl;
        return -1;
    }
    else if (root->getNodeType() == "BLACK"){
        // Obtiene cada uno de los puntos
        double xCoord = root->getPoint().x;
        double yCoord = root->getPoint().y;
        // Obtiene la diferencia absoluta entre ellos
        double xDiff = abs(x - xCoord);
        double yDiff = abs(y - yCoord);
        // Dado que la forma en que se almacenan los double pierde precisión en los últimos dígitos, no se puede
        // comparar utilizando ==. Si la diferencia es mínima (basada en el epsilon de la máquina en que se corra
        // el código), entonces se considera que son iguales.
        if (xDiff < std::numeric_limits<double>::epsilon() && yDiff < std::numeric_limits<double>::epsilon()){
            cout << "Punto consultado encontrado satisfactoriamente" << endl;
            cout << "Su población es: " << root->getData().getPopulation();
            return stod(root->getData().getPopulation());
        }
        else{
            cout << "El punto consultado no se encuentra ingresado" << endl;
            return -1;
        }
    }
    else if (root->getNodeType() == "GRAY"){
        // Obtiene el cuadrante para saber a qué nodo dirigirse
        string quad = getQuadrant(Point(x, y), pr);
        // Se dirige al cuadrante correspondiente (al correspondiente PR Quadtree) y llama recursivamente
        if (quad == "NW"){
            populationAtPoint(x, y, pr->NWNode);
        }
        else if (quad == "NE"){
            populationAtPoint(x, y, pr->NENode);
        }
        else if (quad == "SW"){
            populationAtPoint(x, y, pr->SWNode);
        }
        else{
            populationAtPoint(x, y, pr->SENode);
        }
    }
}


#endif //PROYECTOALGORITMOS_PRQUADTREE_HPP
