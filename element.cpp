/*
* Tyler Bainbridge
* UMass Lowell
* Tyler_Bainbridge@student.uml.edu
* Assignment 3
* (c) UMass Lowell 2015
* created 9/8/15* 
* last edited 10/31/15 at 11PM
*/
#include <iterator>
#include "element.h"

using namespace std;

/**
 * adds an id and value to a map of attributes
 * @param attribute to be put in map
 * @param value to be put in map
 */
void element::addAtrribute(string attribute, string value) //adding attributes to the element
{
	attributes.insert(pair<string, string>(attribute, value));
}

/**
 * prints the attribute map to terminal
 */
void element::printAttributes(void)
{
	map<string, string>::iterator it;

	for(it=attributes.begin(); it != attributes.end(); ++it) // iterates through the map
	{
		cout<<"Attribute: "<<it->first<<" Value: "<<it->second<< " "; 
		cout<<"\""<<it->first<<"\": "<<	"\""<<" Value: "<<it->second<<endl; 
	}
	cout<<endl;
}

/**
 * prints the attributes to the file
 * @param outFile the file you want to be accessing
 */
void element::printAttributesToFile(ofstream& outFile)
{
	map<string, string>::iterator it; //for iterating through the map of attributes and values

	if(attributes.size()>1) //if there are more than one attribute, make an array
	{
		outFile << "[" << endl;
	}
	for(it=attributes.begin(); it != attributes.end(); ++it) // iterates through the map
	{
		outFile << "{" << endl; //brackets surrounding ever attribute
		outFile<<"\""<<it->first<<"\": "<<	"\""<< it->second<<","<<endl; 
		if(!content.empty()){ // if there is content, print it
		outFile<<"\"content\": "<<	"\""<< content << "\"" <<endl; 
		}
		outFile << "}" << endl;
		if(next(it) != attributes.end()) //is this the last attribute
		{
			outFile << "," << endl; //if not, print the comma
		}else if(next(it)==attributes.end()){ //if it is
			outFile << endl; //print a new line
		}
	}
	if(attributes.size()>1) //closing the array
	{
		outFile << "]" << endl;
	}
	//outFile<<"]"<<endl;
}

/**
 * set content in between the tag
 * @param line to be parsed into the element class object
 */
void element::setContent(string line) //sets the content
{
	int startPos, endPos;					//start and end position of line substr
	string elementContent;					//after editing line

	startPos = line.find(">"); 
	endPos = line.find("<",startPos+1);		//getting what's between the tags aka Content
	elementContent = line.substr(startPos+1, (endPos - startPos) - 1); //getting content

	content = elementContent;											//content being set
}

/**
 * does it have content? return true if so
 * @return a bool
 */
bool element::hasContent(void)				//checking if there is 
{
	if(!content.empty())					//if content isnt empty aka there is content
	{
		return true;
	}

	return false;
}

/**
 * return content
 * @return 
 */
string element::getContent(void) //returns the content string
{
	return content; 
}

/**
 * 
 * @return the size 
 */
int element::getAttributeSize()
{
	return attributes.size();
}

/**
 * 
 * @return whether or not it has attributes 
 */
bool element::hasAttributes(void) //checks if it is empty (self explanatory) 
{
	if(attributes.empty())
	{
		return false;
	}
	if(!attributes.empty())
	{
		return true;
	}
}

/**
 * 
 * @param child to be added to the vector
 */
void element::addChild(element* child) //adds a new children to the vector
{
	children.push_back(child); //pushes a new element pointer onto the vector
}

/**
 * 
 * @return the vector
 */
vector<element*> element::getVector(void) //returns the child vector for printing
{
	return children;
}

/**
 * 
 * @return the size of the child vector 
 */
int element::getChildSize(void) //returns the size of the vector aka the amount of children
{
	return children.size();
}

/**
 * 
 * @return how many levels "Deep" 
 */
int element::getLevel() //how many tags deep is the element
{
	return level;
}

/**
 * constructor
 * @param finalTag
 * @param level
 * @param lineNumber
 */
element::element(string finalTag, int level, int lineNumber)
{
	this->elementName = finalTag; //setting the name
	this->lineNum = lineNumber; //setting the line number
	this->level = level; //setting the level
}

/**
 * constructor 
 * @param finalTag
 * @param lineNumber
 */
element::element(string finalTag, int lineNumber)
{
	this->elementName = finalTag; //setting the name
	this->lineNum = lineNumber; //setting the line number
}

/**
 * 
 * @return the name 
 */
string element::getName(){
	return elementName; //for printing info in main
}

/**
 * 
 * @return the linenumber
 */
int element::getlineNum(){
	return lineNum; //for printing info in main
}

