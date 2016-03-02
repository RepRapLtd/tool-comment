/*
 * Little program to comment out calls to Tn except for a band of Z values in a G-Code file.
 *
 * This assumes that the file has a certain structure:
 *
 *   1. Tool change lines look like: "Tn..." with the T as the first character,
 *   2. Z increment lines look like: "G1 Zz..." with the G as the first character, and
 *   3. The Z values increase monotonically.
 *
 *
 *
 * Adrian Bowyer
 * http://reprapltd.com
 * March 2016
 *
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

using namespace std;

std::ifstream inputFile;
std::ofstream outputFile;

#define MAX_LENGTH 1000

int main(int argc, char** argv)
{
        int ifArg = -1;
	int ofArg = -1;
	int zlArg = -1;
	int zhArg = -1;
	int toolArg = -1;

	if(argc != 11)
	{
		cerr << "Usage: tool-comment -i input-file -o output-file -zl z-low-height -zh z-high-height -t tool-number-to-comment-out\n";
		return EXIT_FAILURE;
	}

	for(int i = 0; i < argc; i++)
	{
		if(strcmp("-i", argv[i]) == 0)
			ifArg = i + 1;
		if(strcmp("-o", argv[i]) == 0)
			ofArg = i + 1;
		if(strcmp("-zl", argv[i]) == 0)
			zlArg = i + 1;
		if(strcmp("-zh", argv[i]) == 0)
			zhArg = i + 1;
		if(strcmp("-t", argv[i]) == 0)
			toolArg = i + 1;
	}

	if(ifArg < 0)
	{
		cerr << "tool-comment: no input file defined (no -i input-file.g)\n";
		return EXIT_FAILURE;
	}

	if(ofArg < 0)
	{
		cerr << "tool-comment: no output file defined (no -o output-file.g)\n";
		return EXIT_FAILURE;
	}

	if(zlArg < 0)
	{
		cerr << "tool-comment: no z low height defined (no -zl z-height)\n";
		return EXIT_FAILURE;
	}

	if(zhArg < 0)
	{
		cerr << "tool-comment: no z high height defined (no -zh z-height)\n";
		return EXIT_FAILURE;
	}

	if(toolArg < 0)
	{
		cerr << "tool-comment: no tool number defined (no -t tool-number-to-comment-out)\n";
		return EXIT_FAILURE;
	}

	float z = -1.0;
	float zl = (float)strtod(argv[zlArg], 0);
	float zh = (float)strtod(argv[zhArg], 0);
	int tool = (int)strtol(argv[toolArg], 0, 0);
	int t = -1;
	char inputLine[MAX_LENGTH];


	inputFile.open(argv[ifArg], std::ifstream::in);
	outputFile.open(argv[ofArg], std::ofstream::out);
	bool first = true;
	float zLast = -1.0;

	while(!inputFile.eof())
	{
		inputFile.getline(inputLine, MAX_LENGTH);
		if(strstr(inputLine, "G1 Z") == inputLine)
			z = (float)strtod(&inputLine[4], 0);
		if(inputLine[0] == 'T')
		{
			t = (int)strtol(&inputLine[1], 0, 0);
			if(t == tool)
			{
				if(z < zl || z > zh)
				{	
					outputFile << ';';
				} else
				{
					if(first)
					{
						cout << "First uncommented height: " << z << endl;
						first = false;
					}
					zLast = z;
				}
			}
		}
		outputFile << inputLine << endl;	
	}	

	cout << "Last uncommented height: " << zLast << endl;
	inputFile.close();
	outputFile.close();

    	return EXIT_SUCCESS;
}
