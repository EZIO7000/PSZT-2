#include <iostream>
#include "gen_algorithm.hpp"
#include <chrono>
#include <fstream>
#include <sstream>
#include "pugixml.hpp"
#include "string"

using namespace std;
using namespace pugi;

int main() {

    int populationSize = 200;
    int pathCount = 3;
    int mutationPropability = 50;   // w promilach
    int crossPropability = 100;      // w procentach
    int modularity = 10;
    int gen_count = 200;
    std::string log;
    
    log += "rozmiar_populacji,ilosc_scezek,prawdopodobienstwo_mutacji,prawdopodobienstwo_krzyzowania,modularnosc,ilosc generacji,";
    log += "kr_wa_modules,kr_wa_filling,kr_rz_modules,kr_rz_filling,bi_rz_modules,bi_rz_filling,gd_bi_modules,gd_bi_filling,";
    log += "gd_ko_modules,gd_ko_filling,ko_sz_modules,ko_sz_filling,po_sz_modules,po_sz_filling,po_wr_modules,po_wr_filling,";
    log += "lo_wa_modules,lo_wa_filling,ka_lo_modules,ka_lo_filling,ka_wr_modules,ka_wr_filling,by_wa_modules,by_wa_filling,";
    log += "by_po_modules,by_po_filling,ka_kr_modules,ka_kr_filling,lo_wr_modules,lo_wr_filling,bi_wa_modules,bi_wa_filling,";
    log += "gd_wa_modules,gd_wa_filling,by_ko_modules,by_ko_filling,czas,wartosc_funkcji_celu\n";

    for(int j = 0; j < 11; ++j ) {

        for(int i = 0; i < 10; ++i ) {

            //std::cout << "j: " << j << "   i: " << i << std::endl;
    
            gen_algorithm algorithm( populationSize, mutationPropability, crossPropability, modularity, pathCount, gen_count);

            auto start = std::chrono::system_clock::now();
            std::time_t time2 = std::chrono::system_clock::to_time_t( start );

            Chromosome temp = algorithm.startFullAgregation();
            //Chromosome temp = algorithm.startFullDezagregation();

            auto end = std::chrono::system_clock::now();
            std::time_t time1 = std::chrono::system_clock::to_time_t( end );

            auto seconds = time1 - time2;
            std::stringstream ss;
            ss << seconds;
            std::string ts = ss.str();
            
            log += std::to_string(populationSize);
            log += ",";
            log += std::to_string(pathCount);
            log += ",";
            log += std::to_string(mutationPropability);
            log += ",";
            log += std::to_string(crossPropability);
            log += ",";
            log += std::to_string(modularity);
            log += ",";
            log += std::to_string(gen_count);
            log += ",";
            log += std::to_string(algorithm.getBestEdgeModules(4,10));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeFilling(4,10));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeModules(4,8));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeFilling(4,8));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeModules(5,8));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeFilling(5,8));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeModules(0,5));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeFilling(0,5));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeModules(0,2));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeFilling(0,2));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeModules(2,9));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeFilling(2,9));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeModules(7,9));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeFilling(7,9));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeModules(7,11));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeFilling(7,11));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeModules(6,10));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeFilling(6,10));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeModules(3,6));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeFilling(3,6));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeModules(3,11));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeFilling(3,11));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeModules(1,10));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeFilling(1,10));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeModules(1,7));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeFilling(1,7));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeModules(3,4));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeFilling(3,4));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeModules(6,11));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeFilling(6,11));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeModules(5,10));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeFilling(5,10));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeModules(0,10));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeFilling(0,10));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeModules(1,2));
            log += ",";
            log += std::to_string(algorithm.getBestEdgeFilling(1,2));
            log += ",";
            log += ts;
            log += ",";
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