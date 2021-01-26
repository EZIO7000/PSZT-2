#include <iostream>
#include "gen_algorithm.hpp"
#include <chrono>
#include <fstream>
#include <sstream>
#include "pugixml.hpp"

using namespace std;
using namespace pugi;

int main() {

    int populationSize = 500;
    int elemSize = 50;
    int mutationPropability = 30;   // w promilach
    int crossPropability = 80;      // w procentach
    int parm_t = 15;
    int gen_count = 500;
    std::string log;

    //////////////////////////////

    cout << "\nParsing .....\n\n";
    xml_document doc;

    if (!doc.load_file("bin/test.xml")) return -1;
    xml_node tools = doc.child("network").child("networkStructure").child("nodes");

    for (xml_node_iterator it = tools.begin(); it != tools.end(); ++it)
    {

        for (xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
        {
            cout << "Node " << ait->value();
            
        }

        xml_node node = it->child("coordinates");

        for (xml_node_iterator ait = node.begin(); ait != node.end(); ++ait)
        {
            
            cout << " " << ait->child_value();
        }

        cout << endl;
    }

    //////////////////////////////

    log += "Rozmiar populacji;Rozmiar genu;Prawdopodobienstwo mutacji;Prawdopodobienstwo krzyzowania;parametr t;ilosc generacji;Najlepsze rozwiazanie;Czas;Wartosc funkcji celu\n";

    for(int j = 0; j < 10; ++j ) {

        for(int i = 0; i < 35; ++i ) {

            std::cout << "j: " << j << "   i: " << i << std::endl;
    
            gen_algorithm algorithm( populationSize, mutationPropability, crossPropability, parm_t, elemSize, gen_count);

            auto start = std::chrono::system_clock::now();
            std::time_t time2 = std::chrono::system_clock::to_time_t( start );

            individual temp = algorithm.start();

            auto end = std::chrono::system_clock::now();
            std::time_t time1 = std::chrono::system_clock::to_time_t( end );

            auto seconds = time1 - time2;
            std::stringstream ss;
            ss << seconds;
            std::string ts = ss.str();
            
            log += std::to_string(populationSize);
            log += ";";
            log += std::to_string(elemSize);
            log += ";";
            log += std::to_string(mutationPropability);
            log += ";";
            log += std::to_string(crossPropability);
            log += ";";
            log += std::to_string(parm_t);
            log += ";";
            log += std::to_string(gen_count);
            log += ";";
            log += "'";
            log += temp;
            log += "'";
            log += ";";
            log += ts;
            log += ";";
            log += std::to_string(temp.get_fitness());
            log += "\n";
            
        }
        log += "\n";
        log += "\n";
    }

    std::fstream file;

    file.open( "./log.csv", std::ios::out );
    file << log;
    file.close();
}