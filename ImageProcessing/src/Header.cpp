#include <iostream>
#include <fstream>
#include <vector>
#include "Header.h"
using namespace std;

float Header::CheckFlowBounds(float x) {
	// check underflow
	if (x < 0) {
		x = 0;
	}
	else if (x > 255) {
		x = 255;
	}
	return x;
}
void Header::ReadDataFromFile(string file) {
	ifstream inFile(file, ios_base::binary);

	inFile.read(&idLength, sizeof(idLength));
	inFile.read(&colorMapType, sizeof(colorMapType));
	inFile.read(&dataTypeCode, sizeof(dataTypeCode));
	inFile.read((char*)&colorMapOrigin, sizeof(colorMapOrigin));
	inFile.read((char*)&colorMapLength, sizeof(colorMapLength));
	inFile.read((char*)&colorMapDepth, sizeof(colorMapDepth));
	inFile.read((char*)&xOrigin, sizeof(xOrigin));
	inFile.read((char*)&yOrigin, sizeof(yOrigin));

	inFile.read((char*)&width, sizeof(width));
	inFile.read((char*)&height, sizeof(height));

	inFile.read(&bitsPerPixel, sizeof(bitsPerPixel));
	inFile.read(&imageDescriptor, sizeof(imageDescriptor));

	// size of pixel 
	size = width * height;
	Pixel pixel;
	// read in BGR values, push back into vector of pixels (as a pixel object)
	for (int i = 0; i < size; i++) {
		inFile.read((char*)&pixel.blue, sizeof(pixel.blue));
		inFile.read((char*)&pixel.green, sizeof(pixel.green));
		inFile.read((char*)&pixel.red, sizeof(pixel.red));

		pixels.push_back(pixel);
	}
}

void Header::WriteDataFromFile(string file) {
	ofstream inFile(file, ios_base::binary);

	inFile.write(&idLength, sizeof(idLength));
	inFile.write(&colorMapType, sizeof(colorMapType));
	inFile.write(&dataTypeCode, sizeof(dataTypeCode));
	inFile.write((char*)&colorMapOrigin, sizeof(colorMapOrigin));
	inFile.write((char*)&colorMapLength, sizeof(colorMapLength));
	inFile.write((char*)&colorMapDepth, sizeof(colorMapDepth));
	inFile.write((char*)&xOrigin, sizeof(xOrigin));
	inFile.write((char*)&yOrigin, sizeof(yOrigin));

	inFile.write((char*)&width, sizeof(width));
	inFile.write((char*)&height, sizeof(height));

	inFile.write(&bitsPerPixel, sizeof(bitsPerPixel));
	inFile.write(&imageDescriptor, sizeof(imageDescriptor));

	// write out specific pixel's BGR values
	for (int i = 0; i < size; i++) {
		inFile.write((char*)&pixels[i].blue, sizeof(pixels[i].blue));
		inFile.write((char*)&pixels[i].green, sizeof(pixels[i].green));
		inFile.write((char*)&pixels[i].red, sizeof(pixels[i].red));

	}
}

void Header::Multiply(Header& img, Header& img_2) {
	pixels.clear(); // refers to pixels vector belonging to function calling it (can write this->)
	Pixel pixel;

	for (int i = 0; i < size; i++) {
		// layer 1
		float red = img_2.pixels[i].red / 255.0f;
		float green = img_2.pixels[i].green / 255.0f;
		float blue = img_2.pixels[i].blue / 255.0f;

		// pattern 1
		float red2 = img.pixels[i].red / 255.0f;
		float green2 = img.pixels[i].green / 255.0f;
		float blue2 = img.pixels[i].blue / 255.0f;

		// multiplication + rounding
		pixel.red = (red * red2 * 255.0f) + 0.5f;
		pixel.green = (green * green2 * 255.0f) + 0.5f;
		pixel.blue = (blue * blue2 * 255.0f) + 0.5f;

		pixels.push_back(pixel);
	}
}

// not dividing by 255.0f because its a subtraction function (not multiplication, so it'll be less than 255)
void Header::Subtract(Header& img, Header& img_2) {
	// C = B1 - B2 -> C = car (img) - layer2 (img_2)
	// layer2 (top layer), car (bottom layer), subtract top FROM bottom layer and check underflow

	pixels.clear(); // refers to pixels vector belonging to function calling it (can write this->)
	Pixel pixel;

	for (int i = 0; i < size; i++) {
		// car
		float red = img.pixels[i].red;
		float green = img.pixels[i].green;
		float blue = img.pixels[i].blue;

		// layer2
		float red2 = img_2.pixels[i].red;
		float green2 = img_2.pixels[i].green;
		float blue2 = img_2.pixels[i].blue;

		// subtraction operator + rounding
		pixel.red = CheckFlowBounds(red - red2) + 0.5f;
		pixel.green = CheckFlowBounds(green - green2) + 0.5f;
		pixel.blue = CheckFlowBounds(blue - blue2) + 0.5f;

		pixels.push_back(pixel);
	}
}

void Header::Screen(Header& img, Header& img_2) {
	// f(a, b) = 1 - (1 - a)(1 - b)
	// a = part3a (result using multiply function), b = text.tga (need to blend using screen function)

	pixels.clear();
	Pixel pixel;
	for (int i = 0; i < size; i++) {
		// multiply function can be greater than 255.0f, hence why we divide by 255.0f (normalize values)
		// part3a
		float red = img.pixels[i].red / 255.0f;
		float green = img.pixels[i].green / 255.0f;
		float blue = img.pixels[i].blue / 255.0f;

		// text.tga
		float red2 = img_2.pixels[i].red / 255.0f;
		float green2 = img_2.pixels[i].green / 255.0f;
		float blue2 = img_2.pixels[i].blue / 255.0f;

		// blending operation
		// pieces (red, blue, green pieces)
		float red_part1 = 1 - red;
		float red_part2 = 1 - red2;
		float red_part3 = red_part1 * red_part2;

		float green_part1 = 1 - green;
		float green_part2 = 1 - green2;
		float green_part3 = green_part1 * green_part2;

		float blue_part1 = 1 - blue;
		float blue_part2 = 1 - blue2;
		float blue_part3 = blue_part1 * blue_part2;;

		pixel.red = CheckFlowBounds((1 - red_part3) * 255.0f) + 0.5f;
		pixel.green = CheckFlowBounds((1 - green_part3) * 255.0f) + 0.5f;
		pixel.blue = CheckFlowBounds((1 - blue_part3) * 255.0f) + 0.5f;

		pixels.push_back(pixel);
	}
}

void Header::Overlay(Header& img, Header& img_2) {
	// pattern1 (img), layer1 (img_2): top layer is img_2
	// a = pattern1, b = layer1
	pixels.clear();
	Pixel pixel;

	for (int i = 0; i < size; i++) {
		// img, pattern1
		float red = img.pixels[i].red / 255.0f;
		float green = img.pixels[i].green / 255.0f;
		float blue = img.pixels[i].blue / 255.0f;

		// img2, top layer, layer1 (second parameter passed in)
		float red2 = img_2.pixels[i].red / 255.0f;
		float green2 = img_2.pixels[i].green / 255.0f;
		float blue2 = img_2.pixels[i].blue / 255.0f;

		// overlay operation
		// if a < 0.5 f(a, b) so a = img or red
		if (red <= 0.5f) {
			pixel.red = CheckFlowBounds(2.0f * red * red2 * 255.0f) + 0.5f;
		}
		else{
			float red_operation = 1.0f - 2.0f * (1.0f - red) * (1.0f - red2);
			pixel.red = (red_operation * 255.0f) + 0.5f;
		}
		if (green <= 0.5f) {
			pixel.green = CheckFlowBounds(2.0f * green * green2 * 255.0f) + 0.5f;
		}
		else {
			float green_operation = 1.0f - 2.0f * (1.0f - green) * (1.0f - green2);
			pixel.green = (green_operation * 255.0f) + 0.5f;
		}
		if (blue <= 0.5f) {
			pixel.blue = CheckFlowBounds(2.0f * blue * blue2 * 255.0f) + 0.5f;
		}
		else {
			float blue_operation = 1.0f - 2.0f * (1.0f - blue) * (1.0f - blue2);
			pixel.blue = (blue_operation * 255.0f) + 0.5f;
		}
		pixels.push_back(pixel);
	}
}
void Header::Green() {
	for (int i = 0; i < size; i++) {
		// dont need to push back since we're doing direct access first
		pixels[i].green = CheckFlowBounds(pixels[i].green + 200);
	}
}

void Header::Scale() {
	for (int i = 0; i < size; i++) {
		// dont need to push back since we're doing direct access again
		pixels[i].red = CheckFlowBounds(pixels[i].red * 4.0f);
		pixels[i].blue = CheckFlowBounds(pixels[i].blue * 0.0f);
	}
}


// don't need Header& img since we're overriding loaded image (car.tga) for each channel
void Header::SeparateChannels(char channel) {
	// overriding each part8 image 
		for (int i = 0; i < size; i++) {
			if (channel == 'r') {
				// changing each channel to RED
				pixels[i].red = pixels[i].red;
				pixels[i].green = pixels[i].red;
				pixels[i].blue = pixels[i].red;
			}
			else if (channel == 'g') {
				//change each channel to GREEN
				pixels[i].red = pixels[i].green;
				pixels[i].green = pixels[i].green;
				pixels[i].blue = pixels[i].green;
			}
			else if (channel == 'b') {
				//change each channel to BLUE
				pixels[i].red = pixels[i].blue;
				pixels[i].green = pixels[i].blue;
				pixels[i].blue = pixels[i].blue;
			}
		}
}
// Part 9
void Header::Combine(Header& red, Header& green, Header& blue) {
	// overriding part9 image, don't need to return a new image
	for (int i = 0; i < size; i++) {
		pixels[i].red = red.pixels[i].red;
		pixels[i].green = green.pixels[i].green;
		pixels[i].blue = blue.pixels[i].blue;
	}
}

// Part 10
// overriding image
void Header::Rotate() {
	Pixel pixel;
	// swap pixel indexes in a vector (ex: first and last, 2nd and 2nd to last)
	for (int i = 0; i < size / 2; i++) {
		// swap 1/2 of the image only, not the entire image (doing the entire image would reverse what we've done)
		int j = size - 1 - i;
		pixel = pixels[i];
		pixels[i] = pixels[j]; // first pixel = last pixel
		pixels[j] = pixel; // last pixel = temp pixel
		 
	}
}
