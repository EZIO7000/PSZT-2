#include "Chromosome.hpp"


Chromosome::Chromosome() {
    
    fitness = std::numeric_limits<unsigned>::max();
}

Chromosome::~Chromosome() {

}

Chromosome::Chromosome(const Chromosome & other) {

    this->fitness = other.fitness;
    this->chromosome = other.chromosome;
}

Chromosome & Chromosome::operator=(const Chromosome & other) {

    if(&other == this)
        return *this;

    this->fitness = other.fitness;
    this->chromosome = other.chromosome;

    return *this;
}

bool Chromosome::operator< ( Chromosome const & other) const {

    return this->fitness > other.fitness;
}

std::vector<individual> const & Chromosome::get_chromosome() const {

    return this->chromosome;
}


unsigned Chromosome::get_fitness() const {

    return this->fitness;
}

void Chromosome::set_fitness( unsigned f ) {

    this->fitness = f;
}

void Chromosome::set_chromosome( std::vector<individual> & g ) {

    this->chromosome = g;
}

 void Chromosome::set_gene(int i, std::vector<unsigned> & g){

     this->chromosome[i].set_gene(g);
 }

std::ostream & operator<< ( std::ostream & s, const Chromosome & in ) {

    for(auto & j : in.get_chromosome())
    {
        s << j;
    }
    return s;
}
