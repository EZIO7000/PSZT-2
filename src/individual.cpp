#include "individual.hpp"

individual::individual() {

    fitness = 0;
}

individual::individual(const individual & other) {

    this->fitness = other.fitness;
    this->gene = other.gene;
}

individual & individual::operator=(const individual & other) {

    if(&other == this)
            return *this;

    this->fitness = other.fitness;
    this->gene = other.gene;

    return *this;
}

bool individual::operator< ( individual const & other) const {

    return this->fitness < other.fitness;
}

std::vector<unsigned> const & individual::get_gene() const {

    return this->gene;
}

unsigned individual::get_fitness() const {

    return this->fitness;
}

void individual::set_fitness( unsigned f ) {

    this->fitness = f;
}

void individual::set_gene( std::vector<unsigned> & g ) {

    this->gene = g;
}

std::ostream & operator<< ( std::ostream & s, const individual & in ) {
   
    for( auto & j : in.get_gene() ) {

        s << j;
    }
   
    s << " " << in.get_fitness() << std::endl;

    return s;
}

std::string & operator+=( std::string & s, const individual & in ) {

    for( auto & j : in.get_gene() )
        s += std::to_string(j);
   
    return s;
}