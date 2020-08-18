#include <iostream>
#include "City.hpp"
#include "PRQuadTree.hpp"
#include <sys/stat.h>




/**
* Lee la base de datos provista de usuarios de twitter y la almacena en un vector usando la clase User
* @return: Retorna un vector con todos los usuarios leídos
*/
vector<City> readFile(){
    // Puntero a archivo
    ifstream fin;
    // Abre el archivo con la base de datos
    fin.open("/home/krampus/CLionProjects/ProyectoAlgoritmos/worldcitiespop_fixed.csv", ios::in);
    string line, word, temp;
    // Vector de strings para almacenar cada palabra
    vector<string> tempInfo;
    // Vector para almacenar todos los usuarios
    vector<City> cities;
    // Lee la primera línea, que son los nombres de las columnas. No es relevante, hace nada.
    getline(fin, line);
    // Lee hasta EOF
    /**
     * TEST, PARA EVITAR DEMORAS AL HACER PRUEBAS
     */
    while (getline(fin, line)){
        // Borra la info anterior que pudiera haber en el vector
        tempInfo.clear();
        // Crea un stringstream para cada palabra individual
        stringstream s(line);
        // Ahora almacena cada palabra de forma independiente, separadas por \t
        while(getline(s, word, '\t')){
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
    if (stat("/home/krampus/CLionProjects/ProyectoAlgoritmos/results_tests.txt", &buffer) == 0)
        remove("/home/krampus/CLionProjects/ProyectoAlgoritmos/results_tests.txt");
    // Stream para guardar los resultados
    ofstream file;
    // Abre el archivo
    file.open("/home/krampus/CLionProjects/ProyectoAlgoritmos/results_tests.txt");
    // Vector para almacenar los tiempos que tarda
    vector<double> timesTaken;
    // Comienza a leer los archivos
    cities = readFile();
    // Latitud va de -90 a 90 (eje y), longitud de -180 a 80 (eje x)
    PRQuadTree pr(-256, 256, -256, 256);
    // Itera para cada uno de los tests en el vector
    for (const auto& test: tests){
        if (test == "insert"){
            file << "**************** COMIENZA PRUEBA DE INSERCIÓN **************** \n";
            for (int numTest = 1; numTest <= numTests; numTest++){
                file << "**** Resultados parciales test ****" << numTest << "\n";
                // Inserta 100 elementos y comienza medición de tiempo
                auto start = std::chrono::system_clock::now();
                for (int i = 0; i < 1000; i++)
                    pr.insert(cities[i], &pr);
                // Detiene el reloj
                auto end = std::chrono::system_clock::now();
                double time = chrono::duration<double>(end - start).count();
                timesTaken.push_back(time);
                file << "Tiempo parcial en insertar 100 elementos: " << time << "[s] \n";
                // Repite insertando 10.000 elementos
//                pr = PRQuadTree(-180, 80, -90, 90);
//                start = std::chrono::system_clock::now();
//                for (int i = 0; i < 10000; i++)
//                    pr.insert(cities[i], &pr);
//                // Detiene el reloj
//                end = std::chrono::system_clock::now();
//                time = chrono::duration<double>(end - start).count();
//                timesTaken.push_back(time);
//                file << "Tiempo parcial en insertar 10.000 elementos: " << time << "[s] \n";
            }
        }
    }
    // Cierra el archivo
    file.close();


}



void getCoordinate2 (std::string s, double *x, double *y){
    stringstream linestream(s);
    string currentString;
    getline(linestream, currentString, ',');
    *x = stod(currentString);
    getline(linestream, currentString, ',');
    *y = stod(currentString);
}




int main() {
    vector<string> tests;
    tests.emplace_back("insert");
    run_tests(1, tests);


}
