#include <iostream>
#include <fstream>
#include <cstring>
#include <cxxabi.h>
#include <limits>
#include <set>
#include <unordered_map>
#include <vector>
#include <exception>
#include <stdexcept>
#include <type_traits>

using std::runtime_error;

///-------------------------------(ENUMS)-------------------------------///
enum class Water_Species {Squirtle, Blastoise, Seel, Horsea, Vaporeon};
enum class Fire_Species { Charmander, Magmar, Tepig, Litten, Raboot};
enum class Steel_Species { Klink, Klang, Meltan, Melmetal};
enum class Electric_Species { Pikachu,  Raichu,  Manectric, Electivire};

enum class Water_Steel_Species {Empoleon, ew};
///---------------------------------------------------------------------///

template<class a>
void ERROR_TYPES(a)
{
    if(std::is_same<a, std::string>::value) throw false;
}


template <class P>
struct ITF{
    virtual std::ostream& print(std::ostream&) const = 0;
    virtual std::istream& read(std::istream&) = 0;

    virtual  P& EQUAL(const P& p) = 0;
};

class POKEMON : public virtual ITF<POKEMON>{

private:
    char* Name;
    int HP;
    float WEIGHT;
    float CP;

public:

    virtual int getSPECS() const = 0;
    virtual bool operator<(const POKEMON& other) const = 0;
    virtual bool operator==(const POKEMON& other) const = 0;

    POKEMON(){

        Name = new char[strlen("Anonim") + 1];
        strcpy(Name, "Anonim");
        HP = 0;
        CP = 0;
        WEIGHT = 0;
    }

    POKEMON(char* N, int HP, float W, float CP) : HP(HP), WEIGHT(W), CP(CP)
    {
        Name = new char[strlen(N) + 1];
        strcpy(Name, N);
    }

    virtual ~POKEMON()
    {
        if(Name == nullptr)
            delete[] Name;
    }

    POKEMON(const POKEMON& P) : HP(P.HP), WEIGHT(P.WEIGHT), CP(P.CP)
    {
        Name = new char[strlen(P.Name) + 1];
        strcpy(Name, P.Name);
    }

    char* getName() const { return Name; }
    int getHP() const { return HP; }
    float getW() const { return WEIGHT; }

    const char* getType() const{

        int status;
        char* demangledName = abi::__cxa_demangle(typeid(*this).name(), nullptr, nullptr, &status);
        if(status == 0){
            const char* typeName = demangledName;
            return typeName;
        }else {
            return typeid(*this).name();
        }
    }

    void SETCP(float cp){ CP = cp; };
    void SETNAME(char* name ) { Name = name;}

    float getCP() const {
        return CP;
    }


    std::ostream& print(std::ostream&) const override;
    std::istream& read(std::istream&) override;

    friend std::ostream& operator << (std::ostream& out, const POKEMON& P);
    friend std::istream& operator >> (std::istream& in, POKEMON& P);

    POKEMON& EQUAL(const POKEMON& P) override;
    POKEMON& operator = (const POKEMON& P);

};

POKEMON& POKEMON::EQUAL(const POKEMON &P){

    if(this != &P)
    {
        if(this -> Name != nullptr)
        {
            delete[] Name;
            (*this).Name = nullptr;
        }

        this -> Name = new char[strlen(P.Name) + 1];
        strcpy(Name, P.Name);

        HP = P.HP;
        WEIGHT = P.WEIGHT;
        CP = P.CP;
    }

    return *this;

}

POKEMON& POKEMON::operator = (const POKEMON& P){
    return (*this).EQUAL(P);
}


std::ostream& POKEMON::print(std::ostream& out) const {

    out<<"Name: " << this->Name << std::endl;
    out<<"STATS: " << std::endl;
    out<<"-HP = " << this->HP << std::endl;
    out<<"-WEIGHT = " << this->WEIGHT << std::endl;
    out<<"-CP = " << this ->CP << std::endl;
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
    std::cout<<"Insert the Combat Power(CP) of your pokemon: " << std::endl;
    in >> this -> CP;

    return in;
}

std::istream& operator >> (std::istream& in, POKEMON& P){
    return P.read(in);
}

///-------------------------------------------(WATER)-------------------------------------------///

class WATER_TYPE : virtual public POKEMON{

private:

    Water_Species WS;

public:


    int getSPECS() const override {
        return static_cast<int>(WS);
    }

    bool operator<(const POKEMON& other) const override {
        if (const WATER_TYPE* otherWaterType = dynamic_cast<const WATER_TYPE*>(&other)) {
            return WS < otherWaterType->WS;
        }
        return false;
    }

    bool operator==(const POKEMON& other) const override {
        if (const WATER_TYPE* otherWaterType = dynamic_cast<const WATER_TYPE*>(&other)) {
            return WS == otherWaterType -> WS;
        }
        return false;
    }

    WATER_TYPE(){}

    WATER_TYPE(char* N, int HP, float W, float CP, Water_Species Wt) : POKEMON(N, HP, W, CP), WS(Wt){ }

    WATER_TYPE(const WATER_TYPE& W) : POKEMON(W), WS(W.WS){}

    virtual ~WATER_TYPE()
    {
        if(POKEMON::getName() == nullptr)
        {
            delete[] getName();
        }
    }

    Water_Species getSpecies() const {
        return WS;
    };

    void operator + (WATER_TYPE& other)  {

        try {
            if (this->WS == other.WS) {
                float W1 = this->getCP();
                float AddedCP = other.getCP() + W1;
                if (AddedCP >= 1500) {
                    this->SETCP(1500);
                    other.SETCP(AddedCP - 1500);
                } else {
                    this->SETCP(AddedCP);
                    other.SETCP(0);
                }
            } else {
                throw std::runtime_error("Invalid operation: Cannot add Pokemon of different water species.");
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    static std::string WaterSpeciesToString(Water_Species species) {
    switch (species) {
        case Water_Species::Squirtle:
            return "Squirtle";
        case Water_Species::Blastoise:
            return "Blastoise";
        case Water_Species::Seel:
            return "Seel";
        case Water_Species::Horsea:
            return "Horsea";
        case Water_Species::Vaporeon:
            return "Vaporeon";
        default:
            return "Unknown";
    }
}

    std::istream& read(std::istream& in) override;
    std::ostream& print(std::ostream& out) const override;

    friend std::istream& operator >> (std::istream& in, WATER_TYPE& W);
    friend std::ostream& operator << (std::ostream& out, const WATER_TYPE& W);

    WATER_TYPE& EQUAL(const WATER_TYPE& W);
    WATER_TYPE &operator=(const WATER_TYPE &W);
};

WATER_TYPE& WATER_TYPE::EQUAL(const WATER_TYPE& W){
    POKEMON::EQUAL(W);
    WS = W.WS;
    return *this;
}

WATER_TYPE& WATER_TYPE::operator=(const WATER_TYPE &W) {
    return (*this).EQUAL(W);
}

std::istream& WATER_TYPE::read(std::istream &in) {

    POKEMON::read(in);
    bool valid_input = false;
    std::string input;

    while (!valid_input) {
        std::cout << "Insert the steel species: " << std::endl;
        try {
            in >> input;

            if (input == "Horsea") {
                WS = Water_Species::Horsea;
                valid_input = true;
            } else if (input == "Vaporeon") {
                WS = Water_Species::Vaporeon;
                valid_input = true;
            } else if (input == "Seel") {
                WS = Water_Species::Seel;
                valid_input = true;
            } else if (input == "Blastoise") {
                WS = Water_Species::Blastoise;
                valid_input = true;
            } else if (input == "Squirtle") {
                WS = Water_Species::Squirtle;
                valid_input = true;
            }else {
                throw std::invalid_argument("Invalid input. Please try a valid water species");
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            std::cout << "Please insert a valid information for this field: ";
            in.clear();
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return in;
}

std::istream& operator >> (std::istream& in, WATER_TYPE& W){
    return W.read(in);
}

std::ostream& WATER_TYPE::print(std::ostream &out) const {
    POKEMON::print(out);
    out<<"Water type species: ";
    switch (WATER_TYPE::WS) {
        case Water_Species::Squirtle:
            out << "Squirtle";
            break;
        case Water_Species::Blastoise:
            out << "Blastoise";
            break;
        case Water_Species::Seel:
            out << "Seel";
            break;
        case Water_Species::Horsea:
            out << "Horsea";
            break;
        case Water_Species::Vaporeon:
            out << "Vaporeon";
            break;
    }
    out<<std::endl;

    return out;
}

std::ostream& operator << (std::ostream& out, const WATER_TYPE& W){
    W.print(out);
    return out;
}

///-------------------------------------------(STEEL)-------------------------------------------///

class STEEL_TYPE : virtual public POKEMON{
private:

    //const char* Weakness = "Fire_Type";
    Steel_Species SS;

public:

    int getSPECS() const override {
        return static_cast<int>(SS);
    }

    bool operator<(const POKEMON& other) const override {
        if (const STEEL_TYPE* otherSteelType = dynamic_cast<const STEEL_TYPE*>(&other)) {
            return SS < otherSteelType->SS;
        }

        return false;
    }


    bool operator==(const POKEMON& other) const override {
        if (const STEEL_TYPE* otherSteelType = dynamic_cast<const STEEL_TYPE*>(&other)) {
            return SS == otherSteelType -> SS;
        }
        return false;
    }

    STEEL_TYPE(){}

    STEEL_TYPE(char* N, int HP, float W, float CP, Steel_Species ss) : POKEMON(N, HP, W, CP), SS(ss){}

    STEEL_TYPE(const STEEL_TYPE& S) : POKEMON(S), SS(S.SS){}

    virtual ~STEEL_TYPE()
    {
        if(POKEMON::getName() == nullptr)
        {
            delete[] getName();
        }
    }

    Steel_Species getSpecies() const { return SS; }

   static std::string SteelSpeciesToString(Steel_Species species) {
    switch (species) {
        case Steel_Species::Klink:
            return "Klink";
        case Steel_Species::Klang:
            return "Klang";
        case Steel_Species::Meltan:
            return "Meltan";
        case Steel_Species::Melmetal:
            return "Melmetal";
        default:
            return "Unknown";
    }
}

    std::istream& read(std::istream& in) override;
    std::ostream& print(std::ostream& out) const override;

    friend std::istream& operator >> (std::istream& in, STEEL_TYPE& S);
    friend std::ostream& operator << (std::ostream& out, const STEEL_TYPE& S);

    STEEL_TYPE& EQUAL(const STEEL_TYPE& S);
    STEEL_TYPE& operator = (const STEEL_TYPE& S);
};

STEEL_TYPE& STEEL_TYPE::EQUAL(const STEEL_TYPE& S){
    POKEMON::EQUAL(S);
    SS = S.SS;
    return *this;
}

STEEL_TYPE& STEEL_TYPE::operator=(const STEEL_TYPE &S) {
    return (*this).EQUAL(S);
}

std::istream& STEEL_TYPE::read(std::istream &in) {
    POKEMON::read(in);
    bool valid_input = false;
    std::string input;

    while (!valid_input) {
        std::cout << "Insert the steel species: " << std::endl;
        try {
            in >> input;

            if (input == "Klink") {
                SS = Steel_Species::Klink;
                valid_input = true;
            } else if (input == "Klang") {
                SS = Steel_Species::Klang;
                valid_input = true;
            } else if (input == "Meltan") {
                SS = Steel_Species::Meltan;
                valid_input = true;
            } else if (input == "Melmetal") {
                SS = Steel_Species::Melmetal;
                valid_input = true;
            } else {
                throw std::invalid_argument("Invalid input. Please try a valid steel species");
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            std::cout << "Please insert a valid information for this field: ";
            in.clear();
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return in;
}

std::istream& operator >> (std::istream& in, STEEL_TYPE& S){
    return S.read(in);
}

std::ostream& STEEL_TYPE::print(std::ostream &out) const {
    POKEMON::print(out);
    out<<"Steel type species: ";
    switch (STEEL_TYPE::SS) {
        case Steel_Species::Klink:
            out << "Klink";
            break;
        case Steel_Species::Klang:
            out << "Klang";
            break;
        case Steel_Species::Meltan:
            out << "Meltan";
            break;
        case Steel_Species::Melmetal:
            out << "Melmetal";
            break;
    }
    out<<std::endl;

    return out;
}

std::ostream& operator << (std::ostream& out, const STEEL_TYPE& S){
    S.print(out);
    return out;
}

///-------------------------------------------(WATER_STEEL)-------------------------------------------///

class WATER_STEEL_TYPE : public WATER_TYPE, public STEEL_TYPE{

private:
    std::string WaterSteelSpecies;
    Water_Steel_Species WSS;
public:

    int getSPECS() const override {
        if(!WaterSteelSpecies.empty()){
            return std::stoi(WaterSteelSpecies);
        }
        else {
            return static_cast<int>(WSS);
        }
    }

public:
    bool operator<(const POKEMON& other) const override {
        if (const WATER_STEEL_TYPE* otherWaterSteelType = dynamic_cast<const WATER_STEEL_TYPE*>(&other)) {
            return WSS < otherWaterSteelType->WSS;
        }
        return false;
    }

    bool operator==(const POKEMON& other) const override {
        if (const WATER_STEEL_TYPE* otherWaterSteelType = dynamic_cast<const WATER_STEEL_TYPE*>(&other)) {
            return WSS == otherWaterSteelType -> WSS;
        }
        return false;
    }

    WATER_STEEL_TYPE(){}

    WATER_STEEL_TYPE(char* N, int HP, float W, float CP, Water_Steel_Species WSS) :
            POKEMON(N, HP, W, CP),
//            WATER_TYPE(N, HP, W, CP, WS),
//            STEEL_TYPE(N, HP, W, CP, SS),
            WSS(WSS){}

    WATER_STEEL_TYPE(char* N, int HP, float W, float CP) :
            POKEMON(N, HP, W, CP)
//            WATER_TYPE(N, HP, W, CP, WS),
//            STEEL_TYPE(N, HP, W, CP, SS),
{}


    std::string getSpecString() const {return WaterSteelSpecies;}
    Water_Steel_Species getSpecSpec() const { return WSS;}
    void SetSpecSpec(Water_Steel_Species S) { WSS = S;}
    void SetSpecString(std::string s) { WaterSteelSpecies = s;}

    WATER_STEEL_TYPE(char* N, WATER_TYPE& W, STEEL_TYPE&S) :
        POKEMON(N, W.getHP() + S.getHP(), W.getW() + S.getW(), 100),
        WATER_TYPE(N, W.getHP() + S.getHP(), W.getW() + S.getW(), 100, W.getSpecies()),
        STEEL_TYPE(N, W.getHP() + S.getHP(), W.getW() + S.getW(), 100, S.getSpecies())
    {
        std::string waterString = WaterSpeciesToString(W.getSpecies());
        std::string steelString = SteelSpeciesToString(S.getSpecies());
        SetSpecString(waterString.substr(0, waterString.size() / 2) +
                                      steelString.substr(steelString.size() / 2));

        
    }

    WATER_STEEL_TYPE(const WATER_STEEL_TYPE& WS) : POKEMON(WS), WSS(WS.WSS){}

    Water_Steel_Species GetSpecies() const {
        return WSS;
    }

    std::istream& read(std::istream&) override;
    std::ostream& print(std::ostream&) const override;

    friend std::istream& operator >> (std::istream& in, WATER_STEEL_TYPE& WS);
    friend std::ostream& operator << (std::ostream& out, const WATER_STEEL_TYPE& WS);

    bool operator<(const WATER_STEEL_TYPE& other) const {
        return getCP() < other.getCP();
    }

    bool operator==(const WATER_STEEL_TYPE& other) const {
        if(!WaterSteelSpecies.empty()) {
            return getSpecString() == other.getSpecString();
        }
        else
            return getSpecSpec() == other.getSpecSpec();

    }
    WATER_STEEL_TYPE& EQUAL(const WATER_STEEL_TYPE& WS);
    WATER_STEEL_TYPE& operator = (const WATER_STEEL_TYPE& WS);
};

WATER_STEEL_TYPE& WATER_STEEL_TYPE::EQUAL(const WATER_STEEL_TYPE& WS){
    POKEMON::EQUAL(WS);
    WSS = WS.WSS;
    return *this;
}

WATER_STEEL_TYPE& WATER_STEEL_TYPE::operator=(const WATER_STEEL_TYPE &WS) {
    return (*this).EQUAL(WS);
}

std::istream& WATER_STEEL_TYPE::read(std::istream& in) {
    POKEMON::read(in);
    bool valid_input = false;
    std::string input;

    while (!valid_input) {
        std::cout << "Insert the watersteel species: " << std::endl;
        in >> input;
        if (input == "Empoleon") {
            WSS = Water_Steel_Species::Empoleon;
            valid_input = true;
        }
        else
            std::cerr
                    << "Invalid input. Please try a valid watersteel species"
                    << std::endl
                    <<"Please insert a valid information for this field: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    }
    return in;
}

std::istream& operator >> (std::istream& in, WATER_STEEL_TYPE& WS){
    return WS.read(in);
}

std::ostream& WATER_STEEL_TYPE::print(std::ostream& out) const{
    POKEMON::print(out);
    out << "WaterSteel type species: ";
    if (WSS == Water_Steel_Species::Empoleon) {
        out << "Empoleon";
    } else {
//        out << getSpecString();
        out<<"Trebuie sa vad care e faza :D.";
        out<<std::endl;
    }

    out<<std::endl;
    return out;
}

std::ostream& operator << (std::ostream& out, const WATER_STEEL_TYPE& WS){
    WS.print(out);
    return out;
}


//class FIRE_TYPE{};
//class ELECTRIC_TYPE{};
//class FIRE_ELECTRIC_TYPE : public FIRE_TYPE, public ELECTRIC_TYPE{};

template<typename Type3 ,typename Type1, typename Type2>
class MORF : public WATER_STEEL_TYPE /* public FIRE_ELECTRIC_TYPE*/{
private:
//    POKEMON* pokemon_;
//    WATER_STEEL_TYPE* WST;
public:
    MORF(char* name, Type1& type1, Type2& type2){
             Type3 MM(name, static_cast<Type1&>(type1).getHP() + static_cast<Type2&>(type2).getHP(), static_cast<Type2&>(type2).getW(), 10) ;
             std::cout<<MM;
    }

//    MORF(char* name, const FIRE_TYPE& type1, const ELECTRIC_TYPE& type2)
//            : FIRE_ELECTRIC_TYPE(name, type1, type2) {}


};

struct PokemonComparator {
    bool operator()(const POKEMON* p1, const POKEMON* p2) const {
        // Compare based on species
        return p1->getSPECS() < p2->getSPECS();
    }
};

class POKEDEX {
private:
    std::unordered_map<std::string, std::set<POKEMON*, PokemonComparator>> PDEX;
    int status;

public:
    POKEDEX() {
        PDEX[abi::__cxa_demangle(typeid(WATER_TYPE).name(), nullptr, nullptr, &status)] = std::set<POKEMON*, PokemonComparator>();
        PDEX[abi::__cxa_demangle(typeid(STEEL_TYPE).name(), nullptr, nullptr, &status)] = std::set<POKEMON*, PokemonComparator>();
        PDEX[abi::__cxa_demangle(typeid(WATER_STEEL_TYPE).name(), nullptr, nullptr, &status)] = std::set<POKEMON*, PokemonComparator>();
    }

    void addPokemon(POKEMON* pokemon) {
        std::string typeName = abi::__cxa_demangle(typeid(*pokemon).name(), nullptr, nullptr, &status);
        PDEX[typeName].insert(pokemon);
        std::cout << "Added " << typeName << " to the Pokedex." << std::endl;
    }

    void printPokedex() {
        for (const auto& entry : PDEX) {
            std::cout << "Type: " << entry.first << std::endl;
            std::cout << "Pokemons:" << std::endl;
            for (const auto& pokemon : entry.second) {
                if (typeid(*pokemon) == typeid(WATER_TYPE)) {
                    WATER_TYPE* waterType = dynamic_cast<WATER_TYPE*>(pokemon);
                    if (waterType != nullptr) {
                        std::cout << "WaterType: " << waterType->getType() << std::endl;
                        std::cout<<*waterType;
                        // Print waterType specific details
                    }
                } else if (typeid(*pokemon) == typeid(STEEL_TYPE)) {
                    STEEL_TYPE* steelType = dynamic_cast<STEEL_TYPE*>(pokemon);
                    if (steelType != nullptr) {
                        std::cout << "SteelType: " << steelType->getType() << std::endl;
                        std::cout<< *steelType;
                        // Print steelType specific details
                    }
                } else if (typeid(*pokemon) == typeid(WATER_STEEL_TYPE)) {
                    WATER_STEEL_TYPE* waterSteelType = dynamic_cast<WATER_STEEL_TYPE*>(pokemon);
                    if (waterSteelType != nullptr) {
                        std::cout<<*waterSteelType;
                        std::cout << "WaterSteelType: " << waterSteelType->getType() << std::endl;
                        // Print waterSteelType specific details
                    }
                }
            }
            std::cout << std::endl;
        }
    }

    const std::set<POKEMON*, PokemonComparator>& getPokemons(const std::string& typeName) const {
        static std::set<POKEMON*, PokemonComparator> emptySet;
        auto it = PDEX.find(typeName);
        if (it != PDEX.end()) {
            return it->second;
        }
        return emptySet;
    }

//    void clearPokedexSets() {
//        for (auto& entry : PDEX) {
//            entry.second.clear();
//        }
//        std::cout << "CCC." << std::endl;
//    }
};

class BAG {
private:
    std::vector<POKEDEX> bag;
public:
    BAG() {
        bag.resize(2);
    }

    void addPokedex(const POKEDEX& pokedex) {
            bag.push_back(pokedex);
            std::cout << "Added Pokedex to the Bag." << std::endl;

    }

    void printBag() {
//        for (POKEDEX& pokedex : bag) {
//            pokedex.printPokedex();
//        }
        int cnt = 1;
        for (POKEDEX& pokedex : bag) {
            std::cout << "|-----------------"<<"("<<cnt<<")"<<"-----------------|" << std::endl;
            pokedex.printPokedex();

        }

    }

    const std::vector<POKEDEX>& getPokedexList() const {
        return bag;
    }

    void clearPokedexList() {
        bag.clear();
        std::cout << "Cleared the  Bag." << std::endl;
    }

};

class CHEST {
private:
    BAG& bag;

public:
    CHEST(BAG& bag) : bag(bag) {}

    void printPokedexesToFile() {
        std::ofstream fout("casa.bin", std::ios::binary | std::ios::app);
        if (!fout) {
            std::cout << "Failed to open file for writing." << std::endl;
            return;
        }

        for (const auto& pokedex : bag.getPokedexList()) {
            std::string line = "--------------------\n";
            fout.write(line.c_str(), line.size());

            const std::set<POKEMON*, PokemonComparator>& waterPokemons = pokedex.getPokemons("WATER_TYPE");
            fout << "Water Type Pokemons:\n";
            for (const auto& pokemon : waterPokemons) {
                WATER_TYPE* waterType = dynamic_cast<WATER_TYPE*>(pokemon);
                if (waterType != nullptr) {

                    const std::string& name = waterType->getName();
                    int hp = waterType->getHP();
                    double weight = waterType->getW();
                    int cp = waterType->getCP();
                    fout << "Name: " << name << std::endl;
                    fout << "HP: " << hp << std::endl;
                    fout << "Weight: " << weight << std::endl;
                    fout << "CP: " << cp << std::endl;


                }
            }
            const std::set<POKEMON*, PokemonComparator>& steelPokemons = pokedex.getPokemons("STEEL_TYPE");
            fout << "Steel Type Pokemons:\n";
            for (const auto& pokemon : steelPokemons) {
                STEEL_TYPE* steelType = dynamic_cast<STEEL_TYPE*>(pokemon);
                if (steelType != nullptr) {
                    const std::string& name = steelType->getName();
                    int hp = steelType->getHP();
                    double weight = steelType->getW();
                    float cp = steelType->getCP();


                    fout << "Name: " << name << std::endl;
                    fout << "HP: " << hp << std::endl;
                    fout << "Weight: " << weight << std::endl;
                    fout << "CP: " << cp << std::endl;
                }
            }

            const std::set<POKEMON*, PokemonComparator>& waterSteelPokemons = pokedex.getPokemons("WATER_STEEL_TYPE");
            fout << "Water Steel Type Pokemons:\n";
            for (const auto& pokemon : waterSteelPokemons) {
                WATER_STEEL_TYPE* waterSteelType = dynamic_cast<WATER_STEEL_TYPE*>(pokemon);
                if (waterSteelType != nullptr) {
                    const std::string& name = waterSteelType->getName();
                    int hp = waterSteelType->getHP();
                    double weight = waterSteelType->getW();
                    int cp = waterSteelType->getCP();

                    fout << "Name: " << name << std::endl;
                    fout << "HP: " << hp << std::endl;
                    fout << "Weight: " << weight << std::endl;
                    fout << "CP: " << cp << std::endl;
                }
            }

            fout << "--------------------\n\n";
        }

        fout.close();
        bag.clearPokedexList(); ///!!
        std::cout << "The bag has been emptied." << std::endl;
    }

    void printPokedexesToConsole() {
        std::ifstream fin("casa.bin", std::ios::binary);

        if (!fin) {
            std::cout << "Failed to open file for reading." << std::endl;
            return;
        }

        std::string line;
        while (std::getline(fin, line)) {
            std::cout << line << std::endl;
        }

        fin.close();
    }

//    void emptyBag() {
//        bag.clearPokedexList(); ///!!
//        std::cout << "The bag has been emptied." << std::endl;
//    }
};

class PokePhone{

private:
    static PokePhone *Pphone;
    POKEDEX pokedex1;
    BAG bag;
    PokePhone()=default;
    PokePhone(const PokePhone&)=delete;
    static int nrOfInstance;
    PokePhone& operator =(PokePhone&) = delete;

public:
    int a = 1;
    static PokePhone* getInstance(){
        nrOfInstance++;
        if(!Pphone)
            Pphone = new PokePhone();
        return Pphone;
    }
    void OpenPPhone(){

        int k = 1;
        while(k == 1){
            std::cout << "1. Add pokemons to your POKEDEX.\n";
            std::cout << "2. Check POKEDEX.\n";
            std::cout << "3. Check BAG.\n";
            std::cout << "4. Empty bag in chest.\n";
            std::cout << "5. Check your house chest.\n";
            std::cout << "6. Close PokePhone.\n";

            int option;
            std::string input;
            try {
                std::cin >> input;
                option = std::stoi(input);
            } catch (const std::invalid_argument&) {
                std::cerr << "Invalid input. Please enter a number corresponding to a valid option." << std::endl;
                continue;
            }
            switch(option){
                case 1:{
                    int suboption = 1;
                    while(suboption)
                    {
                        std::cout<<std::endl;
                        std::cout<<"What type of pokemon do you want to add: \n";
                        std::cout<<"Press 1 for a WATER_TYPE pokemon.\n";
                        std::cout<<"Press 2 for a STEEL_TYPE pokemon.\n";
                        std::cout<<"Press 3 for a STEEL_TYPE pokemon.\n";
                        std::cout << std::endl;
                        std::cout << "Press 0 to return to the previous menu.\n";

                        std::string input2;
                        try {
                            std::cin >> input2;
                            suboption = std::stoi(input2);
                        } catch (const std::invalid_argument&) {
                            std::cerr << "Invalid input. Please enter a number corresponding to a valid option." << std::endl;
                            continue;
                        }

                        switch(suboption)
                        {
                            case 1:
                            {
                                auto* aux = new WATER_TYPE();
                                std::cin>> *aux;
                                pokedex1.addPokemon(aux);
                                break;
                            }
                            case 2:
                            {
                                auto* aux = new STEEL_TYPE();
                                std::cin>> *aux;
                                pokedex1.addPokemon(aux);
                                break;
                            }
                            case 3:
                            {
                                auto* aux = new WATER_STEEL_TYPE();
                                std::cin>> *aux;
                                pokedex1.addPokemon(aux);
                                break;
                            }
                            default: {
                                suboption = 0;
                                break;
                            }
                        }
                    }
                    break;
                }///case1///

                case 2:{
                    pokedex1.printPokedex();
                    break;
                }

                case 3:{
//                    pokedex1.printPokedex();
                    bag.addPokedex(pokedex1);
                    bag.printBag();
                    break;
                }

                case 4:{
                    CHEST chest(bag);
                    chest.printPokedexesToFile();
                    break;
                }

                case 5:{
                    CHEST chest(bag);
                    chest.printPokedexesToConsole();
                    break;
                }

                case 7:{
                    std::cout<<"Insert a WaterType pokemon: ";
                    WATER_TYPE A;
                    std::cin >> A;
                    std::cout<<std::endl;
                    std::cout<<"Insert a WaterType pokemon: ";
                    STEEL_TYPE B;
                    std::cin >> B;
                    std::cout<<std::endl;
                    std::cout<<"Now please insert the name of your pokemon: ";
                    char* name = new char[100];
                    std::cin>>name;
                    std::cout<<std::endl;
                    MORF<WATER_STEEL_TYPE, WATER_TYPE, STEEL_TYPE> COR(name, A, B);
                }
                case 6:{
                    k = 0;
                    break;
                }
            }

        }
    }

    ~PokePhone(){
        nrOfInstance--;
        if(nrOfInstance==0)
            if(Pphone)
                delete Pphone;

    }
};

PokePhone* PokePhone::Pphone=0;
int PokePhone::nrOfInstance=0;

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
    std::cout << "----------------------------------------------------------------------------------------------------------" << std::endl;


    PokePhone* b=b->getInstance();
    b->OpenPPhone();

//    WATER_TYPE A("DAN", 2312, 23.2, 3221, Water_Species::Vaporeon);
//    STEEL_TYPE B("KORI", 12, 123, 213,  Steel_Species::Melmetal);
//    MORF<WATER_STEEL_TYPE, WATER_TYPE, STEEL_TYPE> COR("DANEE", A, B);

//
//    std::ofstream file("casa.bin", std::ios::binary | std::ios::trunc);
//    if (file) {

//        std::cout << "Wipe Cleaned" << std::endl;
//    } else {
//        std::cout << " -_-" << std::endl;
//    }

//    WATER_TYPE W("DAN", 13, 32112, 321, Water_Species::Vaporeon);
//    WATER_TYPE Q("CORI", 123, 32121, 12, Water_Species::Squirtle);
//    W+Q;
//    return 0;

}


/////FA clasa POKEDEX templated ca sa faca adugare bazat pe class obiectului////////////




