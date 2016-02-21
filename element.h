/*
* Tyler Bainbridge
* UMass Lowell
* Tyler_Bainbridge@student.uml.edu
* Assignment 3
* (c) UMass Lowell 2015
* created 9/8/15
* last edited 10/31/15 at 11PM
*/

#ifndef ELEMENT_H
#define ELEMENT_H

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
#include <iomanip>
#include <stack>

using namespace std;

class element{
public:
	element();
	element(string finalTag, int lineNumber);           //creates a new element
	element(string finalTag, int level, int lineNumber);//for level
	int getLevel();                                     //returns the level
	string getName();                                   //returns element name
	int getlineNum();                                   //returns the line number
	void addChild(element* child);                      //adding children to the vector within element
	int getChildSize(void);                             //the size of the vector aka how many children
	vector<element*> getVector(void);                   //returns the vector within element with all the children	
	void setContent(string line);                       //for complete elements where content is available
	bool hasContent(void);                              //checking if there is content
	string getContent(void);                            //returns the content string
	void addAtrribute(string  attribute, string value); //adding attributes
	void printAttributes(void);                         //printing attributes
	bool hasAttributes(void);                           //does it have attributes
	void printAttributesToFile(ofstream& outFile);      //print them to the file
	int getAttributeSize();                             //return the size of the map

private:
	
	map<string, string> attributes;		//a map for holding the attributes of an element (id, value)
	string content;                   //the content in between tags
	string elementName;               //name of the tag
	int level;                        //how many tags deep is it
	int lineNum;                      //line number its found on
	vector<element*> children;        //points to the children
};

#endif