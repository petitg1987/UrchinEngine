#ifndef ENGINE_SORTPOINTSTEST_H
#define ENGINE_SORTPOINTSTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/Test.h>

class SortPointsTest : public CppUnit::TestFixture
{
	public:
		static CppUnit::Test *suite();

		void threePointsClockwise();
		void threePointsCounterClockwise();

		void fourPointsClockwise();
		void fourPointsCounterClockwise();

};

#endif