#ifndef PROYECTOALGORITMOS_PRQUADTREE_HPP
#define PROYECTOALGORITMOS_PRQUADTREE_HPP
#include <bits/stdc++.h>

#include <utility>
#include "City.hpp"
using namespace std;

bool activeCompact = false;
double populationCount = 0;
int pointCount = 0;

struct Point{
    double x;
    double y;
    Point(double x = 0, double y = 0): x(x), y(y){};
};

struct Region {
    double xMin, xMax;
    double yMin, yMax;
    Region(double xMin = 0, double xMax = 0, double yMin = 0, double yMax = 0) : xMin(xMin), xMax(xMax), yMin(yMin), yMax(yMax) {};
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
    int static numNodes;
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
    void deletePointDriver(Point point, PRQuadTree* pr);
    void deletePoint(Point point, PRQuadTree* pr, PRQuadTree* prParent);
    void backtrackCompact(PRQuadTree* pr, PRQuadTree* prParent, string quad);
    int pointsAtRegionDriver(Point point, double distance, PRQuadTree* pr);
    double populationAtRegionDriver(Point point, double distance, PRQuadTree* pr);
    void pointsAtRegion(Region region, PRQuadTree* pr);
    void populationAtRegion(Region region, PRQuadTree* pr);
    bool isSubRegion(Region region, Region regionRef);
    bool isPointInRegion(Point point, Region region);
    Node* getRootNode();
    string getQuadrant(Point point);
    bool insert(const City &city, PRQuadTree *pr);
    double populationAtPoint(double x, double y, PRQuadTree* pr);

    string getQuadrant(Point point, PRQuadTree *pr);
};

PRQuadTree::~PRQuadTree() {
    delete(rootNode);
}

Node *PRQuadTree::getRootNode() {
    return rootNode;
}

int PRQuadTree::numNodes = 0;


/**
 * Obtiene las coordenadas del punto contenidas en el string de entrada.
 * @param s: String con las coordenadas de la forma "x,y"
 * @param *x: Puntero a double para almacenar la coordenada x
 * @param *y: Puntero a double para almacenar la coordenada y
 */
void getCoordinate (std::string s, double *x, double *y){
    stringstream linestream(s);
    string currentString;
    getline(linestream, currentString, ',');
    *y = stod(currentString);
    getline(linestream, currentString, ',');
    *x = stod(currentString);
}

/**
 * Realiza la inserción de la ciudad en el Quadtree entregado
 * @param city: clase City que contiene los datos de interés por ciudad. Entre ellos, su población y su geolocalización
 * @param pr: Quadtree en el cual preguntar por/hacer la inserción. El resto son llamadas recursivas de sus hijos
 * manejadas por la función
 * @return: booleano que indica si la inserción fue hecha con éxito (true), o si el dato se encuentra repetido (false)
 */
bool PRQuadTree::insert(const City& city, PRQuadTree *pr) {
    // Obtiene las coordenadas del punto a insertar
    double xCoord;
    double yCoord;
    getCoordinate(city.getGeopoint(), &xCoord, &yCoord);
    // Revisa si el root de este árbol está vacío
    if (pr->getRootNode() == nullptr){
        numNodes++;
        // RootNode está vacío, el punto lo inserta en la raíz
        pr->rootNode = new Node(Point(xCoord, yCoord), city, "BLACK");
        // Se ingresó correctamente
        return true;
    }
    // Nodo raíz está ocupado con datos, se debe reestructurar
    else if (pr->getRootNode()->getNodeType() == "BLACK") {
        Node *root = pr->getRootNode();
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
            Node currentNode = *(pr->getRootNode());
            // Actualiza el nuevo nodo. City no contiene información relevante. Punto almacenado en nodo representa el centro
            pr->rootNode = new Node(Point((pr->xMax + pr->xMin) / (double) 2.0, (pr->yMax + pr->yMin) / (double) 2.0), City(),
                                   "GRAY");
            // Determina dónde pertenece el nuevo nodo
            if (quad == "NW") {
                // Calcula las nuevas coordenadas del nuevo QuadTree a crear
                double newXMin = pr->xMin;
                double newXMax = (pr->xMax + pr->xMin) / (double) 2.0;
                double newYMin = (pr->yMax + pr->yMin) / (double) 2.0;
                double newYMax = pr->yMax;
                pr->NWNode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);

                // Inserta el nuevo nodo
                (pr->NWNode)->insert(city, pr->NWNode);
            } else if (quad == "NE") {
                // Calcula las nuevas coordenadas del nuevo QuadTree a crear
                double newXMin = (pr->xMax + pr->xMin) / (double) 2.0;
                double newXMax = pr->xMax;
                double newYMin = (pr->yMax + pr->yMin) / (double) 2.0;
                double newYMax = pr->yMax;
                // Crea el nuevo PR Tree donde corresponda
                pr->NENode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);

                // Inserta nuevo nodo
                (pr->NENode)->insert(city, (pr->NENode));
            } else if (quad == "SW") {
                // Calcula las nuevas coordenadas del nuevo QuadTree a crear
                double newXMin = pr->xMin;
                double newXMax = (pr->xMax + pr->xMin) / (double) 2.0;
                double newYMin = pr->yMin;
                double newYMax = (pr->yMax + pr->yMin) / (double) 2.0;
                // Crea el nuevo PR Tree donde corresponda
                pr->SWNode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);

                // Inserta nuevo nodo
                (pr->SWNode)->insert(city, (pr->SWNode));
            } else {
                // Calcula las nuevas coordenadas del nuevo QuadTree a crear
                double newXMin = (pr->xMax + pr->xMin) / (double) 2.0;
                double newXMax = pr->xMax;
                double newYMin = pr->yMin;
                double newYMax = (pr->yMax + pr->yMin) / (double) 2.0;
                // Crea el nuevo PR Tree donde corresponda
                pr->SENode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);
                // Inserta nuevo nodo
                (pr->SENode)->insert(city, (pr->SENode));
            }
            // Reinserta el nodo original
            pr->insert(currentNode.getData(), pr);
        }
    }
   // Nodo es un nodo interno
    else if (pr->getRootNode()->getNodeType() == "GRAY"){
        string quad = getQuadrant(Point(xCoord,yCoord));
        if (quad == "NW"){
            PRQuadTree* prToInsert = pr->NWNode;
            if (prToInsert == nullptr){
                // Calcula las nuevas coordenadas del nuevo QuadTree a crear
                double newXMin = pr->xMin;
                double newXMax = (pr->xMax + pr->xMin) / (double) 2.0;
                double newYMin = (pr->yMax + pr->yMin) / (double) 2.0;
                double newYMax = pr->yMax;
                pr->NWNode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);
            }
            prToInsert->insert(city, pr->NWNode);
        }
        else if (quad == "NE"){
            if (pr->NENode == nullptr){
                double newXMin = (pr->xMax + pr->xMin) / (double) 2.0;
                double newXMax = pr->xMax;
                double newYMin = (pr->yMax + pr->yMin) / (double) 2.0;
                double newYMax = pr->yMax;
                pr->NENode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);
            }
            (pr->NENode)->insert(city, pr->NENode);
        }
        else if (quad == "SW"){
            if (pr->SWNode == nullptr){
                double newXMin = pr->xMin;
                double newXMax = (pr->xMax + pr->xMin) / (double) 2.0;
                double newYMin = pr->yMin;
                double newYMax = (pr->yMax + pr->yMin) / (double) 2.0;
                pr->SWNode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);
            }
            (pr->SWNode)->insert(city, pr->SWNode);
        }
        else{
            if (pr->SENode == nullptr){
                double newXMin = (pr->xMax + pr->xMin) / (double) 2.0;
                double newXMax = pr->xMax;
                double newYMin = pr->yMin;
                double newYMax = (pr->yMax + pr->yMin) / (double) 2.0;
                pr->SENode = new PRQuadTree(newXMin, newXMax, newYMin, newYMax);
            }
            (pr->SENode)->insert(city, pr->SENode);
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
        return -1;
    }
    Node* root = pr->getRootNode();
    // Rootnode puede ser null (en cuyo caso paramos), black (llegamos a leaf node, comprobamos si es el punto
    // que estamos buscando) o gray (seguimos buscando recursivamente)
    if (root == nullptr) {
        return -1;
    }
    else if (root->getNodeType() == "BLACK") {
        // Obtiene cada uno de los puntos
        double xCoord = root->getPoint().x;
        double yCoord = root->getPoint().y;
        // Obtiene la diferencia absoluta entre ellos
        double xDiff = abs(x - xCoord);
        double yDiff = abs(y - yCoord);
        // Dado que la forma en que se almacenan los double pierde precisión en los últimos dígitos, no se puede
        // comparar utilizando ==. Si la diferencia es mínima (basada en el epsilon de la máquina en que se corra
        // el código), entonces se considera que son iguales.
        if (xDiff < std::numeric_limits<double>::epsilon() && yDiff < std::numeric_limits<double>::epsilon()) {
            return stod(root->getData().getPopulation());
        }
        else {
            return -1;
        }
    }
    else if (root->getNodeType() == "GRAY") {
        // Obtiene el cuadrante para saber a qué nodo dirigirse
        string quad = getQuadrant(Point(x, y), pr);
        // Se dirige al cuadrante correspondiente (al correspondiente PR Quadtree) y llama recursivamente
        if (quad == "NW") {
            populationAtPoint(x, y, pr->NWNode);
        }
        else if (quad == "NE") {
            populationAtPoint(x, y, pr->NENode);
        }
        else if (quad == "SW") {
            populationAtPoint(x, y, pr->SWNode);
        }
        else {
            populationAtPoint(x, y, pr->SENode);
        }
    }
}

/**
 * Obtiene la cantidad estimada de ciudades registradas dentro de una región en el dataset, basada en coordenadas geográficas.
 * @param point: Coordenada base (central) de la búsqueda
 * @param distance: Distancia de la región de búsqueda a partir de point (la región se delimita como un cuadrado)
 * @param region: Región de búsqueda delimitada por sus coordenadas x,y mínimas y máximas.
 * @param pr: Puntero al Quadtree. Inicialmente se debe llamar con el Quadtree creado. El resto son llamadas recursivas
 * manejadas por la función
 * @return: Retorna el número de ciudades (no repetidas)insertadas dentro de la región
 * @Driver: Setea el contador inicial (pointCount) en 0, y crea la región a partir de @point y @distance .
 */
int PRQuadTree::pointsAtRegionDriver(Point point, double distance, PRQuadTree* pr) {
    pointCount = 0;
    Region region = Region(point.x - distance, point.x + distance, point.y - distance, point.y + distance);
    pointsAtRegion(region, pr);
    return pointCount;
}

void PRQuadTree::pointsAtRegion(Region region, PRQuadTree* pr) {
    // En caso de que el PRQUadtree consultado todavía no haya sido creado, pointCount no se incrementa
    if (pr == nullptr) {
        return;
    }
    // Nodo raíz del sub-nodo actual
    Node* root = pr->getRootNode();

    // root puede ser null (en cuyo caso, no hay ciudades insertadas en el nodo actual),
    // black (se llega a una hoja con una ciudad insertada, por lo que se aumenta el contador) 
    // o gray (se sigue buscando recursivamente y se comprueba si los nodos hijos están dentro de la región de interés)
    if (root == nullptr) {
        return;
    }
    else if (root->getNodeType() == "BLACK") {
        // Se comprueba si la ciudad efectivamente está dentro de la región
        if (isPointInRegion(root->getPoint(), region)) {
            pointCount += 1;
        }
        return;
    }
    else if (root->getNodeType() == "GRAY") {
        // Se construye la región a la que pertenece el padre
        Region regionRef = Region(pr->xMin, pr->xMax, pr->yMin, pr->yMax);

        // En caso de que los nodos hijos no sean nulos, se verifica si la región de interés y la del padre coinciden en algún punto,
        // y se propaga la búsqueda a un nivel más bajo
        if (pr->NWNode != nullptr) {
            if (isSubRegion(region, regionRef)) {
                pointsAtRegion(region, pr->NWNode);
            }
        }
        if (pr->NENode != nullptr) {
            if (isSubRegion(region, regionRef)) {
                pointsAtRegion(region, pr->NENode);
            }
        }
        if (pr->SWNode != nullptr) {
            if (isSubRegion(region, regionRef)) {
                pointsAtRegion(region, pr->SWNode);
            }
        }
        if (pr->SENode != nullptr) {
            if (isSubRegion(region, regionRef)) {
                pointsAtRegion(region, pr->SENode);
            }
        }

    }
}

/**
 * Obtiene la cantidad estimada de población registrada dentro de una región en el dataset, basada en coordenadas geográficas y 
 * en las ciudades insertadas.
 * @param point: Coordenada base (central) de la búsqueda
 * @param distance: Distancia de la región de búsqueda a partir de point (la región se delimita como un cuadrado)
 * @param region: Región de búsqueda delimitada por sus coordenadas x,y mínimas y máximas.
 * @param pr: Puntero al Quadtree. Inicialmente se debe llamar con el Quadtree creado. El resto son llamadas recursivas
 * manejadas por la función
 * @return: Retorna el total de población entre las ciudades (no repetidas) insertadas dentro de la región
 * @Driver: Setea el contador inicial (populationCount) en 0, y crea la región a partir de @point y @distance .
 */
double PRQuadTree::populationAtRegionDriver(Point point, double distance, PRQuadTree* pr) {
    populationCount = 0;
    Region region = Region(point.x - distance, point.x + distance, point.y - distance, point.y + distance);
    populationAtRegion(region, pr);
    return populationCount;
}

void PRQuadTree::populationAtRegion(Region region, PRQuadTree* pr) {
    // En caso de que el PRQUadtree consultado todavía no haya sido creado, populationCount no se incrementa
    if (pr == nullptr) {
        return;
    }
    // Nodo raíz del sub-nodo actual
    Node* root = pr->getRootNode();

    // root puede ser null (en cuyo caso, no hay ciudades insertadas en el nodo actual),
    // black (se llega a una hoja con una ciudad insertada, por lo que se aumenta el contador de población) 
    // o gray (se sigue buscando recursivamente y se comprueba si los nodos hijos están dentro de la región de interés)
    if (root == nullptr) {
        return;
    }
    else if (root->getNodeType() == "BLACK") {
        if (isPointInRegion(root->getPoint(), region)) {
            populationCount += stod(root->getData().getPopulation());
        }
        return;
    }
    else if (root->getNodeType() == "GRAY") {
        Region regionRef = Region(pr->xMin, pr->xMax, pr->yMin, pr->yMax);
        if (pr->NWNode != nullptr) {
            if (isSubRegion(region, regionRef)) {
                populationAtRegion(region, pr->NWNode);
            }
        }
        if (pr->NENode != nullptr) {
            if (isSubRegion(region, regionRef)) {
                populationAtRegion(region, pr->NENode);
            }
        }
        if (pr->SWNode != nullptr) {
            if (isSubRegion(region, regionRef)) {
                populationAtRegion(region, pr->SWNode);
            }
        }
        if (pr->SENode != nullptr) {
            if (isSubRegion(region, regionRef)) {
                populationAtRegion(region, pr->SENode);
            }
        }

    }
}

/**
 * Determina si una región tiene elementos en común con una región de referencia.
 * @param region: Región de interés
 * @param regionRef: Región de referencia
 * @return: true si las regiones tienen elementos en común, false caso contrario
 */
bool PRQuadTree::isSubRegion(Region region, Region regionRef) {
    // La revisión se hace en función de las coordenadas mínimas y máximas, por lo que es necesario verificar si region
    // está contenido en regionRef, o si regionRef está contenido en region

    // Determina si region está contenido en regionRef
    if (region.yMax >= regionRef.yMin && region.yMax <= regionRef.yMax) {
        if (region.xMax >= regionRef.xMin && region.xMax <= regionRef.xMax) {
            return true;
        }
        else if (region.xMin >= regionRef.xMin && region.xMin <= regionRef.xMax) {
            return true;
        }
    }
    else if (region.yMin >= regionRef.yMin && region.yMin <= regionRef.yMax) {
        if (region.xMax >= regionRef.xMin && region.xMax <= regionRef.xMax) {
            return true;
        }
        else if (region.xMin >= regionRef.xMin && region.xMin <= regionRef.xMax) {
            return true;
        }
    }

    // Determina si regionRef está contenido en region
    if (regionRef.yMax >= region.yMin && regionRef.yMax <= region.yMax) {
        if (regionRef.xMax >= region.xMin && regionRef.xMax <= region.xMax) {
            return true;
        }
        else if (regionRef.xMin >= region.xMin && regionRef.xMin <= region.xMax) {
            return true;
        }
    }
    else if (regionRef.yMin >= region.yMin && regionRef.yMin <= region.yMax) {
        if (regionRef.xMax >= region.xMin && regionRef.xMax <= region.xMax) {
            return true;
        }
        else if (regionRef.xMin >= region.xMin && regionRef.xMin <= region.xMax) {
            return true;
        }
    }

    // Ninguno de los dos casos se cumple 
    return false;
}

/**
 * Determina si un punto es parte de una región
 * @param point: Punto de interés
 * @param region: Region a consultar
 * @return: true si el punto pertenece a la región, false caso contrario
 */
bool PRQuadTree::isPointInRegion(Point point, Region region) {
    // Determina si point está contenido en region
    if (point.y >= region.yMin && point.y <= region.yMax) {
        if (point.x >= region.xMin && point.x <= region.xMax) {
            return true;
        }
    }
    // False si ninguno de los dos casos se cumple 
    return false;
}



/**
 * Se elimina algún punto de interés. Primero se consulta si este existe, y luego se inicia la eliminación por backtracking
 * @param point: Punto a eliminar
 * @param pr: Puntero al Quadtree. Inicialmente se debe llamar con el Quadtree creado. El resto son llamadas recursivas
 * manejadas por la función
 * @param prParent: Puntero al Quadtree padre. Inicialmente se debe llamar con el puntero nulo. El resto son llamadas recursivas
 * manejadas por la función
 * @Driver: Se setea el inicio del backtracking como false y se llama a deletePoint con QuadTree-padre nulo
 */
void PRQuadTree::deletePointDriver(Point point, PRQuadTree* pr) {
    activeCompact = false;
    deletePoint(point, pr, nullptr);
}

void PRQuadTree::deletePoint(Point point, PRQuadTree* pr, PRQuadTree* prParent) {
    // En caso de que el PRQUadtree consultado todavía no haya sido creado, se sale de la recursión
    if (pr == nullptr) {
        return;
    }
    // Nodo raíz del sub-nodo actual
    Node* currentNode = pr->getRootNode();

    /* Inicio de la Búsqueda*/

    // root puede ser null (en cuyo caso, no hay ciudades insertadas en el nodo actual),
    // black (se llega a una hoja con una ciudad insertada, por lo que se verifica si coincide con la que se desea eliminar) 
    // o gray (se sigue buscando recursivamente)
    if (currentNode == nullptr) {
        return;
    }
    // Nodo raíz está ocupado con datos
    else if (currentNode->getNodeType() == "BLACK") {
        // Obtiene cada uno de los puntos
        double xCoord = currentNode->getPoint().x;
        double yCoord = currentNode->getPoint().y;
        // Obtiene la diferencia absoluta entre ellos
        double xDiff = abs(point.x - xCoord);
        double yDiff = abs(point.y - yCoord);
        // Dado que la forma en que se almacenan los double pierde precisión en los últimos dígitos, no se puede
        // comparar utilizando ==. Si la diferencia es mínima (basada en el epsilon de la máquina en que se corra
        // el código), entonces se considera que son iguales.
        if (xDiff < std::numeric_limits<double>::epsilon() && yDiff < std::numeric_limits<double>::epsilon()) {
            // Si se encuentra una coincidencia, se elimina la información del nodo raíz y se inicia la eliminación por backtracking
            pr->rootNode = nullptr;
            activeCompact = true;
        }

    }
    // Nodo es un nodo interno
    else if (currentNode->getNodeType() == "GRAY") {
        // Obtiene el cuadrante para saber a qué nodo dirigirse
        string quad = getQuadrant(point, pr);
        // Se dirige al cuadrante correspondiente (al correspondiente PR Quadtree) y llama recursivamente a deletePoint para retomar la
        // búsqueda por el hijo correspondiente
        if (quad == "NW") {
            deletePoint(point, pr->NWNode, pr);
        }
        else if (quad == "NE") {
            deletePoint(point, pr->NENode, pr);
        }
        else if (quad == "SW") {
            deletePoint(point, pr->SWNode, pr);
        }
        else {
            deletePoint(point, pr->SENode, pr);
        }
    }

    /* Inicio de la compactación por backtracking*/
    if (activeCompact) {
        backtrackCompact(pr, prParent, getQuadrant(point, prParent));
    }
    return;
}

/**
 * Se inicia la eliminación por backtracking. Se comprimen todos los nodos con un único hijo "BLACK" restante.
 * @param pr: Puntero al Quadtree hijo.
 * @param prParent: Puntero al Quadtree padre. Se analiza el contenido de sus 3 hijos distintos a @pr
 * @param quad: Quadrante al que pertenece @pr
 */
void PRQuadTree::backtrackCompact(PRQuadTree* pr, PRQuadTree* prParent, string quad) {
    // whiteCount es contador de los nodos vacíos; grayCount es contador de los nodos con sub-divisiones
    int whiteCount = 0, grayCount = 0;
    
    // Arreglo con los nodos a ser compactados
    vector<PRQuadTree*> storedNode;

    // Se consulta a los 3 cuadrantes diferentes a quad por el elemento que contienen. 
    // Si el nodo hijo está vacío, o el nodo raíz dentro del hijo está vacío, se incrementa whiteCount
    // Si el nodo raíz dentro del hijo es gris, se incrementa grayCount
    // Si el nodo raíz dentro del hijo es black, se añade dicho nodo a storeNode
    if (quad != "NE") {
        //
        if (prParent->NENode == nullptr || prParent->NENode->getRootNode() == nullptr) {
            whiteCount++;
        }
        else if (prParent->NENode->getRootNode()->getNodeType() == "GRAY") {
            grayCount++;
        }
        else if (prParent->NENode->getRootNode()->getNodeType() == "BLACK") {
            storedNode.push_back(prParent->NENode);
        }
    }
    if (quad != "NW") {
        //
        if (prParent->NWNode == nullptr || prParent->NWNode->getRootNode() == nullptr) {
            whiteCount++;
        }
        else if (prParent->NWNode->getRootNode()->getNodeType() == "GRAY") {
            grayCount++;
        }
        else if (prParent->NWNode->getRootNode()->getNodeType() == "BLACK") {
            storedNode.push_back( prParent->NWNode);
        }
    }
    if (quad != "SE") {
        if (prParent->SENode == nullptr || prParent->SENode->getRootNode() == nullptr) {
            whiteCount++;
        }
        else if (prParent->SENode->getRootNode()->getNodeType() == "GRAY") {
            grayCount++;
        }
        else if (prParent->SENode->getRootNode()->getNodeType() == "BLACK") {
            storedNode.push_back( prParent->SENode);
        }
    }
    if (quad != "SW"){
        //
        if (prParent->SWNode == nullptr || prParent->SWNode->getRootNode() == nullptr) {
            whiteCount++;
        }
        else if (prParent->SWNode->getRootNode()->getNodeType() == "GRAY") {
            grayCount++;
        }
        else if (prParent->SWNode->getRootNode()->getNodeType() == "BLACK") {
            storedNode.push_back(prParent->SWNode);
        }
    }

    // Si storeNode tiene un único elemento, y el resto de los hermanos de quad son nulos, se compacta prParent
    if (storedNode.size() == 1 && grayCount == 0 && pr->getRootNode() == nullptr) {
        prParent->rootNode = storedNode[0]->getRootNode();
        storedNode[0]->rootNode = nullptr;
        return;
    }
    // Si los hermanos de quad son todos nulos, la información del nodo padre se elimina si quad está vacío
    else if (whiteCount == 3) {
        if (pr->getRootNode() == nullptr) {
            prParent->rootNode = nullptr;
            return;
        }
        // Caso contrario, se compacta prParent
        else {
            prParent->rootNode = pr->getRootNode();
            pr->rootNode = nullptr;
            return;
        }
    }
    // Si no hay casos para compactación, se detiene el backtracking
    else {
        activeCompact = false;
        return;
    }
    
}


#endif //PROYECTOALGORITMOS_PRQUADTREE_HPP
