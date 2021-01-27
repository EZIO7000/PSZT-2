#pragma once
#include <vector>
#include <iostream>
#include "individual.hpp"

class Chromosome {

    std::vector<individual> chromosome;
    unsigned fitness;

public:
    Chromosome();
    ~Chromosome();
    Chromosome(const Chromosome & other);
    Chromosome & operator=(const Chromosome & other);
    bool operator< ( Chromosome const & other) const;
    //std::vector<unsigned>  const & get_gene(unsigned i) const;
    std::vector<individual>  const & get_chromosome() const;
    //std::vector<unsigned>  const & get_gene(int i) const;
    unsigned get_fitness() const;
    void set_fitness( unsigned f );
    void set_chromosome( std::vector<individual> & c );
    friend std::ostream & operator<< ( std::ostream & s, const Chromosome & in );
    //friend std::string & operator+=( std::string & s, const Chromosome & in );
};