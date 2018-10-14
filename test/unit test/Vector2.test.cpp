#include <gtest/gtest.h>

#include <random>
#include <chrono>

#include <Algebra.hpp>

using std::chrono::system_clock;
using std::uniform_real_distribution;
using std::default_random_engine;
using miniGL::Vector;
using miniGL::TWO;
using miniGL::TWO;

//===============================================================================================//
// Test fixtures for typed tests
//===============================================================================================//

template <typename T>
class TestVectorTWOBase : public ::testing::Test
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

	std::array<T, 4> rand;
	double err = 1.0;
};

template <typename T>
class TestVectorTWOConstructor : public TestVectorTWOBase<T>
{
public:
	virtual void SetUp(void) final
	{
		TestVectorTWOBase<T>::SetUp();
	}
};

template <typename T>
class TestVectorTWOComparision: public ::testing::Test
{
public:
	virtual void SetUp(void) final
	{
		// Give a seed to the generator to initilize it
		mGenerator.seed(system_clock::now().time_since_epoch().count());

		// Get random coordinate for the int vector
		uniform_real_distribution<float> tmpFloat(0.0f, 100.0f);
		mDistributionFloat.param(tmpFloat.param());
		mVectorInitializedWithFloat[0] = mDistributionFloat(mGenerator);
		mVectorInitializedWithFloat[1] = mDistributionFloat(mGenerator);

		// Get random coordinate for the int vector
		uniform_real_distribution<double> tmpDouble(0.0, 100.0);
		mDistributionDouble.param(tmpDouble.param());
		mVectorInitializedWithDouble[0] = mDistributionDouble(mGenerator);
		mVectorInitializedWithDouble[1] = mDistributionDouble(mGenerator);
	}

public:
	default_random_engine mGenerator;
	uniform_real_distribution<float> mDistributionFloat;
	uniform_real_distribution<double> mDistributionDouble;

	Vector<T, TWO, 2u> mVectorInitializedWithFloat;
	Vector<T, TWO, 2u> mVectorInitializedWithDouble;
};

template <typename T>
class TestVectorTWOAccessor : public ::testing::Test
{
public:
	virtual void SetUp(void) final
	{
		// Give a seed to the generator to initilize it
		mGenerator.seed(system_clock::now().time_since_epoch().count());

		// Get random float values
		uniform_real_distribution<float> tmpFloat(0.0f, 100.0f);
		mDistributionFloat.param(tmpFloat.param());

		f0 = mDistributionFloat(mGenerator);
		f1 = mDistributionFloat(mGenerator);

		// Get random double values
		uniform_real_distribution<double> tmpDouble(0.0, 100.0);
		mDistributionDouble.param(tmpDouble.param());

		d0 = mDistributionDouble(mGenerator);
		d1 = mDistributionDouble(mGenerator);
	}

public:
	default_random_engine mGenerator;
	uniform_real_distribution<float> mDistributionFloat;
	uniform_real_distribution<double> mDistributionDouble;

	float f0 = 0.0f, f1 = 0.0f;
	double d0 = 0.0, d1 = 0.0;
};

template <typename T>
class TestVectorTWOArithmeticOperator : public TestVectorTWOBase<T>
{
public:
	virtual void SetUp(void) final
	{
		TestVectorTWOBase<T>::SetUp();
	}
};

template <typename T>
class TestVectorTWOMethods : public TestVectorTWOBase<T>
{
public:
	virtual void SetUp(void) final
	{
		TestVectorTWOBase<T>::SetUp();
	}
};

typedef ::testing::Types<float, double> scalarTypes;
TYPED_TEST_CASE(TestVectorTWOComparision, scalarTypes);
TYPED_TEST_CASE(TestVectorTWOConstructor, scalarTypes);
TYPED_TEST_CASE(TestVectorTWOAccessor, scalarTypes);
TYPED_TEST_CASE(TestVectorTWOArithmeticOperator, scalarTypes);
TYPED_TEST_CASE(TestVectorTWOMethods, scalarTypes);

//===============================================================================================//
// Tests 
//===============================================================================================//

TYPED_TEST (TestVectorTWOComparision, default)
{
	ASSERT_TRUE(  this->mVectorInitializedWithFloat == this->mVectorInitializedWithFloat) << "All coordinates of identical vectors should be equal";
	ASSERT_TRUE(this->mVectorInitializedWithDouble == this->mVectorInitializedWithDouble) << "All coordinates of identical vectors should be equal";

	auto lModifiedFloat = this->mVectorInitializedWithFloat;
	lModifiedFloat[0] += 1.0f;
	auto lModifiedDouble = this->mVectorInitializedWithDouble;
	lModifiedDouble[0] += 1.0f;
	
	ASSERT_FALSE(  this->mVectorInitializedWithFloat == lModifiedFloat) << "Vectors with at least 1 distinct coordinate should not be identicals";
	ASSERT_FALSE(this->mVectorInitializedWithDouble == lModifiedDouble) << "Vectors with at least 1 distinct coordinate should not be identicals";
}

TYPED_TEST (TestVectorTWOAccessor, read)
{
	// Case where the vector is initialized with float parameters (the vector itself is also from different internal type)
	Vector<TypeParam, TWO, 2u> vf(this->f0, this->f1);

	// Case where the vector is initialized with double parameters (the vector itself is also from different internal type)
	Vector<TypeParam, TWO, 2u> vd(this->d0, this->d1);

	ASSERT_EQ(vf[0], static_cast<TypeParam>(this->f0)) << "[0] should return the x coordinate";
	ASSERT_EQ(vf[1], static_cast<TypeParam>(this->f1)) << "[1] should return the y coordinate";

	ASSERT_EQ(vd[0], static_cast<TypeParam>(this->d0)) << "[0] should return the x coordinate";
	ASSERT_EQ(vd[1], static_cast<TypeParam>(this->d1)) << "[1] should return the y coordinate";
}

TYPED_TEST (TestVectorTWOAccessor, write)
{
	// Case where the vector is initialized with float parameters (the vector itself is also from different internal type)
	Vector<TypeParam, TWO, 2u> vf;
	vf[0] = this->f0;
	vf[1] = this->f1;

	// Case where the vector is initialized with double parameters (the vector itself is also from different internal type)
	Vector<TypeParam, TWO, 2u> vd;
	vd[0] = this->d0;
	vd[1] = this->d1;

	ASSERT_EQ(vf[0], static_cast<TypeParam>(this->f0)) << "[0] should return the x coordinate";
	ASSERT_EQ(vf[1], static_cast<TypeParam>(this->f1)) << "[1] should return the y coordinate";

	ASSERT_EQ(vd[0], static_cast<TypeParam>(this->d0)) << "[0] should return the x coordinate";
	ASSERT_EQ(vd[1], static_cast<TypeParam>(this->d1)) << "[1] should return the y coordinate";
}

TYPED_TEST (TestVectorTWOAccessor, getterRead)
{
	// Case where the vector is initialized with float parameters (the vector itself is also from different internal type)
	Vector<TypeParam, TWO, 2u> vf(this->f0, this->f1);

	// Case where the vector is initialized with double parameters (the vector itself is also from different internal type)
	Vector<TypeParam, TWO, 2u> vd(this->d0, this->d1);

	ASSERT_EQ(vf.x(), static_cast<TypeParam>(this->f0)) << ".x() should return the x coordinate";
	ASSERT_EQ(vf.y(), static_cast<TypeParam>(this->f1)) << ".y() should return the y coordinate";

	ASSERT_EQ(vd.x(), static_cast<TypeParam>(this->d0)) << ".x() should return the x coordinate";
	ASSERT_EQ(vd.y(), static_cast<TypeParam>(this->d1)) << ".y() should return the y coordinate";
}

TYPED_TEST (TestVectorTWOAccessor, getterWrite)
{
	Vector<TypeParam, TWO, 2u> vf;
	vf.x() = this->f0;
	vf.y() = this->f1;

	Vector<TypeParam, TWO, 2u> vd;
	vd.x() = this->d0;
	vd.y() = this->d1;

	ASSERT_EQ(vf.x(), static_cast<TypeParam>(this->f0)) << ".x() should enable the x coordinate to be set";
	ASSERT_EQ(vf.y(), static_cast<TypeParam>(this->f1)) << ".y() should enable the y coordinate to be set";

	ASSERT_EQ(vd.x(), static_cast<TypeParam>(this->d0)) << ".x() should enable the x coordinate to be set";
	ASSERT_EQ(vd.y(), static_cast<TypeParam>(this->d1)) << ".y() should enable the y coordinate to be set";
}

TYPED_TEST (TestVectorTWOConstructor, default)
{
	Vector<TypeParam, TWO, 2u> v;

	EXPECT_EQ(v.x(), static_cast<TypeParam>(0.0)) << "x coordinate default value should be 0";
	EXPECT_EQ(v.y(), static_cast<TypeParam>(0.0)) << "y coordinate default value should be 0";
}

TYPED_TEST (TestVectorTWOConstructor, withSpecificCoordinates)
{
	Vector<TypeParam, TWO, 2u> v(this->rand[0], this->rand[1]);

	EXPECT_EQ(v.x(), this->rand[0]);
	EXPECT_EQ(v.y(), this->rand[1]);
}

TYPED_TEST (TestVectorTWOConstructor, withIdenticalCoordinates)
{
	Vector<TypeParam, TWO, 2u> v0(this->rand[0]);

	EXPECT_EQ(v0.x(), this->rand[0]);
	EXPECT_EQ(v0.y(), this->rand[0]);
}

TYPED_TEST (TestVectorTWOConstructor, copy)
{
	Vector<TypeParam, TWO, 2u> v1(this->rand[0], this->rand[1]);
	auto v2 = v1;
	auto v3(v1);

	EXPECT_TRUE(v2 == v1) << "v2 should be equal to v1";
	EXPECT_TRUE(v1 == v2) << "v2 should be equal to v1";
	EXPECT_TRUE(v3 == v1) << "v3 should be equal to v1";
	EXPECT_TRUE(v1 == v3) << "v3 should be equal to v1";
}

TYPED_TEST (TestVectorTWOArithmeticOperator, Addition)
{
	Vector<TypeParam, TWO, 2u> v0(this->rand[0], this->rand[1]), v1(this->rand[2], this->rand[3]);
	auto v2 = v0 + v1;

	Vector<TypeParam, TWO, 2u> lSum(this->rand[0] + this->rand[2], this->rand[1] + this->rand[3]);

	EXPECT_TRUE(v2 == lSum) << "v2 should be equal to v0 + v1";
}

TYPED_TEST (TestVectorTWOArithmeticOperator, Substraction)
{
	Vector<TypeParam, TWO, 2u> v0(this->rand[0], this->rand[1]), v1(this->rand[2], this->rand[3]);
	auto v2 = v0 - v1;

	Vector<TypeParam, TWO, 2u> lSubs(this->rand[0] - this->rand[2], this->rand[1] - this->rand[3]);

	EXPECT_TRUE(v2 == lSubs) << "v2 should be equal to v0 - v1";
}

TYPED_TEST (TestVectorTWOArithmeticOperator, ScalarMultiplication)
{
	Vector<TypeParam, TWO, 2u> v0(this->rand[0], this->rand[1]);
	auto v1 = v0 * this->rand[2];

	Vector<TypeParam, TWO, 2u> lScalarMul(this->rand[0] * this->rand[2], this->rand[1] * this->rand[2]);

	EXPECT_TRUE(v1 == lScalarMul) << "vd2 should be equal to vd0 * scalar value";
}

TYPED_TEST (TestVectorTWOArithmeticOperator, ScalarDivision)
{
	Vector<TypeParam, TWO, 2u> v0(this->rand[0], this->rand[1]);
	auto v1 = v0 / this->rand[2];

	Vector<TypeParam, TWO, 2u> lScalarDiv(this->rand[0] / this->rand[2], this->rand[1] / this->rand[2]);

	EXPECT_TRUE(v1 == lScalarDiv) << "vd2 should be equal to vd0 / scalar value";
}

TYPED_TEST (TestVectorTWOArithmeticOperator, DotProduct)
{
	Vector<TypeParam, TWO, 2u> v0(this->rand[0], this->rand[1]), v1(this->rand[2], this->rand[3]);
	auto lDot = v0.dot(v1);

	auto lDot2 = this->rand[0] * this->rand[2] + this->rand[1] * this->rand[3];

	EXPECT_EQ(lDot, lDot2) << "lDot should be equal to v0.x * v1.x + v0.y * v1.y";

	Vector<TypeParam, TWO, 2u> lOrtho0(static_cast<TypeParam>(0.0), static_cast<TypeParam>(1.0)), lOrtho1(static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0));
	auto lDotOrtho = lOrtho0.dot(lOrtho1);
	EXPECT_TRUE(lDotOrtho == static_cast<TypeParam>(0.0)) << "Orthogonal vectors have a null dot product";
}

TYPED_TEST (TestVectorTWOMethods, Length)
{
	Vector<TypeParam, TWO, 2u> v(this->rand[0], this->rand[1]);

	EXPECT_EQ(v.length(), sqrt(this->rand[0] * this->rand[0] + this->rand[1] * this->rand[1]));
}

TYPED_TEST (TestVectorTWOMethods, Normalize)
{
	Vector<TypeParam, TWO, 2u> v(this->rand[0], this->rand[1]);
	v.normalize();

	EXPECT_NEAR(v.length(), 1.0, this->err);
}
