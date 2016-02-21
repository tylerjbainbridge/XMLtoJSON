/*
* Tyler Bainbridge
* UMass Lowell
* Tyler_Bainbridge@student.uml.edu
* Assignment 5
* (c) UMass Lowell 2015
* created 9/30/15
* last edited 10/31/15 at 11PM
*/

#include "parsertools.h";

using namespace std;


//ASSIGNMENT 5: CONVERT TO JSON
/**
 * This functions prints a vector of elements into JSON
 * @param elementTree a tree of vector elements
 */
void convertToJSON(vector<element> elementTree)
{
	ofstream outFile("test.json");

	vector<element>::reverse_iterator it;	// iterating the elementTree which is put in backwards because of the stack
	vector<element*>::iterator ij;			// for iterating through the child vector
	vector<element*>::iterator jt;			// for iterating through the child vector
	map<int, element> elementMap;			// for printing in an organized way
	map<int, element>::iterator itr;		// for iterating elementMap
	vector<element*> childVec;				// temporary holder for bring the children vectors
	
	cout<<"Converting test.xml to JSON"<<endl;

	// adding to a map based off line number and element to get rid of possible duplicates
	for ( it = elementTree.rbegin(); it<elementTree.rend(); ++it ) {
		elementMap.insert(pair<int, element>( it->getlineNum(),(*it) ) );

		if ( it->getChildSize() != 0 ) {    // if the element has children
			childVec = it->getVector();

			//navigating the childvec which points to other elements
			for ( jt=childVec.begin() ; jt<childVec.end() ; ++jt ) { 
				elementMap.insert(pair<int, element>((*jt)->getlineNum(), *(*jt)));
			}

		}
	}


	outFile << "{" << endl;//opening bracket for the json
	for (itr = elementMap.begin(); itr != elementMap.end(); ++itr) //moving through the map which is sorted by line number 
	{
		//different print for when an element has content
		if ( itr->second.hasContent() )											
		{
			// if it has attributes
			if ( itr->second.hasAttributes() )										
			{
				//check if next(itr) is the end of the vector
				if ( next(itr) != elementMap.end() )								
				{
					//if there are multiple elements in progression with different attributes put them together in an array
					if( next(itr)->second.getName() == itr->second.getName() )	
					{
						outFile <<"\""<< itr->second.getName()<<"\": " << endl; // print the name in parenthesis followed by a colon and a space
						outFile<<"["<<endl;																	//open the bracket for an array 
						itr->second.printAttributesToFile(outFile);					//print the first element's attributes
						outFile << "," << endl;															//comma because we know there will be another
						next(itr)->second.printAttributesToFile(outFile);		//print the next one's attributes as well
						itr++;																							//this is important, since we are printing the next elements attributes up there ^ then we skip past printing its name since it's in an array
						outFile<<"]"<<endl;																	//close the array

						if( next(itr)  !=  elementMap.end() )								//is there another element?
						{
							outFile << "," << endl;														//if there is, print a comma
						}else if ( next(itr) == elementMap.end() ){					//if there isn't another
							outFile << endl;																	//just print a newline
						}

					}else
					{
						outFile <<"\""<< itr->second.getName()<<"\": " << endl; // print the name in parenthesis followed by a colon and a space
						itr->second.printAttributesToFile(outFile);		//print the first element's attributes
						if(next(itr) != elementMap.end())							//is there another element?
						{
							outFile << "," << endl;											//if there is, print a comma
						}else if(next(itr)==elementMap.end()){				//if there isn't another	
							outFile << endl;														//just print a newline
						}
					}
				}
			//this case is for a regular ol' complete element with no attributes
			}else 
			{
				outFile <<"\""<< itr->second.getName()<<"\": " << "\""<< itr->second.getContent()<<"\""; //print it's name and it's content
				if(next(itr) != elementMap.end())						//is there another element?
				{
					outFile << "," << endl;								//if there is, print a comma
				}else if(next(itr)==elementMap.end()){					//if there isnt another
					outFile << endl;									//just print a newline
				}
			}
			
		}else //if there is an opening tag a/k/a for handling children 
		{
			//if the element has children, and is not the root
			if ( itr->second.getChildSize() > 0 && itr->second.getName() != elementMap.begin()->second.getName() )
			{
				outFile <<"\""<< itr->second.getName()<<"\": "<<endl;				//print the name and a colon
				outFile << "{" << endl;																			//open the bracket for it's children
				childVec = itr->second.getVector();													//temporary holder for handling children
				for( ij = childVec.begin();  ij != childVec.end() ; ++ij )	//print all of that element's children
				{
					// if the child has attributes
					if((*ij)->hasAttributes()) 
					{
						outFile <<"\""<< (*ij)->getName()<<"\": ";				//print the child's name and a colon
						(*ij)->printAttributesToFile(outFile);						//print the child's attributes
					}
					//if the child has no attributes
					else 
					{
						outFile <<"\""<< (*ij)->getName()<<"\": "<< "\""<< (*ij)->getContent()<<"\""; //print normally: "elementName": "elementContent"
					}
					if ( next(ij) != childVec.end() )			//are there more children?
					{
						outFile << "," << endl;				//if so, print a comma
					}else if ( next(ij)==childVec.end() ){		//if this is the last child
						outFile << endl;					//newline
					}
					itr++;
				}
				outFile << "}";

				if ( next(itr) != elementMap.end() )			//is there another element?
				{
					outFile << "," << endl;					//if there is, print a comma
				}else if ( next(itr)==elementMap.end() ){		//if there isn't another
					outFile << endl;						//just print a newline
				}
			}
		}
	}
	outFile << endl << "}"; //closing bracket for the JSON file
}

/**
 * This function is the powerhouse, it parses the entire file into a vector of elements
 * @param fileName the XML file to be parsed
 * @return  a vector of elements
 */
vector<element> parseFile(string fileName)
{
	ifstream inFile(fileName);							//setting the filename
	string line, finalTag, initialTag;			//strings for different stages of the parsing stages
	element* tag;														//element pointer for creating new elements
	element* closeTag;											//for closing elements
	stack<element> songstack;								//stack for opening tags
	vector<element> elementTree;						//the "tree" that holds all the elements and their children
	int startPos, endPos,lineNumber=1;			//for STL library functions
	string tagname;													//for complete elements

	cout << "Parsing "<<"\""<<fileName<<"\" "<<endl; //for the user to know which file is being parsed

	while(getline(inFile, line))
	{
		if(line.at(0)=='<' && line.at(1)=='?')
		{
			//cout << lineNumber << "    " << getOpen(line) << " is an attribute, not an open tag." << endl; 
		}

		if(line.find("<!--")!=string::npos) //looking for comments
		{
			//cout<< lineNumber <<"    Starting comment: " << line << endl; //shows the first part of the comment
			lineNumber++; 
			getline(inFile, line); //gets next line
			while(line.find("-->")==string::npos) //searching for the end of the comment
			{
				lineNumber++;
				//cout << lineNumber << "    Comment: " << line << endl;
				getline(inFile, line); //gets next line
			}
			//cout<< lineNumber << "    Ending comment: " << line << endl<<endl;
			getline(inFile, line); //gets next line
			lineNumber++; //counts the line again
		}

		/*
		COMPLETE ELEMENTS
		*/

		if(line.at((line.find("<"))+1)!='/') //if it is not a closing tag, then proceed to see if its closed inline
		{
			int backSlash = line.find('/'); //checks if there is a slash to then see if it is a closing tag
			if(backSlash!= string::npos&&line.at(backSlash-1)=='<') //checking for complete elements
			{
				tag = new element(getOpen(line),songstack.size(),lineNumber);
				if(tag->getLevel()>(songstack.top().getLevel())) //if the level is less then the one currently at the top of the stack, add to top of veciitkrieri
				{
					tag->setContent(line); //setting the content between the tags
					getAtrribute(line, tag);
					songstack.top().addChild(tag); 
				}
				//printCompleteElement(line, lineNumber); //prints the full line, and the content between tags
			}
		}

		/*
		OPEN ELEMENTS
		*/

		if(line.at((line.find("<"))+1)!='/'&&!ispunct(line.at((line.find("<"))+1))&&line.at((line.find(">"))-1)!='/') //if the character proceeding the opening bracket is not a backslash or punct (to get rid of closing tags and other tags) and there is no slash before the closing tags(self closing)
		{
			tag = new element(getOpen(line),songstack.size(),lineNumber); //creates a new Element with the line number and 

			int backSlash = line.find('/'); //checks if there is a slash to then see if it is a closing tag

			if(backSlash!= string::npos&&line.at(backSlash-1)!='<') //safeguard for tags that have forward slashes but are not for closing tags
			{
				getAtrribute(line, tag);
				songstack.push(*tag); //pushes the tag onto the stack
			}

			if(backSlash == string::npos)//if there is no slash aka it is an open tag
			{
				getAtrribute(line, tag);
				songstack.push(*tag); 
			}


			//song.push_back(*tag); //adds onto a vector
		}else if(line.at((line.find("<"))+1)!='/'&&ispunct(line.at((line.find("<"))+1))) //xml
		{

		}else if(line.at((line.find("<"))+1)!='/'&&!ispunct(line.at((line.find("<"))+1))&&line.at((line.find(">"))-1)=='/') //self closing tags
		{
			//	cout << lineNumber << "   " << getOpen(line) << " is a self closing tag, not adding to stack. " << endl;
		}

		if(line.at((line.find("<"))+1)=='/') //if the character proceeding the opening bracket is a backslash (to find closing tags)
		{


			closeTag = new element(getClosed(line), songstack.size(), lineNumber); //creates a new Element with the line number and tagName
			if(songstack.top().getName() ==  closeTag->getName()){
				//checks if the top of the stack is equal to the closing tag found first
				//printClosedElement(*tag); //prints info



				if(!songstack.empty()) // if songstack isn't empty
				{
					if(!searchChildVec(songstack.top().getVector(), *tag)) //if tag is not in the songstack's child vector
					{
						songstack.top().addChild(tag); //add the child
					}
				}
				elementTree.push_back(songstack.top()); //pushing the elements on the vector for printing and organizing
				songstack.pop(); //pops the stack



				//printStack(songstack); //displays the stack's contents
			}

		}




		lineNumber++; //counting lines
	}

	return elementTree;

	cout<<"The following has successfully been converted to JSON: "<<"\""<<fileName<<"\""<<endl<<endl;



}

/**
 * gets the attributes from the element
 * @param line the line from the file
 * @param tag the tag you want to add it to
 */
void getAtrribute(string line, element* tag)
{
	int startPos;                                               //where to start the substr
	int endPos;   
	int idstartPos;                                             //where to start the substr
	int idendPos; 
	int valuestartPos;                                          //where to start the substr
	int valueendPos=0;   
	string insideBrackets, attribute, value;										//strings which we will use to parse data
	int equalSign;
	startPos = line.find("<");
	endPos = line.find(">", startPos + 1);											//goes through "line" and finds what's in the brackets

	insideBrackets = line.substr(startPos + 1, (endPos - startPos) - 1); //creates a new string that is just what's in the brackets

	while(insideBrackets.find('=',valueendPos)!=string::npos)//while there is an equal sign ahead of the previous attribute valueendpos starts at zero
	{ 
		idstartPos = insideBrackets.find(' ',valueendPos); // looks for the space in between ex.) idstartPos-id-idendPos
		idendPos = insideBrackets.find("=",idstartPos); //gets the equal sign 
		attribute = insideBrackets.substr(idstartPos + 1, (idendPos - idstartPos) - 1);

		valuestartPos = idendPos+1;  //valuestartPos starts right after the equal sign
		if(insideBrackets.find(' ',idendPos)!=string::npos) //if there is a space ahead, end at the space
		{
			valueendPos = insideBrackets.find(' ',idendPos);
			value = insideBrackets.substr(valuestartPos + 1, ((valueendPos) - valuestartPos) - 1);
		}else //if there is a bracket at the end end right before that
		{
			valueendPos = insideBrackets.find('\"', valuestartPos); //looks for the last 
			value = insideBrackets.substr(valuestartPos + 1, (valueendPos - valuestartPos) - 1);
		}

		tag->addAtrribute(attribute, value);  
	}

}

/**
 * searches the childvec for tag
 * @param temp vector you'll be using
 * @param tag the element you're looking for
 * @return true or false
 */
bool searchChildVec(vector<element*> temp, element tag)
{
	bool found = false; //did it find the tag?
	vector<element*>::iterator jt; //for iterating the temporary vector

	for(jt=temp.begin(); jt != temp.end(); ++jt)
	{
		if((*jt)->getName()==tag.getName()) //if tag is located in temp (child vec)
		{
			found = true;
		}
	}

	return found;
}

/**
 * print Open element
 * @param tag to be printed
 */
void printOpenElement(element tag)
{
	int nameWidth = 30; //width of the name
	int numWidth = 5; //width of the number
	char separator = ' '; //using the i/o manipulation library adding spaces to fill the rest of the width

	cout << left << setw(numWidth) << setfill(separator) <<  tag.getlineNum(); //line number
	cout << left << setw(nameWidth) << setfill(separator) << "Opened Element: "+tag.getName()<< endl; //prints the element opened
}

/**
 * print Closed element
 * @param tag to be printed
 */
void printClosedElement(element tag)
{
	int nameWidth = 30; //width of the name
	int numWidth = 5; //width of the number
	char separator = ' '; //using the i/o manipulation library adding spaces to fill the rest of the width

	cout << left << setw(numWidth) << setfill(separator) <<  tag.getlineNum(); //line number
	cout << left << setw(nameWidth) << setfill(separator) << "Closed Element: "+tag.getName()<< endl; //prints the element closed
	cout << endl;
}

/**
 * print the closed element
 * @param line to be parsed and printed
 * @param lineNumber the line number to be included
 */
void printCompleteElement(string line, int lineNumber)
{
	int nameWidth = 30; //width of the name
	int numWidth = 5; //width of the number
	int startPos, endPos; //start and end pos for substr
	string fullLine; //for the full line without XML indentation
	string elementContent; //for what is between the tags

	char separator = ' '; //using the i/o manipulation library adding spaces to fill the rest of the width

	startPos = line.find("<");
	endPos = line.find("/n ",startPos+1); //getting the full line without spacing from XML
	fullLine = line.substr(startPos, (endPos - startPos) - 1); //getting the line with no spacing

	startPos = line.find(">"); 
	endPos = line.find("<",startPos+1); //getthing what's between the tags aka Content
	elementContent = line.substr(startPos+1, (endPos - startPos) - 1); //getting content

	cout << left << setw(numWidth) << setfill(separator) <<  lineNumber; //prints the name
	cout << left << setw(nameWidth) << setfill(separator) << "Complete Element: "+fullLine<< endl; //prints the complete line
	cout << left << setw(numWidth) << setfill(separator) << " "; //spacing for next line
	cout << left << setw(nameWidth) << setfill(separator) << "Element Name: "+ getOpen(fullLine)<< endl; //prints what is between the tags
	cout << left << setw(numWidth) << setfill(separator) << " "; //spacing for next line
	cout << left << setw(nameWidth) << setfill(separator) << "Element Content: "+ elementContent<< endl; //prints what is between the tags
	cout << endl;
}

/**
 * displays the stack for assignment 3
 * @param songstack
 */
void printStack(stack<element> songstack)
{
	cout << "     Vector containts: ";
	while(songstack.size()!=0) //if the stack isn't empty pop and print
	{
		cout<<songstack.top().getName()<<" ";
		songstack.pop(); 
	}
	cout << endl; //spacing for neatness
	cout << endl;
}

/**
 * parsed a closing line
 * @param line to be parsed
 * @return parsed string
 */
string getClosed(string line)
{
	int startPos; //where to start the substr
	int endPos; //where to end the substr
	string finalTag;

	startPos = line.find("/"); //to get tagname without slash
	endPos = line.find(">",startPos+1); //sets end pos to the closing bracket
	finalTag = line.substr(startPos+1, (endPos-startPos)-1); //creates a new string that is the closing tag

	return finalTag;
}

/**
 * parses an open line
 * @param line to be parsed
 * @return a parsed string
 */
string getOpen(string line)
{

	int startPos; //where to start the substr
	int endPos; //where to end the substr
	string initialTag, finalTag;

	startPos = line.find("<");
	endPos = line.find(">",startPos+1); //goes through "line" and finds what's in the brackets

	initialTag = line.substr(startPos+1, (endPos-startPos)-1); //creates a new string that is just what's in the brackets
	startPos = initialTag.find("<");
	endPos = initialTag.find(">",startPos+1); //resets end positions to correspond with the new string

	if(initialTag.find(" ", startPos+1)!=string::npos) //checks if there is a space in the string (ids, etc)
	{
		if(ispunct(initialTag.at(startPos+1))) //to avoid random punct.
		{
			startPos++;
		}
		endPos = initialTag.find(" ", startPos+1); //sets the endPos to where the first instance of the space is
		finalTag = initialTag.substr(startPos+1,(endPos-startPos)-1); //get's rid of what is after the space and stores in finalTag
	}

	if (initialTag.find(" ", startPos + 1) == string::npos) //if there is no space in the tag
	{
		if(ispunct(initialTag.at(startPos+1))) //to avoud random punct.
		{
			startPos++;
		}
		finalTag = initialTag.substr(startPos + 1, (endPos - startPos) - 1); //getting the finalTag
	}

	return finalTag;
}
