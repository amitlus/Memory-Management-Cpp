#include "../include/Studio.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cctype>
#include <typeinfo>
#include <string>

Studio::Studio(){}

std::vector<std::string> splitStr(std::string &line){
    std::vector<std::string> vec;
    std::stringstream str(line);

    while(str.good()){
        std::string subStr;
        getline(str,subStr,',');
        vec.push_back(subStr);
    }
    return vec;
}

void makeCustomerList(std::string cusList, std::vector<Customer*>& CustomerListPointers, int customerId){
    int space=0;
    while(!cusList.empty()&&space!=-1){
        int psik = cusList.find(',');
        space = cusList.find(' ');
        std::string cusName = cusList.substr(0,psik);///finding the name of the customer
        std::string cusType = cusList.substr(psik+1 ,space-psik-1); ///finding the customer type;
        if(cusType=="swt")
            CustomerListPointers.push_back(new SweatyCustomer(cusName, customerId));
        else if(cusType=="chp")
            CustomerListPointers.push_back(new CheapCustomer(cusName, customerId));
        else if(cusType=="mcl")
            CustomerListPointers.push_back(new HeavyMuscleCustomer(cusName, customerId));
        else if(cusType=="fbd")
            CustomerListPointers.push_back(new FullBodyCustomer(cusName, customerId));
        cusList=cusList.substr(space+1);
        customerId++;
    }
}








Studio::Studio(const std::string &configFilePath): trainers(), workout_options(), actionsLog(),open(false){
    std::ifstream file(configFilePath);
    std::string line;
    int i=0;
    while(file){///Getting rid of the unnecessary lines
        getline(file, line);
        if(line[0] =='#' || line[0]=='\0')
            continue;
    char psik=',';
        if (i == 0) {
            numOfTrainers = std::stoi(line);
            i++;
        }
        else if (i == 1) {
            std::string trainerCap;
            int j=0;
            while(line[j]!='\0'){
                if (line[j] == psik || line[j]==' ') {
                    j++;
                }
                else{
                    while((line[j] != psik && line[j] != ' ')&&line[j]!='\0'){
                        trainerCap+=line[j];
                        j++;
                    }
                    trainers.push_back(new Trainer(std::stoi(trainerCap)));
                    trainerCap="";
                }
            }
            i++;
        }

        else {
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace),line.end()); /// removing the spaces

            std::vector<std::string> vec = splitStr(line);
           if(vec[1]=="Anaerobic")
               workout_options.push_back(Workout (i-2, vec[0], std::stoi(vec[2]), ANAEROBIC));
           else if(vec[1]=="Mixed")
               workout_options.push_back(Workout (i-2, vec[0], std::stoi(vec[2]), MIXED));
           else
               workout_options.push_back(Workout (i-2, vec[0], std::stoi(vec[2]), CARDIO));
            i++;

        }
    }
    file.close();
};





void Studio::start() {
    open = true;
    std::cout << "Studio is now open!" << std::endl;
    std::string act;
    int customerId=0;
    do{
        std::getline(std::cin, act);
        std::size_t fOpen = act.find("open");
        std::size_t fClose = act.find("close");
        std::size_t fStat = act.find("status");
        std::size_t fMove = act.find("move");
        std::size_t fOrder = act.find("order");
        std::size_t fRestore = act.find("restore");
        std::size_t fBackUp = act.find("backup");
        std::size_t fLog = act.find("log");
        std::size_t fWork = act.find("workout");
        std::size_t fCloseALl = act.find("closeall");

        if (fOpen != std::string::npos) {
            int fSpace = act.find(' ');
            act = act.substr(fSpace + 1);///getting rid of the command - open.
            int sSpace = act.find(' ');
            std::string trainerId = act.substr(0, sSpace);
            act = act.substr(sSpace + 1); ///making the act string only the customers to create a customer list from it.
            std::vector<Customer *> makeCus;
            makeCustomerList(act, makeCus, customerId);
            OpenTrainer *ot = new OpenTrainer(std::stoi(trainerId), makeCus);
            ot->act(*this);
            int nextCustomerId = ot->getNextCustomerId();
            if (nextCustomerId != -1) ///checking for an empty opening
                customerId = nextCustomerId;
            else
                customerId = makeCus.size();

            for (unsigned int i = 0; i < makeCus.size(); i++) {
                if (makeCus[i]->getId() >= nextCustomerId) {
                    delete makeCus[i];
                    makeCus[i] = nullptr;
                }
            }
            actionsLog.push_back(ot);
        }

        else if (fClose != std::string::npos && act!="closeall") {
            int space = act.find(' ');
            int id= std::stoi(act.substr(space+1));
            Close *close1 = new Close(id);
            close1->act(*this);
            actionsLog.push_back(close1);
        }

        else if (fStat != std::string::npos) {
            int fSpace = act.find(' ');
            std::string trainerId = act.substr(fSpace + 1);
            PrintTrainerStatus *pts = new PrintTrainerStatus(std::stoi(trainerId));
            pts->act(*this);
            actionsLog.push_back(pts);
        }

        else if (fMove != std::string::npos) {
            int fSpace = act.find(' ');
            act=act.substr(fSpace+1); // we got rid of the command word
            int sSpace=act.find(' ');
            std::string srcId = act.substr(0, sSpace);
            act=act.substr(sSpace+1); // we got rid of the command word
            int tSpace = act.find(' ');
            std::string dstId = act.substr(0, tSpace);
            act=act.substr(tSpace+1);
            std::string cusId = act.substr(0);
            MoveCustomer *moveCus = new MoveCustomer(std::stoi(srcId), std::stoi(dstId), std::stoi(cusId));
            moveCus->act(*this);
            actionsLog.push_back(moveCus);
        }

        else if (fOrder != std::string::npos) {
            int fSpace = act.find(' '); ///finding the first space so we can tell which trainer we need to open
            std::string trainerId = act.substr(fSpace + 1); //getting the number from the input
            int id = std::stoi(trainerId);
            Order *ord =new Order(id);
            ord->act(*this);
            actionsLog.push_back(ord);
        }

        else if (fRestore != std::string::npos) {
            RestoreStudio *rs = new RestoreStudio();
            rs->act(*this);
            actionsLog.push_back(rs);

        }

        else if (fBackUp != std::string::npos) {
            BackupStudio *bu =  new BackupStudio();
            actionsLog.push_back(bu);
            bu->act(*this);
        }

        else if (fLog != std::string::npos) {
            PrintActionsLog *pal = new PrintActionsLog();
            pal->act(*this);
            actionsLog.push_back(pal);
        }

        else if(fCloseALl!=std::string::npos){
                CloseAll *close = new CloseAll();
                close->act(*this);
//                delete this;
            actionsLog.push_back(close);

        }
        else if(fWork!=std::string::npos){
            PrintWorkoutOptions* wo = new PrintWorkoutOptions();
            wo->act(*this);
            actionsLog.push_back(wo);
        }
    }while (act!="closeall");

}



int Studio::getNumOfTrainers() const {
    return trainers.size();
}

Trainer* Studio::getTrainer(int tid) {
    unsigned int i=trainers.size();
    if(tid>=0 &&  tid<=i)
        return trainers[tid];
    return nullptr;
}



const std::vector<BaseAction*>& Studio::getActionsLog() const {
    return actionsLog;
}



std::vector<Workout>& Studio::getWorkoutOptions() {
    return workout_options;
}


void Studio::close(){
    if(open)
        open = false;
    std::cout<<"Studio is now closed!"<<std::endl;
}






///------------------------------------RULE OF 5--------------------------------------

///CHECK IF THE BACKUP IS CORRECT
Studio::Studio(const Studio &other):open(other.open),numOfTrainers(other.numOfTrainers), trainers(), workout_options(other.workout_options),actionsLog(){//copy constructor

    for(unsigned int i=0; i<other.trainers.size();i++)
        trainers.push_back(new Trainer(*other.trainers[i]));


    for(unsigned int i=0; i<other.actionsLog.size();i++)
        actionsLog.push_back(other.actionsLog[i]->clone());

}


Studio::~Studio() {//destructor
    for(unsigned int i=0;i<trainers.size();i++) {
        if (trainers[i] != nullptr) {
            delete trainers[i];
            trainers[i] = nullptr;
        }
    }
    trainers.clear();

    for(unsigned int i=0;i<actionsLog.size();i++) {
        if (actionsLog[i] != nullptr) {
            delete actionsLog[i];
            actionsLog[i] = nullptr;
        }
    }
    actionsLog.clear();

    workout_options.clear();
    numOfTrainers=0;
}

Studio& Studio::operator=(const Studio &other){//copy assignment operator
    if(this!=&other) {
        open = other.open;
        numOfTrainers = other.numOfTrainers;

        for(unsigned int i=0;i<trainers.size();i++) {
            if (trainers[i] != nullptr) {
                delete trainers[i];
                trainers[i] = nullptr;
            }
        }
        trainers.clear();

        for (unsigned int i = 0; i < other.trainers.size(); i++)
            trainers.push_back(new Trainer(*other.trainers[i]));

        actionsLog.clear();

        for (unsigned int i = 0; i < other.actionsLog.size(); i++)
            actionsLog.push_back(other.actionsLog[i]->clone());
    }
    return *this;
}

Studio::Studio(Studio &&other):open(other.open),numOfTrainers(other.numOfTrainers),trainers(other.trainers),workout_options(other.workout_options),actionsLog(other.actionsLog){//move constructor
}


Studio& Studio::operator=(Studio &&other){//move assignment operator
    open = other.open;
    numOfTrainers = other.numOfTrainers;


    trainers.clear();

    trainers = other.trainers;
    workout_options=other.workout_options;
    actionsLog = other.actionsLog;

    return *this;

}
