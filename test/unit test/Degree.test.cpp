#include <gtest/gtest.h>

#include <tuple>
#include <array>

#include <Angle.hpp>

using std::array;
using std::get;
using std::tuple;

//===============================================================================================//
// Test fixtures 
//===============================================================================================//

class DegreeTest : public ::testing::TestWithParam<unsigned int>
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

	virtual void TearDown(void) final {}

public:
	degreef mDegree;
	array<tuple<float, float>, 10> mPairDegreeRadians;
};

//===============================================================================================//
// Tests 
//===============================================================================================//

TEST (DegreeConstructor, default)
{
	degreef d;

	ASSERT_EQ(d, 0.0f);	
}

TEST (DegreeConstructor, withValue)
{
	degreef d0 = 60.0f;
	degreef d1 = -90.0f;

	ASSERT_EQ(d0,  60.0f);	
	ASSERT_EQ(d1, -90.0f);	
}

TEST (DegreeOperator, type)
{
	degreef d = 45.0f;

	EXPECT_FLOAT_EQ( d * 2.0f, 90.0f) << "The operator type should allow arithmetic operations on Degree type";
	EXPECT_FLOAT_EQ( 2.0f * d, 90.0f) << "The operator type should allow arithmetic operations on Degree type";
}

TEST_F (DegreeTest, conversionToRadian)
{
	enum E { Deg = 0, Rad = 1 };

	for (const auto it : mPairDegreeRadians)
	{
		mDegree = get<E::Deg>(it);
		EXPECT_FLOAT_EQ(mDegree.toRadian(), get<E::Rad>(it))  <<  "Conversion to radian should return the value of the angle in radians";
	}
}


//INSTANTIATE_TEST_CASE_P(InstantiationName, DegreeTest, ::testing::Range(1.0f, 10.0f));
//INSTANTIATE_TEST_CASE_P(Degree, DegreeTest, ::testing::Range(0u, 10u));
// 
//TEST_P(DegreeTest, conversionToRadian)
//{
//	mDegree = get<0>(mPairDegreeRadians[GetParam()]);
//	ASSERT_FLOAT_EQ(mDegree.toRadian(), get<1>(mPairDegreeRadians[GetParam()]))  <<  "Conversion to radian should return the value of the angle in radians";
//}
