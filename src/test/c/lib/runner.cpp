/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ErrorPrinter.h>

int main() {
 return CxxTest::ErrorPrinter().run();
}
#include "irioCorelib-test.hpp"

static irioCoreLibTestSuite suite_irioCoreLibTestSuite;

static CxxTest::List Tests_irioCoreLibTestSuite = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_irioCoreLibTestSuite( "irioCorelib-test.hpp", 53, "irioCoreLibTestSuite", suite_irioCoreLibTestSuite, Tests_irioCoreLibTestSuite );

static class TestDescription_irioCoreLibTestSuite_testInitialize : public CxxTest::RealTestDescription {
public:
 TestDescription_irioCoreLibTestSuite_testInitialize() : CxxTest::RealTestDescription( Tests_irioCoreLibTestSuite, suiteDescription_irioCoreLibTestSuite, 68, "testInitialize" ) {}
 void runTest() { suite_irioCoreLibTestSuite.testInitialize(); }
} testDescription_irioCoreLibTestSuite_testInitialize;

static class TestDescription_irioCoreLibTestSuite_testFinalize : public CxxTest::RealTestDescription {
public:
 TestDescription_irioCoreLibTestSuite_testFinalize() : CxxTest::RealTestDescription( Tests_irioCoreLibTestSuite, suiteDescription_irioCoreLibTestSuite, 75, "testFinalize" ) {}
 void runTest() { suite_irioCoreLibTestSuite.testFinalize(); }
} testDescription_irioCoreLibTestSuite_testFinalize;

static class TestDescription_irioCoreLibTestSuite_testconfigureTarget : public CxxTest::RealTestDescription {
public:
 TestDescription_irioCoreLibTestSuite_testconfigureTarget() : CxxTest::RealTestDescription( Tests_irioCoreLibTestSuite, suiteDescription_irioCoreLibTestSuite, 83, "testconfigureTarget" ) {}
 void runTest() { suite_irioCoreLibTestSuite.testconfigureTarget(); }
} testDescription_irioCoreLibTestSuite_testconfigureTarget;

#include <cxxtest/Root.cpp>
