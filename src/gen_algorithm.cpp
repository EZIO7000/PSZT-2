#include "gen_algorithm.hpp"

gen_algorithm::gen_algorithm() = default;

gen_algorithm::~gen_algorithm() = default;

gen_algorithm::gen_algorithm(unsigned p_size, float m_probability, float c_probability, unsigned t, unsigned length_of_vector, unsigned iter_count)
{

    population_size = p_size;
    mutation_probability = m_probability;
    cross_probability = c_probability;
    parm_t = t;
    lengthOfVector = length_of_vector;
    iteration_count = iter_count;
}

/*
*   Metoda wykonujaca krzyzowanie. Wybiera losowo ktore z pierwszych a elementow, potem kolejnych, bedzie pochodzic od ktorego rodzica.
*/
void gen_algorithm::crossMethod(int method_number, int changed_element_numberP, int crossed_element_numberP, std::vector<std::vector<unsigned>> &vec, bool isNotSingle)
{
    int a = generate_number() % lengthOfVector;

    std::vector<unsigned> tmp;
    tmp.reserve(lengthOfVector);
    std::vector<unsigned> tmp2;
    tmp2.reserve(lengthOfVector);
    std::vector<unsigned> changed_element_number = population[changed_element_numberP].get_gene();
    std::vector<unsigned> crossed_element_number = population[crossed_element_numberP].get_gene();

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
void gen_algorithm::cross()
{
    std::vector<std::vector<unsigned>> vec;
    vec.reserve(population.size());

    std::vector<int> first_half;
    std::vector<int> second_half;

    first_half.reserve(population.size() / 2 + 1);
    second_half.reserve(population.size() / 2);

    bool first = true;

    for (unsigned i = 0; i < population.size(); i++)
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
            vec.push_back(population[i].get_gene());
        }
    }

    if (second_half.size() == 0)                            // gdy prawdopodobienstwo jest tak male, ze zostal wylosowany tylko jeden osobnik do 
    {                                                       // skrzyzowania, jest on skrzyzowany z losowym innym osobnikiem, natomiast ten drugi
        int i = generate_number() % population.size();      // nie zostaje nadpisany przez dziecko
        while (i == 0)
        {
            i = generate_number() % population.size();
        }
        crossMethod(generate_number() % 2, first_half[0], i, vec, false);
    }
    else
    {
        for (unsigned i = 0; i < first_half.size(); i++)
        {
            if (second_half.size() == 0)
            {
                int i = generate_number() % population.size();
                while (i == first_half[0])  
                {
                    i = generate_number() % population.size();
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
        population[i].set_gene(vec[i]);
    }
}
/*
*   pierwsza wersja mutacji. Przechodzi po kazdym bicie kazdego elementu i wylicza prawdopodobienstwo
*/
void gen_algorithm::mutate_long()
{
    for(unsigned i = 0; i < population.size(); i++)
    {
        std::vector<unsigned> tmp = population[i].get_gene();
        for (unsigned x = 0; x < lengthOfVector; x++)
        {
            if (generate_number() % 100 < mutation_probability)
            {
                tmp[x] = (tmp[x] + 1) % 2;
            }
        }
        population[i].set_gene(tmp);
    }
}

void gen_algorithm::mutate()
{
    int numberOfElementsToChange = mutation_probability * lengthOfVector * population.size() / 1000;    // liczenie ilosci bitow do zmiany
    std::vector<std::vector<bool>> tmpCheckVec;
    std::vector<bool> tmpCheck;

    for (unsigned i = 0; i < population_size; i++)      // tworzenie tablicy przechowywującej zmnienione bity
    {
        for (unsigned j = 0; j < lengthOfVector; j++)
        {
            tmpCheck.push_back(false);
        }
        tmpCheckVec.push_back(tmpCheck);
    }

    for (int i = 0; i < numberOfElementsToChange; i++)      // zmiana kolejnych bitow
    {
        int x = generate_number() % population.size();
        int y = generate_number() % lengthOfVector;

        while (tmpCheckVec[x][y] == true)                   // sprawdzenie czy bit zostal juz zmieniony i ewentualne wylosowanie nowego
        {
            x = generate_number() % population.size();
            y = generate_number() % lengthOfVector;
        }
        tmpCheckVec[x][y] = true;

        std::vector<unsigned> tmpVec = population[x].get_gene();
        tmpVec[y] = (tmpVec[y] + 1) % 2;
        population[x].set_gene(tmpVec);
    }
}

void gen_algorithm::gen_function()
{
}

void gen_algorithm::initPopulation()
{
    for (unsigned i = 0; i < population_size; i++)
    {
        std::vector<unsigned> tmp;
        for (unsigned x = 0; x < lengthOfVector; x++)
        {
            tmp.push_back(generate_number() % 2);
        }
        individual ind;
        ind.set_gene(tmp);
        population.push_back(ind);
    }
}

unsigned gen_algorithm::generate_number()
{

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    return generator();
}

void gen_algorithm::fintess_calc() {

    unsigned ones_score = 0;
    unsigned zeros_score = 0;
    unsigned score = 0;
    std::vector<unsigned> a;

    for (unsigned i = 0; i < lengthOfVector; ++i)
        a.push_back(0);

    l_best_so_far.set_fitness(0);
    r_best_so_far.set_fitness(0);
    l_best_so_far.set_gene(a);
    r_best_so_far.set_gene(a);

    for (auto &i : population) {

        ones_score = 0;
        zeros_score = 0;
        score = 0;

        for (auto x = i.get_gene().begin(); x != i.get_gene().end(); ++x) { //
                                                                            //
            if (*x == 0)                                                    //
                break;                                                      //
            else                                                            //
                ++ones_score;                                               //
        }                                                                   // zliczanie jedynek od początku genu.

        for (auto j = i.get_gene().rbegin(); j != i.get_gene().rend(); ++j) {   //
                                                                                //
            if (*j == 1)                                                        //
                break;                                                          //
            else                                                                //
                ++zeros_score;                                                  //
        }                                                                       // zliczanie zer od końca genu.

        if (zeros_score >= parm_t && ones_score >= parm_t)  //
            score += 100;                                   // wartość super.

        score += std::max(ones_score, zeros_score); // dodajemy maksymalną wartośc pochodzącą od zer lub jedynek.

        i.set_fitness(score);   // ustawiamy funkcję celu dla danego osobnika.

        if (l_best_so_far < i && ones_score > zeros_score)  //
            l_best_so_far = i;                              //
        if (r_best_so_far < i && zeros_score > ones_score)  //
            r_best_so_far = i;                              //
        if (best_so_far < i)                                //
            best_so_far = i;                                // funkcje potrzebne do realizacji wyświetlania najlepszych osobników z zerami i jedynkami dla danej generacji.
    }
}

void gen_algorithm::selection() {

    long unsigned number;
    long unsigned sum;
    int count;
    long unsigned fitness_sum = 0;
    std::vector<individual> temp;

    for (auto i : population)               //
        fitness_sum += i.get_fitness();     // liczymy sumę wartości funkcji celu całej populacji.

    for (unsigned x = 0; x < population_size; ++x) { // powtarzamy tyle razy ile wynosi wielkość populacji.

        number = generate_number() % fitness_sum; // losujemy liczbę z przedziału < 0, suma funkcji celu >
        sum = 0;
        count = 0;

        for (auto &i : population) { // iterujemy po elementach populacji
        
            if ((sum += i.get_fitness()) > number)  {//jeśli dotychczasowa suma funkcji celu jest większa niż zadana liczba
    
                temp.push_back(population[count]);  // wybierz osobnika
                break;
            }
            count++;
        }
    }

    population.clear();

    for (auto &i : temp)            //
        population.push_back(i);    // ładujemy nową populację. 
}

void gen_algorithm::selection_tournament() {

    std::vector<individual> temp;

    for( unsigned x = 0; x < population_size; ++x ) { // powtarzamy tyle razy ile wynosi wielkość populacji.

        unsigned temp1 = generate_number() % population_size; //
        unsigned temp2 = generate_number() % population_size; // losujemy 2 osobników z populacji.

        if( population[temp1] < population[temp2] ) // 
            temp.push_back(population[temp2]);      //
        else                                        //
            temp.push_back(population[temp1]);      // wybieramy lepszego.
    }

    population.clear(); // czyścimy dotychczasową populację.

    for( auto & i : temp )          //
        population.push_back(i);    // ładujemy nową populację.                              
}

individual gen_algorithm::start() {

    initPopulation();
    fintess_calc();
    for (unsigned i = 0; i < iteration_count; ++i)
    {
        selection();
        cross();
        mutate();
        fintess_calc();
        //std::cout << "lewy: " << l_best_so_far;
        //std::cout << "prawy: " << r_best_so_far;
        //std::cout << best_so_far;
    }
    return best_so_far;
}