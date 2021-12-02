#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    Trainer(int t_capacity);
    virtual ~Trainer();//Destructor
    Trainer(const Trainer &other);//Copy constructor
    const Trainer& operator=(const Trainer &other);//Assignment operator
    Trainer(Trainer &&other);//Move constructor
    const Trainer& operator=(Trainer&& other);//Move assignment operator
    void clear();
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    bool isOpen();
    bool hasFreeSpots();//WE ADDED
    void updateOrderList(std::vector<OrderPair> &list);//WE ADDED
    void updateSalary(int addToSalary);//WE ADDED

private:
    int salary;
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
};


#endif