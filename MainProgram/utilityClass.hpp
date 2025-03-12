#include <fstream> //read and write
#include <iostream> 
#include <string> 
#include <vector>


using namespace std;

/*
Class: Utility class
Purpose: 
    -To hold and create infromation about the company.This includes
        -Name of the company
        -Contact info 
        -Types of services 
Output:
    -creating a utility company object
*/
class Utility {
    private:
    //name of the utiltiy company
    string utilityName; 
    //utility company id
    int utilityID; 
    //vector of services: Internet, Gas, Hydro
    vector<string>servicesOffered;

    public:
    //Constructor that creates a utility company object
    Utility(string name) : utilityName(name){};


    static Utility createCompany(){
        string nameOfUtility;
        cout<<"Enter the name of your utility company:\n";
        cin >> nameOfUtility; 

        //create object:
        Utility newCompany(nameOfUtility);

        //call the selectServiceOptions method to display the types of services utility companies can offer
        newCompany.selectServiceOptions();
        int value;
        //allow the user to select more than one service
        do{
            cout<<"Enter a value:\t";
            cin >> value;

            if (value == 1){
                newCompany.addServices("Natural Gas");
                cout<<"Added 1\n";
            }
            else if (value == 2){
                newCompany.addServices("Hydro");
                cout<<"Added 2\n";
            }
            else if(value == 3){
                newCompany.addServices("Internet");
                cout<<"Added 3\n";
            }
            else{
                cout << "Please enter a valid input\n";
            }
        }while(value !=0);

        return newCompany;
      
    }
    //Method: shows what types of services can utility company offers
    void selectServiceOptions(){
        cout<<"Service Options:\n";
        cout<<"Enter 1: Natural Gas\nEnter 2:Internet Service\nEnter 3:Hydro\nEnter 0: To end selection\n";
    }

    //Method: adds a service to company 
    void addServices(const string& service){
        //loop through the current services and check if the "new service" already exists or not
        for(const auto& str : servicesOffered){
           if(str == service){
               return;
           }
        }
        //add this to vector of services 
        servicesOffered.push_back(service);

    }

    void displayServices(){
        cout<<"Services Offered by "<<utilityName<<"\n";
        //check if the service vector is empty
        if(servicesOffered.empty()){
            //display that there are no services 
            cout<<"There are no services available\n";
        }
        //if vector is not empty show the services
        else{
            for(const auto& str : servicesOffered){
                cout<<"Service: "<<str<<"\n";
            }
        }
    }
};




