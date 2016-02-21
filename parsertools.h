/*
* Tyler Bainbridge
* UMass Lowell
* Tyler_Bainbridge@student.uml.edu
* Assignment 5
* (c) UMass Lowell 2015
* created 9/30/15
* last edited 10/31/15 at 11PM
*/

#ifndef PARSERTOOLS_H
#define PARSERTOOLS_H

#include "element.h"                                      //the header file

string getOpen(string line);                              //get the name of the opening tag
string getClosed(string line);                            //get the name of the closing tag

vector<element> parseFile(string fileName);               //the workhorse of the program, organizes all the utilities to complete the task

void printOpenElement(element tag);                       //prints the info on an opening tag
void printClosedElement(element tag);                     //prints the info on a closing tag
void printCompleteElement(string line, int lineNumber);   //print the complete tag
void printStack(stack<element> songstack);                //prints the contents of the stack without modifying the original
bool searchChildVec(vector<element*> temp, element tag);  //for seeing if the vector is already located there
void getAtrribute(string line, element* tag);             //parses a line into an attribute

void convertToJSON(vector<element> elementTree);          //converts the XML file into a JSON file

#endif