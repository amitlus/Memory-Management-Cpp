
#include <vector>
#include "../include/Trainer.h"

Trainer::Trainer(int t_capacity): salary(0),capacity(t_capacity), open(false),customersList(),orderList() {}//CONSTRUCTOR

Trainer::~Trainer() {//DESTRUCTOR
    clear();
}

//COPY CONSTRUCTOR
Trainer::Trainer(const Trainer &other):salary(other.salary), capacity(other.capacity), open(other.open) ,customersList() ,orderList(other.orderList) {
    for(unsigned int i=0; i<other.customersList.size(); i++)
        customersList.push_back(other.customersList[i]->clone());
}

//COPY ASSIGNMENT OPERATOR
const Trainer& Trainer::operator=(const Trainer &other){
    if(this==&other)
        return *this;
    clear();
    salary = other.salary;
    capacity = other.capacity;
    open = other.open;
    orderList = other.orderList;
    for(unsigned int i=0; i<other.customersList.size(); i++)
        customersList.push_back(other.customersList[i]->clone());
}

//MOVE CONSTRUCTOR
Trainer::Trainer(Trainer &&other):salary(other.salary), capacity(other.capacity), open(other.open),customersList(other.customersList), orderList(other.orderList){
}

//MOVE ASSIGNMENT OPERATOR
const Trainer& Trainer::operator=(Trainer&& other){   ////RETURN TO THIS SHOULD WE DELETE HIS DATA??
    clear();
    salary = other.salary;
    capacity = other.capacity;
    open = other.open;
    orderList = other.orderList;
    customersList = other.customersList;
    return *this;
}

void Trainer::clear() {
    for(unsigned int i=0; i<customersList.size(); i++) {
        if (customersList[i] != nullptr) {
            delete customersList[i];
            customersList[i] = nullptr;
        }
    }
    customersList.clear();
}

int Trainer::getCapacity() const{
    return capacity;
}

void Trainer::addCustomer(Customer* customer){
    if(hasFreeSpots())
        customersList.push_back(customer);
}

void Trainer::removeCustomer(int id) {//CHECK THE IMPLEMENTATION
    bool found = false;
    for (unsigned int i = 0; i < customersList.size() && !found; i = i + 1) {
        if (customersList[i]->getId() == id)
            found = true;
        if (found) {
            delete customersList[i];
            ///ERASING JUST A SINGLE CUSTOMER FROM THE VECTOR AND PUSHING THE OTHER ELEMENTS ONE SPOT TO THE 'LEFT'
            customersList.erase(customersList.begin() + i);
        }
    }
}

void Trainer::updateSalary(int addToSalary) {//WE ADDED
    salary += addToSalary;
}

Customer* Trainer::getCustomer(int id) {
    Customer *p= nullptr;
    bool found=false;
    for(unsigned int i=0;i<customersList.size() && !found; i=i+1){
        if(customersList[i]->getId()==id)
            found=true;
        if(found) {
            Customer *p = customersList[i];
            return p;
        }
    }
    return p;    ///In case we didn't find the customer we are looking for.
}

std::vector<Customer*>& Trainer::getCustomers(){
    return customersList;
}

std::vector<OrderPair>& Trainer::getOrders(){
    return orderList;
}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options){
    for(unsigned int i=0; i<workout_ids.size(); i++)
        orderList.push_back(std::make_pair(customer_id,workout_options[workout_ids[i]]));
}

bool Trainer::hasFreeSpots(){//WE ADDED
    return getCapacity()-customersList.size()>0;
}

void Trainer::openTrainer(){
    if(!open)
        open=true;
}

void Trainer::closeTrainer(){
    if(open) {
        for (unsigned int i=0; i<customersList.size(); i++)
            delete customersList[i];
        getCustomers().clear();
        orderList.clear();
        open = false;
    }
}

int Trainer::getSalary(){
    return salary;
}

bool Trainer::isOpen(){
    return open;
}

void Trainer::updateOrderList(std::vector<OrderPair> &list){//WE ADDED
    orderList.clear();
    orderList = list;
}