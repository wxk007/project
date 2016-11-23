//
// Created by vagrant on 19.11.16.
//
//This file is the RSH server
#include "PracticalSocket.h" // For Socket, ServerSocket, and SocketException
#include <iostream>          // For cerr and cout
#include <cstdlib>           // For atoi()
#include <string.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <wait.h>
#include <sys/param.h>
#include "app.h"
using namespace std;
void HandleTCPClient (TCPSocket *sock); // TCP client handling function
//this execute function is used to execute command and send them back to the main.cpp
void execute(string basic_string , int descriptor);

int main()
{
    unsigned short echoServePort=1234;
    try
    {
        //this function do socket, bind, listen,
        TCPServerSocket servSock(echoServePort);

        for( ; ; )
        {
            HandleTCPClient(servSock.accept());
        }
    }
    catch(SocketException &e)
    {
        cerr << e.what () << endl;
        exit (1);
    }
    //will never reached
    return 0;
}

//Socket server handle function
void HandleTCPClient (TCPSocket *sock)
{
    cout<<"handling client ";
    try
    {
        cout << sock->getForeignAddress () << ":";
    }
    catch (SocketException e)
    {
        cerr << "Unable to get foreign address" << endl;
    }
    try
    {
        cout << sock->getForeignPort ();
    }
    catch (SocketException e)
    {
        cerr << "Unable to get foreign port" << endl;
    }
    cout << endl;
    //the echoBuffer is used to store the input command
    char echoBuffer[32];
    int recvMsgSize;
    //if the received message is 0, that means there's no more messages left in the socket, we could move on.
    while ((recvMsgSize = sock->recv (echoBuffer, 32)) > 0)
    { // Zero means
        // end of transmission
        // Echo message back to client
        string command=echoBuffer;
        cout << "received: " << command<< endl;
        int descriptor=sock->getsockDesc();
        execute(command,descriptor);
        cout<<"this part has been executed"<<endl;
        //eliminate the char array echoBuffer
        memset(echoBuffer,0,sizeof(echoBuffer));
    }
    //delete sock;
}

void execute(string str , int descriptor)
{
    //this execute function is used to execvp the command
    //firstly, I set a copy of the descriptor of stdin, stdout, stderr
    int stdin=dup(0);
    int stdout=dup(1);
    int stderr=dup(3);
    //then, I set the file descriptor 0,1,2 equal to the file descriptor of the socket.
    dup2(descriptor , 0);
    dup2(descriptor , 1);
    dup2(descriptor , 2);
    //then, I will do the fork and the execvp
    pid_t w;
    int status;
    istringstream iss (str);
    std::vector < string > command;
    //Command string can contain the main command and a number of command line arguments.
    do
    {
        string sub;
        iss >> sub;
        command.push_back (sub);
    }
    while (iss);
    char **args = (char **) malloc (command.size () * sizeof (char *));
    for (int i = 0; i < command.size (); i++)
    {
        args[i] = strdup (command[i].c_str ());

    }
    args[command.size ()-1] = 0;
    w=fork();
    //fork here to start a child process
    if(w==0){
        //in the child process, let's do the execvp
        std::cout<<"Command Output:"<<std::endl;
        if(execvp(args[0],args)<=0){
            //do execvp here and judge whether the function ran correctly.
            //What's more, I know that execvp could be done without a if function, but I prefer
            //to use if here to make it more reasonable
            std::cout<<"\nExec Failed "<<errno<<"\n";
            //if there's anything wrong with execvp, print exec failed
            exit(1);
            //the child process will exit 1 if there's something wrong with Exec
        }
    }
    else if(w<0){
        std::cout<<"\nFork Failed "<<errno<<"\n";
        //print error information if fork failed
    }
    else{
        free(args);
        int retvalue = 0;
        while (retvalue != w)
        {
            status = 0;
            retvalue = waitpid (w, &status, 0);
            //waitpid here and give the pid value of child process to retvalue
            if (retvalue < 0)
                //if there's something wrong with waitpid, we will do the following things
            {
                char buffer[256];
                strerror_r (errno, buffer, 256);
                printf ("error occured %s\n", buffer);
                //return the error information if waitpid failed.
                break;
            }
            else
            {
                if (WIFEXITED (status))
                    //check if the child process terminated by itself
                {
                    if(WEXITSTATUS(status)==0){
                        //check if the child process exited in a correct way
                        //if the exit code of the child process is 0, the child exited successfully
                        std::cout<<"\nChild Exited Successfully\n";
                        //print if child process exit correctly
                    }
                    else{
                        //if the exit code is not 0, this function would be executed
                        int exitcode=WEXITSTATUS(status);
                        //get the exit code
                        std::cout<<"\nChild Self Terminated With Exit Code "<<exitcode<<"\n";
                        //return the exitcode if child process exited in a wrong way
                    }
                }
                else if (WIFSIGNALED (status))
                    //check if the child process is terminated by a signal
                {
                    int signalsent=WTERMSIG(status);
                    //if so, get the signal code
                    std::cout<<"\nChild Terminated Due to Signal "<<signalsent<<"\n";
                    //print the signal code
                }
            }
        }
    }
    //then set the stdin, stdout, stderr back to the file descriptor 0,1,2
    dup2(stdin,0);
    dup2(stdout,1);
    dup2(stderr,2);

}

