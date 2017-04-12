#ifndef ENGINE_CONVEXHULLSHAPE2DTEST_H
#define ENGINE_CONVEXHULLSHAPE2DTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/Test.h>

class ConvexHullShape2DTest : public CppUnit::TestFixture
{
	public:
		static CppUnit::Test *suite();

		void convexHullBox();
		void identicalPoints();
};

#endif
