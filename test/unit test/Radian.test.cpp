#include <gtest/gtest.h>

#include <tuple>
#include <array>
#include <cmath>
#include <functional>

#include <Angle.hpp>

using std::array;
using std::get;
using std::tuple;

//===============================================================================================//
// Test fixtures 
//===============================================================================================//

class RadianTest : public ::testing::Test
{
public:
	virtual void SetUp(void) final
	{
		enum E { Deg = 0, Rad = 1 };

		get<E::Deg>(mPairDegreeRadians[0]) = 0.0f;
		get<E::Rad>(mPairDegreeRadians[0]) = 0.0f;

		get<E::Deg>(mPairDegreeRadians[1]) = 10.0f;
		get<E::Rad>(mPairDegreeRadians[1]) = 0.1745329252f;

		get<E::Deg>(mPairDegreeRadians[2]) = 20.0f;
		get<E::Rad>(mPairDegreeRadians[2]) = 0.3490658504f;

		get<E::Deg>(mPairDegreeRadians[3]) = 45.0f;
		get<E::Rad>(mPairDegreeRadians[3]) = 0.7853981634f;

		get<E::Deg>(mPairDegreeRadians[4]) = 60.0f;
		get<E::Rad>(mPairDegreeRadians[4]) = 1.0471975512f;

		get<E::Deg>(mPairDegreeRadians[5]) = 90.0f;
		get<E::Rad>(mPairDegreeRadians[5]) = 1.5707963268f;

		get<E::Deg>(mPairDegreeRadians[6]) = 180.0f;
		get<E::Rad>(mPairDegreeRadians[6]) = 3.1415926536f;

		get<E::Deg>(mPairDegreeRadians[7]) = 360.0f;
		get<E::Rad>(mPairDegreeRadians[7]) = 6.2831853072f;

		get<E::Deg>(mPairDegreeRadians[8]) = -90.0f;
		get<E::Rad>(mPairDegreeRadians[8]) = -1.5707963268f;

		get<E::Deg>(mPairDegreeRadians[9]) = -180.0f;
		get<E::Rad>(mPairDegreeRadians[9]) = -3.1415926536f;
	}

public:
	radianf mRadian;
	array<tuple<float, float>, 10> mPairDegreeRadians;
};


//===============================================================================================//
// Tests 
//===============================================================================================//

TEST (RadianConstructor, default)
{
	radianf r;

	ASSERT_EQ(r, 0.0f);	
}

TEST (RadianConstructor, withValue)
{
	radianf r0 = M_PI;
	radianf r1 = -M_PI / 2.0f;

	EXPECT_FLOAT_EQ(r0, M_PI);	
	EXPECT_FLOAT_EQ(r1, -M_PI / 2.0f);	
}

TEST (RadianOperator, type)
{
	radianf r = M_PI / 4.0f;

	EXPECT_FLOAT_EQ( r * 4.0f, M_PI) << "The operator type should allow arithmetic operations on Radian type";
	EXPECT_FLOAT_EQ( 4.0f * r, M_PI) << "The operator type should allow arithmetic operations on Radian type";
}

TEST_F (RadianTest, conversionToDegree)
{
	enum E { Deg = 0, Rad = 1 };

	for (const auto it : mPairDegreeRadians)
	{
		mRadian = get<E::Rad>(it);
		EXPECT_FLOAT_EQ(mRadian.toDegree(), get<E::Deg>(it))  <<  "Conversion to degree should return the value of the angle in degrees";
	}
}
