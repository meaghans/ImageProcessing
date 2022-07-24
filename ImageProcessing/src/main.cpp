#include <iostream>
#include <fstream>
#include <vector>
#include "Header.h"
using namespace std;

// helper functions (testing pixel by pixel)
bool Test(Header& example, Header& result) {
	for (int i = 0; i < example.size; i++) {
		if (example.pixels[i].red != result.pixels[i].red) {
			cout << "Red pixel not correct!" << endl;
			cout << "Expected: " << +example.pixels[i].red << endl; // "+" note: 0-255 converts to random char -> + converts it back to 0-255
			cout << "Actual: " << +result.pixels[i].red << endl;
			cout << "Error index: " << i << endl;
			return false;
		}
		if (example.pixels[i].blue != result.pixels[i].blue) {
			cout << "Blue pixel not correct!" << endl;
			cout << "Expected: " << +example.pixels[i].blue << endl; // "+" note: 0-255 converts to random char -> + converts it back to 0-255
			cout << "Actual: " << +result.pixels[i].blue << endl;
			cout << "Error index: " << i << endl;
			return false;
		}
		if (example.pixels[i].green != result.pixels[i].green) {
			cout << "Green pixel not correct!" << endl;
			cout << "Expected: " << +example.pixels[i].green << endl; // "+" note: 0-255 converts to random char -> + converts it back to 0-255
			cout << "Actual: " << +result.pixels[i].green << endl;
			cout << "Error index: " << i << endl;
			return false;
		}
	}
	cout << "Success!" << endl;
	return true;
}

int main() {
	
	// Part 1
	Header layer1;
	Header pattern1;

	layer1.ReadDataFromFile("input/layer1.tga");
	pattern1.ReadDataFromFile("input/pattern1.tga");

	Header part1;
	part1.ReadDataFromFile("input/layer1.tga"); // read any file in
	// part 1 operation
	part1.Multiply(pattern1, layer1);
	part1.WriteDataFromFile("output/part1.tga");
	// load example file to prepare comparison
	Header ex_1;
	ex_1.ReadDataFromFile("examples/EXAMPLE_part1.tga");
	// compare result and example using helper function
	cout << "Task 1: ";
	Test(ex_1, part1);

	// Part 2
	Header layer2;
	Header car;
	layer2.ReadDataFromFile("input/layer2.tga");
	car.ReadDataFromFile("input/car.tga");

	Header part2;
	part2.ReadDataFromFile("input/layer2.tga"); // read any file in

	// operation
	part2.Subtract(car, layer2);
	part2.WriteDataFromFile("output/part2.tga");

	// load example file to prep comparison
	Header ex_2;
	ex_2.ReadDataFromFile("examples/EXAMPLE_part2.tga");

	// compare result and example using helper function
	cout << "Task 2: ";
	Test(ex_2, part2);

	// Part 3
	Header pattern2;
	pattern2.ReadDataFromFile("input/pattern2.tga");
	Header part3a;
	part3a.ReadDataFromFile("input/pattern2.tga");

	part3a.Multiply(layer1, pattern2); // multiplied temp result

	// load text.tga
	Header text;
	text.ReadDataFromFile("input/text.tga");  // top layer
	Header part3b;
	part3b.ReadDataFromFile("input/pattern2.tga");

	part3b.Screen(part3a, text);
	part3b.WriteDataFromFile("output/part3.tga");

	Header ex_3;
	ex_3.ReadDataFromFile("examples/EXAMPLE_part3.tga");

	cout << "Task 3: ";
	Test(ex_3, part3b);

	// Part 4
	Header circles;
	circles.ReadDataFromFile("input/circles.tga");
	Header part4;
	part4.ReadDataFromFile("input/circles.tga");

	part4.Multiply(layer2, circles);
	Header part4b;
	part4b.ReadDataFromFile("input/pattern2.tga");
	part4b.Subtract(part4, pattern2); // pattern2, parameter 2 is second layer

	part4b.WriteDataFromFile("output/part4.tga");
	Header ex_4;
	ex_4.ReadDataFromFile("examples/EXAMPLE_part4.tga");
	cout << "Task 4: ";
	Test(ex_4, part4b);

	// Part 5
	Header part5;
	part5.ReadDataFromFile("input/layer1.tga");
	part5.Overlay(pattern1, layer1); // a = pattern1, b = layer1 (top layer)
	part5.WriteDataFromFile("output/part5.tga");

	Header ex_5;
	ex_5.ReadDataFromFile("examples/EXAMPLE_part5.tga");
	cout << "Task 5: ";
	Test(ex_5, part5);

	// Part 6
	Header part6;
	part6.ReadDataFromFile("input/car.tga");

	// operation
	part6.Green();
	part6.WriteDataFromFile("output/part6.tga");

	// compare
	Header ex_6;
	ex_6.ReadDataFromFile("examples/EXAMPLE_part6.tga");
	cout << "Task 6: ";
	Test(ex_6, part6);

	// Part 7
	Header part7;
	part7.ReadDataFromFile("input/car.tga");
	part7.Scale(); // operation
	part7.WriteDataFromFile("output/part7.tga"); // write out results from operation
	Header ex_7; // example header
	ex_7.ReadDataFromFile("examples/EXAMPLE_part7.tga");
	cout << "Task 7: ";
	Test(ex_7, part7); // compare

	// Part 8
	// red channel
	Header part8_r;
	part8_r.ReadDataFromFile("input/car.tga");
	part8_r.SeparateChannels('r');
	// blue channel
	Header part8_b;
	part8_b.ReadDataFromFile("input/car.tga");
	part8_b.SeparateChannels('b');
	// green channel
	Header part8_g;
	part8_g.ReadDataFromFile("input/car.tga");
	part8_g.SeparateChannels('g');

	// write out each channel
	part8_r.WriteDataFromFile("output/part8_r.tga");
	part8_b.WriteDataFromFile("output/part8_b.tga");
	part8_g.WriteDataFromFile("output/part8_g.tga");
	// load example channels
	Header ex8_r;
	Header ex8_b;
	Header ex8_g;

	ex8_r.ReadDataFromFile("examples/EXAMPLE_part8_r.tga");
	ex8_b.ReadDataFromFile("examples/EXAMPLE_part8_b.tga");
	ex8_g.ReadDataFromFile("examples/EXAMPLE_part8_g.tga");

	cout << "Task 8 Red: ";
	Test(ex8_r, part8_r); // tests red channel
	cout << "Task 8 Green: ";
	Test(ex8_g, part8_g); // tests green channel
	cout << "Task 8 Blue: ";
	Test(ex8_b, part8_b); // tests blue channel

	// Part 9
	Header layer_red;
	layer_red.ReadDataFromFile("input/layer_red.tga");
	Header layer_green;
	layer_green.ReadDataFromFile("input/layer_green.tga");
	Header layer_blue;
	layer_blue.ReadDataFromFile("input/layer_blue.tga");

	Header part9;
	part9.ReadDataFromFile("input/layer_red.tga");
	part9.Combine(layer_red, layer_green, layer_blue);
	part9.WriteDataFromFile("output/part9.tga");

	Header ex_9;
	ex_9.ReadDataFromFile("examples/EXAMPLE_part9.tga");
	cout << "Part 9: ";
	Test(ex_9, part9);

	// Part 10
	// dont need a Header text2 for text2.tga since it's only one image
	Header part10;
	part10.ReadDataFromFile("input/text2.tga");
	part10.Rotate();
	part10.WriteDataFromFile("output/part10.tga");

	Header ex_10;
	ex_10.ReadDataFromFile("examples/EXAMPLE_part10.tga");
	cout << "Task 10 ";
	Test(ex_10, part10);
	return 0;
}
