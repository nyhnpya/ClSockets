#include <cppunit/extensions/HelperMacros.h>

#ifdef _LINUX
  #define SLEEP(x)               sleep(x)
  #define THREAD_RETURN          void *
  #define THREAD_RETURN_VALUE    NULL
  #define PTHREAD_T              pthread_t
  #define THREAD(a,b,c,d,e,f)    pthread_create(a, b, c, d)
#endif

#ifdef WIN32
  #define SLEEP(x)             Sleep(x)
  #define THREAD_RETURN        DWORD WINAPI 
  #define THREAD_RETURN_VALUE  1
  #define PTHREAD_T            int
  #define THREAD(a,b,c,d,e,f)  CreateThread(NULL, b, c, d, e, f)

#endif

struct thread_data
{
    char     *pszServerAddr;
    short int nPort;
    int       nNumBytesToReceive;
    int       nTotalPayloadSize;
};


// Basic Unit Test Definition
class NameOfTest : public CppUnit::TestFixture
{
 public:
  CPPUNIT_TEST_SUITE( NameOfTest ); //  Always first macro!

  CPPUNIT_TEST( CreateTCPSocket ); 
  CPPUNIT_TEST( CreateUDPSocket ); 
  CPPUNIT_TEST( InitializeTCPSocketActive ); 
  CPPUNIT_TEST( InitializeTCPSocketPassive ); 
  CPPUNIT_TEST( InitializeUDPSocket ); 
  CPPUNIT_TEST( CloseOnce ); 
  CPPUNIT_TEST( CloseTwice ); 
  CPPUNIT_TEST( CloseMany ); 
  CPPUNIT_TEST( OpenAllPaths ); 
  CPPUNIT_TEST( ConnectSuccess ); 
  CPPUNIT_TEST( ConnectFail ); 
  CPPUNIT_TEST( ConnectTimeout ); 
  CPPUNIT_TEST( SetSocketNonblocking );
  CPPUNIT_TEST( SetSocketBlocking );
  CPPUNIT_TEST( SetSendWindowsSizeActive );
  CPPUNIT_TEST( SetReceiveWindowSizeActive );
  CPPUNIT_TEST( TranslateError ); 
  CPPUNIT_TEST( TCPEchoClient );
  CPPUNIT_TEST( TCPEchoClientAsync );
  CPPUNIT_TEST( UDPEchoClient );
  CPPUNIT_TEST( TCPOneByteAtTime );                              
  CPPUNIT_TEST( Writev );

  CPPUNIT_TEST_SUITE_END();         // Must supply as last macro!

  //  void setUp(void);
  //  void tearDown(void);

 private:
  void CreateTCPSocket(void);
  void CreateUDPSocket(void);
  void CloseOnce(void);
  void CloseTwice(void);
  void CloseMany(void);
  void InitializeTCPSocketActive(void);
  void InitializeTCPSocketPassive(void);
  void InitializeUDPSocket(void);
  void OpenAllPaths(void);
  void ConnectSuccess(void);
  void ConnectFail(void);
  void ConnectTimeout(void);
  void SetSocketNonblocking(void);
  void SetSocketBlocking(void);
  void TranslateError(void);
  void TCPEchoClient(void);
  void TCPOneByteAtTime(void);
  void UDPEchoClient(void);
  void Writev(void);
  void SetSendWindowsSizeActive(void);
  void SetReceiveWindowSizeActive(void);
  void TCPEchoClientAsync(void);
};


