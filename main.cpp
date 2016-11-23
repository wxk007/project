#include <iostream>
#include <cstring>
#include "app.h"
#include "PracticalSocket.h" // For Socket and SocketException
//this file is the RSH Client
int main() {
    //the following variables is for TCP socket client
    std::string servAddress = "localhost";
    char* echoString=NULL;
    unsigned short echoServPort=1234;
    //the following variables is for getting command
    std::string command;
    app a;
    //starting to get the input of the command from console
    command=a.start();
    while(command!="exit")
    {
        //this part, I detect if the input command is exit, if so, the process would terminate.
        //I inread the input command and transfer the structure to char* array in order to send them
        echoString=(char*)command.data();
        int echoStringLen = strlen (echoString);
        try
        {
            //create socket and send the command here.
            TCPSocket sock (servAddress, echoServPort);
            sock.send (echoString, echoStringLen);
            char echoBuffer[1000]; // Buffer for echo string + \0, I set the length of char array to 1000 in order to receive enough input
            int bytesReceived = 0;           // Bytes read on each recv()
            // Receive the same string back from the server
            cout << "Received: "; // Setup to print the echoed string
                //in our project, the received message should be
                // the result of the command, so the length of the string might not be 321
                // Receive up to the buffer size bytes from the sender.
                // in order to set enough memory in cache for the
                // received message, I set the length to 1000.
                if ((bytesReceived = (sock.recv(echoBuffer, 1000))) <= 0) {
                    cerr << "Unable to read";
                    exit(1);
                }
                //output the size of the result of the commands
                cout<<"the size of the result is:"<<bytesReceived<<endl;
                echoBuffer[bytesReceived] = '\0';    // Terminate the string!
                cout << echoBuffer;                  // Print the echo buffer
            cout<<endl;
        }
            //catch the exception
        catch (SocketException &e)
        {
            cerr << e.what () << endl;
            exit (1);
        }
        //start next round.
        command=a.start();
    }
    return 0;

}