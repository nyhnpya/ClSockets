#include <iostream>
#include <exception>
#include <string>
#include <vector>
#include <fstream>

#include <cppunit/XmlOutputter.h>
#include <cppunit/TextOutputter.h>

#include "TestOptions.h"

// Main Test Function
int main(int argc, char **argv) 
{
    TestOptions options;
	// Get the top level suite from the registry
	CppUnit::TestSuite *suite = (CppUnit::TestSuite*)CppUnit::TestFactoryRegistry::getRegistry().makeTest();

	// Adds the test to the list of test to run
	CppUnit::TestRunner runner;
	CppUnit::TestResult controller;
	CppUnit::TestResultCollector results;
	controller.addListener(&results);

	runner.addTest(suite);

    // Run the tests.
    std::string strTestName(options.getTestName().c_str());
    runner.run(controller, strTestName);

	CppUnit::Outputter *outputter = NULL;
	std::ofstream outputfile ;

    outputfile.open(options.getFileName().c_str(), std::ios::out);

    CppUnit::XmlOutputter *tmpOutputter = new CppUnit::XmlOutputter(&results, outputfile);
    std::string styleSheet = options.getResourceLocation() + "/report.xsl";
    tmpOutputter->setStyleSheet(styleSheet.c_str());
    outputter = tmpOutputter;	

    if (outputfile.is_open())
    {
        outputter->write();
        outputfile.close();
    }
    else
    {
        std::string errMsg;
        errMsg = "ERROR: Unable to open test result output file ";
        errMsg += options.getFileName();
        printf("%s\n", errMsg.c_str());
    }
    delete outputter;

	bool wasSucessful = results.testFailuresTotal() == 0;
	if (wasSucessful)
    {
		printf("Test Succeeded\n");
	}
	else
	{
		printf("Test Failed!\n");
	}

	printf("Tests Run %d\n", results.runTests());
	printf("Tests Errors: %d\n", results.testErrors());
	printf("Tests Failures: %d\n", results.testFailuresTotal());
}
