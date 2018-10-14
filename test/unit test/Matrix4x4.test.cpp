#include <gtest/gtest.h>

#include <random>
#include <chrono>
#include <array>

#include <Algebra.hpp>

#include "UnitTestHelperFunctions.hpp"

using std::chrono::system_clock;
using std::uniform_real_distribution;
using std::default_random_engine;
using miniGL::Vector;
using miniGL::Matrix;
using miniGL::FOUR;
using miniGL::ANY_DIMENSION;

//===============================================================================================//
// Test fixtures for typed tests
//===============================================================================================//

template <typename T>
class TestMatrix4x4Base : public ::testing::Test
{
public:
	virtual void SetUp(void) override
	{
		// Give a seed to the generator to initilize it
		mGenerator.seed(system_clock::now().time_since_epoch().count());

		// Get random double number
		uniform_real_distribution<double> tmpDouble(-10.0, 10.0);
		mDistributionDouble.param(tmpDouble.param());

		for (auto & it : rand)
			it  = mDistributionDouble(mGenerator);
		
		if (sizeof(T) == 4)
		{
			err = 0.0001;		
			TEST_COUT("Precision: 0.0001");
		}
		else if (sizeof(T) == 8)
		{
			err = 0.00000001;		
			TEST_COUT("Precision: 0.00000001");
		}
	}

public:
	default_random_engine mGenerator;
	uniform_real_distribution<double> mDistributionDouble;

	std::array<double, 32> rand;
	double err = 1.0f;
};

template <typename T>
class TestMatrix4x4Constructor : public TestMatrix4x4Base<T>
{
public:
	virtual void SetUp(void) final
	{
		TestMatrix4x4Base<T>::SetUp();
	}
};

template <typename T>
class TestMatrix4x4Accessor : public TestMatrix4x4Base<T>
{
public:
	virtual void SetUp(void) final
	{
		TestMatrix4x4Base<T>::SetUp();
	}
};

template <typename T>
class TestMatrix4x4ArithmeticOperator : public TestMatrix4x4Base<T>
{
public:
	virtual void SetUp(void) final
	{
		TestMatrix4x4Base<T>::SetUp();
	}
};

typedef ::testing::Types<float, double> scalarTypes;
TYPED_TEST_CASE(TestMatrix4x4Constructor, scalarTypes);
TYPED_TEST_CASE(TestMatrix4x4Accessor, scalarTypes);
TYPED_TEST_CASE(TestMatrix4x4ArithmeticOperator, scalarTypes);

//===============================================================================================//
// Tests 
//===============================================================================================//

TYPED_TEST (TestMatrix4x4Constructor, Default)
{
	// Create a default matrix
	Matrix<TypeParam, FOUR, FOUR, 4, 4> m;

	// Ensure all coefficients are 0 by default
	for (unsigned int i = 0; i < 4; ++i)
		for (unsigned int j = 0; j < 4; ++j)
			ASSERT_EQ(m(i,j), static_cast<TypeParam>(0.0));
}

TYPED_TEST (TestMatrix4x4Constructor, DiagonalCoefficients)
{
	// Create a matrix from 1 scalar value
	Matrix<TypeParam, FOUR, FOUR, 4, 4> indentity(1.0f), randomlyInitialized(this->rand[0]);

	// Ensure all diagonal coefficients corresponds to the scalar value and all other coefficients are 0
	for (unsigned int i = 0; i < 4; ++i)
		for (unsigned int j = 0; j < 4; ++j)
		{
			if (i == j)
			{
				ASSERT_EQ(indentity(i,j), static_cast<TypeParam>(1.0));
				ASSERT_NEAR(randomlyInitialized(i,j), this->rand[0], this->err);
			}
			else
			{
				ASSERT_EQ(indentity(i,j), static_cast<TypeParam>(0.0));
				ASSERT_EQ(randomlyInitialized(i,j), static_cast<TypeParam>(0.0));
			}
		}
}

TYPED_TEST (TestMatrix4x4Constructor, Copy)
{
	// Create 2 default matrix
	Matrix<TypeParam, FOUR, FOUR, 4, 4> m, mCopy;

	// Set the coefficients of m
	m(0,0) = this->rand[0]; m(0,1) = this->rand[4]; m(0,2) = this->rand[ 8]; m(0,3) = this->rand[12];
	m(1,0) = this->rand[1]; m(1,1) = this->rand[5]; m(1,2) = this->rand[ 9]; m(1,3) = this->rand[13];
	m(2,0) = this->rand[2]; m(2,1) = this->rand[6]; m(2,2) = this->rand[10]; m(2,3) = this->rand[14];
	m(3,0) = this->rand[3]; m(3,1) = this->rand[7]; m(3,2) = this->rand[11]; m(3,3) = this->rand[15];

	// Copy m to mCopy
	mCopy = m;

	// Ensure mCopy has the same coefficients as the ones used to initialized m
	ASSERT_NEAR(mCopy(0,0), this->rand[0], this->err); ASSERT_NEAR(mCopy(0,1), this->rand[4], this->err); ASSERT_NEAR(mCopy(0,2), this->rand[ 8], this->err); ASSERT_NEAR(mCopy(0,3), this->rand[12], this->err);
	ASSERT_NEAR(mCopy(1,0), this->rand[1], this->err); ASSERT_NEAR(mCopy(1,1), this->rand[5], this->err); ASSERT_NEAR(mCopy(1,2), this->rand[ 9], this->err); ASSERT_NEAR(mCopy(1,3), this->rand[13], this->err);
	ASSERT_NEAR(mCopy(2,0), this->rand[2], this->err); ASSERT_NEAR(mCopy(2,1), this->rand[6], this->err); ASSERT_NEAR(mCopy(2,2), this->rand[10], this->err); ASSERT_NEAR(mCopy(2,3), this->rand[14], this->err);
	ASSERT_NEAR(mCopy(3,0), this->rand[3], this->err); ASSERT_NEAR(mCopy(3,1), this->rand[7], this->err); ASSERT_NEAR(mCopy(3,2), this->rand[11], this->err); ASSERT_NEAR(mCopy(3,3), this->rand[15], this->err);
}

TYPED_TEST (TestMatrix4x4Accessor, ReadAndWrite)
{
	// Create a default matrix m
	Matrix<TypeParam, FOUR, FOUR, 4, 4> m;

	// Set the coefficients of m using the accessor operator
	m(0,0) = this->rand[0]; m(0,1) = this->rand[4]; m(0,2) = this->rand[ 8]; m(0,3) = this->rand[12];
	m(1,0) = this->rand[1]; m(1,1) = this->rand[5]; m(1,2) = this->rand[ 9]; m(1,3) = this->rand[13];
	m(2,0) = this->rand[2]; m(2,1) = this->rand[6]; m(2,2) = this->rand[10]; m(2,3) = this->rand[14];
	m(3,0) = this->rand[3]; m(3,1) = this->rand[7]; m(3,2) = this->rand[11]; m(3,3) = this->rand[15];

	// Get the coefficients of m using the accessor operator and ensure they are the same as the ones set above
	ASSERT_NEAR(m(0,0), this->rand[0], this->err); ASSERT_NEAR(m(0,1), this->rand[4], this->err); ASSERT_NEAR(m(0,2), this->rand[ 8], this->err); ASSERT_NEAR(m(0,3), this->rand[12], this->err);
	ASSERT_NEAR(m(1,0), this->rand[1], this->err); ASSERT_NEAR(m(1,1), this->rand[5], this->err); ASSERT_NEAR(m(1,2), this->rand[ 9], this->err); ASSERT_NEAR(m(1,3), this->rand[13], this->err);
	ASSERT_NEAR(m(2,0), this->rand[2], this->err); ASSERT_NEAR(m(2,1), this->rand[6], this->err); ASSERT_NEAR(m(2,2), this->rand[10], this->err); ASSERT_NEAR(m(2,3), this->rand[14], this->err);
	ASSERT_NEAR(m(3,0), this->rand[3], this->err); ASSERT_NEAR(m(3,1), this->rand[7], this->err); ASSERT_NEAR(m(3,2), this->rand[11], this->err); ASSERT_NEAR(m(3,3), this->rand[15], this->err);
}

TYPED_TEST (TestMatrix4x4ArithmeticOperator, Comparision)
{
	// Create a two default matrices  m0 and m1
	Matrix<TypeParam, FOUR, FOUR, 4, 4> m0, m1;

	// Set the coefficients of m0
	m0(0,0) = this->rand[0]; m0(0,1) = this->rand[4]; m0(0,2) = this->rand[ 8]; m0(0,3) = this->rand[12];
	m0(1,0) = this->rand[1]; m0(1,1) = this->rand[5]; m0(1,2) = this->rand[ 9]; m0(1,3) = this->rand[13];
	m0(2,0) = this->rand[2]; m0(2,1) = this->rand[6]; m0(2,2) = this->rand[10]; m0(2,3) = this->rand[14];
	m0(3,0) = this->rand[3]; m0(3,1) = this->rand[7]; m0(3,2) = this->rand[11]; m0(3,3) = this->rand[15];

	// Copy m0 to m1
	m1 = m0;

	EXPECT_TRUE(m0 == m1) << "m0 must equal m1";

	// Change a coefficient in m
	m1(2,2) = m0(3,3) + static_cast<TypeParam>(1.0);

	EXPECT_FALSE(m0 == m1) << "m0 and m1 must be different sinze they have at least one distinct coefficient";
}

TYPED_TEST (TestMatrix4x4ArithmeticOperator, Addition)
{
	// Create a two default matrices  m0 and m1
	Matrix<TypeParam, FOUR, FOUR, 4, 4> m0, m1;

	// Set the coefficients of m0
	m0(0,0) = this->rand[0]; m0(0,1) = this->rand[4]; m0(0,2) = this->rand[ 8]; m0(0,3) = this->rand[12];
	m0(1,0) = this->rand[1]; m0(1,1) = this->rand[5]; m0(1,2) = this->rand[ 9]; m0(1,3) = this->rand[13];
	m0(2,0) = this->rand[2]; m0(2,1) = this->rand[6]; m0(2,2) = this->rand[10]; m0(2,3) = this->rand[14];
	m0(3,0) = this->rand[3]; m0(3,1) = this->rand[7]; m0(3,2) = this->rand[11]; m0(3,3) = this->rand[15];

	// Set the coefficients of m1
	m1(0,0) = this->rand[16]; m1(0,1) = this->rand[20]; m1(0,2) = this->rand[24]; m1(0,3) = this->rand[28];
	m1(1,0) = this->rand[17]; m1(1,1) = this->rand[21]; m1(1,2) = this->rand[25]; m1(1,3) = this->rand[29];
	m1(2,0) = this->rand[18]; m1(2,1) = this->rand[22]; m1(2,2) = this->rand[26]; m1(2,3) = this->rand[30];
	m1(3,0) = this->rand[19]; m1(3,1) = this->rand[23]; m1(3,2) = this->rand[27]; m1(3,3) = this->rand[31];

	// Compute addition
	Matrix<TypeParam, FOUR, FOUR, 4, 4> lSum = m0 + m1;
	
	for (unsigned int i = 0; i < 4; ++i)
		for (unsigned int j = 0; j < 4; ++j)
			EXPECT_NEAR(lSum(i,j), this->rand[4*j+i] + this->rand[16 + 4*j+i], this->err);
}

TYPED_TEST (TestMatrix4x4ArithmeticOperator, Subtraction)
{
	// Create a two default matrices  m0 and m1
	Matrix<TypeParam, FOUR, FOUR, 4, 4> m0, m1;

	// Set the coefficients of m0
	m0(0,0) = this->rand[0]; m0(0,1) = this->rand[4]; m0(0,2) = this->rand[ 8]; m0(0,3) = this->rand[12];
	m0(1,0) = this->rand[1]; m0(1,1) = this->rand[5]; m0(1,2) = this->rand[ 9]; m0(1,3) = this->rand[13];
	m0(2,0) = this->rand[2]; m0(2,1) = this->rand[6]; m0(2,2) = this->rand[10]; m0(2,3) = this->rand[14];
	m0(3,0) = this->rand[3]; m0(3,1) = this->rand[7]; m0(3,2) = this->rand[11]; m0(3,3) = this->rand[15];

	// Set the coefficients of m1
	m1(0,0) = this->rand[16]; m1(0,1) = this->rand[20]; m1(0,2) = this->rand[24]; m1(0,3) = this->rand[28];
	m1(1,0) = this->rand[17]; m1(1,1) = this->rand[21]; m1(1,2) = this->rand[25]; m1(1,3) = this->rand[29];
	m1(2,0) = this->rand[18]; m1(2,1) = this->rand[22]; m1(2,2) = this->rand[26]; m1(2,3) = this->rand[30];
	m1(3,0) = this->rand[19]; m1(3,1) = this->rand[23]; m1(3,2) = this->rand[27]; m1(3,3) = this->rand[31];

	// Compute subtraction
	Matrix<TypeParam, FOUR, FOUR, 4, 4> lSub = m0 - m1;
	
	for (unsigned int i = 0; i < 4; ++i)
		for (unsigned int j = 0; j < 4; ++j)
			EXPECT_NEAR(lSub(i,j), this->rand[4*j+i] - this->rand[16 + 4*j+i], this->err);
}

TYPED_TEST (TestMatrix4x4ArithmeticOperator, Multiplication)
{
	// Create two default matrices m0 and m1
	Matrix<TypeParam, FOUR, FOUR, 4, 4> m0, m1;

	// Set the coefficients of m0
	m0(0,0) = this->rand[0]; m0(0,1) = this->rand[4]; m0(0,2) = this->rand[ 8]; m0(0,3) = this->rand[12];
	m0(1,0) = this->rand[1]; m0(1,1) = this->rand[5]; m0(1,2) = this->rand[ 9]; m0(1,3) = this->rand[13];
	m0(2,0) = this->rand[2]; m0(2,1) = this->rand[6]; m0(2,2) = this->rand[10]; m0(2,3) = this->rand[14];
	m0(3,0) = this->rand[3]; m0(3,1) = this->rand[7]; m0(3,2) = this->rand[11]; m0(3,3) = this->rand[15];

	// Set the coefficients of m1
	m1(0,0) = this->rand[16]; m1(0,1) = this->rand[20]; m1(0,2) = this->rand[24]; m1(0,3) = this->rand[28];
	m1(1,0) = this->rand[17]; m1(1,1) = this->rand[21]; m1(1,2) = this->rand[25]; m1(1,3) = this->rand[29];
	m1(2,0) = this->rand[18]; m1(2,1) = this->rand[22]; m1(2,2) = this->rand[26]; m1(2,3) = this->rand[30];
	m1(3,0) = this->rand[19]; m1(3,1) = this->rand[23]; m1(3,2) = this->rand[27]; m1(3,3) = this->rand[31];

	// Compute multiplication
	Matrix<TypeParam, FOUR, FOUR, 4, 4> lMul = m0 * m1;
	
	EXPECT_NEAR(lMul(0,0), this->rand[4*0+0] * this->rand[16 + 4*0+0] + this->rand[4*1+0] * this->rand[16 + 4*0+1] + this->rand[4*2+0] * this->rand[16 + 4*0+2] + this->rand[4*3+0] * this->rand[16 + 4*0+3], this->err);
	EXPECT_NEAR(lMul(1,0), this->rand[4*0+1] * this->rand[16 + 4*0+0] + this->rand[4*1+1] * this->rand[16 + 4*0+1] + this->rand[4*2+1] * this->rand[16 + 4*0+2] + this->rand[4*3+1] * this->rand[16 + 4*0+3], this->err);
	EXPECT_NEAR(lMul(2,0), this->rand[4*0+2] * this->rand[16 + 4*0+0] + this->rand[4*1+2] * this->rand[16 + 4*0+1] + this->rand[4*2+2] * this->rand[16 + 4*0+2] + this->rand[4*3+2] * this->rand[16 + 4*0+3], this->err);
	EXPECT_NEAR(lMul(3,0), this->rand[4*0+3] * this->rand[16 + 4*0+0] + this->rand[4*1+3] * this->rand[16 + 4*0+1] + this->rand[4*2+3] * this->rand[16 + 4*0+2] + this->rand[4*3+3] * this->rand[16 + 4*0+3], this->err);

	EXPECT_NEAR(lMul(0,1), this->rand[4*0+0] * this->rand[16 + 4*1+0] + this->rand[4*1+0] * this->rand[16 + 4*1+1] + this->rand[4*2+0] * this->rand[16 + 4*1+2] + this->rand[4*3+0] * this->rand[16 + 4*1+3], this->err);
	EXPECT_NEAR(lMul(1,1), this->rand[4*0+1] * this->rand[16 + 4*1+0] + this->rand[4*1+1] * this->rand[16 + 4*1+1] + this->rand[4*2+1] * this->rand[16 + 4*1+2] + this->rand[4*3+1] * this->rand[16 + 4*1+3], this->err);
	EXPECT_NEAR(lMul(2,1), this->rand[4*0+2] * this->rand[16 + 4*1+0] + this->rand[4*1+2] * this->rand[16 + 4*1+1] + this->rand[4*2+2] * this->rand[16 + 4*1+2] + this->rand[4*3+2] * this->rand[16 + 4*1+3], this->err);
	EXPECT_NEAR(lMul(3,1), this->rand[4*0+3] * this->rand[16 + 4*1+0] + this->rand[4*1+3] * this->rand[16 + 4*1+1] + this->rand[4*2+3] * this->rand[16 + 4*1+2] + this->rand[4*3+3] * this->rand[16 + 4*1+3], this->err);

	EXPECT_NEAR(lMul(0,2), this->rand[4*0+0] * this->rand[16 + 4*2+0] + this->rand[4*1+0] * this->rand[16 + 4*2+1] + this->rand[4*2+0] * this->rand[16 + 4*2+2] + this->rand[4*3+0] * this->rand[16 + 4*2+3], this->err);
	EXPECT_NEAR(lMul(1,2), this->rand[4*0+1] * this->rand[16 + 4*2+0] + this->rand[4*1+1] * this->rand[16 + 4*2+1] + this->rand[4*2+1] * this->rand[16 + 4*2+2] + this->rand[4*3+1] * this->rand[16 + 4*2+3], this->err);
	EXPECT_NEAR(lMul(2,2), this->rand[4*0+2] * this->rand[16 + 4*2+0] + this->rand[4*1+2] * this->rand[16 + 4*2+1] + this->rand[4*2+2] * this->rand[16 + 4*2+2] + this->rand[4*3+2] * this->rand[16 + 4*2+3], this->err);
	EXPECT_NEAR(lMul(3,2), this->rand[4*0+3] * this->rand[16 + 4*2+0] + this->rand[4*1+3] * this->rand[16 + 4*2+1] + this->rand[4*2+3] * this->rand[16 + 4*2+2] + this->rand[4*3+3] * this->rand[16 + 4*2+3], this->err);

	EXPECT_NEAR(lMul(0,3), this->rand[4*0+0] * this->rand[16 + 4*3+0] + this->rand[4*1+0] * this->rand[16 + 4*3+1] + this->rand[4*2+0] * this->rand[16 + 4*3+2] + this->rand[4*3+0] * this->rand[16 + 4*3+3], this->err);
	EXPECT_NEAR(lMul(1,3), this->rand[4*0+1] * this->rand[16 + 4*3+0] + this->rand[4*1+1] * this->rand[16 + 4*3+1] + this->rand[4*2+1] * this->rand[16 + 4*3+2] + this->rand[4*3+1] * this->rand[16 + 4*3+3], this->err);
	EXPECT_NEAR(lMul(2,3), this->rand[4*0+2] * this->rand[16 + 4*3+0] + this->rand[4*1+2] * this->rand[16 + 4*3+1] + this->rand[4*2+2] * this->rand[16 + 4*3+2] + this->rand[4*3+2] * this->rand[16 + 4*3+3], this->err);
	EXPECT_NEAR(lMul(3,3), this->rand[4*0+3] * this->rand[16 + 4*3+0] + this->rand[4*1+3] * this->rand[16 + 4*3+1] + this->rand[4*2+3] * this->rand[16 + 4*3+2] + this->rand[4*3+3] * this->rand[16 + 4*3+3], this->err);
}

TYPED_TEST (TestMatrix4x4ArithmeticOperator, MultiplicationByScalar)
{
	// Create a default matrices m0
	Matrix<TypeParam, FOUR, FOUR, 4, 4> m0;

	m0(0,0) = this->rand[0]; m0(0,1) = this->rand[4]; m0(0,2) = this->rand[ 8]; m0(0,3) = this->rand[12];
	m0(1,0) = this->rand[1]; m0(1,1) = this->rand[5]; m0(1,2) = this->rand[ 9]; m0(1,3) = this->rand[13];
	m0(2,0) = this->rand[2]; m0(2,1) = this->rand[6]; m0(2,2) = this->rand[10]; m0(2,3) = this->rand[14];
	m0(3,0) = this->rand[3]; m0(3,1) = this->rand[7]; m0(3,2) = this->rand[11]; m0(3,3) = this->rand[15];

	// Compute multiplication
	TypeParam lScalar0 = this->rand[0];
	Matrix<TypeParam, FOUR, FOUR, 4, 4> lMul0 = m0 * lScalar0;

	EXPECT_NEAR(lMul0(0,0), m0(0,0) * lScalar0, this->err); 
	EXPECT_NEAR(lMul0(1,0), m0(1,0) * lScalar0, this->err); 
	EXPECT_NEAR(lMul0(2,0), m0(2,0) * lScalar0, this->err); 
	EXPECT_NEAR(lMul0(3,0), m0(3,0) * lScalar0, this->err); 
                                                         
	EXPECT_NEAR(lMul0(0,1), m0(0,1) * lScalar0, this->err); 
	EXPECT_NEAR(lMul0(1,1), m0(1,1) * lScalar0, this->err); 
	EXPECT_NEAR(lMul0(2,1), m0(2,1) * lScalar0, this->err); 
	EXPECT_NEAR(lMul0(3,1), m0(3,1) * lScalar0, this->err); 
                                                         
	EXPECT_NEAR(lMul0(0,2), m0(0,2) * lScalar0, this->err); 
	EXPECT_NEAR(lMul0(1,2), m0(1,2) * lScalar0, this->err); 
	EXPECT_NEAR(lMul0(2,2), m0(2,2) * lScalar0, this->err); 
	EXPECT_NEAR(lMul0(3,2), m0(3,2) * lScalar0, this->err); 
                                                         
	EXPECT_NEAR(lMul0(0,3), m0(0,3) * lScalar0, this->err); 
	EXPECT_NEAR(lMul0(1,3), m0(1,3) * lScalar0, this->err); 
	EXPECT_NEAR(lMul0(2,3), m0(2,3) * lScalar0, this->err); 
	EXPECT_NEAR(lMul0(3,3), m0(3,3) * lScalar0, this->err); 

	// Create a default matrices m1 equal to m0
	Matrix<TypeParam, FOUR, FOUR, 4, 4> m1 = m0;

	// Compute multiplication and assignment
	TypeParam lScalar1 = this->rand[1];
	m1 *= lScalar1;

	EXPECT_NEAR(m1(0,0), m0(0,0) * lScalar1, this->err); 
	EXPECT_NEAR(m1(1,0), m0(1,0) * lScalar1, this->err); 
	EXPECT_NEAR(m1(2,0), m0(2,0) * lScalar1, this->err); 
	EXPECT_NEAR(m1(3,0), m0(3,0) * lScalar1, this->err); 
                                                    
	EXPECT_NEAR(m1(0,1), m0(0,1) * lScalar1, this->err); 
	EXPECT_NEAR(m1(1,1), m0(1,1) * lScalar1, this->err); 
	EXPECT_NEAR(m1(2,1), m0(2,1) * lScalar1, this->err); 
	EXPECT_NEAR(m1(3,1), m0(3,1) * lScalar1, this->err); 
                                                    
	EXPECT_NEAR(m1(0,2), m0(0,2) * lScalar1, this->err); 
	EXPECT_NEAR(m1(1,2), m0(1,2) * lScalar1, this->err); 
	EXPECT_NEAR(m1(2,2), m0(2,2) * lScalar1, this->err); 
	EXPECT_NEAR(m1(3,2), m0(3,2) * lScalar1, this->err); 
                                                    
	EXPECT_NEAR(m1(0,3), m0(0,3) * lScalar1, this->err); 
	EXPECT_NEAR(m1(1,3), m0(1,3) * lScalar1, this->err); 
	EXPECT_NEAR(m1(2,3), m0(2,3) * lScalar1, this->err); 
	EXPECT_NEAR(m1(3,3), m0(3,3) * lScalar1, this->err); 
}

TYPED_TEST (TestMatrix4x4ArithmeticOperator, DivisionByScalar)
{
	// Create a default matrices m0
	Matrix<TypeParam, FOUR, FOUR, 4, 4> m0;

	m0(0,0) = this->rand[0]; m0(0,1) = this->rand[4]; m0(0,2) = this->rand[ 8]; m0(0,3) = this->rand[12];
	m0(1,0) = this->rand[1]; m0(1,1) = this->rand[5]; m0(1,2) = this->rand[ 9]; m0(1,3) = this->rand[13];
	m0(2,0) = this->rand[2]; m0(2,1) = this->rand[6]; m0(2,2) = this->rand[10]; m0(2,3) = this->rand[14];
	m0(3,0) = this->rand[3]; m0(3,1) = this->rand[7]; m0(3,2) = this->rand[11]; m0(3,3) = this->rand[15];

	// Compute multiplication
	TypeParam lScalar0 = this->rand[0];

	// Avoid dividing by zero
	if (lScalar0 == 0.0f)
		lScalar0 += 0.1f;

	Matrix<TypeParam, FOUR, FOUR, 4, 4> lDiv0 = m0 / lScalar0;

	EXPECT_NEAR(lDiv0(0,0), m0(0,0) / lScalar0, this->err); 
	EXPECT_NEAR(lDiv0(1,0), m0(1,0) / lScalar0, this->err); 
	EXPECT_NEAR(lDiv0(2,0), m0(2,0) / lScalar0, this->err); 
	EXPECT_NEAR(lDiv0(3,0), m0(3,0) / lScalar0, this->err); 
                                                       
	EXPECT_NEAR(lDiv0(0,1), m0(0,1) / lScalar0, this->err); 
	EXPECT_NEAR(lDiv0(1,1), m0(1,1) / lScalar0, this->err); 
	EXPECT_NEAR(lDiv0(2,1), m0(2,1) / lScalar0, this->err); 
	EXPECT_NEAR(lDiv0(3,1), m0(3,1) / lScalar0, this->err); 
                                                       
	EXPECT_NEAR(lDiv0(0,2), m0(0,2) / lScalar0, this->err); 
	EXPECT_NEAR(lDiv0(1,2), m0(1,2) / lScalar0, this->err); 
	EXPECT_NEAR(lDiv0(2,2), m0(2,2) / lScalar0, this->err); 
	EXPECT_NEAR(lDiv0(3,2), m0(3,2) / lScalar0, this->err); 
                                                       
	EXPECT_NEAR(lDiv0(0,3), m0(0,3) / lScalar0, this->err); 
	EXPECT_NEAR(lDiv0(1,3), m0(1,3) / lScalar0, this->err); 
	EXPECT_NEAR(lDiv0(2,3), m0(2,3) / lScalar0, this->err); 
	EXPECT_NEAR(lDiv0(3,3), m0(3,3) / lScalar0, this->err); 

	// Create a default matrices m1 equal to m0
	Matrix<TypeParam, FOUR, FOUR, 4, 4> m1 = m0;

	// Compute multiplication and assignment
	TypeParam lScalar1 = this->rand[1];

	// Avoid dividing by zero
	if (lScalar1 == 0.0f)
		lScalar1 += 0.1f;

	m1 /= lScalar1;

	EXPECT_NEAR(m1(0,0), m0(0,0) / lScalar1, this->err); 
	EXPECT_NEAR(m1(1,0), m0(1,0) / lScalar1, this->err); 
	EXPECT_NEAR(m1(2,0), m0(2,0) / lScalar1, this->err); 
	EXPECT_NEAR(m1(3,0), m0(3,0) / lScalar1, this->err); 
                                                    
	EXPECT_NEAR(m1(0,1), m0(0,1) / lScalar1, this->err); 
	EXPECT_NEAR(m1(1,1), m0(1,1) / lScalar1, this->err); 
	EXPECT_NEAR(m1(2,1), m0(2,1) / lScalar1, this->err); 
	EXPECT_NEAR(m1(3,1), m0(3,1) / lScalar1, this->err); 
                                                    
	EXPECT_NEAR(m1(0,2), m0(0,2) / lScalar1, this->err); 
	EXPECT_NEAR(m1(1,2), m0(1,2) / lScalar1, this->err); 
	EXPECT_NEAR(m1(2,2), m0(2,2) / lScalar1, this->err); 
	EXPECT_NEAR(m1(3,2), m0(3,2) / lScalar1, this->err); 
                                                    
	EXPECT_NEAR(m1(0,3), m0(0,3) / lScalar1, this->err); 
	EXPECT_NEAR(m1(1,3), m0(1,3) / lScalar1, this->err); 
	EXPECT_NEAR(m1(2,3), m0(2,3) / lScalar1, this->err); 
	EXPECT_NEAR(m1(3,3), m0(3,3) / lScalar1, this->err); 
}

TYPED_TEST (TestMatrix4x4ArithmeticOperator, MultiplicationByVector)
{
	// Create a default matrix m
	Matrix<TypeParam, FOUR, FOUR, 4, 4> m;

	// Set the coefficients of m
	m(0,0) = this->rand[0]; m(0,1) = this->rand[4]; m(0,2) = this->rand[ 8]; m(0,3) = this->rand[12];
	m(1,0) = this->rand[1]; m(1,1) = this->rand[5]; m(1,2) = this->rand[ 9]; m(1,3) = this->rand[13];
	m(2,0) = this->rand[2]; m(2,1) = this->rand[6]; m(2,2) = this->rand[10]; m(2,3) = this->rand[14];
	m(3,0) = this->rand[3]; m(3,1) = this->rand[7]; m(3,2) = this->rand[11]; m(3,3) = this->rand[15];

	Vector<TypeParam, FOUR, 4> v(this->rand[0], this->rand[1], this->rand[2], this->rand[3]);

	Vector<TypeParam, FOUR, 4> lMul = m * v;

	EXPECT_EQ(lMul[0], m(0,0) * v[0] +  m(0,1) * v[1] +  m(0,2) * v[2] +  m(0,3) * v[3]); 
	EXPECT_EQ(lMul[1], m(1,0) * v[0] +  m(1,1) * v[1] +  m(1,2) * v[2] +  m(1,3) * v[3]); 
	EXPECT_EQ(lMul[2], m(2,0) * v[0] +  m(2,1) * v[1] +  m(2,2) * v[2] +  m(2,3) * v[3]); 
	EXPECT_EQ(lMul[3], m(3,0) * v[0] +  m(3,1) * v[1] +  m(3,2) * v[2] +  m(3,3) * v[3]); 
}

TYPED_TEST (TestMatrix4x4ArithmeticOperator, Transpose)
{
	// Create a default matrix m
	Matrix<TypeParam, FOUR, FOUR, 4, 4> m, mCopy;

	// Set the coefficients of m
	m(0,0) = this->rand[0]; m(0,1) = this->rand[4]; m(0,2) = this->rand[ 8]; m(0,3) = this->rand[12];
	m(1,0) = this->rand[1]; m(1,1) = this->rand[5]; m(1,2) = this->rand[ 9]; m(1,3) = this->rand[13];
	m(2,0) = this->rand[2]; m(2,1) = this->rand[6]; m(2,2) = this->rand[10]; m(2,3) = this->rand[14];
	m(3,0) = this->rand[3]; m(3,1) = this->rand[7]; m(3,2) = this->rand[11]; m(3,3) = this->rand[15];

	// Keep a copy of m before applying the transpose
	mCopy = m;
	m.transpose();

	// Check that the coefficients were transposed correctly 
	ASSERT_EQ(m(0,0), static_cast<TypeParam>(this->rand[0])); ASSERT_EQ(m(1,0), static_cast<TypeParam>(this->rand[4])); ASSERT_EQ(m(2,0), static_cast<TypeParam>(this->rand[ 8])); ASSERT_EQ(m(3,0), static_cast<TypeParam>(this->rand[12]));
	ASSERT_EQ(m(0,1), static_cast<TypeParam>(this->rand[1])); ASSERT_EQ(m(1,1), static_cast<TypeParam>(this->rand[5])); ASSERT_EQ(m(2,1), static_cast<TypeParam>(this->rand[ 9])); ASSERT_EQ(m(3,1), static_cast<TypeParam>(this->rand[13]));
	ASSERT_EQ(m(0,2), static_cast<TypeParam>(this->rand[2])); ASSERT_EQ(m(1,2), static_cast<TypeParam>(this->rand[6])); ASSERT_EQ(m(2,2), static_cast<TypeParam>(this->rand[10])); ASSERT_EQ(m(3,2), static_cast<TypeParam>(this->rand[14]));
	ASSERT_EQ(m(0,3), static_cast<TypeParam>(this->rand[3])); ASSERT_EQ(m(1,3), static_cast<TypeParam>(this->rand[7])); ASSERT_EQ(m(2,3), static_cast<TypeParam>(this->rand[11])); ASSERT_EQ(m(3,3), static_cast<TypeParam>(this->rand[15]));

	// Transpose m once more
	m.transpose();

	// Ensure that the transpose of the transpose is the idenity
	ASSERT_TRUE(m == mCopy) << "The transpose of the transpose should equal the idenity";
}


TYPED_TEST (TestMatrix4x4ArithmeticOperator, Determinant)
{
	// Message to ouput
	TEST_COUT("Determinant computation has accuracy limitation according to the coefficients in the matrix, even using double");
	const auto lAccuracy = 0.000001;

	// Create a default matrix m
	Matrix<TypeParam, FOUR, FOUR, 4, 4> m;

	// Set the coefficients of m (randomly chosen but I know that the corresponding determinant is -20372.1664 )
	m(0,0) = static_cast<TypeParam>( 4.0); m(0,1) = static_cast<TypeParam>(-5.1); m(0,2) = static_cast<TypeParam>(-9.9); m(0,3) = static_cast<TypeParam>( 5.5);
	m(1,0) = static_cast<TypeParam>(-8.3); m(1,1) = static_cast<TypeParam>( 4.2); m(1,2) = static_cast<TypeParam>(-1.1); m(1,3) = static_cast<TypeParam>(-9.8);
	m(2,0) = static_cast<TypeParam>( 6.4); m(2,1) = static_cast<TypeParam>( 3.2); m(2,2) = static_cast<TypeParam>( 7.0); m(2,3) = static_cast<TypeParam>( 7.5);
	m(3,0) = static_cast<TypeParam>(-9.1); m(3,1) = static_cast<TypeParam>( 7.2); m(3,2) = static_cast<TypeParam>( 8.7); m(3,3) = static_cast<TypeParam>( 5.5);

	EXPECT_FLOAT_EQ(m.determinant(), 6810.0026) << "The determinant of m should be 6810.0026";

	// Create a matrix that will have 1 null column
	Matrix<TypeParam, FOUR, FOUR, 4, 4> mNullColumn;

	for (unsigned int i = 0; i < 4; ++i)
	{
		// Copy m to mNullColumn
		mNullColumn = m;

		// Set column i to 0
		mNullColumn(0,i) = static_cast<TypeParam>(0.0);
		mNullColumn(1,i) = static_cast<TypeParam>(0.0);
		mNullColumn(2,i) = static_cast<TypeParam>(0.0);
		mNullColumn(3,i) = static_cast<TypeParam>(0.0);

		EXPECT_NEAR(mNullColumn.determinant(), 0.0, lAccuracy) << "The determinant of a matrix with 1 null column should be 0";
	}

	// Create a matrix that will have 1 null row
	Matrix<TypeParam, FOUR, FOUR, 4, 4> mNullRow;

	for (unsigned int i = 0; i < 4; ++i)
	{
		// Copy m to mNullColumn
		mNullRow = m;

		// Set column i to 0
		mNullRow(i,0) = static_cast<TypeParam>(0.0);
		mNullRow(i,1) = static_cast<TypeParam>(0.0);
		mNullRow(i,2) = static_cast<TypeParam>(0.0);
		mNullRow(i,3) = static_cast<TypeParam>(0.0);

		EXPECT_NEAR(mNullRow.determinant(), 0.0,  lAccuracy) << "The determinant of matrix with 1 null row should be 0";
	}

	// Create a matrix with two identical columns
	Matrix<TypeParam, FOUR, FOUR, 4, 4> mTwoIdenticalColumns;

	mTwoIdenticalColumns(0,0) = this->rand[0]; mTwoIdenticalColumns(0,1) = this->rand[0]; mTwoIdenticalColumns(0,2) = this->rand[ 8]; mTwoIdenticalColumns(0,3) = this->rand[12];
	mTwoIdenticalColumns(1,0) = this->rand[1]; mTwoIdenticalColumns(1,1) = this->rand[1]; mTwoIdenticalColumns(1,2) = this->rand[ 9]; mTwoIdenticalColumns(1,3) = this->rand[13];
	mTwoIdenticalColumns(2,0) = this->rand[2]; mTwoIdenticalColumns(2,1) = this->rand[2]; mTwoIdenticalColumns(2,2) = this->rand[10]; mTwoIdenticalColumns(2,3) = this->rand[14];
	mTwoIdenticalColumns(3,0) = this->rand[3]; mTwoIdenticalColumns(3,1) = this->rand[3]; mTwoIdenticalColumns(3,2) = this->rand[11]; mTwoIdenticalColumns(3,3) = this->rand[15];

	EXPECT_NEAR(mTwoIdenticalColumns.determinant(), 0.0, lAccuracy) << "The determinant of a matrix with 2 identical columns should be 0";

	// Create a matrix with two identical columns
	Matrix<TypeParam, FOUR, FOUR, 4, 4> mTwoIdenticalRows;

	mTwoIdenticalRows(0,0) = this->rand[0]; mTwoIdenticalRows(0,1) = this->rand[4]; mTwoIdenticalRows(0,2) = this->rand[ 8]; mTwoIdenticalRows(0,3) = this->rand[12];
	mTwoIdenticalRows(1,0) = this->rand[1]; mTwoIdenticalRows(1,1) = this->rand[5]; mTwoIdenticalRows(1,2) = this->rand[ 9]; mTwoIdenticalRows(1,3) = this->rand[13];
	mTwoIdenticalRows(2,0) = this->rand[2]; mTwoIdenticalRows(2,1) = this->rand[6]; mTwoIdenticalRows(2,2) = this->rand[10]; mTwoIdenticalRows(2,3) = this->rand[14];
	mTwoIdenticalRows(3,0) = this->rand[0]; mTwoIdenticalRows(3,1) = this->rand[4]; mTwoIdenticalRows(3,2) = this->rand[ 8]; mTwoIdenticalRows(3,3) = this->rand[12];

	EXPECT_NEAR(mTwoIdenticalRows.determinant(), 0.0, lAccuracy) << "The determinant of a matrix with 2 identical rows should be 0";
}

TYPED_TEST (TestMatrix4x4ArithmeticOperator, Inverse)
{
	// Message to ouput
	TEST_COUT("Inverse computation has accuracy limitation according to the coefficients in the matrix, even using double");
	const auto lAccuracy = 0.000001;

	// Create default matrices m0 and m1
	Matrix<TypeParam, FOUR, FOUR, 4, 4> m0, m1;

	// Set the coefficients of m (randomly chosen but I know that the corresponding inverse, computed on wolfram alpha)
	m0(0,0) = static_cast<TypeParam>( 4.0); m0(0,1) = static_cast<TypeParam>(-5.1); m0(0,2) = static_cast<TypeParam>(-9.9); m0(0,3) = static_cast<TypeParam>( 5.5);
	m0(1,0) = static_cast<TypeParam>(-8.3); m0(1,1) = static_cast<TypeParam>( 4.2); m0(1,2) = static_cast<TypeParam>(-1.1); m0(1,3) = static_cast<TypeParam>(-9.8);
	m0(2,0) = static_cast<TypeParam>( 6.4); m0(2,1) = static_cast<TypeParam>( 3.2); m0(2,2) = static_cast<TypeParam>( 7.0); m0(2,3) = static_cast<TypeParam>( 7.5);
	m0(3,0) = static_cast<TypeParam>(-9.1); m0(3,1) = static_cast<TypeParam>( 7.2); m0(3,2) = static_cast<TypeParam>( 8.7); m0(3,3) = static_cast<TypeParam>( 5.5);

	// Keep a copy of m0
	m1 = m0;

	// Compute the inverse
	m0.inverse();

	EXPECT_NEAR(m0(0,0), 0.0100878   , lAccuracy); 
	EXPECT_NEAR(m0(1,0), 0.122482    , lAccuracy); 
	EXPECT_NEAR(m0(2,0), -0.127655   , lAccuracy); 
	EXPECT_NEAR(m0(3,0), 0.0582772   , lAccuracy); 

	EXPECT_NEAR(m0(0,1),  0.0511035  , lAccuracy); 
	EXPECT_NEAR(m0(1,1),  0.231093   , lAccuracy); 
	EXPECT_NEAR(m0(2,1), -0.116827   , lAccuracy); 
	EXPECT_NEAR(m0(3,1), -0.0331692  , lAccuracy); 

	EXPECT_NEAR(m0(0,2),  0.112745   , lAccuracy); 
	EXPECT_NEAR(m0(1,2),  0.215886   , lAccuracy); 
	EXPECT_NEAR(m0(2,2), -0.0633572  , lAccuracy); 
	EXPECT_NEAR(m0(3,2),  0.00414625 , lAccuracy); 

	EXPECT_NEAR(m0(0,3), -0.0727737  , lAccuracy); 
	EXPECT_NEAR(m0(1,3), -0.00510675 , lAccuracy); 
	EXPECT_NEAR(m0(2,3),  0.0058859  , lAccuracy); 
	EXPECT_NEAR(m0(3,3),  0.0587856  , lAccuracy); 

	// Compute the inverse ),ing the const version
	auto m3 = m1.inversed();

	EXPECT_NEAR(m3(0,0), 0.0100878   , lAccuracy); 
	EXPECT_NEAR(m3(1,0), 0.122482    , lAccuracy); 
	EXPECT_NEAR(m3(2,0), -0.127655   , lAccuracy); 
	EXPECT_NEAR(m3(3,0), 0.0582772   , lAccuracy); 

	EXPECT_NEAR(m3(0,1),  0.0511035  , lAccuracy); 
	EXPECT_NEAR(m3(1,1),  0.231093   , lAccuracy); 
	EXPECT_NEAR(m3(2,1), -0.116827   , lAccuracy); 
	EXPECT_NEAR(m3(3,1), -0.0331692  , lAccuracy); 

	EXPECT_NEAR(m3(0,2),  0.112745   , lAccuracy); 
	EXPECT_NEAR(m3(1,2),  0.215886   , lAccuracy); 
	EXPECT_NEAR(m3(2,2), -0.0633572  , lAccuracy); 
	EXPECT_NEAR(m3(3,2),  0.00414625 , lAccuracy); 

	EXPECT_NEAR(m3(0,3), -0.0727737  , lAccuracy); 
	EXPECT_NEAR(m3(1,3), -0.00510675 , lAccuracy); 
	EXPECT_NEAR(m3(2,3),  0.0058859  , lAccuracy); 
	EXPECT_NEAR(m3(3,3),  0.0587856  , lAccuracy); 
}
