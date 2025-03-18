#include "utilityClass.hpp"
#pragma once

using namespace std; 

struct Plan{
    string details;
    double cost;
};


class ServicePlan{
    private:
    //string => mobile phone, electric, tv, home phone...
    //map holds:
        //key: plan id 
        //value: stored in a list that hold plan detials and cost
    map<string,map<int,Plan>>serviceFeature;

    //get the service type from utility class
    //service type has-a service plan 
    //https://dotnettutorials.net/lesson/isa-and-hasa-relationship-in-cpp/
    Utility* utility;

    public:
    //cosntructor: pass in the utility company object 
    //so that each plan is related to that company
    ServicePlan (Utility* utility){
        this->utility = utility;
    }
    //method that adds features: such as plan type and cost
    void addPlan(){
        int planID;
        string serviceType, planDetail,service;
        double cost;


        //check what type of service the given company has:
        //view the services they have
        cout<<"Service offered by: " << utility->getName()<<endl;
        for(const auto& se:utility->getServiceType()){
            cout<<se<<endl;
        }
        
        cout << "Enter the service type:(Hydro, Internet, Gas)\n";
        cin.ignore();
        getline(cin,service);
      
        
        //check if the entered service type exists 
            bool validService = false;
            for (const auto& se : utility->getServiceType()) {
                if (se == service) {
                    validService = true;
                    break;
                }
            }

            // If the service is valid, ask for the plan details, else prompt for a valid service
            if (validService) {
                cout << "Valid service type selected: " << service << endl;
                //hydro:
                if(service == "Hydro"){
                    cout <<"Enter 1 for Electric\nEnter 2 for Water\nEnter 3 for Sewage\n";
                    int serviceOption;
                    cin >> serviceOption;

                    if(serviceOption == 1){
                        serviceType = "Electric";
                    }
                    else if(serviceOption ==2){
                        serviceType = "Water";
                    }
                    else if(serviceOption == 3){
                        serviceType = "Sewage";
                    }
                    else{
                        cout << "invalid option select again\n";
                    }
                }
                //internet
                else if(service == "Internet"){
                    cout <<"Enter 1 for Mobile\nEnter 2 for Home\nEnter 3 for TV\n";
                    cin >> serviceType;
                    int serviceOption;
                    cin >> serviceOption;

                    if(serviceOption == 1){
                        serviceType = "Mobile Phone";
                    }
                    else if(serviceOption ==2){
                        serviceType = "Home Phone";
                    }
                    else if(serviceOption == 3){
                        serviceType = "Tv";
                    }
                    else{
                        cout << "invalid option select again\n";
                    }

                    cout<<"Plan has been added to: Internet \n";
                }
                //gas
                else if (service == "Gas"){
                    cout <<"true on gas\n";
                    serviceType = "Natural Gas";
                }
                //invalid input
                else{
                    cout<<"invalid input\n";
                }

                //generate a random number from 0 to 100
                planID = rand() % 101;
                cin.ignore();
                cout <<"Enter a brief description about the plan:\n";
            
                getline(cin,planDetail);
                cout << "Enter the cost\n";
                cin >> cost;
                //add the plan 
                Plan newPlan;
                newPlan.details = planDetail;
                newPlan.cost = cost;
                serviceFeature[serviceType][planID] = newPlan;

                cout<<"Plan has been added to: " << serviceType<<"\n";
            } else {
                cout << "Invalid service type entered. Please try again with a valid service." << endl;
            }
        }

    //view the plan
    void viewPlan(){
        for(const auto& feature:serviceFeature){
            cout << feature.first << "-";
            for (const auto& plan : feature.second) {
                cout << "Plan ID: " << plan.first << "\n";
                cout << "   Details: " << plan.second.details << "\n";
                cout << "   Cost: $" << plan.second.cost << "\n";
            }
        }
    }

};