#include <iostream>
#include <cstring>

///-------------------------------(ENUMS)-------------------------------///
enum class Water_Species {Squirtle, Blastoise, Seel, Horsea, Vaporeon};
enum class Fire_Species { Charmander, Magmar, Tepig, Litten, Raboot};
enum class Steel_Species { Klink, Klang, Meltan, Melmetal};
enum class Electric_Species { Pikachu,  Raichu,  Manectric, Electivire};
///---------------------------------------------------------------------///

template <class P>
struct ITF{
    virtual std::ostream& print(std::ostream&) const = 0;
    virtual std::istream& read(std::istream&) = 0;
};

class POKEMON : public virtual ITF<POKEMON>{

private:
    char* Name;
    int HP;
    float WEIGHT;

public:

    POKEMON(char* N, int HP, float W) : HP(HP), WEIGHT(W)
    {
        Name = new char[strlen(N) + 1];
        strcpy(Name, N);
    }

    virtual ~POKEMON()
    {
        if(Name == nullptr)
            delete[] Name;
    }

    std::ostream& print(std::ostream&) const override;
    std::istream& read(std::istream&) override;

    friend std::ostream& operator << (std::ostream& out, const POKEMON& P);
    friend std::istream& operator >> (std::istream& in, POKEMON& P);

};

std::ostream& POKEMON::print(std::ostream& out) const {

    out<<"Name: " << this->Name << std::endl;
    out<<"STATS: " << std::endl;
    out<<"-HP = " << this->HP << std::endl;
    out<<"-WEIGHT = " << this->WEIGHT << std::endl;

    return out;
}

std::ostream& operator<<(std::ostream& out, const POKEMON& P){
    return P.print(out);
}

std::istream& POKEMON::read(std::istream& in){
    std::cout<<"Remember, a pokemon is a real friend so chose an appropriate name!"<<std::endl;
    std::cout<<"Chose a name: "<<std::endl;
    in >> this -> Name;
    std::cout<<"Insert the Hit Points(HP): "<<std::endl;
    in >> this -> HP;
    std::cout<<"Insert the weight of the pokemon: "<<std::endl;
    in >> this -> WEIGHT;

    return in;
}

std::istream& operator >> (std::istream& in, POKEMON& P){
    return P.read(in);
}

template <class type>
class WATER_TYPE : virtual public POKEMON{

private:

    Water_Species WS;
    type TYPE;

public:
    WATER_TYPE(char* N, int HP, float W, type  T) : POKEMON(N, HP, W), TYPE(std::move(T)){ std::cout<<"WATHA."<<std::endl;}

};

class STEEL_TYPE : virtual public POKEMON{

public:
    STEEL_TYPE(char* N, int HP, float W) : POKEMON(N, HP, W){std::cout<<"STEEL."<<std::endl;}
};

class WATER_STEEL_TYPE : public WATER_TYPE<int>, public STEEL_TYPE{};





class FIRE_TYPE{};
class ELECTRIC_TYPE{};
class FIRE_ELECTRIC_TYPE{};

int main() {
    std::cout << "Welcome to the Pokemon world!!" << std::endl;
    std::cout << "I see you already caught a pokemon, well done!" << std::endl;
    std::cout << "You have to add it to your POKEDEX" << std::endl;
    std::cout << "What is a POKEDEX?..." << std::endl;
    std::cout << "A POKEDEX is like your own pokemon map that holds all your pokemons." << std::endl;
    std::cout << "Every page of a POKEDEX can hold up to 5 different pokemons, and every page is specific to only one type." << std::endl;
    std::cout << "You can have as many POKEDEX as you can carry." << std::endl;
    std::cout << "So what type is your pokemon?" << std::endl;
    std::cout << "You don't know??" << std::endl;
    std::cout << "Well firs of all there ary 4 basic types: FIRE, WATER, STEEL and ELECTRIC." << std::endl;
    std::cout << "And there are also 2 combination types: FIRE_STEEL type and WATER_ELECTRIC type." << std::endl;
    std::cout << "Now that you know all of this your set to go. Good luck!!" << std::endl;

    std::string input;
    std::cin>>input;
    if(input == "water"){WATER_TYPE<std::string> BULB("POKITON", 123, 12.2, "water");}

    if(input == "Vaporeon"){WATER_TYPE<Water_Species> TRUID("POKassaON", 113, 1212, Water_Species::Vaporeon);
                 std::cin>>TRUID;
                 std::cout<<TRUID;}
    if(input == "Squirtle"){WATER_TYPE<Water_Species> TRUID("POKassaON", 113, 1212, Water_Species::Squirtle);}
    if(input == "Blastoise"){WATER_TYPE<Water_Species> TRUID("POKassaON", 113, 1212, Water_Species::Blastoise);}
    if(input == "Seel"){WATER_TYPE<Water_Species> TRUID("POKassaON", 113, 1212, Water_Species::Seel);}
    if(input == "Horsea"){WATER_TYPE<Water_Species> TRUID("POKassaON", 113, 1212, Water_Species::Horsea);}
    return 0;
}
