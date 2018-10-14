#include <gtest/gtest.h>

#include <random>
#include <chrono>
#include <cmath>

#include <Algebra.hpp>

#include "UnitTestHelperFunctions.hpp"

using std::chrono::system_clock;
using std::uniform_real_distribution;
using std::default_random_engine;
using miniGL::Quaternion;

//===============================================================================================//
// Test fixtures for typed tests
//===============================================================================================//

template <typename T>
class TestQuaternionBase : public ::testing::Test
{
public:
	virtual void SetUp(void) override
	{
		// Give a seed to the generator to initilize it
		mGenerator.seed(system_clock::now().time_since_epoch().count());

		// Get random number
		uniform_real_distribution<T> lDistribution(-100.0, 100.0);
		mDistribution.param(lDistribution.param());

		for (auto & it : rand)
			it  = mDistribution(mGenerator);

		if (sizeof(T) == 4)
			err = 0.0001;		
		else if (sizeof(T) == 8)
			err = 0.00000001;		
	}

public:
	default_random_engine mGenerator;
	uniform_real_distribution<T> mDistribution;

	std::array<T, 8> rand;
	double err = 1.0;
};

template <typename T>
class TestQuaternionConstructor : public TestQuaternionBase<T>
{
public:
	virtual void SetUp(void) final
	{
		TestQuaternionBase<T>::SetUp();
	}
};

template <typename T>
class TestQuaternionComparision: public TestQuaternionBase<T>
{
public:
	virtual void SetUp(void) final
	{
		TestQuaternionBase<T>::SetUp();
	}
};

template <typename T>
class TestQuaternionAccessor : public TestQuaternionBase<T>
{
public:
	virtual void SetUp(void) final
	{
		TestQuaternionBase<T>::SetUp();
	}
};

template <typename T>
class TestQuaternionArithmeticOperator : public TestQuaternionBase<T>
{
public:
	virtual void SetUp(void) final
	{
		TestQuaternionBase<T>::SetUp();
	}
};

template <typename T>
class TestQuaternionMethods : public TestQuaternionBase<T>
{
public:
	virtual void SetUp(void) final
	{
		TestQuaternionBase<T>::SetUp();
	}
};

typedef ::testing::Types<float, double> scalarTypes;
TYPED_TEST_CASE(TestQuaternionComparision, scalarTypes);
TYPED_TEST_CASE(TestQuaternionConstructor, scalarTypes);
TYPED_TEST_CASE(TestQuaternionAccessor, scalarTypes);
TYPED_TEST_CASE(TestQuaternionArithmeticOperator, scalarTypes);
TYPED_TEST_CASE(TestQuaternionMethods, scalarTypes);

//===============================================================================================//
// Tests 
//===============================================================================================//

TYPED_TEST (TestQuaternionComparision, default)
{
	Quaternion<TypeParam> q(this->rand[0], this->rand[1], this->rand[2], this->rand[3]); 
	ASSERT_TRUE(q == q) << "In our definition, all coordinates of identical quaternion should be equal";

	auto lModified = q;
	lModified.x() += 1.0f;
	
	ASSERT_FALSE(q == lModified) << "Quaternions with at least 1 distinct coordinate should not be identicals";
}

TYPED_TEST (TestQuaternionAccessor, getterRead)
{
	// Case where the vector is initialized with float parameters (the vector itself is also from different internal type)
	Quaternion<TypeParam> q(this->rand[0], this->rand[1], this->rand[2], this->rand[3]);

	ASSERT_EQ(q.x(), this->rand[0]) << ".x() should return the x coordinate";
	ASSERT_EQ(q.y(), this->rand[1]) << ".y() should return the y coordinate";
	ASSERT_EQ(q.z(), this->rand[2]) << ".z() should return the z coordinate";
	ASSERT_EQ(q.w(), this->rand[3]) << ".w() should return the w coordinate";
}

TYPED_TEST (TestQuaternionAccessor, getterWrite)
{
	Quaternion<TypeParam> q;
	q.x() = this->rand[0];
	q.y() = this->rand[1];
	q.z() = this->rand[2];
	q.w() = this->rand[3];

	ASSERT_EQ(q.x(), this->rand[0]) << ".x() should enable the x coordinate to be set";
	ASSERT_EQ(q.y(), this->rand[1]) << ".y() should enable the y coordinate to be set";
	ASSERT_EQ(q.z(), this->rand[2]) << ".z() should enable the z coordinate to be set";
	ASSERT_EQ(q.w(), this->rand[3]) << ".w() should enable the w coordinate to be set";
}

TYPED_TEST (TestQuaternionConstructor, default)
{
	Quaternion<TypeParam> q;

	EXPECT_EQ(q.x(), static_cast<TypeParam>(0.0)) << "x coordinate default value should be 0";
	EXPECT_EQ(q.y(), static_cast<TypeParam>(0.0)) << "y coordinate default value should be 0";
	EXPECT_EQ(q.z(), static_cast<TypeParam>(0.0)) << "z coordinate default value should be 0";
	EXPECT_EQ(q.w(), static_cast<TypeParam>(0.0)) << "w coordinate default value should be 0";
}

TYPED_TEST (TestQuaternionConstructor, withSpecificCoordinates)
{
	Quaternion<TypeParam> q(this->rand[0], this->rand[1], this->rand[2], this->rand[3]);

	EXPECT_EQ(q.x(), this->rand[0]);
	EXPECT_EQ(q.y(), this->rand[1]);
	EXPECT_EQ(q.z(), this->rand[2]);
	EXPECT_EQ(q.w(), this->rand[3]);
}

TYPED_TEST (TestQuaternionConstructor, copy)
{
	Quaternion<TypeParam> q1(this->rand[0], this->rand[1], this->rand[2], this->rand[3]);
	auto q2 = q1;
	auto q3(q1);

	EXPECT_TRUE(q2 == q1) << "q2 should be equal to q1";
	EXPECT_TRUE(q1 == q2) << "q2 should be equal to q1";
	EXPECT_TRUE(q3 == q1) << "q3 should be equal to q1";
	EXPECT_TRUE(q1 == q3) << "q3 should be equal to q1";
}

TYPED_TEST (TestQuaternionArithmeticOperator, MultiplicationWithQuaternion)
{
	// Coefficients and result taken from example on wolfram alpha
	Quaternion<TypeParam> q0(static_cast<TypeParam>(3.0), static_cast<TypeParam>( 4.0), static_cast<TypeParam>( 3.0), static_cast<TypeParam>(-sin(M_PI)));
	Quaternion<TypeParam> q1(static_cast<TypeParam>(3.9), static_cast<TypeParam>(-1.0), static_cast<TypeParam>(-3.0), static_cast<TypeParam>(4.0));

	auto q2 = q0 * q1;

	EXPECT_NEAR(q2.x(), static_cast<TypeParam>(3.0), this->err);
	EXPECT_NEAR(q2.y(), static_cast<TypeParam>(36.7), this->err);
	EXPECT_NEAR(q2.z(), static_cast<TypeParam>(-6.6), this->err);
	EXPECT_NEAR(q2.w(), static_cast<TypeParam>(1.3), this->err);
}

TYPED_TEST (TestQuaternionArithmeticOperator, MultiplicationWithVector)
{
	/*! \todo Find a test for the multiplication of a quaternion and a vector3 */
	TEST_COUT("This test is not implemented yet");
	ADD_FAILURE();
}

TYPED_TEST (TestQuaternionMethods, Length)
{
	Quaternion<TypeParam> q(this->rand[0], this->rand[1], this->rand[2], this->rand[3]);

	EXPECT_EQ(q.length(), sqrt(this->rand[0] * this->rand[0] + this->rand[1] * this->rand[1] + this->rand[2] * this->rand[2] + this->rand[3] * this->rand[3]));
}

TYPED_TEST (TestQuaternionMethods, Normalize)
{
	Quaternion<TypeParam> q(this->rand[0], this->rand[1], this->rand[2], this->rand[3]);
	q.normalize();

	EXPECT_NEAR(q.length(), 1.0, this->err);
}

TYPED_TEST (TestQuaternionMethods, Conjugate)
{
	// Coefficients and result taken from example on wolfram alpha
	Quaternion<TypeParam> q0(this->rand[0], this->rand[1], this->rand[2], this->rand[3]);

	// Make a copy of q0
	auto q1 = q0.conjugated();

	EXPECT_EQ(q1.x(), -q0.x());
	EXPECT_EQ(q1.y(), -q0.y());
	EXPECT_EQ(q1.z(), -q0.z());
	EXPECT_EQ(q1.w(),  q0.w());

	// The conjugate of the conjugate should result in identity
	q1.conjugate();

	EXPECT_EQ(q1.x(), q0.x());
	EXPECT_EQ(q1.y(), q0.y());
	EXPECT_EQ(q1.z(), q0.z());
	EXPECT_EQ(q1.w(), q0.w());
}
