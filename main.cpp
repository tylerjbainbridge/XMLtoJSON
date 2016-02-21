/*
* Tyler Bainbridge
* UMass Lowell
* Tyler_Bainbridge@student.uml.edu
* Assignment 3
* (c) UMass Lowell 2015
* created 9/8/15
* last edited 10/31/15 at 11PM
*/

//NOTE: MAKE SURE YOUR COMPILER IS SET TO COMPILE C++ 11

#include "element.h"
#include "parsertools.h"

using namespace std;

int main(int argc, char** argv) {

	vector<element> parsedXMLdata;						//a vector of the XML file's elements

	parsedXMLdata = parseFile("test.xml");		//goes through the file and parses each line

	convertToJSON(parsedXMLdata);							//converting an XML to JSON

	cout << "Done." << endl;									//letting the user know that it is finished
	return 0;
}

