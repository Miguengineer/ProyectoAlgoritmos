#include <iostream>
#include "City.hpp"

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


int main() {
    vector <City> cities = readFile();
    cout << cities[0].getRegion() << endl;
    cout << cities[1].getRegion() << endl;
}
