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
    /**
     * Método para calcular si un punto pertenece a este cuadrante, basado en distancia euclidiana
     * @param a: Punto a verificar
     * @return: true si pertenece, false de otra forma
     */
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
    /**
     * Método para ver si dos cuadrantes se están intersectando
     * @param other: Cuadrante a verificar
     * @return: True si existe intersección, falso de otra forma
     */
    bool intersects(Quadrant other)
    {
        if(centre.x + halfSize.x > other.centre.x - other.halfSize.x || centre.x - halfSize.x < other.centre.x +
        other.halfSize.x)
        {
            // This bottom > that top
            if(centre.y + halfSize.y > other.centre.y - other.halfSize.y || centre.y - halfSize.y < other.centre.y +
            other.halfSize.y)
            {
                return true;
            }
        }
        return false;
    }
};
/**
 * Estructura que representa un nodo. Contiene posición (Point) y contenido, que es un puntero a clase City.
 */
struct Data{
    Point pos;
    City* content;
    Data(Point pos = Point(), City* data = NULL): pos(pos), content(data){};
};


class PRQuadTree {
private:
    // Cada uno de los 4 children (cuadrantes)
    PRQuadTree* nw;
    PRQuadTree* ne;
    PRQuadTree* sw;
    PRQuadTree* se;
    Quadrant quadrant;
    vector<Data> objects;
    int CAPACITY;
public:
    // Constructores
    PRQuadTree();
    PRQuadTree(Quadrant quarant);
    // Destructor
    ~PRQuadTree();

    bool insert(Data data);
    void subdivide();
    vector<Data> queryRange(Quadrant range);
    void deletePoint();
    int quantityOfPoints();
    float populationAtPoint();
    float populationAtRegion();



};


#endif //PROYECTOALGORITMOS_PRQUADTREE_HPP
