#include "utilityClass.hpp"
#include "servicePlanClass.hpp"
#include "customerClass.hpp"

int main(){
    string name,address,phone;
    int status;

    do{
        //loop to create a company
        cout << "Enter an value to start or enter 0 to end\n";
        cin >> status;

        if(status == 0){
            break;
        }
        //create company:
        //Name of company
        cout << "Enter the name of the company\n";
        cin >> name;
        //Address of the company
        cout<<"Enter the address\n";
        cin.ignore();
        getline(cin,address);
        cout << address;
        //Phone number of company
        cout << "Enter the phone number\n";
        cin.ignore();
        getline(cin,phone);
        cin.ignore();

        //create a company object
        Utility company (name,address, phone);

        //view company information such as name,address, and phone number
        company.viewCompanyInfo();
        //add service such as hydro, internet and gas
        company.addServiceType();
        //view the services that have been added
        company.viewServiceType();


        //after selecting the type of service 
        //add the plans 
        ServicePlan servicePlan (&company);
        servicePlan.addPlan();
        servicePlan.viewPlan();

    }
    while(status !=0);

}