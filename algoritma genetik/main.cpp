#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

struct _Populasi_{
    std::string Gen;
    double Fitness;
};

#define POPULASI_BESAR 100
#define Rate_Factor 0.3

void Random_Number(int MIN, int MAX, int jum, std::string& hasil);
void Buat_Gen(std::string Target, std::string& gen_baru);
void Kalkulasi_Fitness(std::string Target, std::string gen_baru, double& fitness);
void Buat_Populasi(std::string Target, int besar_populasi, std::vector<_Populasi_>& populasi);
void Seleksi(std::vector<_Populasi_> populasi, std::vector<_Populasi_>& Parents);
void Crossover_Alternalte(std::vector<_Populasi_> Parents, double Cross_Point, std::vector<_Populasi_>& Childern);
void Mutasi(std::vector<_Populasi_> Childern, double Mutasi_Point, std::string Target, std::vector<_Populasi_>& Mutants);
void Regenerasi_MIN(std::vector<_Populasi_>& populasi, std::vector<_Populasi_> Mutants);
void Check_Fitness(std::vector<_Populasi_> populasi, bool& is_complete);

int main(){
    srand(time(NULL));
    std::string Target = "Improved Fitness Function - The Nature of Code";
    std::vector<_Populasi_> populasi;
    Buat_Populasi(Target, POPULASI_BESAR, populasi);
    
    bool isTrue = true;
    unsigned long generasi = 0;
    while (isTrue){
        std::vector<_Populasi_> Parents, Childern, Mutants;
        Seleksi(populasi, Parents);
        Crossover_Alternalte(Parents, Rate_Factor, Childern);
        Mutasi(Childern, Rate_Factor, Target, Mutants);
        Regenerasi_MIN(populasi, Mutants);    
        bool is_complete = false;
        Check_Fitness(populasi, is_complete);
        std::cout << "===== " << generasi << " =====" << std::endl;
        for (auto &&i : populasi){
            std::cout << i.Gen << " | " << i.Fitness << std::endl;
        }    

        if(is_complete){
            isTrue = false;
        }
        generasi++;
    }
    
    return 0;
}

// random number
void Random_Number(int MIN, int MAX, int jum, std::string& hasil){
    hasil = "";
    for (size_t i = 0; i < jum; i++){
        hasil += (rand() % (MAX-MIN) + MIN);
    }
}

// membuat gen secara acak
void Buat_Gen(std::string Target, std::string& gen_baru){
    Random_Number(32, 126, Target.length(), gen_baru);
}

// mengkalkulasikan fitness
void Kalkulasi_Fitness(std::string Target, std::string gen_baru, double& fitness){
    int sum = 0;
    for (size_t i = 0; i < Target.length(); i++){
        if (Target.at(i) == gen_baru.at(i)){
            sum++;
        }                    
    }

    // fitness = (static_cast<double>(sum)/Target.length())*100;
    fitness = static_cast<double>(sum)/Target.length();
    fitness = pow(fitness, 10);
    fitness *= 100;
}

// membuat populasi
void Buat_Populasi(std::string Target, int besar_populasi, std::vector<_Populasi_>& populasi){
    for (int i = 0; i < besar_populasi; i++){
        _Populasi_ popu;
        std::string gen;
        Buat_Gen(Target,gen);
        popu.Gen = gen;
        Kalkulasi_Fitness(Target, gen, popu.Fitness);
        populasi.push_back(popu);        
    } 
}

void Seleksi(std::vector<_Populasi_> populasi, std::vector<_Populasi_>& Parents){
    std::vector<double> data_fitness;
    // std::max_element(populasi.begin())
    for (auto i : populasi){
        data_fitness.push_back(i.Fitness);
    }

    for (size_t i = 0; i < std::round(POPULASI_BESAR/4); i++){
        int indexparent = std::max_element(data_fitness.begin(), data_fitness.end()) - data_fitness.begin();
        Parents.push_back(populasi[indexparent]);
        data_fitness.erase(data_fitness.begin()+indexparent);
        populasi.erase(populasi.begin()+indexparent);        
    }
}

void Crossover_Alternalte(std::vector<_Populasi_> Parents, double Cross_Point, std::vector<_Populasi_>& Childern){ 
    for (auto i : Parents){
        Childern.push_back(i);
    }
    
    for (size_t k = 0; k < Parents.size()-1; k+=2){
        for (size_t i = 0; i < Parents[k].Gen.size(); i++){
            double f = (double)rand() / RAND_MAX;
            double random_double = 0 + f * (1 - 0);
            if(random_double <= Cross_Point){
                char temp = (Childern[k].Gen.at(i));
                Childern[k].Gen.replace(i, 1, 1, Childern[k+1].Gen.at(i));
                Childern[k+1].Gen.replace(i, 1, 1, temp);
            }
        }        
    }
}

void Mutasi(std::vector<_Populasi_> Childern, double Mutasi_Point, std::string Target, std::vector<_Populasi_>& Mutants){ 
    for (auto i : Childern){
        Mutants.push_back(i);
    }

    for (size_t k = 0; k < Mutants.size(); k++){
        for (size_t i = 0; i < Mutants[k].Gen.size(); i++){
            double f = (double)rand() / RAND_MAX;
            double random_double = 0 + f * (1 - 0);
            if(random_double <= Mutasi_Point){
                Mutants[k].Gen.replace(i, 1, 1, (char)(rand() % (126-32) + 32));
            }
        }
    }

    for (size_t i = 0; i < Mutants.size(); i++){
        Kalkulasi_Fitness(Target, Mutants[i].Gen, Mutants[i].Fitness);        
    }
    
}

void Regenerasi_MIN(std::vector<_Populasi_>& populasi, std::vector<_Populasi_> Mutants){
    std::vector<double> data_fitness;
    for (auto i : populasi){
        data_fitness.push_back(i.Fitness);
    }

    for (size_t i = 0; i < Mutants.size(); i++){
        int indexparent = std::min_element(data_fitness.begin(), data_fitness.end()) - data_fitness.begin();
        data_fitness.erase(data_fitness.begin()+indexparent);
        populasi.erase(populasi.begin()+indexparent);        
    }

    for (size_t i = 0; i < Mutants.size(); i++){
        populasi.push_back(Mutants[i]);
    }
}

void Check_Fitness(std::vector<_Populasi_> populasi, bool& is_complete){
    for (auto i : populasi){
        if(i.Fitness == 100){
            is_complete = true;
            return;
        }
    }
    is_complete = false;
}
