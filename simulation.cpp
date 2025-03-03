#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "shopQueue.h"
#include "customer.h"
#include <vector>
#include <iomanip>

// Define a function to write data to the output file
void outPrint(std::string output, std::vector<shared_ptr<Customer>> completed) {
    ofstream out_stream;
    out_stream.open(output);
    out_stream << setw(8) << "ID" << setw(8) << "Arrive" << setw(8) << "Start" << setw(8) << "Leave" << endl;
    int total = completed.size();
    for (int j = 0; j < total; j++)
    {
        out_stream << setw(8) << completed[j]->ID << setw(8) << completed[j]->arrivalTime << setw(8) << completed[j]->startTime << setw(8) << completed[j]->completedTime << endl;
    }
    out_stream.close();
}

int main(int argc, char *argv[])
{
//Check if the number of command-line arguments is correct
    if (argc != 7)
    {
        cout << "Error: Wrong number of arguments!" << endl;
        cout << "Usage: /run_simulation <seed> <total_min> <arrival_prob> <order_prob> <num_queues> <outputFile>" << endl;
        return 0;
    }


    // Parse command-line arguments
    int seed = std::atoi(argv[1]);
    int total_min = std::atoi(argv[2]);
    float arrive_prob = std::atof(argv[3]);
    float order_prob = std::atof(argv[4]);
    int num_queues = std::atoi(argv[5]);
    std::string outputFile = argv[6];

    int ID = 0;
    int time = 0;

    srand(seed);

    vector<ShopQueue> queues;
    queues.resize(num_queues);
    std::vector<int> cashier_served;
    std::shared_ptr<Customer> previousCust[num_queues];
    cashier_served.resize(num_queues);
    vector<shared_ptr<Customer>> completed;

    queues.resize(num_queues);

    // Add the first customer to the first queue at time 0
    queues[0].addNewCustomer(time, 0, 0, ID);
    ID++;

    // Display welcome message
    std::cout << "Welcome to Gompei Coffee Shop!" << std::endl;
    std::cout << "-- # of Checkout Queues: " << num_queues << " --" << std::endl;

    // Display information about the first customer
    std::cout << "New customer at t = 0." << std::endl;

    // Simulation loop
    while(time < total_min) {
        float randNum = (float)(rand()) / (float)(RAND_MAX);
        if (randNum < arrive_prob)
        {
            int min = 0;
            for (int i = 0; i < num_queues; i++)
            {
                if (queues[i].getLength() < queues[min].getLength()) { min = i; }
            }
            queues[min].addNewCustomer(time, 0, 0, ID);
            std::cout << "New customer at t = " << time << "." << std::endl;
            ID++;

        } // adding customers loop

        // Loop through all queues
        for (int queueIndex = 0; queueIndex < num_queues; queueIndex++)
            if (!queues[queueIndex].isEmpty())
            {
                auto front_Customer = queues[queueIndex].peekFrontCustomer();
                if (previousCust[queueIndex] != front_Customer && front_Customer->startTime == 0)
                {
                    front_Customer->startTime = time;
                }
                previousCust[queueIndex] = queues[queueIndex].peekFrontCustomer();
                randNum = (float)(rand()) / (float)(RAND_MAX);
                if (randNum < order_prob)
                {
                    front_Customer->completedTime = time;
                    std::cout << "Customer #" << front_Customer->ID << " was served from t = " << front_Customer->startTime << " to " << front_Customer->completedTime << "." << std::endl;
                    queues[queueIndex].removeFrontCustomer();

                    if (!queues[queueIndex].isEmpty())
                    {
                        auto front_Customer = queues[queueIndex].peekFrontCustomer();
                        if (front_Customer->startTime == 0)
                        {
                            front_Customer->startTime = time;
                        }
                    }
                    completed.push_back(front_Customer);
                    cashier_served[queueIndex]++;
                }


            }
        time++;

    }

    // Display the number of customers remaining in each queue after the simulation
    for (int i = 0; i < num_queues; i++)
    {
        int queue_length = queues[i].getLength();

        std::cout << "After " << total_min << " minutes, " << queue_length << " customers remain in queue #" << i << std::endl;
    }

    for (int i = 0; i < num_queues; i++)
    {
        int servedCustomers = cashier_served[i];
        std::cout << "Cashier " << i << " served " << servedCustomers << " customers" << std::endl;
    }

    outPrint(outputFile, completed);

} // main

