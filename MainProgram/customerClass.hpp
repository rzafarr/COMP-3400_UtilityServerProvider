#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#pragma once

using namespace std;

//create a struct that will store customer utility service plans choice
struct CustomerChoice{
    string utilityCompanyName,serviceType,planInformation;
};


class Customer{
    private:
    //attributes of a customer
    string name;
    string address;
    string phone;

    public:
    //constructor that creates a customer object
    Customer(string name, string address, string phoneNum){
        this -> name = name;
        this -> address = address;
        this -> phone = phoneNum;
    }

    //method that views all of the existing companies with their information:
    void listCompanies(vector<Utility>& companies){
        for(auto& company:companies){
            company.viewCompanyInfo();
            company.viewServiceType();
        }
    }

    //method that views the corresponding plans:
    void listPlans(vector<ServicePlan>& plans){
        for(auto& plan:plans){
            plan.viewPlan();
        }
    }

    

};