//
//  PosNeg.hpp
//  PosNegUtility
//
//  Created by tridiak on 20/03/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#ifndef PosNeg_hpp
#define PosNeg_hpp

#include <stdio.h>
#include <exception>

typedef class PNPositive PNPositive;
typedef class PNZero PNZero;
typedef class PNNegative PNNegative;

// Class that represents a positive, negative or zero value.
// The object is always immutable.
// See tables at end of file.
class CPosNeg {
	
protected:
	CPosNeg() {};
	// These are immutable.
	static const PNPositive positive;
	static const PNNegative negative;
	static const PNZero pnzero;
	
	friend const CPosNeg& operator!(const CPosNeg& pn);
public:
	struct nonsensicalEx : std::exception {
		virtual const char* what() const _NOEXCEPT {
			return "nonsensical";
			};
		};
	
	virtual char V() const = 0;
	virtual const char* Name() const = 0;
	
	virtual bool isPositive() = 0;
	virtual bool isNegative() = 0;
	virtual bool isZero() = 0;
	
	// Short hand methods for those who don't like to type.
	virtual bool P() const = 0;
	virtual bool N() const = 0;
	virtual bool Z() const = 0;
	
	// Use this method to create an instance of CPosNeg.
	static const CPosNeg& Create(long v);
		// {return v < 0 ? negative : (v > 0 ? PNPositive() : zero);}
		// Classes not yet defined so compiler complains.
	
	// Multiplier
	virtual const CPosNeg& operator*(const CPosNeg& pn) const = 0;
	
	// Division by zero will throw an exception
	virtual const CPosNeg& operator/(const CPosNeg& pn) const = 0;
	
	// Can throw an exception. See tables below.
	virtual const CPosNeg& operator+(const CPosNeg& pn) const = 0;
	
	// Can throw an exception. See tables below.
	virtual const CPosNeg& operator-(const CPosNeg& pn) const = 0;
	
	// Multiply a number
	virtual long long operator*(long long v) const = 0;
	virtual double operator*(double v) const = 0;
	
	virtual bool operator<(const CPosNeg& pn) const = 0;
	virtual bool operator>(const CPosNeg& pn) const = 0;
	virtual bool operator==(const CPosNeg& pn) const = 0;
	virtual bool operator!=(const CPosNeg& pn) const = 0;
	
	//----
	// Postfix op.
	virtual const CPosNeg& operator++(int) const = 0;
	virtual const CPosNeg& operator--(int) const = 0;
	// No prefix op. The class is immutable.
	};

// CPosNeg is RHS.
long long operator*(long long value, const CPosNeg& pn);

double operator*(double value, const CPosNeg& pn);

// Division by zero will throw a nonsensicalEx exception.
long long operator/(long long value, const CPosNeg& pn);

double operator/(double value, const CPosNeg& pn);

/*
There is no addition or subtraction operators.
Adding some arbitary number to a positive or negative is non-sensical.
*/

//---------------------------------------------
#pragma mark - Positive

class PNPositive : public CPosNeg {
public:
	virtual char V() const { return 1; }
	virtual const char* Name() const { return "Positive"; }
	
	virtual bool isPositive() { return true; }
	virtual bool isNegative() { return false; }
	virtual bool isZero() { return false; }
	
	// Short hand methods for those who don't like to type.
	virtual bool P() const { return true; }
	virtual bool N() const { return false; }
	virtual bool Z() const { return false; }
	
	// Multiplier
	virtual const CPosNeg& operator*(const CPosNeg& pn) const {
		return pn;
	};
	
	// Division by zero will throw an exception
	virtual const CPosNeg& operator/(const CPosNeg& pn) const {
		if (pn.Z()) { throw nonsensicalEx(); }
		return pn;
	};

	// Can throw an exception. See tables below.
	virtual const CPosNeg& operator+(const CPosNeg& pn) const {
		if (pn.N()) { throw nonsensicalEx(); }
		return positive;
	};

	
	// Can throw an exception. See tables below.
	virtual const CPosNeg& operator-(const CPosNeg& pn) const {
		if (pn.P()) { throw nonsensicalEx(); }
		return positive;
	};
	
	// Multiply a number
	virtual long long operator*(long long v) const { return v; }
	virtual double operator*(double v) const { return v; }
	
	virtual bool operator<(const CPosNeg& pn) const { return false; }
	virtual bool operator>(const CPosNeg& pn) const { return !pn.P(); }
	virtual bool operator==(const CPosNeg& pn) const { return pn.P(); }
	virtual bool operator!=(const CPosNeg& pn) const { return !pn.P(); }
	
	//----
	// Postfix op.
	virtual const CPosNeg& operator++(int) const { return positive; }
	// class PNZero not yet defined so compiler complains
	virtual const CPosNeg& operator--(int) const; // { return pnzero; }
	// No prefix op. The class is immutable.
};

//---------------------------------------------
#pragma mark - Negative

class PNNegative : public CPosNeg {
public:
	virtual char V() const { return -1; }
	virtual const char* Name() const { return "Negative"; }
	
	virtual bool isPositive() { return false; }
	virtual bool isNegative() { return true; }
	virtual bool isZero() { return false; }
	
	// Short hand methods for those who don't like to type.
	virtual bool P() const { return false; }
	virtual bool N() const { return true; }
	virtual bool Z() const { return false; }
	
	// Multiplier
	virtual const CPosNeg& operator*(const CPosNeg& pn) const {
		return !pn;
	};
	
	// Division by zero will throw an exception
	virtual const CPosNeg& operator/(const CPosNeg& pn) const {
		if (pn.Z()) { throw nonsensicalEx(); }
		return !pn;
	};
	
	// Can throw an exception. See tables below.
	virtual const CPosNeg& operator+(const CPosNeg& pn) const {
		if (pn.P()) { throw nonsensicalEx(); }
		return negative;
	};
	
	// Can throw an exception. See tables below.
	virtual const CPosNeg& operator-(const CPosNeg& pn) const {
		if (pn.N()) { throw nonsensicalEx(); }
		return negative;
	};
	
	// Multiply a number
	virtual long long operator*(long long v) const { return -v; }
	virtual double operator*(double v) const { return -v; }
	
	virtual bool operator<(const CPosNeg& pn) const { return !pn.N(); }
	virtual bool operator>(const CPosNeg& pn) const { return false; }
	virtual bool operator==(const CPosNeg& pn) const { return pn.N(); }
	virtual bool operator!=(const CPosNeg& pn) const { return !pn.N(); }
	
	//----
	// Postfix op.
	// class PNZero not yet defined so compiler complains
	virtual const CPosNeg& operator++(int) const; // { return pnzero; }
	virtual const CPosNeg& operator--(int) const { return negative; }
	// No prefix op. The class is immutable.
};

//---------------------------------------------
#pragma mark - Zero

class PNZero : public CPosNeg {
public:
	virtual char V() const { return 0; }
	virtual const char* Name() const { return "Zero"; }
	
	virtual bool isPositive() { return false; }
	virtual bool isNegative() { return false; }
	virtual bool isZero() { return true; }
	
	// Short hand methods for those who don't like to type.
	virtual bool P() const { return false; }
	virtual bool N() const { return false; }
	virtual bool Z() const { return true; }
	
	// Multiplier
	virtual const CPosNeg& operator*(const CPosNeg& pn) const { return pnzero; }
	
	// Division by zero will throw an exception
	virtual const CPosNeg& operator/(const CPosNeg& pn) const {
		if (pn.Z()) { throw nonsensicalEx(); }
		return *this;
	};
	
	// Can throw an exception. See tables below.
	virtual const CPosNeg& operator+(const CPosNeg& pn) const { return pn; }
	
	// Can throw an exception. See tables below.
	virtual const CPosNeg& operator-(const CPosNeg& pn) const {
		return !pn;
	}
	
	// Multiply a number
	virtual long long operator*(long long v) const { return 0; }
	virtual double operator*(double v) const{ return 0; }
	
	virtual bool operator<(const CPosNeg& pn) const { return pn.P(); }
	virtual bool operator>(const CPosNeg& pn) const { return pn.N(); }
	virtual bool operator==(const CPosNeg& pn) const { return pn.Z(); }
	virtual bool operator!=(const CPosNeg& pn) const { return !pn.Z(); }
	
	//----
	// Postfix op.
	virtual const CPosNeg& operator++(int) const { return positive; }
	virtual const CPosNeg& operator--(int) const { return negative; }
	// No prefix op. The class is immutable.
};

//---------------------------------------------

/*
Algebra tables.
X implies non-sensical and an exception will be thrown.

Add	-	0	+
-	-	-	X
0	-	0	+
+	X	+	+

Sub	-	0	+
-	X	-	-
0	+	0	-
+	+	+	X

Mul	-	0	+
-	+	0	-
0	0	0	0
+	-	0	+

Div	-	0	+
-	+	X	-
0	0	X	0
+	-	X	+

Not	-	0	+
	+	0	-


LT	-	0	+
-	F	T	T
0	F	F	T
+	F	F	F

GT	-	0	+
-	F	F	F
0	T	F	F
+	T	T	F

EQ	-	0	+
-	T	F	F
0	F	T	F
+	F	F	T


--------------------------------------------------------
Variant : -inf, -ve, 0, +ve, +inf
Not implemented.

Add	-I	-	0	+	+I
-I	-I	-I	-I	-I	X
-	-I	-	-	X	+I
0	-I	-	0	+	+I
+	-I	X	+	+	+I
+I	X	+I	+I	+I	+I

Sub	-I	-	0	+	+I
-I	X	-I	-I	-I	-I
-	+I	X	0	-	-I
0	+I	+	0	-	-I
+	+I	+	+	X	-I
+I	+I	+I	+I	+I	0

Mul	-I	-	0	+	+I
-I	+I	+I	0	-I	-I
-	+I	+	0	-	-I
0	0	0	0	0	0
+	-I	-	0	+	+I
+I	-I	-I	0	+I	+I

Div	-I	-	0	+	+I
-I	+I	+I	X	-I	-I
-	+I	+	X	-	-I
0	0	0	X	0	0
+	-I	-	X	+	+I
+I	-I	-I	X	+I	+I

Not	-I	-	0	+	+I
	+I	+	0	-	-I

LT	-I	-	0	+	+I
-I	F	T	T	T	T
-	F	F	T	T	T
0	F	F	F	T	T
+	F	F	F	F	T
+I	F	F	F	F	F

GT	-I	-	0	+	+I
-I	F	F	F	F	F
-	T	F	F	F	F
0	T	T	F	F	F
+	T	T	T	F	F
+I	T	T	T	T	F

EQ	-I	-	0	+	+I
-I	T	F	F	F	F
-	F	T	F	F	F
0	F	F	T	F	F
+	F	F	F	T	F
+I	F	F	F	F	T


	-I	-	0	+	+I
-I
-
0
+
+I

*/

#endif /* PosNeg_hpp */
