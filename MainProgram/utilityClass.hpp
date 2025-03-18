#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#pragma once

using namespace std;

class Utility{
    private:
    //attributes of a utility company contact information
    string name;
    string address;
    string phone;

    //attributes of a utility company service type: hydro,internet, gas
    vector<string>serviceType;

    public:
    //create a constructor that creates a utility company object
    Utility (string name, string address, string phoneNum){
        this->name = name;
        this->address = address;
        this->phone = phoneNum;
    }

    //method that view utility company info:
    void viewCompanyInfo(){
        cout << "Name:\t" << name;
        cout << "Address:\t" << address;
        cout << "Phone Number:\t" << phone;
    }

    //method that returns the name of the company
    string getName(){
        return name;
    }

    //method that allows the company to add service:
    void addServiceType(){
        cout << "\nSelect a service:\n";
        cout << "Enter 1 for Hydro services\nEnter 2 for Internet services\nEnter 3 for Gas services\nEnter 0 to go back to main menu\n";
        int selectedService;
        cin >> selectedService;
        cin.ignore();
        int value;
        //allow the user to select more than one service
        do{
            cout<<"Enter a value to select a service:\n";
            cin >> value;

            //loop through the serviceType array to check if the service select 
            //does not exist:
            bool dne = false;
            for(const auto& sType:serviceType){
                //checks if natural gas existed as a service
                if (value == 1 && sType == "Natural Gas"){
                    cout<<"Natural Gas has been already added\n";
                    dne = true;
                } 
                //check if hydro existed as a service
                else if (value ==2 && sType == "Hydro"){
                    cout<<"Hydro has been already added\n";
                    dne = true;
                }
                //check if internet existed as a service
                else if (value == 3 && sType == "Internet"){
                    cout << "Internet has been already added\n";
                    dne = true;
                //none of the of the services exists
                }else{
                    dne = false;
                }
        
            }
            //if the selected service does not exist then add
            if(dne == false){
                //add natural gas
                if (value == 1){
                    serviceType.push_back("Natural Gas");
                    cout<<"Added Natural Gas\n";
                }
                //add hydro
                else if (value == 2){
                    serviceType.push_back("Hydro");
                    cout<<"Added Hydro\n";
                }
                //add internet
                else if(value == 3){
                    serviceType.push_back("Internet");
                    cout<<"Added Internet\n";
                }
                //exit 
                else if (value == 0){
                    cout <<"Heading back to main menu...\n";
                }
                //invalid input
                else{
                    cout << "Please enter a valid input\n";
                }
            }
        //loop to allow the company to add as much service until they enter 0
        }while(value !=0);    
    }

    //method to view the company service type:
    void viewServiceType(){
        cout<<"Company Service:\t" << name << "\n";
        //loop through the vector of service (assuming that a company can offer more than one service)
        for(const auto& s:serviceType){
            cout<<s<<endl;
        }
    }

};
