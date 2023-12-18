#include <iostream>
#include <string>
#include <vector>

//utility function prototypes
void populate_matrices(std::vector<std::pair<int, int>> &matrix_vector);
void print_matrices(std::vector<std::pair<int, int>> &matrix_vector);
void print_result(std::vector<std::vector<int>> &result_matrix);
void print_line() { std::cout << "========================================" << std::endl; };

//compute diagonal cost matrix and return optimal cost 
int analyze_matrices(std::vector<std::pair<int, int>> &matrix_vector, std::vector<std::vector<int>> &result_matrix, std::vector<std::vector<int>> &parenthetic_matrix);
//print parenthetic equation
void print_parenthesis(std::vector<std::vector<int>> &parenthetic_matrix, int i, int j);
//start lettering for parenthetic print
char letter = 'A';

int main()
{
	//declare vector to hold input matrices
	std::vector<std::pair<int, int>> matrices;
	//prompt user for input matrices
	populate_matrices(matrices);

	//create 2d vectors to hold diagonal matrix and parenthetic derivation matrix and then populate with INT_MAX
	std::vector<std::vector<int>> result_matrix(matrices.size(), std::vector<int>(matrices.size(), INT_MAX));
	std::vector<std::vector<int>> parenthetic_matrix(matrices.size(), std::vector<int>(matrices.size(), INT_MAX));

	//compute optimal cost and result/parenthetic matrices
	int optimal_cost = analyze_matrices(matrices, result_matrix, parenthetic_matrix);

	//print resultant diagonal matrix
	print_result(result_matrix);

	//print optimal work amount
	std::cout << "Optimal work: " << optimal_cost << std::endl << std::endl;

	//print parenthetic equation for optimal work
	std::cout << "Parenthetic Equation: ";
	print_parenthesis(parenthetic_matrix, 0, parenthetic_matrix.size() - 1);
	std::cout << std::endl << std::endl;

	return 0;
}

///populates vector of int pairs to hold matrix sizes
void populate_matrices(std::vector<std::pair<int, int>> &matrix_vector)
{
	int rows = 0;
	int cols = 0;

	//allow up to 10 matrices for computation
	for (int i = 0; i <= 10; i++)
	{
		std::pair<int, int> matrix;
		std::string another;

		//prompt for rows and then columns with input validation
		do
		{
			std::cout << "Matrix " << i + 1 << " # rows: ";
			while (!(std::cin >> rows)) {
				std::cout << "Incorrect input. Please enter an integer.\n";
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} while (rows == 0);

		do
		{
			std::cout << "Matrix " << i + 1 << " # cols: ";
			while (!(std::cin >> cols)) {
				std::cout << "Incorrect input. Please enter an integer.\n";
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} while (cols == 0);

		matrix.first = rows;
		matrix.second = cols;

		//push matrix m * n to vector of matrices
		matrix_vector.push_back(matrix);

		//ask if user wishes to enter another matrix
		std::cout << "Add another matrix? 'n' to quit : ";
		std::cin >> another;
		if (another == "n" || another == "N") { i = 10; }
		std::cout << std::endl;
	}
}

///iterates through vector of matrices diagonally and computes the optimal cost and parenthetic matrix
int analyze_matrices(std::vector<std::pair<int, int>> &matrix_vector, std::vector<std::vector<int>> &result_matrix, std::vector<std::vector<int>> &parenthetic_matrix)
{
	//for each row make the 0 diagonal values 0
	for (int i = 0; i < matrix_vector.size(); i++)
		result_matrix[i][i] = 0;
	//starting with diagonal 1 through final diagonal ( # diagonals = # cols = # rows)
	for (int d = 1; d < matrix_vector.size(); d++) {
		//iterate through all elements of diagonal (# elements = # rows - # diagonal)
		for (int i = 0; i < matrix_vector.size() - d; i++) {
			//j is last element to iterate through
			int j = i + d;
			//iterate through all values between i and j and compute cost of multiplication
			for (int k = i; k < j; k++) {
				int test_min = result_matrix[i][k] + result_matrix[k + 1][j] + matrix_vector[i].first * matrix_vector[k].second * matrix_vector[j].second;
				//if the calculated cost is less that cost already saved in spot, replace and update parenthetic matrix with k index
				if (test_min < result_matrix[i][j])
				{
					result_matrix[i][j] = test_min;
					parenthetic_matrix[i][j] = k;
				}
			}
		}
	}

	return result_matrix[0][result_matrix.size() - 1];
}

///recursively prints parenthesis order for optimal results
void print_parenthesis(std::vector<std::vector<int>> &parenthetic_matrix, int i, int j)
{
	//if matrix isolated, print matrix name
	if (i == j)
	{
		std::cout << letter;
		letter++;
	}
	//else print parenthesis and split where parenthetic matrix says to
	else
	{
		std::cout << "(";
		print_parenthesis(parenthetic_matrix, i, parenthetic_matrix[i][j]);
		print_parenthesis(parenthetic_matrix, parenthetic_matrix[i][j] + 1, j);
		std::cout << ")";
	}
}

///iterates through vector of input matrices and prints it
void print_matrices(std::vector<std::pair<int, int>> &matrix_vector)
{
	std::cout << "Matrix count: " << matrix_vector.size() << std::endl;
	for (std::pair<int, int> matrix : matrix_vector) {
		std::cout << matrix.first << " x " << matrix.second << std::endl;
	}
	std::cout << std::endl;
}

///iterates through matrix and prints it
void print_result(std::vector<std::vector<int>> &result_matrix)
{
	//iterate through resultant diagonal matrix and print values. X for MAX_INT that hasn't been changed.
	print_line();
	std::cout << "\tM[i,j] Diagonal Matrix" << std::endl;
	for (int i = 0; i < result_matrix.size(); i++) {
		for (int j = 0; j < result_matrix.size(); j++)
		{
			if (result_matrix[i][j] == INT_MAX)
				std::cout << "X\t";
			else
				std::cout << result_matrix[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}