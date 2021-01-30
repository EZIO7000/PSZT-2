#include "gen_algorithm.hpp"

gen_algorithm::gen_algorithm() = default;

gen_algorithm::~gen_algorithm() = default;

gen_algorithm::gen_algorithm(unsigned p_size, float m_probability, float c_probability, unsigned mod, unsigned path_count, unsigned iter_count)
{

    population_size = p_size;
    mutation_probability = m_probability;
    cross_probability = c_probability;
    lengthOfVector = 66; //length_of_vector;
    path_count_global = path_count;
    iteration_count = iter_count;
    modularity = mod;
    load_data();
}

/*
*   Metoda wykonujaca krzyzowanie. Wybiera losowo ktore z pierwszych a elementow, potem kolejnych, bedzie pochodzic od ktorego rodzica.
*/

void gen_algorithm::crossMethod(int method_number, int changed_element_numberP, int crossed_element_numberP, std::vector<std::vector<individual>> &vec, bool isNotSingle)
{
    int a = generate_number() % lengthOfVector;

    std::vector<individual> tmp;
    tmp.reserve(lengthOfVector);
    std::vector<individual> tmp2;
    tmp2.reserve(lengthOfVector);
    std::vector<individual> changed_element_number = population_chromosome[changed_element_numberP].get_chromosome();
    std::vector<individual> crossed_element_number = population_chromosome[crossed_element_numberP].get_chromosome();

    switch (method_number)
    {
    case 0: // Ab

        for (int i = 0; i < a; i++)
        {
            tmp.push_back(crossed_element_number[i]);
            if (isNotSingle)
                tmp2.push_back(crossed_element_number[i]);
        }
        for (unsigned i = a; i < lengthOfVector; i++)
        {
            tmp.push_back(changed_element_number[i]);
            if (isNotSingle)
                tmp2.push_back(changed_element_number[i]);
        }
        break;

    case 1: // aB

        for (int i = 0; i < a; i++)
        {
            tmp.push_back(changed_element_number[i]);
            if (isNotSingle)
                tmp2.push_back(changed_element_number[i]);
        }
        for (unsigned i = a; i < lengthOfVector; i++)
        {
            tmp.push_back(crossed_element_number[i]);
            if (isNotSingle)
                tmp2.push_back(crossed_element_number[i]);
        }
        break;

    default:
        break;
    }

    vec.push_back(tmp);
    if (isNotSingle)
        vec.push_back(tmp2);
}

/*
*   Krzyzowanie jendopunktowe. Na podstawie prawdopodobienstwa tworzone sa 2 grupy, nastepnie elementy z grupy pierwszej lacza
*   sie z elementami z grupy drugiej. Z dwoch rodzicow powstaje dwojka dzieci. 
*/

std::vector<Chromosome> gen_algorithm::crossChromosome(std::vector<Chromosome> pop)
{
    std::vector<std::vector<individual>> vec;
    vec.reserve(pop.size());

    std::vector<int> first_half;
    std::vector<int> second_half;

    first_half.reserve(pop.size() / 2 + 1);
    second_half.reserve(pop.size() / 2);

    bool first = true;

    for (unsigned i = 0; i < pop.size(); i++)
    {
        if (generate_number() % 100 < cross_probability)
        {
            if (first)
            {
                first_half.push_back(i);
                first = false;
            }
            else
            {
                second_half.push_back(i);
                first = true;
            }
        }
        else
        {
            vec.push_back(pop[i].get_chromosome());
        }
    }

    if (second_half.size() == 0)                            // gdy prawdopodobienstwo jest tak male, ze zostal wylosowany tylko jeden osobnik do 
    {                                                       // skrzyzowania, jest on skrzyzowany z losowym innym osobnikiem, natomiast ten drugi
        int i = generate_number() % pop.size();      // nie zostaje nadpisany przez dziecko
        while (i == 0)
        {
            i = generate_number() % pop.size();
        }
        crossMethod(generate_number() % 2, first_half[0], i, vec, false);
    }
    else
    {
        for (unsigned i = 0; i < first_half.size(); i++)
        {
            if (second_half.size() == 0)
            {
                int i = generate_number() % pop.size();
                while (i == first_half[0])  
                {
                    i = generate_number() % pop.size();
                }
                crossMethod(generate_number() % 2, first_half[0], i, vec, false);
            }
            else
            {
                unsigned x = generate_number() % second_half.size();

                crossMethod(generate_number() % 2, first_half[i], second_half[x], vec, true);

                second_half.erase(second_half.begin() + x);
            }
        }
    }

    for (unsigned i = 0; i < vec.size(); i++)
    {
        pop[i].set_chromosome(vec[i]);
    }

    return pop;
}

std::vector<Chromosome> gen_algorithm::mutateChromosomes(std::vector<Chromosome> pop)
{
    long int numberOfAllElements = pop.size() * pop[0].get_chromosome().size(); //*
                                     //population_chromosome[0].get_chromosome()[0].get_gene().size();

    long int numberOfElementsToMutate = numberOfAllElements * mutation_probability / 1000;

    for(long int i = 0; i < numberOfElementsToMutate; i++)
    {
        int x = generate_number() % pop.size();
        int y = generate_number() % pop[0].get_chromosome().size();

        std::vector<unsigned> tmpVec = pop[x].get_chromosome()[y].get_gene();

        int paths = pop[x].get_chromosome()[y].get_gene().size();

        std::vector<int> tmp;

        tmp.push_back(0);
        for(int a = 1; a < paths; a++)
        {
            tmp.push_back(generate_number() % 101);
        }
        tmp.push_back(100);
        
        std::sort(tmp.begin(), tmp.end());

        for(int a = 0; a < paths; a++)
        {
            tmpVec[a] = tmp[a+1] - tmp[a];
        }

        pop[x].set_gene(y, tmpVec);
    }

    return pop;
}

std::vector<Chromosome> gen_algorithm::mutateChromosomesOnePath(std::vector<Chromosome> pop)
{
    long int numberOfAllElements = pop.size() * pop[0].get_chromosome().size(); //*
                                     //population_chromosome[0].get_chromosome()[0].get_gene().size();

    long int numberOfElementsToMutate = numberOfAllElements * mutation_probability / 1000;

    for(long int i = 0; i < numberOfElementsToMutate; i++)
    {
        int x = generate_number() % pop.size();
        int y = generate_number() % pop[0].get_chromosome().size();

        std::vector<unsigned> tmpVec = pop[x].get_chromosome()[y].get_gene();

        int paths = pop[x].get_chromosome()[y].get_gene().size();
        for(int a = 0; a < paths; a++)
        {
            tmpVec[a] = 0;
        }
        tmpVec[generate_number() % paths] = 100;
        pop[x].set_gene(y, tmpVec);

    }
    return pop;
}

void gen_algorithm::init_population_chromosome_one_path(unsigned path_count){

        for (unsigned i = 0; i < population_size; i++) {
        
        std::vector<individual> temp;

        for(unsigned j = 0; j < 66; ++j) {

            std::vector<unsigned> tmp, tmp2;

            tmp2.resize(path_count);

            for(unsigned a = 0; a < path_count; a++)
            {
                tmp2[a] = 0;
            }

            tmp2[generate_number() % path_count] = 100;

            individual ind;
            ind.set_gene(tmp2);
            temp.push_back(ind);
        }

        Chromosome ch;
        ch.set_chromosome(temp);
        population_chromosome.push_back(ch);
    }
}

void gen_algorithm::init_population_chromosome(unsigned path_count) {
    
    for (unsigned i = 0; i < population_size; i++) {
        
        std::vector<individual> temp;

        for(unsigned j = 0; j < 66; ++j) {

            std::vector<unsigned> tmp, tmp2;

            tmp2.resize(path_count);
            tmp.push_back(0);

            for(unsigned a = 1; a < path_count; a++) {
                
                tmp.push_back(generate_number() % 101);
            }

            tmp.push_back(100);
        
            std::sort(tmp.begin(), tmp.end());

            for(unsigned a = 0; a < path_count; a++) {
                tmp2[a] = tmp[a+1] - tmp[a];
            }

            individual ind;
            ind.set_gene(tmp2);
            temp.push_back(ind);
        }

        Chromosome ch;
        ch.set_chromosome(temp);
        population_chromosome.push_back(ch);
    }
}

unsigned gen_algorithm::generate_number()
{

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    return generator();
}

void gen_algorithm::fintess_calc_chromosome(unsigned path_count, unsigned modularity, std::vector<Chromosome> &p) {
    
    unsigned module_count;

    for(auto &ch : p) {

        module_count = 0;

        for(int i = 0; i < 12; i++)
            for(int j = 0; j < 12; j++)
                edges[i][j] = 0;

        for(int i = 0; i < 66; i++)
            for(unsigned j = 0; j < path_count; j++)
                for(auto &link : demand_paths[i][j])
                    edges[link[0]][link[1]] += ch.get_chromosome()[i].get_gene()[j] * demands[i];
        
        for(int i = 0; i < 12; i++)
            for(int j = 0; j < 12; j++)
                module_count += (unsigned)std::ceil((double)edges[i][j] / (modularity * 100));

        ch.set_fitness(module_count);
        if(best_so_far.get_fitness() > ch.get_fitness()) {

            best_so_far = ch;
            for(int i = 0; i < 12; i++)
                for(int j = 0; j < 12; j++)
                    best_edges[i][j] = edges[i][j];
        }
    }
}

std::vector<Chromosome> gen_algorithm::selection_tournament_chromosome(std::vector<Chromosome> p) {

    std::vector<Chromosome> temp;

    for(unsigned x = 0; x < population_size; ++x) { // powtarzamy tyle razy ile wynosi wielkość populacji.

        unsigned temp1 = generate_number() % population_size; //
        unsigned temp2 = generate_number() % population_size; // losujemy 2 osobników z populacji.

        if( p[temp1] < p[temp2] ) // 
            temp.push_back(p[temp2]);      //
        else                                        //
            temp.push_back(p[temp1]);      // wybieramy lepszego.
    }

    return temp;                            
}

void gen_algorithm::succession_chromosome(std::vector<Chromosome> p, unsigned k) {

    std::sort(population_chromosome.begin(), population_chromosome.end());
    std::reverse(population_chromosome.begin(), population_chromosome.end());
    std::sort(p.begin(), p.end());

    for(unsigned i = 0; i < population_size - k; ++i) {

        population_chromosome.pop_back();
    }
    
    for(unsigned i = 0; i < population_size - k; ++i) {

        population_chromosome.push_back(p.back());
        p.pop_back();
    }

}

Chromosome gen_algorithm::startFullDezagregation() {

    init_population_chromosome(path_count_global);
    fintess_calc_chromosome(path_count_global, modularity, population_chromosome);
    for (unsigned i = 0; i < iteration_count; ++i)
    {
        std::vector<Chromosome> new_population = selection_tournament_chromosome(population_chromosome);
        new_population = crossChromosome(new_population);
        new_population = mutateChromosomes(new_population);
        fintess_calc_chromosome(path_count_global, modularity, new_population);
        succession_chromosome(new_population, population_size*0.4);
        show_edges();
        std::cout << "Numer generacji: " << i << std::endl;
    }
    return best_so_far;
}

Chromosome gen_algorithm::startFullAgregation() {

    init_population_chromosome_one_path(path_count_global);
    fintess_calc_chromosome(path_count_global, modularity, population_chromosome);
    for (unsigned i = 0; i < iteration_count; ++i)
    {
        std::vector<Chromosome> new_population = selection_tournament_chromosome(population_chromosome);
        new_population = crossChromosome(new_population);
        new_population = mutateChromosomesOnePath(new_population);
        fintess_calc_chromosome(path_count_global, modularity, new_population);
        succession_chromosome(new_population, population_size*0.4);
        show_edges();
        std::cout << "Numer generacji: " << i << std::endl;
    }
    return best_so_far;
}

void gen_algorithm::load_data() {

    pugi::xml_document doc;
    doc.load_file("bin/test.xml");
    pugi::xml_node tools = doc.child("network").child("demands");

    for (pugi::xml_node_iterator it = tools.begin(); it != tools.end(); ++it) {
        
        demands.push_back(std::stoi(it->child("demandValue").child_value()));
        pugi::xml_node path = it->child("admissiblePaths");
        std::vector<std::vector<std::vector<unsigned>>> tmp_path;

        for (pugi::xml_node_iterator pit = path.begin(); pit != path.end(); ++pit) {
            
            std::vector<std::vector<unsigned>> tmp_links;

            for(pugi::xml_node_iterator lit = pit->children().begin(); lit != pit->children().end(); ++lit) {

                unsigned a;
                unsigned b;
                bool pierwsze = true;
                std::string a_char;
                std::string b_char;
                int pomin = 5;
                std::string tmp_string = lit->child_value();

                for(auto &it : tmp_string) {
                    
                    if(pomin <= 0) {
                        
                        if(it == '_')
                            pierwsze = false;

                        if(it != '_') {
                            
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

                if(a < b) {
                    
                    tmp_link.push_back(a);
                    tmp_link.push_back(b);
                }

                else {
                    
                    tmp_link.push_back(b);
                    tmp_link.push_back(a);
                }

                tmp_links.push_back(tmp_link);
            }
            tmp_path.push_back(tmp_links);
        }
        demand_paths.push_back(tmp_path);
    }
}

void gen_algorithm::show_edges()
{
    std::cout << "\x1B[2J\x1B[H";
    std::cout << "Krakow - Warsaw:       " << (unsigned)std::ceil((double)best_edges[4][10] / (modularity * 100)) << "  " << (unsigned)std::ceil((double)(best_edges[4][10]%(modularity * 100)/100)) << "/" << modularity << std::endl;
    std::cout << "Krakow - Rzeszow:      " << (unsigned)std::ceil((double)best_edges[4][8] / (modularity * 100)) << "  " << (unsigned)std::ceil((double)(best_edges[4][8]%(modularity * 100)/100)) << "/" << modularity << std::endl;
    std::cout << "Bialystok - Rzeszow:   " << (unsigned)std::ceil((double)best_edges[5][8] / (modularity * 100)) << "  " << (unsigned)std::ceil((double)(best_edges[5][8]%(modularity * 100)/100)) << "/" << modularity << std::endl;
    std::cout << "Gdansk - Bialystok:    " << (unsigned)std::ceil((double)best_edges[0][5] / (modularity * 100)) << "  " << (unsigned)std::ceil((double)(best_edges[0][5]%(modularity * 100)/100)) << "/" << modularity << std::endl;
    std::cout << "Gdansk - Kolobrzeg:    " << (unsigned)std::ceil((double)best_edges[0][2] / (modularity * 100)) << "  " << (unsigned)std::ceil((double)(best_edges[0][2]%(modularity * 100)/100)) << "/" << modularity << std::endl;
    std::cout << "Kolobrzeg - Szczecin:  " << (unsigned)std::ceil((double)best_edges[2][9] / (modularity * 100)) << "  " << (unsigned)std::ceil((double)(best_edges[2][9]%(modularity * 100)/100)) << "/" << modularity << std::endl;
    std::cout << "Poznan - Szczecin:     " << (unsigned)std::ceil((double)best_edges[7][9] / (modularity * 100)) << "  " << (unsigned)std::ceil((double)(best_edges[7][9]%(modularity * 100)/100)) << "/" << modularity << std::endl;
    std::cout << "Poznan - Wroclaw:      " << (unsigned)std::ceil((double)best_edges[7][11] / (modularity * 100)) << "  " << (unsigned)std::ceil((double)(best_edges[7][11]%(modularity * 100)/100)) << "/" << modularity << std::endl;
    std::cout << "Lodz - Warsaw:         " << (unsigned)std::ceil((double)best_edges[6][10] / (modularity * 100)) << "  " << (unsigned)std::ceil((double)(best_edges[6][10]%(modularity * 100)/100)) << "/" << modularity << std::endl;
    std::cout << "Katowice - Lodz:       " << (unsigned)std::ceil((double)best_edges[3][6] / (modularity * 100))<< "  " << (unsigned)std::ceil((double)(best_edges[3][6]%(modularity * 100)/100)) << "/" << modularity << std::endl;
    std::cout << "Katowice - Wroclaw:    " << (unsigned)std::ceil((double)best_edges[3][11] / (modularity * 100)) << "  " << (unsigned)std::ceil((double)(best_edges[3][11]%(modularity * 100)/100)) << "/" << modularity << std::endl;
    std::cout << "Bydgoszcz - Warsaw:    " << (unsigned)std::ceil((double)best_edges[1][10] / (modularity * 100)) << "  " << (unsigned)std::ceil((double)(best_edges[1][10]%(modularity * 100)/100)) << "/" << modularity << std::endl;
    std::cout << "Bydgoszcz - Poznan:    " << (unsigned)std::ceil((double)best_edges[1][7] / (modularity * 100)) << "  " << (unsigned)std::ceil((double)(best_edges[1][7]%(modularity * 100)/100)) << "/" << modularity << std::endl;
    std::cout << "Katowice - Krakow:     " << (unsigned)std::ceil((double)best_edges[3][4] / (modularity * 100)) << "  " << (unsigned)std::ceil((double)(best_edges[3][4]%(modularity * 100)/100)) << "/" << modularity << std::endl;
    std::cout << "Lodz - Wroclaw:        " << (unsigned)std::ceil((double)best_edges[6][11] / (modularity * 100)) << "  " << (unsigned)std::ceil((double)(best_edges[6][11]%(modularity * 100)/100)) << "/" << modularity << std::endl;
    std::cout << "Bialystok - Warsaw:    " << (unsigned)std::ceil((double)best_edges[5][10] / (modularity * 100)) << "  " << (unsigned)std::ceil((double)(best_edges[5][10]%(modularity * 100)/100)) << "/" << modularity << std::endl;
    std::cout << "Gdansk - Warsaw:       " << (unsigned)std::ceil((double)best_edges[0][10] / (modularity * 100)) << "  " << (unsigned)std::ceil((double)(best_edges[0][10]%(modularity * 100)/100)) << "/" << modularity << std::endl;
    std::cout << "Bydgoszcz - Kolobrzeg: " << (unsigned)std::ceil((double)best_edges[1][2] / (modularity * 100)) << "  " << (unsigned)std::ceil((double)(best_edges[1][2]%(modularity * 100)/100)) << "/" << modularity << std::endl;
    std::cout << "Najlepszy wynik:       " << best_so_far.get_fitness() << std::endl;
}