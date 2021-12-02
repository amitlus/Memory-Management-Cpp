//
// Created by spl211 on 29/11/2021.
//

#include <iostream>
#include "../include/Workout.h"
#include <sstream>
using namespace std;

Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type):id(w_id), name(w_name), price(w_price), type(w_type){
}

int Workout::getId() const{
    return id;
}

std::string Workout::getName() const {
    return name;
}

int Workout::getPrice() const{
    return price;
}

WorkoutType Workout::getType() const {
    return type;
}

std::string Workout::toString() const{

}

Workout &Workout::operator=(const Workout& wk) {
    return *this;
}