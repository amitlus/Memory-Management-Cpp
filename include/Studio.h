#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"

class Trainer;
class BaseAction;
class Customer;
class Studio{
public:
    Studio();
    Studio(const std::string &configFilePath);

    Studio(const Studio &other);//Copy constructor
    virtual~Studio();//Destructor
    Studio &operator=(const Studio &other);//Copy assignment operator
    Studio(Studio &&other);//Move constructor
    Studio& operator=(Studio &&other);//Move assignment operator
    ///END OF RULE OF 5 :)
    Studio* backup = nullptr;
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    void close();///@
    //bool isOpen();///@



private:
    bool open;
    bool test;
    std::vector<Trainer *> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    int numOfTrainers;///@
};

#endif