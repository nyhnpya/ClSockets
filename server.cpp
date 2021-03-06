//------------------------------------------------------------------------------
// ECHO SERVER CODE
//------------------------------------------------------------------------------
#define _LINUX
#include <iostream>
using namespace std;
#include "PassiveSocket.h"       // Include header for active socket object definition

#define MAX_PACKET 4096 

int main(int argc, char **argv)
{
    CPassiveSocket socket;
    CActiveSocket *pClient = NULL;

    //--------------------------------------------------------------------------
    // Initialize our socket object 
    //--------------------------------------------------------------------------
    socket.Initialize();
                                        
    socket.Listen((const uint8 *)"127.0.0.1", 6789);

    while (true)
    {
        if ((pClient = socket.Accept()) != NULL)
        {
                                
            //------------------------------------------------------------------
            // Receive request from the client.
            //------------------------------------------------------------------
            if (pClient->Receive(MAX_PACKET))
            {
                // ERROR HERE # 7
                //--------------------------------------------------------------
                // Send response to client and close connection to the client.
                //--------------------------------------------------------------
                pClient->Send((const uint8 *)pClient->GetData(),
                              pClient->GetBytesReceived());
                
                pClient->Close();
            }

            delete pClient;
        }
    }

    //--------------------------------------------------------------------------
    // Receive request from the client.
    //--------------------------------------------------------------------------
    socket.Close();

    return 1;
}
