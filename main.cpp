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
vector<City> readFile(string filename = "worldcitiespop_fixed.csv" ){
    // Puntero a archivo
    ifstream fin;
    // Abre el archivo con la base de datos
    fin.open(filename, ios::in);
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
void run_tests(int numTests, vector<string> const& tests){
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
    // Comienza a leer los archivos
    cities = readFile();
    cout << "Tamaño: " << cities.size() << endl;
    // Latitud va de -90 a 90 (eje y), longitud de -180 a 80 (eje x)
    PRQuadTree pr(-256, 256, -256, 256);
    // Itera para cada uno de los tests en el vector
    for (const auto& test: tests){
        if (test == "insert"){
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

                double xCoord;
                double yCoord;
                getCoordinate(cities[0].getGeopoint(), &xCoord, &yCoord);
                file << "Buscando punto: ( " << xCoord << "," << yCoord << ")" << endl;

                start = std::chrono::system_clock::now();
                double pop = pr.populationAtPoint(xCoord, yCoord, &pr);
                // Detiene el reloj
                end = std::chrono::system_clock::now();
                time = chrono::duration<double>(end - start).count();

                if (pop == -1) {
                    file << "El punto consultado no se encuentra ingresado" << endl;
                }
                else {
                    file << "Punto consultado encontrado satisfactoriamente" << endl;
                    file << "Su población es: " << pop << endl;
                }
                file << "Tiempo parcial en buscar punto: " << time << " [s] \n" << endl;

                start = std::chrono::system_clock::now();
                file << "Elementos verificados en la región completa: " << pr.pointsAtRegionDriver(Point(0, 0), 256, &pr) << endl;
                // Detiene el reloj
                end = std::chrono::system_clock::now();
                time = chrono::duration<double>(end - start).count();
                file << "Tiempo parcial en buscar ciudades insertadas en la región completa: " << time << " [s] \n" << endl;


                start = std::chrono::system_clock::now();
                file << "Población verificada en la región completa: " << pr.populationAtRegionDriver(Point(0, 0), 256, &pr) << endl;
                // Detiene el reloj
                end = std::chrono::system_clock::now();
                time = chrono::duration<double>(end - start).count();
                file << "Tiempo parcial en buscar población total insertada en la región completa: " << time << " [s] \n" << endl;

                getCoordinate(cities[50].getGeopoint(), &xCoord, &yCoord);
                file << "Eliminando punto: ( " << xCoord << "," << yCoord << ")" << endl;

                start = std::chrono::system_clock::now();
                pr.deletePointDriver(Point(xCoord, yCoord), &pr);
                // Detiene el reloj
                end = std::chrono::system_clock::now();
                time = chrono::duration<double>(end - start).count();
                file << "Tiempo parcial de eliminación: " << time << " [s] \n" << endl;


                file << "Elementos verificados: " << pr.pointsAtRegionDriver(Point(0, 0), 256, &pr) << endl;
                file << endl;
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
                pr = PRQuadTree(-256, 256, -256, 256);
                auto start = std::chrono::system_clock::now();
                for (int i = 0; i < 10000; i++)
                    pr.insert(cities[i], &pr);
                // Detiene el reloj
                auto end = std::chrono::system_clock::now();
                double time = chrono::duration<double>(end - start).count();
                timesTaken.push_back(time);
                file << "Tiempo parcial en insertar 10.000 elementos: " << time << "[s] \n";

                double xCoord;
                double yCoord;
                getCoordinate(cities[0].getGeopoint(), &xCoord, &yCoord);
                file << "Buscando punto: ( " << xCoord << "," << yCoord << ")" << endl;

                start = std::chrono::system_clock::now();
                double pop = pr.populationAtPoint(xCoord, yCoord, &pr);
                // Detiene el reloj
                end = std::chrono::system_clock::now();
                time = chrono::duration<double>(end - start).count();

                if (pop == -1) {
                    file << "El punto consultado no se encuentra ingresado" << endl;
                }
                else {
                    file << "Punto consultado encontrado satisfactoriamente" << endl;
                    file << "Su población es: " << pop << endl;
                }
                file << "Tiempo parcial en buscar punto: " << time << " [s] \n" << endl;

                start = std::chrono::system_clock::now();
                file << "Elementos verificados en la región completa: " << pr.pointsAtRegionDriver(Point(0, 0), 256, &pr) << endl;
                // Detiene el reloj
                end = std::chrono::system_clock::now();
                time = chrono::duration<double>(end - start).count();
                file << "Tiempo parcial en buscar ciudades insertadas en la región completa: " << time << " [s] \n" << endl;


                start = std::chrono::system_clock::now();
                file << "Población verificada en la región completa: " << pr.populationAtRegionDriver(Point(0, 0), 256, &pr) << endl;
                // Detiene el reloj
                end = std::chrono::system_clock::now();
                time = chrono::duration<double>(end - start).count();
                file << "Tiempo parcial en buscar población total insertada en la región completa: " << time << " [s] \n" << endl;

                getCoordinate(cities[50].getGeopoint(), &xCoord, &yCoord);
                file << "Eliminando punto: ( " << xCoord << "," << yCoord << ")" << endl;

                start = std::chrono::system_clock::now();
                pr.deletePointDriver(Point(xCoord, yCoord), &pr);
                // Detiene el reloj
                end = std::chrono::system_clock::now();
                time = chrono::duration<double>(end - start).count();
                file << "Tiempo parcial de eliminación: " << time << " [s] \n" << endl;


                file << "Elementos verificados: " << pr.pointsAtRegionDriver(Point(0, 0), 256, &pr) << endl;
                file << endl;
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
                pr = PRQuadTree(-256, 256, -256, 256);
                auto start = std::chrono::system_clock::now();
                for (int i = 0; i < 100000; i++)
                    pr.insert(cities[i], &pr);
                // Detiene el reloj
                auto end = std::chrono::system_clock::now();
                double time = chrono::duration<double>(end - start).count();
                timesTaken.push_back(time);
                file << "Tiempo parcial en insertar 100.000 elementos: " << time << "[s] \n";

                double xCoord;
                double yCoord;
                getCoordinate(cities[0].getGeopoint(), &xCoord, &yCoord);
                file << "Buscando punto: ( " << xCoord << "," << yCoord << ")" << endl;

                start = std::chrono::system_clock::now();
                double pop = pr.populationAtPoint(xCoord, yCoord, &pr);
                // Detiene el reloj
                end = std::chrono::system_clock::now();
                time = chrono::duration<double>(end - start).count();

                if (pop == -1) {
                    file << "El punto consultado no se encuentra ingresado" << endl;
                }
                else {
                    file << "Punto consultado encontrado satisfactoriamente" << endl;
                    file << "Su población es: " << pop << endl;
                }
                file << "Tiempo parcial en buscar punto: " << time << " [s] \n" << endl;

                start = std::chrono::system_clock::now();
                file << "Elementos verificados en la región completa: " << pr.pointsAtRegionDriver(Point(0, 0), 256, &pr) << endl;
                // Detiene el reloj
                end = std::chrono::system_clock::now();
                time = chrono::duration<double>(end - start).count();
                file << "Tiempo parcial en buscar ciudades insertadas en la región completa: " << time << " [s] \n" << endl;


                start = std::chrono::system_clock::now();
                file << "Población verificada en la región completa: " << pr.populationAtRegionDriver(Point(0, 0), 256, &pr) << endl;
                // Detiene el reloj
                end = std::chrono::system_clock::now();
                time = chrono::duration<double>(end - start).count();
                file << "Tiempo parcial en buscar población total insertada en la región completa: " << time << " [s] \n" << endl;

                getCoordinate(cities[50].getGeopoint(), &xCoord, &yCoord);
                file << "Eliminando punto: ( " << xCoord << "," << yCoord << ")" << endl;

                start = std::chrono::system_clock::now();
                pr.deletePointDriver(Point(xCoord, yCoord), &pr);
                // Detiene el reloj
                end = std::chrono::system_clock::now();
                time = chrono::duration<double>(end - start).count();
                file << "Tiempo parcial de eliminación: " << time << " [s] \n" << endl;


                file << "Elementos verificados: " << pr.pointsAtRegionDriver(Point(0, 0), 256, &pr) << endl;
                file << endl;
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
                pr = PRQuadTree(-256, 256, -256, 256);
                auto start = std::chrono::system_clock::now();
                for (int i = 0; i < 1000000; i++)
                    pr.insert(cities[i], &pr);
                // Detiene el reloj
                auto end = std::chrono::system_clock::now();
                double time = chrono::duration<double>(end - start).count();
                timesTaken.push_back(time);
                file << "Tiempo parcial en insertar 1.000.000 elementos: " << time << "[s] \n";

                double xCoord;
                double yCoord;
                getCoordinate(cities[0].getGeopoint(), &xCoord, &yCoord);
                file << "Buscando punto: ( " << xCoord << "," << yCoord << ")" << endl;

                start = std::chrono::system_clock::now();
                double pop = pr.populationAtPoint(xCoord, yCoord, &pr);
                // Detiene el reloj
                end = std::chrono::system_clock::now();
                time = chrono::duration<double>(end - start).count();

                if (pop == -1) {
                    file << "El punto consultado no se encuentra ingresado" << endl;
                }
                else {
                    file << "Punto consultado encontrado satisfactoriamente" << endl;
                    file << "Su población es: " << pop << endl;
                }
                file << "Tiempo parcial en buscar punto: " << time << " [s] \n" << endl;

                start = std::chrono::system_clock::now();
                file << "Elementos verificados en la región completa: " << pr.pointsAtRegionDriver(Point(0, 0), 256, &pr) << endl;
                // Detiene el reloj
                end = std::chrono::system_clock::now();
                time = chrono::duration<double>(end - start).count();
                file << "Tiempo parcial en buscar ciudades insertadas en la región completa: " << time << " [s] \n" << endl;


                start = std::chrono::system_clock::now();
                file << "Población verificada en la región completa: " << pr.populationAtRegionDriver(Point(0, 0), 256, &pr) << endl;
                // Detiene el reloj
                end = std::chrono::system_clock::now();
                time = chrono::duration<double>(end - start).count();
                file << "Tiempo parcial en buscar población total insertada en la región completa: " << time << " [s] \n" << endl;

                getCoordinate(cities[50].getGeopoint(), &xCoord, &yCoord);
                file << "Eliminando punto: ( " << xCoord << "," << yCoord << ")" << endl;

                start = std::chrono::system_clock::now();
                pr.deletePointDriver(Point(xCoord, yCoord), &pr);
                // Detiene el reloj
                end = std::chrono::system_clock::now();
                time = chrono::duration<double>(end - start).count();
                file << "Tiempo parcial de eliminación: " << time << " [s] \n" << endl;


                file << "Elementos verificados: " << pr.pointsAtRegionDriver(Point(0, 0), 256, &pr) << endl;
                file << endl;
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
                pr = PRQuadTree(-256, 256, -256, 256);
                auto start = std::chrono::system_clock::now();
                for (int i = 0; i < 3100000; i++)
                    pr.insert(cities[i], &pr);
                // Detiene el reloj
                auto end = std::chrono::system_clock::now();
                double time = chrono::duration<double>(end - start).count();
                timesTaken.push_back(time);
                file << "Tiempo parcial en insertar 3.100.000 elementos: " << time << "[s] \n";

                double xCoord;
                double yCoord;
                getCoordinate(cities[0].getGeopoint(), &xCoord, &yCoord);
                file << "Buscando punto: ( " << xCoord << "," << yCoord << ")" << endl;

                start = std::chrono::system_clock::now();
                double pop = pr.populationAtPoint(xCoord, yCoord, &pr);
                // Detiene el reloj
                end = std::chrono::system_clock::now();
                time = chrono::duration<double>(end - start).count();

                if (pop == -1) {
                    file << "El punto consultado no se encuentra ingresado" << endl;
                }
                else {
                    file << "Punto consultado encontrado satisfactoriamente" << endl;
                    file << "Su población es: " << pop << endl;
                }
                file << "Tiempo parcial en buscar punto: " << time << " [s] \n" << endl;

                start = std::chrono::system_clock::now();
                file << "Elementos verificados en la región completa: " << pr.pointsAtRegionDriver(Point(0, 0), 256, &pr) << endl;
                // Detiene el reloj
                end = std::chrono::system_clock::now();
                time = chrono::duration<double>(end - start).count();
                file << "Tiempo parcial en buscar ciudades insertadas en la región completa: " << time << " [s] \n" << endl;


                start = std::chrono::system_clock::now();
                file << "Población verificada en la región completa: " << pr.populationAtRegionDriver(Point(0, 0), 256, &pr) << endl;
                // Detiene el reloj
                end = std::chrono::system_clock::now();
                time = chrono::duration<double>(end - start).count();
                file << "Tiempo parcial en buscar población total insertada en la región completa: " << time << " [s] \n" << endl;

                getCoordinate(cities[50].getGeopoint(), &xCoord, &yCoord);
                file << "Eliminando punto: ( " << xCoord << "," << yCoord << ")" << endl;

                start = std::chrono::system_clock::now();
                pr.deletePointDriver(Point(xCoord, yCoord), &pr);
                // Detiene el reloj
                end = std::chrono::system_clock::now();
                time = chrono::duration<double>(end - start).count();
                file << "Tiempo parcial de eliminación: " << time << " [s] \n" << endl;


                file << "Elementos verificados: " << pr.pointsAtRegionDriver(Point(0, 0), 256, &pr) << endl;
                file << endl;
            }
            avg = 0;
            for (auto it: timesTaken) avg += it;
            avg /= numTests;
            file << "Tiempo promedio en insertar 3.100.000 elementos: " << avg << " [s] \n";
            timesTaken.clear();
            
        } // end if insert


        } // End tests

    // Cierra el archivo
    file.close();

}






int main() {
    vector<string> tests;
    tests.emplace_back("insert");
    run_tests(1, tests);


}
