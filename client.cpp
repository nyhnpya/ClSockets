//------------------------------------------------------------------------------
// Echo Server Client
//------------------------------------------------------------------------------
#include <iostream>
#include <string.h>
#include "StatTimer.h"
#define _LINUX

#include "ActiveSocket.h"       // Include header for active socket object definition

//#define TEST_PACKET "TestPacket"
#define TEST_PACKET "TestPacketTestPacketTestPacketTestPacketTestPacketTestPacketTestPacketTestPacketTestPacketTestPacket"
#define MAX_PACKET  1024

int main(int argc, char **argv)
{
    CActiveSocket socket;       // Instantiate active socket object (defaults to TCP).
    char          szData[MAX_PACKET];
    CStatTimer    timer;
    
    memset(szData, 0, MAX_PACKET);

    //--------------------------------------------------------------------------
    // Initialize our socket object
    //--------------------------------------------------------------------------
    for (int i = 0; i < 100; i++)
    {
    socket.Initialize();
    timer.Initialize();

    //--------------------------------------------------------------------------
    // Create a connection to the time server so that data can be sent
    // and received.
    //--------------------------------------------------------------------------
    if (socket.Open((const uint8 *)"localhost", 6789))
    {
        
        //----------------------------------------------------------------------
        // Send a requtest the server requesting the current time.
        //----------------------------------------------------------------------
        timer.SetStartTime();
        if (socket.Send((const uint8 *)TEST_PACKET, strlen(TEST_PACKET)))
        {
                                        
            //------------------------------------------------------------------
            // Receive response from the server.
            //------------------------------------------------------------------
            socket.Receive(100);
            timer.SetEndTime();

            strncpy(szData, (const char *)socket.GetData(), socket.GetBytesReceived());
            printf("packet = %s, time = %d ms, %d us\n", szData, 
                   timer.GetMilliSeconds(),
                   timer.GetMicroSeconds());

            //------------------------------------------------------------------
            // Close the connection.
            //------------------------------------------------------------------
            socket.Close();
        }
    }}


    return 1;
}

