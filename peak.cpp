/*
File:	peak.cpp
Author:	Austin Sands
Date:	04/06/2023
Desc:	This program first reads integers from an input file called "peak.txt". It then searches that vector for the peak value 
	utilizing an algorithm similar to a binary search.
*/

#include <fstream>
#include <vector>
#include <iostream>

#define INPUT_FILENAME "peak.txt"

//function prototype(s)
void populateArray(std::vector<int> &intVector);
void printVector(std::vector<int> intVector);
int findPeak(std::vector<int> &intVector, int begin, int end);

int main() {
	std::vector<int> myInts;

	//read ints from input file and push to vector
	populateArray(myInts);
	//prints contents of vector
	printVector(myInts);

	//calls peakIndex function with initial parameters of 0 begin index and size of vector minus 1 end index
	int peakIndex = findPeak(myInts, 0, myInts.size() - 1);
	
	std::cout << "Peak value in vector is at index: " << peakIndex << " with a value of: " << myInts[peakIndex] << std::endl;

	return 0;
}

//reads integers from peak.txt and pushes them to vector of ints
void populateArray(std::vector<int>& intVector)
{
	//create file stream and attempt to open input file
	std::fstream inFile;
	inFile.open(INPUT_FILENAME);
	//check for file open errors
	if(inFile.fail())
	{
		std::cout << "Error: input file - " << INPUT_FILENAME << " - not found.\n";
		exit(1);
	}
	else { std::cout << "Reading from input file...\n"; }

	//read ints from input and push to vector
	int number;
	while(inFile >> number)
	{
		intVector.push_back(number);
	}

	//close file after reading
	inFile.close();
}

//simple function to iterate through vector of ints and print them to console
void printVector(std::vector<int> intVector)
{
	std::cout << "Int Vector: ";
	if (intVector.empty()) { std::cout << "Empty\n"; }
	else {
		for (auto number : intVector)
		{
			std::cout << number;
			if (number != intVector.back())
			{
				std::cout << ", ";
			}
		}
		std::cout << std::endl << std::endl;
	}
}

//function to find peak value in array
int findPeak(std::vector<int>& intVector, int begin, int end)
{
	int middle = (begin + end) / 2;

	//base cases
	//if the begin and end index is the same, return it
	if (begin == end) { return end; }

	//if value at middle is greater than both indices before and after, it is the peak value. Return this index
	if (intVector[middle] > intVector[middle - 1] && intVector[middle] > intVector[middle + 1]) { return middle; }

	//if not base cases, divide vector in two and search again
	//if middle value is less than value to right, vector is still ascending, so peak must be between middle and end
	if(intVector[middle] < intVector[middle + 1])
	{
		//call findPeak from middle + 1 to end, excluding middle as we know it is not the peak
		return findPeak(intVector, middle + 1, end);
	}
	else {
		//otherwise, if middle value is greater than value to right, vector is already descending so peak must be between middle and begin
		//call findPeak from begin to middle - 1, excluding middle since we know it is not the peak
		return findPeak(intVector, begin, middle - 1);
	}

}