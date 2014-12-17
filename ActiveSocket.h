/*---------------------------------------------------------------------------*/
/*                                                                           */
/* ActiveSocket.h - Active Socket Decleration                                */
/*                                                                           */
/* Author : Mark Carrier (mark@carrierlabs.com)                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Copyright (c) 2013 CarrierLabs, LLC. All Rights Reserved.                  *
 *                                                                            *
 * Permission is hereby granted, free of charge, to any person obtaining a    *
 * copy of this software and associated documentation files (the "Software"), *
 * to deal in the Software without restriction, including without limitation  *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,   *
 * and/or sell copies of the Software, and to permit persons to whom the      *
 * Software is furnished to do so, subject to the following conditions:       *
 *                                                                            *
 * The above copyright notice and this permission notice shall be included in *
 * all copies or substantial portions of the Software.                        *
 *                                                                            *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS   *
 *  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF                *
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN *
 *  NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,  *
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR     *
 *  OTHERWISE, ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE  *
 *  USE OR OTHER DEALINGS IN THE SOFTWARE.                                    *
 *                                                                            *
 *----------------------------------------------------------------------------*/
#ifndef __ACTIVESOCKET_H__
#define __ACTIVESOCKET_H__

#include "SimpleSocket.h"

class CPassiveSocket;

/// Provides a platform independent class to create an active socket.
/// An active socket is used to create a socket which connects to a server.  
/// This type of object would be used when an application needs to send/receive
/// data from a server. 
class CActiveSocket : public CSimpleSocket {
public:
    friend class CPassiveSocket;

    CActiveSocket(CSocketType type = SocketTypeTcp);
    virtual ~CActiveSocket() { Close(); };

    /// Established a connection to the address specified by pAddr.  
    /// Connection-based protocol sockets (CSocket::SocketTypeTcp) may 
    /// successfully call Open() only once, however; connectionless protocol 
    /// sockets (CSocket::SocketTypeUdp) may use Open() multiple times to 
    /// change their association.  
    ///  @param pAddr specifies the destination address to connect.
    ///  @param nPort specifies the destination port.
    ///  @return true if successful connection made, otherwise false.
    virtual bool Open(const uint8 *pAddr, int16 nPort);

private:
	/// Utility function used to create a TCP connection, called from Open().
    ///  @return true if successful connection made, otherwise false.
    bool ConnectTCP(const uint8 *pAddr, int16 nPort);

	/// Utility function used to create a UDP connection, called from Open().
    ///  @return true if successful connection made, otherwise false.
    bool ConnectUDP(const uint8 *pAddr, int16 nPort);

	/// Utility function used to create a RAW connection, called from Open().
    ///  @return true if successful connection made, otherwise false.
    bool ConnectRAW(const uint8 *pAddr, int16 nPort);

 private:
    struct hostent *m_pHE;
};

#endif /*  __ACTIVESOCKET_H__  */

