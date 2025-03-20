#include "utilityClass.hpp"
#include "servicePlanClass.hpp"
#include "customerClass.hpp"


//function that creates company
void createUtilityCompany(vector<Utility>&companies, vector<ServicePlan>&plans){
    string name,address,phone;
    //loop to create a company
    cout << "Welcome to create company:\n";

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

//functoin that creates customer
void createCustomer(vector<Customer>&customers,vector<Utility>&companies, vector<ServicePlan>&plans){
    string name,address,phone;
    //loop to create a company
    cout << "Welcome to create Customer:\n";

    //create company:
    //Name of customer
    cout << "Enter customer name\n";
    cin >> name;
    //Address of the customer
    cout<<"Enter the address\n";
    cin.ignore();
    getline(cin,address);
    cout << address;
    //Phone number of customer
    cout << "Enter the phone number\n";
    cin.ignore();
    getline(cin,phone);
    cin.ignore();

    //create customer object:
    Customer customer(name,address,phone);
    //allow the customer to view companies:
    customer.listCompanies(companies);
    //allow the customer to view the plans:
    customer.listPlans(plans);
}


int main(){
    string name,address,phone;
    int status = 1;

    //a vector of 10 companies
    vector<Utility>companies;
    //vector of plans
    vector<ServicePlan>plans;
    //a vector of 100 customers
    vector<Customer>customers;
    //user type
    int user;
    do{
        cout<<"Enter 1 for company or 2 for customer\n";
        cin >> user;
        if(user == 1){
            createUtilityCompany(companies,plans);
        }
        else if (user == 2){
            createCustomer(customers,companies,plans);
        }
        else{
            cout<<"Please enter a valid option\n";
        }
        
        cout<< "Enter 0 to exit:\n";
        cin >> status;

    }
    while(status !=0);
}