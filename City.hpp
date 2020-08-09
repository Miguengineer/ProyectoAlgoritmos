
#ifndef PROYECTOALGORITMOS_CITY_HPP
#define PROYECTOALGORITMOS_CITY_HPP
#include <bits/stdc++.h>

using namespace std;

class City {
private:
    string country;
    string city;
    string accentCity;
    string region;
    string population;
    string latitude;
    string longitude;
    string geopoint;
public:
    City()= default;
    City(string cntry, string cty, string accent, string rgn, string pop, string lat, string longi, string geo){
        this->country = cntry;
        this->city = cty;
        this->accentCity = accent;
        this->region = rgn;
        this->population = pop;
        this->latitude = lat;
        this->longitude = longi;
        this->geopoint = geo;
    }

    const string &getCountry() const {
        return country;
    }

    const string &getCity() const {
        return city;
    }

    const string &getAccentCity() const {
        return accentCity;
    }

    const string &getRegion() const {
        return region;
    }

    const string &getPopulation() const {
        return population;
    }

    const string &getLatitude() const {
        return latitude;
    }

    const string &getLongitude() const {
        return longitude;
    }

    const string &getGeopoint() const {
        return geopoint;
    }
};


#endif //PROYECTOALGORITMOS_CITY_HPP
