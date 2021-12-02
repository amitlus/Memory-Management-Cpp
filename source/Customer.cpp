#include "../include/Customer.h"
#include <algorithm>
#include <vector>
#include <utility>
#include <list>

using namespace std;

Customer::~Customer(){};//DESTRUCTOR
Customer::Customer(std::string c_name, int c_id): name(c_name), id(c_id){}//CONSTRUCTOR

std::string Customer::getName() const{
    return name;
}

int Customer::getId() const{
    return id;
}

//SWEATY
SweatyCustomer::SweatyCustomer(std::string name, int id): Customer(name,id){}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> vec1;///initializing vector which will keep all the Cardio activities, since the Sweaty Customer loves to sweat
    for(unsigned int i =0; i<workout_options.size();i=i+1){///Picking all the Cardio workouts and puts them in our vector
        if(workout_options[i].getType()==CARDIO)
            vec1.push_back(workout_options[i].getId());
    }
    return vec1;
}

std::string SweatyCustomer::toString() const{
    return this->getName() + ", swt";
}

Customer* SweatyCustomer::clone() const{
    return new SweatyCustomer(*this);
}

//CHEAP
CheapCustomer::CheapCustomer(std::string name, int id): Customer(name,id){}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> vec;
    int min=workout_options[0].getPrice();
    int minIndex=0;
    for (unsigned int i=1; i<workout_options.size();i=i+1){ ///Looking for the cheapest activity in the list
        if(workout_options[i].getPrice()<min) {
            min = workout_options[i].getPrice();
            minIndex = i;
        }
    }
    vec.push_back(workout_options[minIndex].getId());///The type of the activity does not suppose to interest us in here!!!
    return vec;
}

std::string CheapCustomer::toString() const{
    return this->getName() + ", chp";
}

Customer* CheapCustomer::clone() const{
    return new CheapCustomer(*this);
}

//HEAVY
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id): Customer(name,id){}

auto compare= [](const Workout *workout1, const Workout *workout2){
   return(workout1->getPrice() > workout2->getPrice());
};

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<Workout*> vec;
    for(int i=0; i<workout_options.size();i=i+1){ ///searching the Anaerobic activities
        if(workout_options[i].getType()==ANAEROBIC)
            vec.push_back(new Workout(workout_options[i])); ///Now we have all the Anaerobic activities in vec
    }
    std::stable_sort(vec.begin(), vec.end(), compare);

    std::vector<int> idVec;
    for(unsigned int i=0; i<vec.size(); i++){
        idVec.push_back(vec[i]->getId());
        if(vec[i]!= nullptr)
            delete vec[i];
    }
    return idVec;
}

std::string HeavyMuscleCustomer::toString() const{
    return this->getName() + ", mcl";
}

Customer* HeavyMuscleCustomer::clone() const{
    return new HeavyMuscleCustomer(*this);
}
//FULL BODY
FullBodyCustomer::FullBodyCustomer(std::string name, int id): Customer(name,id){}
//RETURN TO THIS IMPLEMENTATION
std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> vec;
    int cardioIndx = -1;
    int mixIndx = -1;
    int anaIndx = -1;

    for (unsigned int i=0; i<workout_options.size(); i=i+1){
        if((workout_options[i].getType() == ANAEROBIC  && anaIndx==-1) || (workout_options[i].getType()==ANAEROBIC && workout_options[i].getPrice()<workout_options[anaIndx].getPrice()))
            anaIndx=i;
        if((workout_options[i].getType()==MIXED  && mixIndx==-1) || (workout_options[i].getType()==MIXED && workout_options[i].getPrice()>workout_options[mixIndx].getPrice()))
            mixIndx=i;
        if((workout_options[i].getType()==CARDIO  && cardioIndx==-1) || (workout_options[i].getType()==CARDIO && workout_options[i].getPrice()<workout_options[cardioIndx].getPrice()))
            cardioIndx=i;
    }

    ///IF NOT FOUND - DONT INITIALIZ!

    if(cardioIndx!=-1)
        vec.push_back(workout_options[cardioIndx].getId());
    if(mixIndx!=-1)
        vec.push_back(workout_options[mixIndx].getId());
    if(anaIndx!=-1)
        vec.push_back(workout_options[anaIndx].getId());

    return vec;
}

std::string FullBodyCustomer::toString() const {
    return this->getName() + ", fbd";
}

Customer *FullBodyCustomer::clone() const {
    return new FullBodyCustomer(*this);
}