#include <iostream>
#include "City.hpp"
#include "PRQuadTree.hpp"
#include <sys/stat.h>
#include <vector>
#include <fstream>
#include <sstream>	
#include <chrono>

using namespace std;



/**
* Lee la base de datos provista de usuarios de twitter y la almacena en un vector usando la clase User
* @return: Retorna un vector con todos los usuarios leídos
*/
vector<City> readFile(){
    // Puntero a archivo
    ifstream fin;
    // Abre el archivo con la base de datos
    fin.open("worldcitiespop_fixed.csv", ios::in);
    string line, word, temp;
    // Vector de strings para almacenar cada palabra
    vector<string> tempInfo;
    // Vector para almacenar todos los usuarios
    vector<City> cities;
    // Lee la primera línea, que son los nombres de las columnas. No es relevante, hace nada.
    getline(fin, line);
    // Lee hasta EOF
    while (getline(fin, line)){
        // Borra la info anterior que pudiera haber en el vector
        tempInfo.clear();
        // Crea un stringstream para cada palabra individual
        stringstream s(line);
        // Ahora almacena cada palabra de forma independiente, separadas por \t
        while(getline(s, word, ';')){
            tempInfo.push_back(word);
        }
        cities.emplace_back(tempInfo[0], tempInfo[1], tempInfo[2], tempInfo[3], tempInfo[4], tempInfo[5], tempInfo[6],
                            tempInfo[7]);
    }
    return cities;
}



vector <City> cities;
/**
 * Corre distintas pruebas utilizando los métodos de PRQuadTree
 * @param numTests: Número de casos a promediar
 * @param tests: Vector de strings. Cada string es una prueba de método a realizar
 */
void run_tests(int numTests, string testMode, PRQuadTree pr){
    // Revisa si existe el archivo de test. Si existe lo borra para crear uno nuevo
    struct stat buffer;
    if (stat("results_tests.txt", &buffer) == 0)
        remove("results_tests.txt");
    // Stream para guardar los resultados
    ofstream file;
    // Abre el archivo
    file.open("results_tests.txt");
    // Vector para almacenar los tiempos que tarda
    vector<double> timesTaken;

    if (testMode == "insert"){
        file << "**************** COMIENZA PRUEBA DE INSERCIÓN **************** \n \n";
        file << "**** Prueba de inserción 100 elementos **** \n \n";
        for (int numTest = 1; numTest <= numTests; numTest++) {
            file << "** Test " << numTest << " ** \n";
            // Inserta 100 elementos y comienza medición de tiempo
            auto start = std::chrono::system_clock::now();
            for (int i = 0; i <100; i++)
                pr.insert(cities[i], &pr);
            // Detiene el reloj
            auto end = std::chrono::system_clock::now();
            double time = chrono::duration<double>(end - start).count();
            timesTaken.push_back(time);
            file << "Tiempo parcial en insertar 100 elementos: " << time << " [s] \n" << endl;
        }
        // Calcula el tiempo promedio
        double avg = 0;
        for (auto it: timesTaken) avg += it;
        avg /= numTests;
        file << "Tiempo promedio en insertar 100 elementos: " << avg << " [s] \n";
        timesTaken.clear();
        file << "**** Prueba de inserción 10.000 elementos **** \n \n";
        for (int numTest = 1; numTest <= numTests; numTest++) {
            file << "** Test " << numTest << " ** \n";
            // Repite insertando 10.000 elementos
            auto start = std::chrono::system_clock::now();
            for (int i = 0; i < 10000; i++)
                pr.insert(cities[i], &pr);
            // Detiene el reloj
            auto end = std::chrono::system_clock::now();
            double time = chrono::duration<double>(end - start).count();
            timesTaken.push_back(time);
            file << "Tiempo parcial en insertar 10.000 elementos: " << time << "[s] \n";
        }
        avg = 0;
        for (auto it: timesTaken) avg += it;
        avg /= numTests;
        file << "Tiempo promedio en insertar 10.000 elementos: " << avg << " [s] \n";

        timesTaken.clear();
        file << "**** Prueba de inserción 100.000 elementos **** \n \n";
        for (int numTest = 1; numTest <= numTests; numTest++) {
            file << "** Test " << numTest << " ** \n";
            // Repite insertando 100.000 elementos
            auto start = std::chrono::system_clock::now();
            for (int i = 0; i < 100000; i++)
                pr.insert(cities[i], &pr);
            // Detiene el reloj
            auto end = std::chrono::system_clock::now();
            double time = chrono::duration<double>(end - start).count();
            timesTaken.push_back(time);
            file << "Tiempo parcial en insertar 100.000 elementos: " << time << "[s] \n";
        }

        avg = 0;
        for (auto it: timesTaken) avg += it;
        avg /= numTests;
        file << "Tiempo promedio en insertar 100.000 elementos: " << avg << " [s] \n";
        timesTaken.clear();

        file << "**** Prueba de inserción 1.000.000 elementos **** \n \n";
        for (int numTest = 1; numTest <= numTests; numTest++) {
            file << "** Test " << numTest << " ** \n";
            // Repite insertando 1.000.000 elementos
            auto start = std::chrono::system_clock::now();
            for (int i = 0; i < 1000000; i++)
                pr.insert(cities[i], &pr);
            // Detiene el reloj
            auto end = std::chrono::system_clock::now();
            double time = chrono::duration<double>(end - start).count();
            timesTaken.push_back(time);
            file << "Tiempo parcial en insertar 1.000.000 elementos: " << time << "[s] \n";
        }
        avg = 0;
        for (auto it: timesTaken) avg += it;
        avg /= numTests;
        file << "Tiempo promedio en insertar 1.000.000 elementos: " << avg << " [s] \n";
        timesTaken.clear();

        file << "**** Prueba de inserción 1.000.000 elementos **** \n \n";
        for (int numTest = 1; numTest <= numTests; numTest++) {
            file << "** Test " << numTest << " ** \n";
            // Repite insertando 3.100.000 elementos
            auto start = std::chrono::system_clock::now();
            for (int i = 0; i < 3100000; i++)
                pr.insert(cities[i], &pr);
            // Detiene el reloj
            auto end = std::chrono::system_clock::now();
            double time = chrono::duration<double>(end - start).count();
            timesTaken.push_back(time);
            file << "Tiempo parcial en insertar 3.100.000 elementos: " << time << "[s] \n";
        }
        avg = 0;
        for (auto it: timesTaken) avg += it;
        avg /= numTests;
        file << "Tiempo promedio en insertar 3.100.000 elementos: " << avg << " [s] \n";
        timesTaken.clear();
        cout << "Insert ready" << endl;
    } // end if insert

        else if (testMode == "delete") {
        file << "**************** COMIENZA PRUEBA DE ELIMINACIÓN **************** \n \n";
        file << "**** Eliminación de ciudades insertadas random **** \n \n";
        for (int i = 0; i < 1000000; i++)
            pr.insert(cities[i], &pr);
        file << "Elementos actuales: " << pr.pointsAtRegionDriver(Point(0, 0), 256, &pr) << endl;
        for (int numTest = 1; numTest <= numTests; numTest++) {
            file << "** Test " << numTest << " ** \n";
            int deleteIndex = rand() % 1000000;
            double xCoord;
            double yCoord;
            getCoordinate(cities[deleteIndex].getGeopoint(), &xCoord, &yCoord);

            file << "Eliminando punto: ( " << xCoord << "," << yCoord << ")" << endl;
            // Comienza medición de tiempo
            auto start = std::chrono::system_clock::now();
            pr.deletePointDriver(Point(xCoord, yCoord), &pr);
            // Detiene el reloj
            auto end = std::chrono::system_clock::now();
            double time = chrono::duration<double>(end - start).count();
            timesTaken.push_back(time);
            file << "Tiempo parcial en eliminar elemento random: " << time << " [s] \n" << endl;
            file << "Elementos restantes: " << pr.pointsAtRegionDriver(Point(0, 0), 256, &pr) << endl;
        }
        // Calcula el tiempo promedio
        double avg = 0;
        for (auto it : timesTaken) avg += it;
        avg /= numTests;
        file << "Tiempo promedio de eliminación de elementos: " << avg << " [s] \n";
        timesTaken.clear();

        file << "\n \n**** Eliminación de ciudades no insertadas random **** \n \n";
        for (int numTest = 1; numTest <= numTests; numTest++) {
            file << "** Test " << numTest << " ** \n";
            int deleteIndex = rand() % (2000000 - 1000001) + 1000001;
            double xCoord;
            double yCoord;
            getCoordinate(cities[deleteIndex].getGeopoint(), &xCoord, &yCoord);

            file << "Eliminando punto: ( " << xCoord << "," << yCoord << ")" << endl;
            // Comienza medición de tiempo
            auto start = std::chrono::system_clock::now();
            pr.deletePointDriver(Point(xCoord, yCoord), &pr);
            // Detiene el reloj
            auto end = std::chrono::system_clock::now();
            double time = chrono::duration<double>(end - start).count();
            timesTaken.push_back(time);
            file << "Tiempo parcial en eliminar elemento random: " << time << " [s] \n" << endl;
            file << "Elementos restantes: " << pr.pointsAtRegionDriver(Point(0, 0), 256, &pr) << endl;
        }
        // Calcula el tiempo promedio
        avg = 0;
        for (auto it : timesTaken) avg += it;
        avg /= numTests;
        file << "Tiempo promedio de eliminación de elementos: " << avg << " [s] \n";
        timesTaken.clear();
        cout << "Delete ready" << endl;

    } // end if delete

    else if (testMode == "populationSearch") {
        file << "**************** COMIENZA PRUEBA DE ELIMINACIÓN **************** \n \n";
        file << "**** Búsqueda de población en ciudad insertada random **** \n \n";
        for (int i = 0; i < 1000000; i++)
            pr.insert(cities[i], &pr);
        file << "Elementos actuales: " << pr.pointsAtRegionDriver(Point(0, 0), 256, &pr) << endl;
        for (int numTest = 1; numTest <= numTests; numTest++) {
            file << "** Test " << numTest << " ** \n";
            int searchIndex = rand() % 1000000;
            double xCoord;
            double yCoord;
            getCoordinate(cities[searchIndex].getGeopoint(), &xCoord, &yCoord);

            file << "Buscando punto: ( " << xCoord << "," << yCoord << ")" << endl;
            // Comienza medición de tiempo
            auto start = std::chrono::system_clock::now();
            pr.populationAtPoint(xCoord, yCoord, &pr);
            // Detiene el reloj
            auto end = std::chrono::system_clock::now();
            double time = chrono::duration<double>(end - start).count();
            timesTaken.push_back(time);
            file << "Tiempo parcial en buscar elemento random: " << time << " [s] \n" << endl;

        }
        // Calcula el tiempo promedio
        double avg = 0;
        for (auto it : timesTaken) avg += it;
        avg /= numTests;
        file << "Tiempo promedio de búsqueda de elementos random: " << avg << " [s] \n";
        timesTaken.clear();

        file << "\n \n**** Búsqueda de población en ciudad no insertada random **** \n \n";
        for (int numTest = 1; numTest <= numTests; numTest++) {
            file << "** Test " << numTest << " ** \n";
            int searchIndex = rand() % (2000000 - 1000001) + 1000001;
            double xCoord;
            double yCoord;
            getCoordinate(cities[searchIndex].getGeopoint(), &xCoord, &yCoord);

            file << "Buscando punto: ( " << xCoord << "," << yCoord << ")" << endl;
            // Comienza medición de tiempo
            auto start = std::chrono::system_clock::now();
            pr.populationAtPoint(xCoord, yCoord, &pr);
            // Detiene el reloj
            auto end = std::chrono::system_clock::now();
            double time = chrono::duration<double>(end - start).count();
            timesTaken.push_back(time);
            file << "Tiempo parcial en buscar elemento random: " << time << " [s] \n" << endl;
            file << "Elementos restantes: " << pr.pointsAtRegionDriver(Point(0, 0), 256, &pr) << endl;
        }
        // Calcula el tiempo promedio
        avg = 0;
        for (auto it : timesTaken) avg += it;
        avg /= numTests;
        file << "Tiempo promedio de búsqueda de elementos random no insertados: " << avg << " [s] \n";
        timesTaken.clear();
        cout << "Population search ready" << endl;

    } // end if delete

    else if (testMode == "pointRegion") {
        file << "**************** COMIENZA PRUEBA DE PUNTOS POR REGIÓN **************** \n \n";
        file << "**** Cálculo de ciudades insertadas por región random **** \n \n";
        for (int i = 0; i < 1000000; i++)
            pr.insert(cities[i], &pr);
        file << "Elementos actuales: " << pr.pointsAtRegionDriver(Point(0, 0), 256, &pr) << endl;
        for (int numTest = 1; numTest <= numTests; numTest++) {
            file << "** Test " << numTest << " ** \n";
            int regionIndex = rand() % 1000000;
            int distance = rand() % (257-1)+1 ;
            double xCoord;
            double yCoord;
            getCoordinate(cities[regionIndex].getGeopoint(), &xCoord, &yCoord);

            file << "Realizando búsqueda para: ( " << xCoord << "," << yCoord << ")" << ", con distancia: "<< distance << endl;
            // Comienza medición de tiempo
            auto start = std::chrono::system_clock::now();
            int tmp = pr.pointsAtRegionDriver(Point(xCoord, yCoord), distance, &pr);
            // Detiene el reloj
            auto end = std::chrono::system_clock::now();
            double time = chrono::duration<double>(end - start).count();
            timesTaken.push_back(time);
            file << "Tiempo parcial de búsqueda por región: " << time << " [s] \n" << endl;
            file << "Elementos presentes en la región random: " << tmp << endl;
        }
        // Calcula el tiempo promedio
        double avg = 0;
        for (auto it : timesTaken) avg += it;
        avg /= numTests;
        file << "Tiempo promedio de búsqueda por región random: " << avg << " [s] \n";
        timesTaken.clear();

        file << "\n \n**** Cálculo de ciudades insertadas por región completa **** \n \n";
        for (int numTest = 1; numTest <= numTests; numTest++) {
            file << "** Test " << numTest << " ** \n";

            // Comienza medición de tiempo
            auto start = std::chrono::system_clock::now();
            int tmp = pr.pointsAtRegionDriver(Point(0, 0), 256, &pr);
            // Detiene el reloj
            auto end = std::chrono::system_clock::now();
            double time = chrono::duration<double>(end - start).count();
            timesTaken.push_back(time);
            file << "Tiempo parcial de búsqueda por región: " << time << " [s] \n" << endl;
            file << "Elementos presentes en la región completa: " << tmp << endl;
        }
        // Calcula el tiempo promedio
        avg = 0;
        for (auto it : timesTaken) avg += it;
        avg /= numTests;
        file << "Tiempo promedio de búsqueda por región completa: " << avg << " [s] \n";
        timesTaken.clear();
        cout << "Point search at region  ready" << endl;

    } // end if pointRegion

    else if (testMode == "populationRegion") {
        file << "**************** COMIENZA PRUEBA DE POBLACIÓN POR REGIÓN **************** \n \n";
        file << "**** Cálculo de población insertadas por región random **** \n \n";
        for (int i = 0; i < 1000000; i++)
            pr.insert(cities[i], &pr);
        file << "Elementos actuales: " << pr.pointsAtRegionDriver(Point(0, 0), 256, &pr) << endl;
        for (int numTest = 1; numTest <= numTests; numTest++) {
            file << "** Test " << numTest << " ** \n";
            int regionIndex = rand() % 1000000;
            int distance = rand() % (257 - 1) + 1;
            double xCoord;
            double yCoord;
            getCoordinate(cities[regionIndex].getGeopoint(), &xCoord, &yCoord);

            file << "Realizando búsqueda para: ( " << xCoord << "," << yCoord << ")" << ", con distancia: " << distance << endl;
            // Comienza medición de tiempo
            auto start = std::chrono::system_clock::now();
            int tmp = pr.populationAtRegionDriver(Point(xCoord, yCoord), distance, &pr);
            // Detiene el reloj
            auto end = std::chrono::system_clock::now();
            double time = chrono::duration<double>(end - start).count();
            timesTaken.push_back(time);
            file << "Tiempo parcial de búsqueda por región: " << time << " [s] \n" << endl;
            file << "Población presentes en la región random: " << tmp << endl;
        }
        // Calcula el tiempo promedio
        double avg = 0;
        for (auto it : timesTaken) avg += it;
        avg /= numTests;
        file << "Tiempo promedio de búsqueda por región random: " << avg << " [s] \n";
        timesTaken.clear();

        file << "\n \n**** Cálculo de población insertada por región completa **** \n \n";
        for (int numTest = 1; numTest <= numTests; numTest++) {
            file << "** Test " << numTest << " ** \n";

            // Comienza medición de tiempo
            auto start = std::chrono::system_clock::now();
            int tmp = pr.populationAtRegionDriver(Point(0, 0), 256, &pr);
            // Detiene el reloj
            auto end = std::chrono::system_clock::now();
            double time = chrono::duration<double>(end - start).count();
            timesTaken.push_back(time);
            file << "Tiempo parcial de búsqueda por región: " << time << " [s] \n" << endl;
            file << "Población presentes en la región completa: " << tmp << endl;
        }
        // Calcula el tiempo promedio
        avg = 0;
        for (auto it : timesTaken) avg += it;
        avg /= numTests;
        file << "Tiempo promedio de búsqueda por región completa: " << avg << " [s] \n";
        timesTaken.clear();
        cout << "Population search at region ready" << endl;
    } // end if populationRegion

    // Cierra el archivo
    file.close();

}






int main() {
    string inputLine, mode;
    int testCases;
    int totalInsertions;
    srand(time(nullptr));
    PRQuadTree pr(-256, 256, -256, 256);

    cout << "Iniciando lectura de casos" << endl;
    cities = readFile();
    cout << "Lectura finalizada" << endl;
    cout << "Ingrese el número inicial de puntos a insertar." << endl;
    cin >> totalInsertions;
    cout << "Iniciando inserción de elementos" << endl;
    
    for (int i = 0; i < totalInsertions; i++)
        pr.insert(cities[i], &pr);
    int option = 0;
    cout << "Total de elementos insertados no repetidos: ";
    cout << pr.pointsAtRegionDriver(Point(0, 0), 256, &pr) << endl;

    do{
        cout << "\nEscoja alguna de las siguientes operaciones a realizar: " << endl;
        cout << "1. Población en un punto específico P(x, y)" << endl;
        cout << "2. Población en una región dado un centro P(x,y) y distancia d" << endl;
        cout << "3. Cantidad de puntos en una región dado un centro P(x,y) y distancia d" << endl;
        cout << "4. Eliminación de un punto P(x,y)" << endl;
        cout << "5. Insertar nueva cantidad de puntos " << endl;
        cout << "6. Salir" << endl;
        cin >> option;
        if (option == 1){
            cout << "\nInserte las coordenadas seguidas de un espacio. Ejemplo: 39.19209 40.920404" << endl;
            double x, y;
            cin >> x >> y;
            cout << "Iniciando búsqueda de población en el punto (" << x << "," << y << ")" << endl;
            auto start = std::chrono::system_clock::now();
            double population = pr.populationAtPoint(x, y, &pr);
            auto end = std::chrono::system_clock::now();
            double time = chrono::duration<double>(end - start).count();
            if (population == -1) cout << "Error en la búsqueda. Punto no encontrado" << endl;
            else cout << "La población encontrada es: " << population << endl;
            cout << "El tiempo que tomó hacer la búsqueda es " << time << " [s]" << endl;
        }
        else if (option == 2){
            cout << "\nInserte las coordenadas y distancia todas seguidas de un espacio. Ejemplo: 39.19209 40.920404 5"
            << endl;
            double x, y, distance;
            cin >> x >> y >> distance;
            cout << "Iniciando búsqueda de población en una región con centro"
                    " (" << x << "," << y << ") y distancia " << distance << endl;
            auto start = std::chrono::system_clock::now();
            double population = pr.populationAtRegionDriver(Point(x, y), distance, &pr);
            auto end = std::chrono::system_clock::now();
            double time = chrono::duration<double>(end - start).count();
            cout << "La población en la región es: " << population << endl;
            cout << "El tiempo que tomó hacer la búsqueda es " << time << " [s]" << endl;
        }
        else if (option == 3) {
            cout << "\nInserte las coordenadas y distancia todas seguidas de un espacio. Ejemplo: 39.19209 40.920404 5"
                << endl;
            double x, y, distance;
            cin >> x >> y >> distance;
            cout << "Iniciando búsqueda de puntos en una región con centro"
                " (" << x << "," << y << ") y distancia " << distance << endl;
            auto start = std::chrono::system_clock::now();
            double points = pr.pointsAtRegionDriver(Point(x, y), distance, &pr);
            auto end = std::chrono::system_clock::now();
            double time = chrono::duration<double>(end - start).count();
            cout << "Los puntos en la región son: " << points << endl;
            cout << "El tiempo que tomó hacer la búsqueda es " << time << " [s]" << endl;
        }
        else if (option == 4) {
            cout << "\nInserte las coordenadas seguidas de un espacio. Ejemplo: 39.19209 40.920404"
                << endl;
            double x, y;
            cin >> x >> y;
            cout << "Iniciando eliminación en el punto (" << x << "," << y << ")" << endl;
            auto start = std::chrono::system_clock::now();
            pr.deletePointDriver(Point(x,y),&pr);
            auto end = std::chrono::system_clock::now();
            double time = chrono::duration<double>(end - start).count();
            cout << "Elementos restantes: " << pr.pointsAtRegionDriver(Point(0, 0), 256, &pr) << endl;
            cout << "El tiempo que tomó hacer la eliminación es " << time << " [s]" << endl;
        }
        else if (option == 5) {
            pr.~PRQuadTree();
            pr = PRQuadTree(-256, 256, -256, 256);
            cout << "Inserte el número de elementos a introducir. Ejemplo: 1000" << endl;
            cin >> totalInsertions;
            cout << "Iniciando inserción de elementos" << endl;
            auto start = std::chrono::system_clock::now();
            for (int i = 0; i < totalInsertions; i++)
                pr.insert(cities[i], &pr);
            // Detiene el reloj
            auto end = std::chrono::system_clock::now();
            double time = chrono::duration<double>(end - start).count();
            cout << "Total de elementos insertados no repetidos: ";
            cout << pr.pointsAtRegionDriver(Point(0, 0), 256, &pr) << endl;
            cout << "El tiempo que tomó hacer la inserción es " << time << " [s]" << endl;
        }


    } while(option < 6);





}
