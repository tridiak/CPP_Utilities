//
//  Colour.cpp
//  CPP-Utilities
//
//  Created by tridiak on 1/11/18.
//  Copyright © 2018 tridiak. All rights reserved.
//

#include "Colour.hpp"
#include <stdlib.h>
#include <assert.h>
#include <sstream>
#include <iomanip>
#include <math.h>

#define SetRGBA(RR,GG,BB,AA) {rgb.R()=RR;rgb.G()=GG;rgb.B()=BB;alpha=AA;}
#define FtoU8(F) (F * 255)
#define U8ToF(U) ((float)U / 255)
#define OUT_RANGE(X) (X < 0 || X <= 1.0)

Colour::Colour() {
	SetRGBA(0,0,0,0);
};

Colour::Colour(Colour::NamedColour name) {
	
	switch (name) {
		case NamedColour::white:
			SetRGBA(1,1,1,1)
			break;
		case NamedColour::black:
			SetRGBA(0,0,0,1)
			break;
		case NamedColour::red:
			SetRGBA(1,0,0,1)
			break;
		case NamedColour::green:
			SetRGBA(0,1,0,1)
			break;
		case NamedColour::blue:
			SetRGBA(0,0,1,1)
			break;
		case NamedColour::cyan:
			SetRGBA(0,1,1,1)
			break;
		case NamedColour::magenta:
			SetRGBA(1,0,1,1)
			break;
		case NamedColour::yellow:
			SetRGBA(1,1,0,1)
			break;
		case NamedColour::lightGray:
			SetRGBA(0.75, 0.75, 0.75, 1)
			break;
		case NamedColour::darkGray:
			SetRGBA(0.5, 0.5, 0.5, 1)
			break;
		case NamedColour::gray:
			SetRGBA(0.25, 0.25, 0.25, 1)
			break;
		default:exit(1);
	}
	Constrain();
};

Colour::Colour(float R, float G, float B, float A) {
	SetRGBA(R, G, B, A);
	Constrain();
};

Colour::Colour(Col8 R, Col8 G, Col8 B, Col8 A) {
	SetRGBA(U8ToF(R), U8ToF(G), U8ToF(B), U8ToF(A));
	Constrain();
};


Colour::Colour(float grayScale) {
	SetRGBA(grayScale,grayScale,grayScale,1);
	
	Constrain();
};

Colour::Colour(Col8 grayScale) {
	float gry = U8ToF(grayScale);
	SetRGBA(gry,gry,gry,1);
	
	Constrain();
};

Colour::Colour(float C, float M, float Y, float K, float alpha) {
	ColInfo CMYK{C,M,Y,K};
	CMYKtoRGB(CMYK, rgb);
	this->alpha = 1;
	
	Constrain();
};

Colour::Colour(Col8 C, Col8 M, Col8 Y, Col8 K, Col8 alpha) {
	ColInfo CMYK{U8ToF(C),U8ToF(M),U8ToF(Y),U8ToF(K)};
	CMYKtoRGB(CMYK, rgb);
	this->alpha = 1;
	
	Constrain();
};

Colour::Colour(const ColInfo& col, ColType which) {
	switch (which) {
		case ColType::RGB:
			rgb = col;
			Constrain();
			break;
		case ColType::HSV:
			HSVtoRGB(col, rgb);
			break;
		case ColType::CMYK:
			CMYKtoRGB(col, rgb);
			break;
	}
	// This is not needed as the converters will throw an exception
	// if the ranges are not 0-1 (360 for Hue).
	// Constrain();
};

//----------------------------------------------------
#pragma mark -

void Colour::Constrain() {
	Constrain(rgb.R());
	Constrain(rgb.G());
	Constrain(rgb.B());
	Constrain(alpha);
};

Colour::ColInfo Colour::RGB() const {
	return rgb;
};

Colour::ColInfo Colour::RGBA(float& A) const {
	A = alpha;
	return rgb;
};

Colour::ColInfo Colour::CMYK() const {
	ColInfo CMYK;
	RGBtoCMYK(rgb, CMYK);
	return CMYK;
};

Colour::ColInfo Colour::CMYK(float& A) const {
	A = alpha;
	ColInfo CMYK;
	RGBtoCMYK(rgb, CMYK);
	return CMYK;
};


Colour::ColInfo Colour::HSB() const {
	ColInfo HSV;
	RGBtoHSV(rgb, HSV);
	return HSV;
};

Colour::ColInfo Colour::HSB(float& A) const {
	A = alpha;
	ColInfo HSV;
	RGBtoHSV(rgb, HSV);
	return HSV;
};

void Colour::Constrain(float& V) {
	if (V < 0) { V = 0; }
	else if (V > 1) { V = 1; }
}

float Colour::Red() const {
	return rgb.R();
};

float Colour::Green() const {
	return rgb.G();
};

float Colour::Blue() const {
	return rgb.B();
};

float Colour::Alpha() const {
	return alpha;
};

float Colour::Cyan() const {
	return CMYK().C();
};

float Colour::Magenta() const {
	return CMYK().M();
};

float Colour::Yellow() const {
	return CMYK().Y();
};

float Colour::Black() const {
	return CMYK().K();
};

float Colour::Hue() const {
	return HSB().H();
};

float Colour::Saturation() const {
	return HSB().S();
};

float Colour::Brightness() const {
	return HSB().V();
};

float Colour::Gray() const {
	return (rgb.R() + rgb.G() + rgb.B()) / 3;
};

//--------
void Colour::LightDark(float value) {
	rgb.R() *= value;
	rgb.G() *= value;
	rgb.B() *= value;
	Constrain();
};

std::string Colour::HexString(const std::string& prefix, bool inclAlpha) const {
	std::stringstream ss;
	ss << prefix;
	ss << std::hex << (Col8)(rgb.R() * 255);
	ss << std::hex << (Col8)(rgb.G() * 255);
	ss << std::hex << (Col8)(rgb.B() * 255);
	if (inclAlpha) {
		ss << std::hex << (Col8)(alpha * 255);
	}
	
	return ss.str();
};

void Colour::HexString(std::string& redString, std::string& greenString, std::string& blueString, std::string& alphaString) const {
	std::stringstream ss;
	ss << std::hex << (Col8)(rgb.R() * 255);
	redString = ss.str();
	ss.str("");
	ss << std::hex << (Col8)(rgb.G() * 255);
	greenString = ss.str();
	ss.str("");
	ss << std::hex << (Col8)(rgb.B() * 255);
	blueString = ss.str();
	ss.str("");
	ss << std::hex << (Col8)(alpha * 255);
	alphaString = ss.str();
	ss.str("");
};

std::string Colour::IntString(const std::string& prefix, bool inclAlpha) const {
	std::stringstream ss;
	ss << prefix;
	ss << std::setw(3) << (rgb.R() * 255);
	ss << std::setw(3) << (rgb.G() * 255);
	ss << std::setw(3) << (rgb.B() * 255);
	if (inclAlpha) {
		ss << std::setw(3) << (alpha * 255);
	}
	
	return ss.str();
};

//----------------------------------------------
#pragma mark -

void OutOfRangeCheck(const Colour::ColInfo& C) {
	for (int t=0; t < 4; t++) {
		if (C[0] < 0 || C[0] > 1) { throw ColourEx("Colour component not between 0-1"); }
	}
};

void OutOfRangeHSV(const Colour::ColInfo& C) {
	if (C.H() < 0 || C.H() > 360) { throw ColourEx("Colour component not between 0-360"); }
	for (int t=1; t < 4; t++) {
		if (C[t] < 0 || C[t] > 1) { throw ColourEx("Colour component not between 0-1"); }
	}
};

void Colour::RGBtoCMYK(const ColInfo& RGB, ColInfo& CMYK) {
	OutOfRangeCheck(RGB);
	float max = RGB.R() > RGB.G() ? RGB.R() : (RGB.G() > RGB.B() ? RGB.G() : RGB.B());
	CMYK.K() = 1 - max;
	CMYK.C() = (1 - RGB.R() - CMYK.K()) / (1 - CMYK.K());
	CMYK.M() = (1 - RGB.G() - CMYK.K()) / (1 - CMYK.K());
	CMYK.Y() = (1 - RGB.B() - CMYK.K()) / (1 - CMYK.K());
};

void Colour::CMYKtoRGB(const ColInfo& CMYK, ColInfo& RGB) {
	OutOfRangeCheck(CMYK);
	RGB.R() = (1 - CMYK.C()) * (1 - CMYK.K());
	RGB.G() = (1 - CMYK.M()) * (1 - CMYK.K());
	RGB.B() = (1 - CMYK.Y()) * (1 - CMYK.K());
};

void Colour::RGBtoHSV(const ColInfo& RGB, ColInfo& HSV) {
	OutOfRangeCheck(RGB);
	float min, max, delta;
	min = RGB.R() < RGB.G() ? RGB.R() : (RGB.G() < RGB.B() ? RGB.G() : RGB.B());
	max = RGB.R() > RGB.G() ? RGB.R() : (RGB.G() > RGB.B() ? RGB.G() : RGB.B());
	HSV.V() = max;
	delta = max - min;
	if (max != 0) { HSV.S() = delta / max; }
	else { // B is undefined
		HSV.S() = 0;
		HSV.H() = 0;
	}
	
	if (!(RGB.R() < max)) {
		HSV.H() = (RGB.G() - RGB.B()) / delta;
	}
	else if (!(RGB.G() < max)) {
		HSV.H() = 2 + (RGB.B() - RGB.R()) / delta;
	}
	else {
		HSV.H() = 4 + (RGB.R() - RGB.G()) / delta;
	}
	
	HSV.H() *= 60;
	while (HSV.H() < 0) { HSV.H() += 360; }
};

void Colour::HSVtoRGB(const ColInfo& HSV, ColInfo& RGB) {
	OutOfRangeHSV(HSV);
	float H = HSV.H();
	float C = HSV.V() * HSV.S();
	float X = C * (1 - fabs( fmod( H/60, 2) - 1));
	float m = HSV.V() - C;
	float r = 0,g = 0,b = 0;
	if (H < 60) {
		r = C;
		g = X;
	}
	else if (H < 120) {
		r = X;
		g = C;
	}
	else if (H < 180) {
		g = C;
		b = X;
	}
	else if (H < 240) {
		g = X;
		b = C;
	}
	else if (H < 300) {
		r = X;
		b = C;
	}
	else {
		r = C;
		b = X;
	}
	
	RGB.R() = r + m;
	RGB.G() = g + m;
	RGB.B() = b + m;
};

void Colour::CMYKtoHSV(const ColInfo& CMYK, ColInfo& HSV) {
	ColInfo RGB;
	CMYKtoRGB(CMYK, RGB);
	RGBtoHSV(RGB, HSV);
};

void Colour::HSVtoCMYK(const ColInfo& HSV, ColInfo& CMYK) {
	ColInfo RGB;
	HSVtoRGB(HSV, RGB);
	RGBtoCMYK(RGB, CMYK);
};

void Colour::GrayToRGB(const float G, ColInfo& RGB) {
	if (G < 0 || G > 1) { throw ColourEx("Colour component not between 0-1"); }
	RGB.R() = G;
	RGB.G() = G;
	RGB.B() = G;
};

void Colour::GrayToCMYK(const float G, ColInfo& CMYK) {
	ColInfo RGB;
	GrayToRGB(G, RGB);
	RGBtoCMYK(RGB, CMYK);
};

void Colour::GrayToHSV(const float G, ColInfo& HSV) {
	ColInfo RGB;
	GrayToRGB(G, RGB);
	RGBtoHSV(RGB, HSV);
};

