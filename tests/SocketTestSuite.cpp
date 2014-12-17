#include "SocketTestSuite.h"
#include "../ActiveSocket.h"
#include "../PassiveSocket.h"

#ifdef _LINUX
  #include <pthread.h>
#endif

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( NameOfTest );

#define MAX_PACKET  4096
#define TEST_PACKET "Test Packet"

/*void NameOfTest::setUp(void)
{
}

void NameOfTest::tearDown(void)
{
}*/

THREAD_RETURN CreateTCPEchoServer(void *param)
{
    CPassiveSocket socket;
    CActiveSocket *pClient = NULL;
    struct thread_data *pData = (struct thread_data *)param;
    int            nBytesReceived = 0;

    socket.Initialize();
    socket.Listen((const uint8 *)pData->pszServerAddr, pData->nPort);

    if ((pClient = socket.Accept()) != NULL)
    {
        while (nBytesReceived != pData->nTotalPayloadSize)
        {
            if (nBytesReceived += pClient->Receive(pData->nNumBytesToReceive))
            {
                pClient->Send((const uint8 *)pClient->GetData(), pClient->GetBytesReceived());
            }
        }

        SLEEP(1);
        pClient->Close();

        delete pClient;
    }

    socket.Close();

    return (THREAD_RETURN_VALUE);
}

THREAD_RETURN CreateTCPEchoServerAsync(void *param)
{
    CPassiveSocket      socket;
    CActiveSocket      *pClient = NULL;
    struct thread_data *pData = (struct thread_data *)param;
    int                 nBytesReceived = 0;

    socket.Initialize();
    socket.Listen((const uint8 *)pData->pszServerAddr, pData->nPort);

    if ((pClient = socket.Accept()) != NULL)
    {
        pClient->SetNonblocking();
        while (nBytesReceived != pData->nTotalPayloadSize)
        {
            int nBytes;
            
            pClient->Select();
            nBytes = pClient->Receive(pData->nNumBytesToReceive);

            if (nBytes != CSimpleSocket::SocketError)
            {
                nBytesReceived += nBytes;
                pClient->Send((const uint8 *)pClient->GetData(), pClient->GetBytesReceived());
            }
        }

        SLEEP(1);
        pClient->SetBlocking();
        pClient->Close();

        delete pClient;
    }

    socket.Close();

    return (THREAD_RETURN_VALUE);
}

THREAD_RETURN CreateUDPEchoServer(void *param)
{
    CPassiveSocket socket(CSimpleSocket::SocketTypeUdp);

    socket.Initialize();
    if (socket.Listen((const uint8 *)"127.0.0.1", 6700))
    {
        if (socket.Receive(MAX_PACKET))
        {
            socket.Send((const uint8 *)socket.GetData(), socket.GetBytesReceived());
            socket.Close();
        }
    }

    socket.Close();

    return (THREAD_RETURN_VALUE);
}

void NameOfTest::TCPEchoClient(void)
{
    CActiveSocket socket;
    PTHREAD_T     thread;
    struct thread_data thData;

    thData.pszServerAddr = "127.0.0.1";
    thData.nPort = 6700;
    thData.nNumBytesToReceive = (int)strlen(TEST_PACKET);
    thData.nTotalPayloadSize = (int)strlen(TEST_PACKET);

    THREAD(&thread, 0, CreateTCPEchoServer, &thData, NULL, NULL);

    SLEEP(1);
    CPPUNIT_ASSERT(socket.Initialize() == true);
    CPPUNIT_ASSERT(socket.Open((const uint8 *)"127.0.0.1", 6700) == true);
    CPPUNIT_ASSERT_EQUAL(socket.Send((const uint8 *)TEST_PACKET, strlen(TEST_PACKET)), (int32)strlen(TEST_PACKET));
    CPPUNIT_ASSERT_EQUAL(socket.Receive((int32)strlen(TEST_PACKET)), (int32)strlen(TEST_PACKET));
    CPPUNIT_ASSERT(socket.Close() == true);
}

void NameOfTest::TCPEchoClientAsync(void)
{
    CActiveSocket socket;
    PTHREAD_T     thread;
    struct thread_data thData;

    thData.pszServerAddr = "127.0.0.1";
    thData.nPort = 6750;
    thData.nNumBytesToReceive = (int)strlen(TEST_PACKET);
    thData.nTotalPayloadSize = (int)strlen(TEST_PACKET);

    THREAD(&thread, 0, CreateTCPEchoServerAsync, &thData, NULL, NULL);

    SLEEP(1);
    CPPUNIT_ASSERT(socket.Initialize() == true);
    CPPUNIT_ASSERT(socket.Open((const uint8 *)"127.0.0.1", 6750) == true);
    CPPUNIT_ASSERT_EQUAL(socket.Send((const uint8 *)TEST_PACKET, strlen(TEST_PACKET)), (int32)strlen(TEST_PACKET));

    int  loopVar = 0;
    int  numBytes = 0;
    char pszBuffer[1024];

    memset(pszBuffer, 0, 1024);
    numBytes = socket.Receive(1024);

    if (numBytes != -1)
    {
        strncat(pszBuffer, (const char *)socket.GetData(), socket.GetBytesReceived());

        while (numBytes != strlen(TEST_PACKET))
        {
            numBytes += socket.Receive(1024);
            strncat(pszBuffer, (const char *)socket.GetData(), socket.GetBytesReceived());
            loopVar++;
        }
    }

    CPPUNIT_ASSERT(strlen(pszBuffer) == (int32)strlen(TEST_PACKET));
    CPPUNIT_ASSERT(socket.Close() == true);
}

void NameOfTest::TCPOneByteAtTime(void)
{
    CActiveSocket socket;
    PTHREAD_T     thread;
    char         *pIdx = TEST_PACKET;
    struct thread_data thData;
    char          pszBuffer[1024];
    int           numBytes;

    thData.pszServerAddr = "127.0.0.1";
    thData.nPort = 6789;
    thData.nNumBytesToReceive = 1;
    thData.nTotalPayloadSize = (int)strlen(TEST_PACKET);

    THREAD(&thread, 0, CreateTCPEchoServer, &thData, NULL, NULL);
    SLEEP(1);
    CPPUNIT_ASSERT(socket.Initialize() == true);
    CPPUNIT_ASSERT(socket.Open((const uint8 *)"127.0.0.1", 6789) == true);
    for (int i = 0; i <= (int32)strlen(TEST_PACKET); i++)
    {
        CPPUNIT_ASSERT_EQUAL(socket.Send((const uint8 *)pIdx, 1), 1);
        pIdx++;
    }

    int loopVar = 0;
    memset(pszBuffer, 0, 1024);
    numBytes = socket.Receive(1024);

    if (numBytes != -1)
    {
        strncat(pszBuffer, (const char *)socket.GetData(), socket.GetBytesReceived());

        while (numBytes != strlen(TEST_PACKET))
        {
            numBytes += socket.Receive(1024);
            strncat(pszBuffer, (const char *)socket.GetData(), socket.GetBytesReceived());
            loopVar++;
        }
    }

    CPPUNIT_ASSERT(strlen(pszBuffer) == (int32)strlen(TEST_PACKET));
    CPPUNIT_ASSERT(socket.Close() == true);
}

void NameOfTest::Writev(void)
{
    CActiveSocket socket;
    PTHREAD_T     thread;
    const char   *pStr1 = "Test";
    const char   *pStr2 = "Packet";
    struct thread_data thData;
    char          pszBuffer[1024];
    struct iovec  vector[2];

    thData.pszServerAddr = "127.0.0.1";
    thData.nPort = 6701;
    thData.nNumBytesToReceive = strlen(pStr1) + strlen(pStr2);
    thData.nTotalPayloadSize = strlen(pStr1) + strlen(pStr2);

    vector[0].iov_base = (void *)pStr1;
    vector[0].iov_len = strlen(pStr1);
    vector[1].iov_base = (void *)pStr2;
    vector[1].iov_len = strlen(pStr2);
    THREAD(&thread, 0, CreateTCPEchoServer, &thData, NULL, NULL);
    SLEEP(1);
    CPPUNIT_ASSERT(socket.Initialize() == true);
    CPPUNIT_ASSERT(socket.Open((const uint8 *)"127.0.0.1", 6701) == true);
    CPPUNIT_ASSERT_EQUAL(socket.Send((const struct iovec *)vector, 2), (int32)(strlen(pStr1) + strlen(pStr2)));
    CPPUNIT_ASSERT_EQUAL(socket.Receive(4), 4);
    memset(pszBuffer,0,1024);
    memcpy(pszBuffer, socket.GetData(), 4);
    CPPUNIT_ASSERT_EQUAL(strcmp(pszBuffer, pStr1), 0);
    CPPUNIT_ASSERT_EQUAL(socket.Receive(6), 6);
    memset(pszBuffer,0,1024);
    memcpy(pszBuffer, socket.GetData(), 6);
    CPPUNIT_ASSERT_EQUAL(strcmp(pszBuffer, pStr2), 0);

    CPPUNIT_ASSERT(socket.Close() == true);
}

void NameOfTest::UDPEchoClient(void)
{
    CActiveSocket socket(CSimpleSocket::SocketTypeUdp);
    PTHREAD_T     thread;

    THREAD(&thread, 0, CreateUDPEchoServer, NULL, NULL, NULL);

    SLEEP(1);
    CPPUNIT_ASSERT(socket.Initialize() == true);
    CPPUNIT_ASSERT(socket.Open((const uint8 *)"127.0.0.1", 6700) == true);
    CPPUNIT_ASSERT_EQUAL(socket.Send((const uint8 *)TEST_PACKET, strlen(TEST_PACKET)), (int32)strlen(TEST_PACKET));
    CPPUNIT_ASSERT_EQUAL(socket.Receive((int32)strlen(TEST_PACKET)), (int32)strlen(TEST_PACKET));
    CPPUNIT_ASSERT(socket.Close() == true);
}

void NameOfTest::CreateTCPSocket(void)
{
    CActiveSocket socket;

    CPPUNIT_ASSERT(socket.IsSocketValid() == false);
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSimpleSocket::SocketInvalidSocket);
    CPPUNIT_ASSERT_EQUAL(socket.GetBytesReceived(), -1);
    CPPUNIT_ASSERT(socket.GetData() == NULL);
    CPPUNIT_ASSERT_EQUAL(socket.GetReceiveTimeoutSec(), 0);
    CPPUNIT_ASSERT_EQUAL(socket.GetReceiveTimeoutUSec(), 0);
    CPPUNIT_ASSERT_EQUAL(socket.GetConnectTimeoutSec(), 1);
    CPPUNIT_ASSERT_EQUAL(socket.GetConnectTimeoutUSec(), 0);
    CPPUNIT_ASSERT_EQUAL(socket.GetSendTimeoutSec(), 0);
    CPPUNIT_ASSERT_EQUAL(socket.GetSendTimeoutUSec(), 0);
    CPPUNIT_ASSERT(socket.IsNonblocking() == false);
    CPPUNIT_ASSERT_EQUAL(socket.GetBytesSent(), -1);
}

void NameOfTest::CreateUDPSocket(void)
{
    CActiveSocket socket(CSimpleSocket::SocketTypeUdp);

    CPPUNIT_ASSERT(socket.IsSocketValid() == false);
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSimpleSocket::SocketInvalidSocket);
    CPPUNIT_ASSERT_EQUAL(socket.GetBytesReceived(), -1);
    CPPUNIT_ASSERT(socket.GetData() == NULL);
    CPPUNIT_ASSERT_EQUAL(socket.GetReceiveTimeoutSec(), 0);
    CPPUNIT_ASSERT_EQUAL(socket.GetReceiveTimeoutUSec(), 0);
    CPPUNIT_ASSERT_EQUAL(socket.GetConnectTimeoutSec(), 1);
    CPPUNIT_ASSERT_EQUAL(socket.GetConnectTimeoutUSec(), 0);
    CPPUNIT_ASSERT_EQUAL(socket.GetSendTimeoutSec(), 0);
    CPPUNIT_ASSERT_EQUAL(socket.GetSendTimeoutUSec(), 0);
    CPPUNIT_ASSERT(socket.IsNonblocking() == false);
    CPPUNIT_ASSERT_EQUAL(socket.GetBytesSent(), -1);
}

void NameOfTest::SetSendWindowsSizeActive(void)
{
    CActiveSocket socket(CSimpleSocket::SocketTypeTcp);

    CPPUNIT_ASSERT(socket.Initialize() == true);
    CPPUNIT_ASSERT(socket.SetSendWindowSize(32000) == 32000);
#ifdef _LINUX
    CPPUNIT_ASSERT(socket.GetSendWindowSize() == 64000);
#else
    CPPUNIT_ASSERT(socket.GetSendWindowSize() == 32000);
#endif
    CPPUNIT_ASSERT(socket.SetSendWindowSize(6000) == 6000);
#ifdef _LINUX
    CPPUNIT_ASSERT(socket.GetSendWindowSize() == 12000);
#else
    CPPUNIT_ASSERT(socket.GetSendWindowSize() == 6000);
#endif
}

void NameOfTest::SetReceiveWindowSizeActive(void)
{
    CActiveSocket socket(CSimpleSocket::SocketTypeTcp);

    CPPUNIT_ASSERT(socket.Initialize() == true);
    CPPUNIT_ASSERT(socket.SetReceiveWindowSize(32000) == 32000);
#ifdef _LINUX
    CPPUNIT_ASSERT(socket.GetReceiveWindowSize() == 64000);
#else
    CPPUNIT_ASSERT(socket.GetReceiveWindowSize() == 32000);
#endif
    CPPUNIT_ASSERT(socket.SetReceiveWindowSize(36000) == 36000);
#ifdef _LINUX
    CPPUNIT_ASSERT((unsigned int)socket.GetReceiveWindowSize() == (unsigned int)72000);
#else
    CPPUNIT_ASSERT(socket.GetReceiveWindowSize() == 36000);
#endif
}

void NameOfTest::InitializeTCPSocketActive(void)
{
    CActiveSocket socket;

    CPPUNIT_ASSERT(socket.Initialize() == true);
    CPPUNIT_ASSERT(socket.GetSocketError() == CSimpleSocket::SocketSuccess);
}

void NameOfTest::InitializeTCPSocketPassive(void)
{
    CPassiveSocket socket;

    CPPUNIT_ASSERT(socket.Initialize() == true);
    CPPUNIT_ASSERT(socket.GetSocketError() == CSimpleSocket::SocketSuccess);
}

void NameOfTest::InitializeUDPSocket(void)
{
    CActiveSocket socket;

    CPPUNIT_ASSERT(socket.Initialize() == true);
    CPPUNIT_ASSERT(socket.GetSocketError() == CSimpleSocket::SocketSuccess);
}


// Test to make sure
void NameOfTest::CloseOnce(void)
{
    CActiveSocket socket;
    
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSimpleSocket::SocketInvalidSocket);

    socket.Initialize();
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSimpleSocket::SocketSuccess);

    CPPUNIT_ASSERT(socket.Close() == true);
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSimpleSocket::SocketSuccess);

}

void NameOfTest::CloseTwice(void)
{
    CActiveSocket socket;

    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSimpleSocket::SocketInvalidSocket);
    CPPUNIT_ASSERT(socket.Close() == false);

    socket.Initialize();
    CPPUNIT_ASSERT(socket.Close() == true);
    CPPUNIT_ASSERT(socket.Close() == false);
}

void NameOfTest::CloseMany(void)
{
    CActiveSocket socket;

    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSimpleSocket::SocketInvalidSocket);
    
    CPPUNIT_ASSERT(socket.Close() == false);
    socket.Initialize();
    CPPUNIT_ASSERT(socket.Close() == true);
    CPPUNIT_ASSERT(socket.Close() == false);
    CPPUNIT_ASSERT(socket.Close() == false);
    CPPUNIT_ASSERT(socket.Close() == false);
    CPPUNIT_ASSERT(socket.Close() == false);
    CPPUNIT_ASSERT(socket.Close() == false);
}

void NameOfTest::OpenAllPaths(void)
{
    CActiveSocket socket;

    CPPUNIT_ASSERT(socket.Open((const uint8 *)"www.google.com", 80) == false);
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSimpleSocket::SocketInvalidSocket);

    socket.Initialize();
    CPPUNIT_ASSERT(socket.Open(NULL, 80) == false);
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSimpleSocket::SocketInvalidAddress);
    socket.Close();

    socket.Initialize();
    CPPUNIT_ASSERT(socket.Open((const uint8 *)"www.google.com", 0) == false);
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSimpleSocket::SocketInvalidPort);
    socket.Close();

    socket.Initialize();
    CPPUNIT_ASSERT(socket.Open((const uint8 *)"www.badipaddress.com", 80) == false);
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSimpleSocket::SocketInvalidAddress);
    socket.Close();
	CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSimpleSocket::SocketInvalidAddress);
}

void NameOfTest::ConnectSuccess(void)
{
    CActiveSocket socket;

    socket.Initialize();
    CPPUNIT_ASSERT(socket.Open((const uint8 *)"www.google.com", 80) == true);
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSimpleSocket::SocketSuccess);
    CPPUNIT_ASSERT(socket.Close() == true);
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSimpleSocket::SocketSuccess);
}

void NameOfTest::ConnectFail(void)
{
    CActiveSocket socket;

    socket.Initialize();
    CPPUNIT_ASSERT(socket.Open((const uint8 *)"localhost", 6100) == false);
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSimpleSocket::SocketConnectionRefused);
    CPPUNIT_ASSERT(socket.Close() == true);
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSimpleSocket::SocketConnectionRefused);
}

void NameOfTest::ConnectTimeout(void)
{
    CActiveSocket socket;

    socket.Initialize();
    socket.SetNonblocking();
    socket.SetConnectTimeout(1,0);
    CPPUNIT_ASSERT(socket.Open((const uint8 *)"www.google.com", 6000) == false);
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSimpleSocket::SocketTimedout);
}

void NameOfTest::SetSocketNonblocking()
{
    CActiveSocket socket;

    CPPUNIT_ASSERT(socket.SetNonblocking() == false);
    CPPUNIT_ASSERT(socket.GetSocketError() == CSimpleSocket::SocketInvalidSocket);

    socket.Initialize();
    CPPUNIT_ASSERT(socket.SetNonblocking() == true);
    socket.Close();
}

void NameOfTest::SetSocketBlocking()
{
    CActiveSocket socket;

    CPPUNIT_ASSERT(socket.SetBlocking() == false);
    CPPUNIT_ASSERT(socket.GetSocketError() == CSimpleSocket::SocketInvalidSocket);

    socket.Initialize();
    CPPUNIT_ASSERT(socket.SetBlocking() == true);
    socket.Close();
}

void NameOfTest::TranslateError()
{
    CActiveSocket socket;
#if 0
    errno = EXIT_SUCCESS;
    socket.TranslateSocketError();
    CPPUNIT_ASSERT(socket.GetSocketError() == CSocket::SocketSuccess);

    errno = ENOTCONN;
    socket.TranslateSocketError();
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSocket::SocketNotconnected);

    errno = EBADF;
    socket.TranslateSocketError();
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSocket::SocketNotconnected);

    errno = EACCES;
    socket.TranslateSocketError();
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSocket::SocketInvalidSocket);

    errno = EAFNOSUPPORT;
    socket.TranslateSocketError();
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSocket::SocketInvalidSocket);

    errno = EINVAL;
    socket.TranslateSocketError();
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSocket::SocketInvalidSocket);

    errno = EMFILE;
    socket.TranslateSocketError();
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSocket::SocketInvalidSocket);

    errno = ENFILE;
    socket.TranslateSocketError();
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSocket::SocketInvalidSocket);

    errno = ENOBUFS;
    socket.TranslateSocketError();
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSocket::SocketInvalidSocket);

    errno = ENOMEM;
    socket.TranslateSocketError();
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSocket::SocketInvalidSocket);

    errno = EPROTONOSUPPORT;
    socket.TranslateSocketError();
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSocket::SocketInvalidSocket);

    errno = ECONNREFUSED;
    socket.TranslateSocketError();
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSocket::SocketConnectionRefused);

    errno = ETIMEDOUT;
    socket.TranslateSocketError();
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSocket::SocketConnectionTimeout);

    errno = EINPROGRESS;
    socket.TranslateSocketError();
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSocket::SocketEinprogress);

    errno = EAGAIN;
    socket.TranslateSocketError();
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSocket::SocketEwouldblock);

    errno = EPROTO;
    socket.TranslateSocketError();
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSocket::SocketProtocolError);

    errno = EPERM;
    socket.TranslateSocketError();
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSocket::SocketFirewallError);

    errno = EFAULT;
    socket.TranslateSocketError();
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSocket::SocketInvalidSocketBuffer);

    errno = ENOTSOCK;
    socket.TranslateSocketError();
    CPPUNIT_ASSERT_EQUAL(socket.GetSocketError(), CSocket::SocketInvalidSocket);
#endif
}


