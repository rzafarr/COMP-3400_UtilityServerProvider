/*
call class and functions from both business and user end 

*/
#include "utilityClass.hpp"


int main(){
    Utility company = Utility::createCompany();

    company.displayServices();

    return 0;

}