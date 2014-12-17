/*---------------------------------------------------------------------------*/
/*                                                                           */
/* HTTPActiveSocket.h - Active Socket Decleration                            */
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
#ifndef __HTTPACTIVESOCKET_H__
#define __HTTPACTIVESOCKET_H__

#include <string>
#include "ActiveSocket.h"

using namespace std;

/// Provides a platform independent class to create an active socket.
/// An active socket is used to create a socket which connects to a server.  
/// This type of object would be used when an application needs to send/receive
/// data from a server. 
class CHttpActiveSocket : public CActiveSocket {

public:
	virtual Send(const uint8 *pBuf, size_t bytesToSend);
	virtual Receive();

private:
// date
	string  m_szServer;
// lastmodified
	int32   m_nContentLength;
	string  m_szHost;
	string  m_szContentType;
	string  m_bConnection;
	string  m_szSetCookie;
	string  m_szLocation;
	string  m_szCacheControl;
};

#endif // __HTTPACTIVESOCKET_H__
