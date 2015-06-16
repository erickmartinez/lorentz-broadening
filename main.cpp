/*
 * main.cpp
 *
 *  Created on: Jun 6, 2015
 *      Author: Erick Martinez
 */

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>

using namespace std;

#define BROAD 15.0
#define DX 0.01
#define XSTART 0
#define XENDS 4000

int main(int argc, char **argv) {
	string inFileNameStr;
	string outFileNameStr;
	std::string line;
	ifstream input_file;
	ofstream output_file;
	int argumentcheck = 0;
	int lines = 0;
	int idx = 0;
	double broadening = BROAD;
	double gamma;
	const double PI = 4 * atan(1.0);
	double x, y, dx;
	dx = DX;
	double steps;
	vector<vector<double> > dataOut(steps, vector<double>(2));
	double factor, sumi;
	double squarex;
	/* Command line initialization */
	if (argc < 5) {
		cout << "Usage is lorentz-broadening -in <input file> -out "
				"<output file> [-b <FWHM> -dx <spacing> ]" << endl;
	} else {
		for (int i = 1; i < argc; i++) {
			if (i + 1 != argc) { // Check that we haven't finished parsing already
				/* Check for input filename */
				if (std::string(argv[i]).compare("-in") == 0) {
					inFileNameStr = argv[i + 1];
					argumentcheck++;
					/* Check for the output filename */
				} else if (std::string(argv[i]).compare("-out") == 0) {
					outFileNameStr = argv[i + 1];
					argumentcheck++;
					/* Check for the FWHM */
				} else if (std::string(argv[i]).compare("-b") == 0) {
					broadening = atof(argv[i + 1]);
					/* Check the x spacing */
				} else if (std::string(argv[i]).compare("-dx") == 0) {
					dx = atof(argv[i + 1]);
				}
			}
		}
		if (argumentcheck < 2) {
			std::cout << "Not enough or invalid arguments, please "
					"try again." << std::endl;
			cout << "Provided argument: " << argumentcheck << endl;
			exit(0);
		}
	}

	steps = (int) rint((XENDS - XSTART) / dx);

	/* Open input file */
	input_file.open(inFileNameStr.c_str());
	if (!input_file.is_open()) { //  Check that file can be accessed
		cerr << "There was a problem opening the input file." << std::endl;
		exit(0);
	}

	std::cout << "Reading input file." << std::endl;
	/* First count the number of lines in the file */
	while (getline(input_file, line)) {
		++lines;
	}

	input_file.clear();
	input_file.seekg(0, input_file.beg);

	cout << "Number of peaks: " << lines << endl;
	cout << "Number of steps: " << steps << endl;
	cout << "dx = " << DX << endl;

	vector<vector<double> > data(lines, vector<double>(2));
	while (input_file >> x >> y) {
		data[idx][0] = x;
		data[idx][1] = y;
		++idx;
	}

	/* Close input file */
	input_file.close();

	gamma = 0.5 * broadening;

	cout << "gamma = " << gamma << endl;

	factor = 1.0 / (gamma * PI);

	/* Open output file */
	output_file.open(outFileNameStr.c_str());
	if (!output_file.is_open()) {
		std::cerr << "There was a problem opening the output file."
				<< std::endl;
		exit(0);
	}

	for (int i = 0; i < steps; ++i) {
		x = i * dx;
		y = 0.0;
		for (int j = 0; j < lines; ++j) {
			squarex = i * dx - data[j][0];
			squarex /= gamma;
			squarex *= squarex;
			sumi = 1 + squarex;
			y += (data[j][1] * factor / sumi);
		}
		output_file.precision(round(log10(fabs(dx))));
		output_file.setf(ios::fixed, ios::floatfield);
		output_file << x << "\t";
		output_file << scientific;
		output_file.precision(10);
		output_file << y << endl;
	}

	output_file.close();

}

