/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Socket.h - Passive Socket Decleration.                                    */
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
#ifndef __PASSIVESOCKET_H__
#define __PASSIVESOCKET_H__
#include "ActiveSocket.h"

/// Provides a platform independent class to create a passive socket.
/// A passive socket is used to create a "listening" socket.  This type
/// of object would be used when an application needs to wait for 
/// inbound connections.  Support for CSimpleSocket::SocketTypeTcp, 
/// CSimpleSocket::SocketTypeUdp, and CSimpleSocket::SocketTypeRaw is handled
/// in a similar fashion.  The big difference is that the method 
/// CPassiveSocket::Accept should not be called on the latter two socket
/// types. 
class CPassiveSocket : public CSimpleSocket {
public:
    CPassiveSocket(CSocketType type = SocketTypeTcp);
    virtual ~CPassiveSocket() { Close(); };

    /// Extracts the first connection request on the queue of pending 
    /// connections and creates a newly connected socket.  Used with 
    /// CSocketType CSimpleSocket::SocketTypeTcp.  It is the responsibility of
    /// the caller to delete the returned object when finished.
    ///  @return if successful a pointer to a newly created CActiveSocket object
    ///          will be returned and the internal error condition of the CPassiveSocket 
    ///          object will be CPassiveSocket::SocketSuccess.  If an error condition was encountered
    ///          the NULL will be returned and one of the following error conditions will be set:
    ///    CPassiveSocket::SocketEwouldblock, CPassiveSocket::SocketInvalidSocket, 
    ///    CPassiveSocket::SocketConnectionAborted, CPassiveSocket::SocketInterrupted
    ///    CPassiveSocket::SocketProtocolError, CPassiveSocket::SocketFirewallError
    virtual CActiveSocket *Accept(void);

    /// Bind to a multicast group on  a specified interface, multicast group, and port
    /// 
    ///  @param pInterface - interface on which to bind.
    ///  @param pGroup - multicast group address to bind.
    ///  @param nPort - port on which multicast 
    ///  @return true if able to bind to interface and multicast group.
    ///      If not successful, the false is returned and one of the following error
    ///      condiitions will be set: CPassiveSocket::SocketAddressInUse, CPassiveSocket::SocketProtocolError, 
    ///      CPassiveSocket::SocketInvalidSocket.  The following socket errors are for Linux/Unix
    ///      derived systems only: CPassiveSocket::SocketInvalidSocketBuffer
    bool BindMulticast(const uint8_t *pInterface, const uint8_t *pGroup, int16_t nPort);

    /// Create a listening socket at local ip address 'x.x.x.x' or 'localhost'
    /// if pAddr is NULL on port nPort.
    /// 
    ///  @param pAddr specifies the IP address on which to listen.
    ///  @param nPort specifies the port on which to listen.
    ///  @param nConnectionBacklog specifies connection queue backlog (default 30,000)
    ///  @return true if a listening socket was created.  
    ///      If not successful, the false is returned and one of the following error
    ///      condiitions will be set: CPassiveSocket::SocketAddressInUse, CPassiveSocket::SocketProtocolError, 
    ///      CPassiveSocket::SocketInvalidSocket.  The following socket errors are for Linux/Unix
    ///      derived systems only: CPassiveSocket::SocketInvalidSocketBuffer
    virtual bool Listen(const uint8_t *pAddr, int16_t nPort, int32_t nConnectionBacklog = 30000);

    /// Attempts to send a block of data on an established connection.
    /// @param pBuf block of data to be sent.
    /// @param bytesToSend size of data block to be sent.
    /// @return number of bytes actually sent, return of zero means the
    /// connection has been shutdown on the other side, and a return of -1
    /// means that an error has occurred.  If an error was signaled then one
    /// of the following error codes will be set: CPassiveSocket::SocketInvalidSocket,
    /// CPassiveSocket::SocketEwouldblock, SimpleSocket::SocketConnectionReset, 
    /// CPassiveSocket::SocketInvalidSocketBuffer, CPassiveSocket::SocketInterrupted, 
    /// CPassiveSocket::SocketProtocolError, CPassiveSocket::SocketNotconnected
    /// <br>\b Note: This function is used only for a socket of type 
	/// CSimpleSocket::SocketTypeUdp
    virtual int32_t Send(const uint8_t *pBuf, size_t bytesToSend);

 private:
    struct ip_mreq  m_stMulticastRequest;   /// group address for multicast

};

#endif // __PASSIVESOCKET_H__
