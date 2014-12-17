#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestResultCollector.h>
#include <string>

class TestOptions
{
public:
    TestOptions() :
        m_testName("All Tests"),
        m_outfileName("./TestOutput.xml"),
        m_resourceLocation("."),
        m_outType(None)
        {};

    enum OutputFormat{
        None = 0,
        Xml = 1,
        Text = 2
    };

    std::string getTestName() const { return m_testName; };
    std::string getResourceLocation() const { return m_resourceLocation; };
    OutputFormat getOutputFormat() const { return m_outType; };
    std::string getFileName() const { return m_outfileName; };

private:
    std::string m_testName;
    std::string m_outfileName;
    std::string m_resourceLocation;
    OutputFormat m_outType;
};
