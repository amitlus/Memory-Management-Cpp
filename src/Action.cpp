
#include "../include/Action.h"
#include "../include/Trainer.h"
#include "../include/Studio.h"
#include <sstream>
#include <string>
using namespace std;

BaseAction::BaseAction():errorMsg(),status(){}

ActionStatus BaseAction::getStatus() const{
    return status;
}

void BaseAction::complete(){
    status = COMPLETED;
}
void BaseAction::error(std::string errorMsg){
    status = ERROR;
    std::cout<<errorMsg;
}

std::string BaseAction::getErrorMsg() const{
    return errorMsg;
}

BaseAction::~BaseAction(){};

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList): BaseAction(),trainerId(id),output(), nextCustomerId(-1),customers(customersList)  {}

void OpenTrainer::act(Studio &studio){
    Trainer *trainer = studio.getTrainer(trainerId);
    if(trainer != nullptr && !(trainer->isOpen()) && trainer->hasFreeSpots()) {
        std::string str;
        for(unsigned int i=0; i<customers.size(); i++) {
            if (trainer->hasFreeSpots()) {
                trainer->addCustomer(customers[i]);
                nextCustomerId = customers[i]->getId() + 1;//Update the field for future use in Studio
                str += customers[i]->getName() + " " + std::to_string(customers[i]->getId()) + ", ";
            }
        }
        trainer->openTrainer();
        complete();
    }
    else
        error("Trainer does not exist or is already open");
}

std::string OpenTrainer::toString() const{
    std::string logStr = "Open" + std::to_string(trainerId);
    for(unsigned int i=0;i<customers.size();i++){
        logStr+= " "+customers[i]->toString();
    }
    return logStr + std::to_string(getStatus());
}

int OpenTrainer::getNextCustomerId() const{
    return nextCustomerId;
}

BaseAction* OpenTrainer::clone() const{
    return new OpenTrainer(*this);
}


Order::Order(int id):BaseAction(),trainerId(id) {}


void Order::act(Studio &studio){
    Trainer *trainer = studio.getTrainer(trainerId);
    if(trainer== nullptr || !trainer->isOpen()){
        error("Workout session does not exist or is already open.");
        cout << getErrorMsg() << endl;
    }
    else {
        std::vector<Customer *> customers = trainer->getCustomers();
        for (unsigned int i = 0;  i < customers.size(); i++) {
            std::vector<int> cusWorkout = customers[i]->order(studio.getWorkoutOptions());
            trainer->order(customers[i]->getId(), cusWorkout, studio.getWorkoutOptions());
            int totalMoney = 0;
            for(unsigned int j=0; j<cusWorkout.size(); j++)
                totalMoney+= studio.getWorkoutOptions()[cusWorkout[j]].getPrice();
            trainer->updateSalary(totalMoney);
        }
        std::string str;
        std::vector<OrderPair> orderVec = trainer->getOrders();
        int count = 0;
        int id = orderVec[0].first;
        unsigned int i = 0;
        while ( i < orderVec.size()) {///CHECK IF THERE IS DOUBLE POINTERS
            while (i <orderVec.size() && orderVec[i].first == id) {
                str += customers[count]->getName() + " " + " Is Doing " + orderVec[i].second.getName() + "\n";
                i++;
            }
            count++;
            if(i <orderVec.size())
                id = orderVec[i].first;
        }
        std::cout << str;
        complete();
    }
}

std::string Order::toString() const{
    return "Order " + std::to_string(trainerId) + std::to_string(getStatus());
}

BaseAction* Order::clone() const{
    return new Order(*this);
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId): BaseAction(),srcTrainer(src),dstTrainer(dst),id(customerId){};

void MoveCustomer::act(Studio &studio) {

    Trainer *tSrc = studio.getTrainer(srcTrainer);
    Trainer *tDst = studio.getTrainer(dstTrainer);
    Customer* movingCus = tSrc->getCustomer(id);

    if (tDst->hasFreeSpots() > 0 && tDst != nullptr && tSrc != nullptr && tDst->isOpen() && tSrc->isOpen() && movingCus!= nullptr) {

        tDst->addCustomer(tSrc->getCustomer(id)->clone());
        tSrc->removeCustomer(id);
        vector<OrderPair>& srcList = tSrc->getOrders();
        vector<OrderPair>& dstList = tDst->getOrders();
        vector<OrderPair> newOrderLIst;
        int sum = 0;
        for(unsigned i = 0; i != srcList.size(); i++) {
            OrderPair pair = srcList[i];
            if(pair.first==id){
                sum += pair.second.getPrice();
                dstList.push_back(pair);
            }
            else
                newOrderLIst.push_back(pair);
        }
        tSrc->updateOrderList(newOrderLIst);
        tDst->updateSalary(sum);
        tSrc->updateSalary(-sum);
        complete();
    }
    else{
        error("Cannot move customer");
        cout << getErrorMsg() << endl; //Printing error
    }
}

std::string MoveCustomer::toString() const{
    return "MoveCustomer " + std::to_string(id) +" from" +std::to_string(srcTrainer)+"to "+ std::to_string(dstTrainer)+ std::to_string(getStatus());
}

BaseAction* MoveCustomer::clone() const{
    return new MoveCustomer(*this);
}


Close::Close(int id):BaseAction(),trainerId(id){}
void Close::act(Studio &studio){
    if(studio.getTrainer(trainerId)!= nullptr && studio.getTrainer(trainerId)->isOpen()) {
        Trainer *t1 = studio.getTrainer(trainerId);
        t1->closeTrainer();
        complete();
        std::cout << "Trainer " + std::to_string(trainerId) + " closed. Salary " + std::to_string(t1->getSalary()) + " NIS"<< std::endl;
    }
    else
        error("Trainer does not exist or is not open");
}





std::string Close::toString() const{
    return "Close "+ std::to_string(trainerId)+ " "+ std::to_string(getStatus());
}

BaseAction* Close::clone() const{
    return new Close(*this);
}

CloseAll::CloseAll(): BaseAction(){}
void CloseAll::act(Studio &studio) {
    for (int i = 0; i < studio.getNumOfTrainers(); i++) {
        if (studio.getTrainer(i)->isOpen()) {
            Close closeTr = Close(i);
            closeTr.act(studio);
        }
    }
    studio.close();
    complete();
}

std::string CloseAll::toString() const{
    return "CloseAll " + std::to_string(getStatus());
}

BaseAction* CloseAll::clone() const{
    return new CloseAll(*this);
}

PrintWorkoutOptions::PrintWorkoutOptions():BaseAction(){}


void PrintWorkoutOptions::act(Studio &studio){
    for(unsigned int i =0 ; i<studio.getWorkoutOptions().size() ; i++){
        std::cout<<studio.getWorkoutOptions()[i].getName()+ ", " + std::to_string(studio.getWorkoutOptions()[i].getType())+
                   ", " +std::to_string(studio.getWorkoutOptions()[i].getPrice())<<std::endl; ///Split the the command for convenience
    }
    complete();
}

std::string PrintWorkoutOptions::toString() const{
    return "PrintWorkoutOptions Completed";///this action never results in an error
}

BaseAction* PrintWorkoutOptions::clone() const{
    return new PrintWorkoutOptions(*this);
}

PrintTrainerStatus::PrintTrainerStatus(int id) : BaseAction(),trainerId(id){}

void PrintTrainerStatus::act(Studio &studio){
    Trainer *t1 = studio.getTrainer(trainerId);
    if(t1->isOpen()){
        std::cout<<"Trainer " + std::to_string(trainerId) + " status: open"<<std::endl;
        std::cout<<"Customers:"<<std::endl;
        for(unsigned int i=0; i<t1->getCustomers().size(); i++){
            std::cout<<std::to_string(t1->getCustomers()[i]->getId())+ " " + t1->getCustomers()[i]->getName()<<std::endl; ///Split the the command for convenience
        }
        for(unsigned int i=0; i<t1->getOrders().size(); i++)
            std::cout<<t1->getOrders()[i].second.getName() + " " +std::to_string(t1->getOrders()[i].second.getPrice())+
                       " NIS " + std::to_string(t1->getOrders()[i].first)<<std::endl;
        std::cout<<"Current Trainer's Salary: "+std::to_string(t1->getSalary())+" NIS"<<std::endl;
        complete();
    }
    else
        std::cout<<"Trainer " + std::to_string(trainerId) + " status: close"<<std::endl;
}

std::string PrintTrainerStatus::toString() const{
    return "PrintTrainerStatus "+ std::to_string(trainerId) + "Completed";
}

BaseAction* PrintTrainerStatus::clone() const{
    return new PrintTrainerStatus(*this);
}



PrintActionsLog::PrintActionsLog():BaseAction(){}
void PrintActionsLog::act(Studio &studio){
    for(unsigned int i=0; i<studio.getActionsLog().size(); i++)
        std::cout<<studio.getActionsLog()[i]->toString()<<std::endl;
}

std::string PrintActionsLog::toString() const{
    return "PrintsActionsLog Completed";
}

BaseAction* PrintActionsLog::clone() const{
    return new PrintActionsLog(*this);
}


BackupStudio::BackupStudio():BaseAction(){ }
void BackupStudio::act(Studio &studio){
    if(backup!= nullptr)
        delete backup;
    backup = new Studio(studio);
    studio.backup = backup;
    complete();
}

std::string BackupStudio::toString() const{
    return "Backup Completed";
}

BaseAction* BackupStudio::clone() const{
    return new BackupStudio(*this);
}

RestoreStudio::RestoreStudio():BaseAction(){}
void RestoreStudio::act(Studio &studio){
    if(backup != nullptr){
        studio = Studio(*backup);
        complete();
    }
    else
        error("No backup available");
}

std::string RestoreStudio::toString() const{
    return "Restore Studio" + std::to_string(getStatus());
}

BaseAction* RestoreStudio::clone() const{
    return new RestoreStudio(*this);
}