#include <fstream>
#include <iostream>
#include <vector>

#define INPUT_FILENAME "incseq.txt"

//function prototype(s)
void populateArray(std::vector<int>& intVector);
void printVector(std::vector<int> intVector);
int longestIncSeq(std::vector<int>& intVector, std::vector<int>& LIS);
void printSequence(std::vector<int>& intVector, std::vector<int>& LIS);

int main()
{
	std::vector<int> myInts;

	//read ints from input file and push to vector
	populateArray(myInts);
	std::vector<int> LISVector(myInts.size(), 0);
	//prints contents of vector
	printVector(myInts);

	int sequence_length = longestIncSeq(myInts, LISVector);

	//print longest sequence length
	std::cout << "Longest sequence length: " << sequence_length << std::endl;
	//print sequence
	printSequence(myInts, LISVector);

	return 0;
}

//reads integers from peak.txt and pushes them to vector of ints
void populateArray(std::vector<int>& intVector)
{
	//create file stream and attempt to open input file
	std::fstream inFile;
	inFile.open(INPUT_FILENAME);
	//check for file open errors
	if (inFile.fail())
	{
		std::cout << "Error: input file - " << INPUT_FILENAME << " - not found.\n";
		exit(1);
	}
	else { std::cout << "Reading from input file...\n"; }

	//read ints from input and push to vector
	int number;
	while (inFile >> number)
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

//iterate through vector of ints to find longest increasing sequence
int longestIncSeq(std::vector<int>& intVector, std::vector<int>& LIS)
{
	int max_val;
	int max_index;

	if (intVector.size() == 1) { return 1; }
	else
	{
		LIS[0] = 1;
		if (intVector[1] > intVector[0])
		{
			LIS[1] = 2;
			max_val = intVector[1];
			max_index = 1;
		}
		else
		{
			LIS[1] = 1;
			max_val = intVector[0];
			max_index = 0;
		}
		for (int i = 2; i < intVector.size(); i++)
		{
			if(intVector[i] > max_val)
			{
				LIS[i] = LIS[max_index] + 1;
				max_val = intVector[i];
				max_index = i;
			}
			else
			{
				int j;
				for (j = i - 1; intVector[i] < intVector[j] && j > 0; j--) {}
				LIS[i] = LIS[j] + 1;
			}
		}
	}
}

//print resultant sequence
void printSequence(std::vector<int>& intVector, std::vector<int>& LIS)
{
	std::cout << "Increasing sequence is: ";
	for (int index: LIS)
	{
		std::cout << intVector[index];
		if (index != intVector.back())
		{
			std::cout << ", ";
		}
	}
	std::cout << std::endl << std::endl;
}
