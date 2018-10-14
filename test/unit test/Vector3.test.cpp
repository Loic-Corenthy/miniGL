#include <gtest/gtest.h>

#include <random>
#include <chrono>

#include <Algebra.hpp>

using std::chrono::system_clock;
using std::uniform_real_distribution;
using std::default_random_engine;
using miniGL::Vector;
using miniGL::ONE;
using miniGL::TWO;
using miniGL::THREE;
using miniGL::FOUR;
using miniGL::ANY_DIMENSION;

//===============================================================================================//
// Test fixtures for typed tests
//===============================================================================================//

template <typename T>
class TestVectorTHREEBase : public ::testing::Test
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

	std::array<T, 6> rand;
	double err = 1.0;
};

template <typename T>
class TestVectorTHREEConstructor : public TestVectorTHREEBase<T>
{
public:
	virtual void SetUp(void) final
	{
		TestVectorTHREEBase<T>::SetUp();
	}
};

template <typename T>
class TestVectorTHREEComparision: public ::testing::Test
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
		mVectorInitializedWithFloat[2] = mDistributionFloat(mGenerator);

		// Get random coordinate for the int vector
		uniform_real_distribution<double> tmpDouble(0.0, 100.0);
		mDistributionDouble.param(tmpDouble.param());
		mVectorInitializedWithDouble[0] = mDistributionDouble(mGenerator);
		mVectorInitializedWithDouble[1] = mDistributionDouble(mGenerator);
		mVectorInitializedWithDouble[2] = mDistributionDouble(mGenerator);
	}

public:
	default_random_engine mGenerator;
	uniform_real_distribution<float> mDistributionFloat;
	uniform_real_distribution<double> mDistributionDouble;

	Vector<T, THREE, 3u> mVectorInitializedWithFloat;
	Vector<T, THREE, 3u> mVectorInitializedWithDouble;
};

template <typename T>
class TestVectorTHREEAccessor : public ::testing::Test
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
		f2 = mDistributionFloat(mGenerator);

		// Get random double values
		uniform_real_distribution<double> tmpDouble(0.0, 100.0);
		mDistributionDouble.param(tmpDouble.param());

		d0 = mDistributionDouble(mGenerator);
		d1 = mDistributionDouble(mGenerator);
		d2 = mDistributionDouble(mGenerator);
	}

public:
	default_random_engine mGenerator;
	uniform_real_distribution<float> mDistributionFloat;
	uniform_real_distribution<double> mDistributionDouble;

	float f0 = 0.0f, f1 = 0.0f, f2 = 0.0f;
	double d0 = 0.0, d1 = 0.0, d2 = 0.0;
};

template <typename T>
class TestVectorTHREEArithmeticOperator : public TestVectorTHREEBase<T>
{
public:
	virtual void SetUp(void) final
	{
		TestVectorTHREEBase<T>::SetUp();
	}
};

template <typename T>
class TestVectorTHREEMethods : public TestVectorTHREEBase<T>
{
public:
	virtual void SetUp(void) final
	{
		TestVectorTHREEBase<T>::SetUp();
	}
};

typedef ::testing::Types<float, double> scalarTypes;
TYPED_TEST_CASE(TestVectorTHREEComparision, scalarTypes);
TYPED_TEST_CASE(TestVectorTHREEConstructor, scalarTypes);
TYPED_TEST_CASE(TestVectorTHREEAccessor, scalarTypes);
TYPED_TEST_CASE(TestVectorTHREEArithmeticOperator, scalarTypes);
TYPED_TEST_CASE(TestVectorTHREEMethods, scalarTypes);

//===============================================================================================//
// Tests 
//===============================================================================================//

TYPED_TEST (TestVectorTHREEComparision, default)
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

TYPED_TEST (TestVectorTHREEAccessor, read)
{
	// Case where the vector is initialized with float parameters (the vector itself is also from different internal type)
	Vector<TypeParam, THREE, 3u> vf(this->f0, this->f1, this->f2);

	// Case where the vector is initialized with double parameters (the vector itself is also from different internal type)
	Vector<TypeParam, THREE, 3u> vd(this->d0, this->d1, this->d2);

	ASSERT_EQ(vf[0], static_cast<TypeParam>(this->f0)) << "[0] should return the x coordinate";
	ASSERT_EQ(vf[1], static_cast<TypeParam>(this->f1)) << "[1] should return the y coordinate";
	ASSERT_EQ(vf[2], static_cast<TypeParam>(this->f2)) << "[2] should return the z coordinate";

	ASSERT_EQ(vd[0], static_cast<TypeParam>(this->d0)) << "[0] should return the x coordinate";
	ASSERT_EQ(vd[1], static_cast<TypeParam>(this->d1)) << "[1] should return the y coordinate";
	ASSERT_EQ(vd[2], static_cast<TypeParam>(this->d2)) << "[2] should return the z coordinate";
}

TYPED_TEST (TestVectorTHREEAccessor, write)
{
	// Case where the vector is initialized with float parameters (the vector itself is also from different internal type)
	Vector<TypeParam, THREE, 3u> vf;
	vf[0] = this->f0;
	vf[1] = this->f1;
	vf[2] = this->f2;

	// Case where the vector is initialized with double parameters (the vector itself is also from different internal type)
	Vector<TypeParam, THREE, 3u> vd;
	vd[0] = this->d0;
	vd[1] = this->d1;
	vd[2] = this->d2;

	ASSERT_EQ(vf[0], static_cast<TypeParam>(this->f0)) << "[0] should return the x coordinate";
	ASSERT_EQ(vf[1], static_cast<TypeParam>(this->f1)) << "[1] should return the y coordinate";
	ASSERT_EQ(vf[2], static_cast<TypeParam>(this->f2)) << "[2] should return the z coordinate";

	ASSERT_EQ(vd[0], static_cast<TypeParam>(this->d0)) << "[0] should return the x coordinate";
	ASSERT_EQ(vd[1], static_cast<TypeParam>(this->d1)) << "[1] should return the y coordinate";
	ASSERT_EQ(vd[2], static_cast<TypeParam>(this->d2)) << "[2] should return the z coordinate";
}

TYPED_TEST (TestVectorTHREEAccessor, getterRead)
{
	// Case where the vector is initialized with float parameters (the vector itself is also from different internal type)
	Vector<TypeParam, THREE, 3u> vf(this->f0, this->f1, this->f2);

	// Case where the vector is initialized with double parameters (the vector itself is also from different internal type)
	Vector<TypeParam, THREE, 3u> vd(this->d0, this->d1, this->d2);

	ASSERT_EQ(vf.x(), static_cast<TypeParam>(this->f0)) << ".x() should return the x coordinate";
	ASSERT_EQ(vf.y(), static_cast<TypeParam>(this->f1)) << ".y() should return the y coordinate";
	ASSERT_EQ(vf.z(), static_cast<TypeParam>(this->f2)) << ".z() should return the z coordinate";

	ASSERT_EQ(vd.x(), static_cast<TypeParam>(this->d0)) << ".x() should return the x coordinate";
	ASSERT_EQ(vd.y(), static_cast<TypeParam>(this->d1)) << ".y() should return the y coordinate";
	ASSERT_EQ(vd.z(), static_cast<TypeParam>(this->d2)) << ".z() should return the z coordinate";
}

TYPED_TEST (TestVectorTHREEAccessor, getterWrite)
{
	Vector<TypeParam, THREE, 3u> vf;
	vf.x() = this->f0;
	vf.y() = this->f1;
	vf.z() = this->f2;

	Vector<TypeParam, THREE, 3u> vd;
	vd.x() = this->d0;
	vd.y() = this->d1;
	vd.z() = this->d2;

	ASSERT_EQ(vf.x(), static_cast<TypeParam>(this->f0)) << ".x() should enable the x coordinate to be set";
	ASSERT_EQ(vf.y(), static_cast<TypeParam>(this->f1)) << ".y() should enable the y coordinate to be set";
	ASSERT_EQ(vf.z(), static_cast<TypeParam>(this->f2)) << ".z() should enable the z coordinate to be set";

	ASSERT_EQ(vd.x(), static_cast<TypeParam>(this->d0)) << ".x() should enable the x coordinate to be set";
	ASSERT_EQ(vd.y(), static_cast<TypeParam>(this->d1)) << ".y() should enable the y coordinate to be set";
	ASSERT_EQ(vd.z(), static_cast<TypeParam>(this->d2)) << ".z() should enable the z coordinate to be set";
}

TYPED_TEST (TestVectorTHREEConstructor, default)
{
	Vector<TypeParam, THREE, 3u> v;

	EXPECT_EQ(v.x(), static_cast<TypeParam>(0.0)) << "x coordinate default value should be 0";
	EXPECT_EQ(v.y(), static_cast<TypeParam>(0.0)) << "y coordinate default value should be 0";
	EXPECT_EQ(v.z(), static_cast<TypeParam>(0.0)) << "z coordinate default value should be 0";
}

TYPED_TEST (TestVectorTHREEConstructor, withSpecificCoordinates)
{
	Vector<TypeParam, THREE, 3u> v(this->rand[0], this->rand[1], this->rand[2]);

	EXPECT_EQ(v.x(), this->rand[0]);
	EXPECT_EQ(v.y(), this->rand[1]);
	EXPECT_EQ(v.z(), this->rand[2]);
}

TYPED_TEST (TestVectorTHREEConstructor, withIdenticalCoordinates)
{
	Vector<TypeParam, THREE, 3u> v0(this->rand[0]);

	EXPECT_EQ(v0.x(), this->rand[0]);
	EXPECT_EQ(v0.y(), this->rand[0]);
	EXPECT_EQ(v0.z(), this->rand[0]);
}

TYPED_TEST (TestVectorTHREEConstructor, copy)
{
	Vector<TypeParam, THREE, 3u> v1(this->rand[0], this->rand[1], this->rand[2]);
	auto v2 = v1;
	auto v3(v1);

	EXPECT_TRUE(v2 == v1) << "v2 should be equal to v1";
	EXPECT_TRUE(v1 == v2) << "v2 should be equal to v1";
	EXPECT_TRUE(v3 == v1) << "v3 should be equal to v1";
	EXPECT_TRUE(v1 == v3) << "v3 should be equal to v1";
}

TYPED_TEST (TestVectorTHREEArithmeticOperator, Addition)
{
	Vector<TypeParam, THREE, 3u> v0(this->rand[0], this->rand[1], this->rand[2]), v1(this->rand[3], this->rand[4], this->rand[5]);
	auto v2 = v0 + v1;

	Vector<TypeParam, THREE, 3u> lSum(this->rand[0] + this->rand[3], this->rand[1] + this->rand[4], this->rand[2] + this->rand[5]);

	EXPECT_TRUE(v2 == lSum) << "v2 should be equal to v0 + v1";
}

TYPED_TEST (TestVectorTHREEArithmeticOperator, Substraction)
{
	Vector<TypeParam, THREE, 3u> v0(this->rand[0], this->rand[1], this->rand[2]), v1(this->rand[3], this->rand[4], this->rand[5]);
	auto v2 = v0 - v1;

	Vector<TypeParam, THREE, 3u> lSubs(this->rand[0] - this->rand[3], this->rand[1] - this->rand[4], this->rand[2] - this->rand[5]);

	EXPECT_TRUE(v2 == lSubs) << "v2 should be equal to v0 - v1";
}

TYPED_TEST (TestVectorTHREEArithmeticOperator, ScalarMultiplication)
{
	Vector<TypeParam, THREE, 3u> v0(this->rand[0], this->rand[1], this->rand[2]);
	auto v1 = v0 * this->rand[3];

	Vector<TypeParam, THREE, 3u> lScalarMul(this->rand[0] * this->rand[3], this->rand[1] * this->rand[3], this->rand[2] * this->rand[3]);

	EXPECT_TRUE(v1 == lScalarMul) << "vd2 should be equal to vd0 * scalar value";
}

TYPED_TEST (TestVectorTHREEArithmeticOperator, ScalarDivision)
{
	Vector<TypeParam, THREE, 3u> v0(this->rand[0], this->rand[1], this->rand[2]);
	auto v1 = v0 / this->rand[3];

	Vector<TypeParam, THREE, 3u> lScalarDiv(this->rand[0] / this->rand[3], this->rand[1] / this->rand[3], this->rand[2] / this->rand[3]);

	EXPECT_TRUE(v1 == lScalarDiv) << "vd2 should be equal to vd0 / scalar value";
}

TYPED_TEST (TestVectorTHREEArithmeticOperator, DotProduct)
{
	Vector<TypeParam, THREE, 3u> v0(this->rand[0], this->rand[1], this->rand[2]), v1(this->rand[3], this->rand[4], this->rand[5]);
	auto lDot = v0.dot(v1);

	auto lDot2 = this->rand[0] * this->rand[3] + this->rand[1] * this->rand[4] + this->rand[2] * this->rand[5];

	EXPECT_EQ(lDot, lDot2) << "lDot should be equal to v0.x * v1.x + v0.y * v1.y + v0.z * v1.z";

	Vector<TypeParam, THREE, 3u> lOrtho0(static_cast<TypeParam>(0.0), static_cast<TypeParam>(1.0), static_cast<TypeParam>(0.0)), lOrtho1(static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(5.0));
	auto lDotOrtho = lOrtho0.dot(lOrtho1);
	EXPECT_TRUE(lDotOrtho == static_cast<TypeParam>(0.0)) << "Orthogonal vectors have a null dot product";
}

TYPED_TEST (TestVectorTHREEMethods, Length)
{
	Vector<TypeParam, THREE, 3u> v(this->rand[0], this->rand[1], this->rand[2]);

	EXPECT_EQ(v.length(), sqrt(this->rand[0] * this->rand[0] + this->rand[1] * this->rand[1] + this->rand[2] * this->rand[2]));
}

TYPED_TEST (TestVectorTHREEMethods, Normalize)
{
	Vector<TypeParam, THREE, 3u> v(this->rand[0], this->rand[1], this->rand[2]);
	v.normalize();

	EXPECT_NEAR(v.length(), 1.0, this->err);
}

TYPED_TEST (TestVectorTHREEArithmeticOperator, CrossProduct)
{
	Vector<TypeParam, THREE, 3u> v0(this->rand[0], this->rand[1], this->rand[2]), v1(this->rand[3], this->rand[4], this->rand[5]);
	auto v2 = v0.cross(v1);

	Vector<TypeParam, THREE, 3u> lCross(this->rand[1] * this->rand[5] - this->rand[2] * this->rand[4],
										this->rand[2] * this->rand[3] - this->rand[0] * this->rand[5],
										this->rand[0] * this->rand[4] - this->rand[1] * this->rand[3]);

	EXPECT_TRUE(v2 == lCross) << "v2 should be equal to v0 cross v1";


	Vector<TypeParam, THREE, 3u> lOrtho0(static_cast<TypeParam>(-2.0), static_cast<TypeParam>(1.0), static_cast<TypeParam>(0.0)), lOrtho1(static_cast<TypeParam>(0.0), static_cast<TypeParam>(0.0), static_cast<TypeParam>(5.0));
	auto lCrossOrtho = lOrtho0.cross(lOrtho1);

	EXPECT_TRUE(lCrossOrtho.dot(lOrtho0) == static_cast<TypeParam>(0.0)) << "Cross product vector is orthogonal to input vectors";
	EXPECT_TRUE(lCrossOrtho.dot(lOrtho1) == static_cast<TypeParam>(0.0)) << "Cross product vector is orthogonal to input vectors";


	Vector<TypeParam, THREE, 3u> lPara0(static_cast<TypeParam>(-2.0), static_cast<TypeParam>(1.0), static_cast<TypeParam>(0.0)), lPara1(static_cast<TypeParam>(-7.0), static_cast<TypeParam>(3.5), static_cast<TypeParam>(0.0));
	auto lCrossPara = lPara0.cross(lPara1);
	Vector<TypeParam, THREE, 3u> nullVector;
	EXPECT_TRUE(lCrossPara == nullVector) << "Parallel vectors have a null cross product";
}
