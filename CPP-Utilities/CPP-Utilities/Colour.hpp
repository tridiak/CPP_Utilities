//
//  Colour.hpp
//  CPP-Utilities
//
//  Created by tridiak on 1/11/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#ifndef Colour_hpp
#define Colour_hpp

#include <stdio.h>
#include <string>
#include <exception>

struct ColourEx : std::exception {
	std::string reason;
	ColourEx(const std::string& reason) : reason(reason) {}
	const char* what() const _NOEXCEPT {
		return (std::string("Colour exception: ") + reason.c_str()).c_str();
	}
};

//-------------------
// Using short instead of unsigned char because in future may allow
// ranges outside of 0-1
typedef short Col8;

//--------------------

// Colour class for colour amateurs like me.
// Handles colour conversion.
// The colour is stored as RGB regardless of what colour space is passed to
// the constructors.
class Colour {
public:
	struct ColInfo {
		// All ranges are 0-1 except for Hue(H), its range is 0-360.
		float V0 = 0; // R, C, H
		float V1 = 0; // G, M, S
		float V2 = 0; // B, Y, V
		float V3 = 0; // -, K, -
		float operator[](unsigned char idx) const {
			switch (idx) {
				case 0: return V0;
				case 1: return V1;
				case 2: return V2;
				case 3: return V3;
				
			};
		throw ColourEx("Out of range");
		};
		
		float& operator[](unsigned char idx) {
			switch (idx) {
				case 0: return V0;
				case 1: return V1;
				case 2: return V2;
				case 3: return V3;
				
			};
		throw ColourEx("Out of range");
		};
		
		float& R() { return V0; }
		float& G() { return V1; }
		float& B() { return V2; }
		float& C() { return V0; }
		float& M() { return V1; }
		float& Y() { return V2; }
		float& K() { return V3; }
		float& H() { return V0; }
		float& S() { return V1; }
		float& V() { return V2; }
		
		float R() const { return V0; }
		float G() const { return V1; }
		float B() const { return V2; }
		float C() const { return V0; }
		float M() const { return V1; }
		float Y() const { return V2; }
		float K() const { return V3; }
		float H() const { return V0; }
		float S() const { return V1; }
		float V() const { return V2; }
	}; // struct ColInfo
private:
	// Colour stored as RGB
	ColInfo rgb;
	float alpha;
	
	void Constrain(float& V);
public:
	enum class NamedColour {
		white,		// 1,1,1
		black,		// 0,0,0
		red,		// 1,0,0
		green,		// 0,1,1
		blue,		// 0,0,1
		cyan,		// 0,1,1
		magenta,	// 1,0,1
		yellow,		// 1,1,0
		lightGray,	// 0.75, 0.75, 0.75
		darkGray,	// 0.25, 0.25, 0.25
		gray,		// 0.5, 0.5, 0.5
	};
	
	//------------------------
	// Constrain() is called inside the constructors.
	Colour(); // black
	Colour(NamedColour name);
	Colour(float R, float G, float B, float A = 1);
	Colour(Col8 R, Col8 G, Col8 B, Col8 A = 255);
	
	Colour(float grayScale);
	Colour(Col8 grayScale);
	
	Colour(float C, float M, float Y, float K, float alpha = 1);
	Colour(Col8 C, Col8 M, Col8 Y, Col8 K, Col8 alpha = 255);
	
	enum class ColType {
		RGB, HSV, CMYK,
	};
	
	Colour(const ColInfo& col, ColType which);
	
	//-------------
	
	void Constrain();
	
	// Get methods
	ColInfo RGB() const;
	ColInfo RGBA(float& A) const;
	
	ColInfo CMYK() const;
	ColInfo CMYK(float& alpha) const;
	
	ColInfo HSB() const;
	ColInfo HSB(float& alpha) const;
	
	float Red() const;
	float Green() const;
	float Blue() const;
	float Alpha() const;
	
	// Converts from RGB o CYMK
	float Cyan() const;
	float Magenta() const;
	float Yellow() const;
	float Black() const;
	
	// Converts from RGB to HSV/B
	float Hue() const;
	float Saturation() const;
	float Brightness() const;
	
	// Will return the average of R,G,B if not equal
	float Gray() const;
	
	//--------
	// Multiply R,G,B by value.
	// Constrain() will be called.
	void LightDark(float value);
	
	//--------
	// Returns <prefix>RRGGBB(AA).
	// If any of R,G,B or A is outside of 0-1, an exception will be thrown.
	std::string HexString(const std::string& prefix, bool inclAlpha) const;
	
	// Return hex string for each component. Will not throw an exception if
	// any value is outside of 0-1.
	void HexString(std::string& redString, std::string& greenString, std::string& blueString, std::string& alphaString) const;
	
	// Returns <prefix>RRRGGGBBB(AAA). Leading zeros will be added if necessary.
	// If any of R,G,B or A is outside of 0-1, an exception will be thrown.
	std::string IntString(const std::string& prefix, bool inclAlpha) const;
	
	// Converters
	
	// ColourEx will be thrown if any input float value is
	// outside of 0-1 except H, it must be between 0 - 360.
	static void RGBtoCMYK(const ColInfo& RGB, ColInfo& CMYK);
	static void CMYKtoRGB(const ColInfo& CMYK, ColInfo& RGB);
	static void RGBtoHSV(const ColInfo& RGB, ColInfo& HSV);
	static void HSVtoRGB(const ColInfo& HSV, ColInfo& RGB);
	static void CMYKtoHSV(const ColInfo& CMYK, ColInfo& HSV);
	static void HSVtoCMYK(const ColInfo& HSV, ColInfo& CMYK);
	static void GrayToRGB(const float G, ColInfo& RGB);
	static void GrayToCMYK(const float G, ColInfo& CMYK);
	static void GrayToHSV(const float G, ColInfo& HSV);
};

#endif /* Colour_hpp */
