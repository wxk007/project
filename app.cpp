//
// Created by vagrant on 18.11.16.
//

#include "app.h"
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <system_error>
#include <unistd.h>
#include <vector>
#include <string>
#include <sstream>
#include <string.h>
#include <stdlib.h>
using namespace std;
app::app()
{
    std::cout<<"============================="<<std::endl;
    std::cout<<"welcome to our final project"<<std::endl;
    std::cout<<"============================="<<std::endl;
}

app::~app()
{
    std::cout<<"============================="<<std::endl;
    std::cout<<"closing the project"<<std::endl;
    std::cout<<"============================="<<std::endl;
}

std::string app::start()
{
    string command_str;
    cout << "\nvbash>>";
    getline (cin, command_str);
    cout << "Command Entered: " << command_str << endl;
    std::vector < std::string > command_options;
    //Read the command string till the end of the line.

    // With the "exit" command it exits the program
    while (command_str != "exit")
    {
        return command_str;
    }
    return "exit";
}

