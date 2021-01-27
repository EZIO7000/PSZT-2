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

    int populationSize = 500;
    int elemSize = 50;
    int mutationPropability = 30;   // w promilach
    int crossPropability = 80;      // w procentach
    int parm_t = 15;
    int gen_count = 500;
    std::string log;

    std::vector<std::vector<std::vector<std::vector<unsigned>>>> tab;
    std::vector<unsigned> demands;

    //////////////////////////////

    std::cout << "\nParsing .....\n\n";
    xml_document doc;

    if (!doc.load_file("bin/test.xml")) return -1;
    xml_node tools = doc.child("network").child("demands");

    std::cout << " Hej";

    int element_counter = 0;
    int path_counter = 0;
    int link_counter = 0;
    for (xml_node_iterator it = tools.begin(); it != tools.end(); ++it)
    {

        for (xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
        {
            std::cout << "Node " << ait->value();   
        }

        // do uzupelnienia tablicy demands
        std::cout << " " << it->child("demandValue").child_value();

        // nie bedzie dzialac
        demands.push_back(std::stoi(it->child("demandValue").child_value()));
        //std::cout << "{{{" << demands[element_counter] << "}}}";

        xml_node path = it->child("admissiblePaths");

        //xml_node_iterator path_it = it->child("admissiblePaths");
        std::cout << "         ";

        std::vector<std::vector<std::vector<unsigned>>> tmp_path;
        for (xml_node_iterator pit = path.begin(); pit != path.end(); ++pit)
        {
            //xml_node link = pit->child("admissiblePath");

            
            std::vector<std::vector<unsigned>> tmp_links;
            for(xml_node_iterator lit = pit->children().begin(); lit != pit->children().end(); ++lit)
            {
                //std::cout << " " << lit->child_value(); 

                // zapisywanie do tablicy 2 elementowej
                unsigned a;
                unsigned b;
                bool pierwsze = true;
                bool drugie = true;

                string a_char;
                string b_char;

                int pomin = 5;

                string tmp_string = lit->child_value();
                for(auto &it : tmp_string)
                {
                    if(pomin <= 0)
                    {
                        if(it == '_')
                            pierwsze = false;

                        if(it != '_')
                        {
                            if(pierwsze)
                                a_char += it;
                            else
                                b_char += it;
                        }
                    }
                    pomin--;
                }

                a = std::stoi(a_char);
                b = std::stoi(b_char);

                std::vector<unsigned> tmp_link;

                if(a < b)
                {
                    tmp_link.push_back(a);
                    tmp_link.push_back(b);
                }
                else
                {
                    tmp_link.push_back(b);
                    tmp_link.push_back(a);
                }

                tmp_links.push_back(tmp_link);
            }

            //std::cout << "   ";
            tmp_path.push_back(tmp_links);
        }

        std::cout << endl;
        tab.push_back(tmp_path);
    }

    //std::cout << " Hej";// << tab[0][0][0][0];

    std::cout << tab.size() << " " << tab[0].size() << " " << tab[0][0].size() << " " << tab[0][0][0].size() << std::endl;



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