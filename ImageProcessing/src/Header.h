#pragma once
#include <iostream>
#include <vector>
using namespace std;

struct Pixel {
	unsigned char blue;
	unsigned char red;
	unsigned char green;
};

struct Header
{
	char idLength;
	char colorMapType;
	char dataTypeCode;
	short colorMapOrigin;
	short colorMapLength;
	char colorMapDepth;
	short xOrigin;
	short yOrigin;
	short width;
	short height;
	char bitsPerPixel;
	char imageDescriptor;

	int size;
	vector <Pixel> pixels; // vector to store pixels

	void ReadDataFromFile(string file);
	void WriteDataFromFile(string file);
	void Multiply(Header& img, Header& img_2);
	void Subtract(Header& img, Header& img_2);
	void Screen(Header& img, Header& img_2);
	void Overlay(Header& img, Header& img_2);
	void Green();
	void Scale();
	void SeparateChannels(char channel);
	void Combine(Header& red, Header& green, Header& blue);
	void Rotate();
	// check underflow and overflow
	float CheckFlowBounds(float x);
	
};
