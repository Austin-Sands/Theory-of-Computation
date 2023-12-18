#include <vector>
#include <queue>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <algorithm>

#define INPUT_FILENAME "freq.txt"
#define OUTPUT_FILENAME "codetable.txt"

//Node struct to hold pertinent information
struct Node
{
	std::string char_name;
	int ascii_code;
	int count;
	std::string huffman_code;
	Node* left;
	Node* right;
};

//overload operator for priority queue comparisons
struct Compare_Nodes {
	bool operator () (const Node* left_node, const Node* right_node) const
	{
		return (left_node->count > right_node->count);
	}
};

//create parent node from two nodes
Node* create_node(Node* left, Node* right)
{
	Node* node = new Node();
	node->ascii_code = -1;
	node->left = left;
	node->right = right;
	node->count = left->count + right->count;

	return node;
}

//create a map to enumerate the non-printable chars
std::map<int, std::string> non_printable = {
	{0, "NUL"}, {1, "SOH"}, {2, "STX"}, {3, "ETX"}, {4, "EOT"}, {5, "ENQ"}, {6, "ACK"}, {7, "BEL"}, {8, "BS"}, {9, "HT"},
	{10, "LF"}, {11, "VT"}, {12, "FF"}, {13, "CR"}, {14, "SO"}, {15, "SI"}, {16, "DLE"}, {17, "DC1"}, {18, "DC2"}, {19, "DC3"},
	{20, "DC4"}, {21, "NAK"}, {22, "SYN"}, {23, "ETB"}, {24, "CAN"}, {25, "EM"}, {26, "SUB"}, {27, "ESC"}, {28, "FS"}, {29, "GS"},
	{30, "RS"}, {31, "US"}, {32, "SP"}, {127, "DEL"}
};

//function prototypes
void populate(std::vector<Node*>& node_vector);
void read_input(std::vector<Node*>& node_vector, std::priority_queue<Node*, std::vector<Node*>, Compare_Nodes>& node_queue);
void print_vector(std::vector<Node*>& node_vector);
void print_queue(std::priority_queue<Node*, std::vector<Node*>, Compare_Nodes>& node_queue);
void create_queue(std::vector<Node*>& node_vector, std::priority_queue<Node*, std::vector<Node*>, Compare_Nodes>&node_queue);

void huffman_tree(std::priority_queue<Node*, std::vector<Node*>, Compare_Nodes>& node_queue);
void assign_code(Node* root, std::string code);
void write_codetable(std::vector<Node*> node_vector);

/*=============================================================================*/
/// main function
int main()
{
	std::vector<Node*> chars;
	std::priority_queue<Node*, std::vector<Node*>, Compare_Nodes> nodes_ascending;

	//populates char vector with all possible ASCII values and initializes count to 0
	populate(chars);

	//gets character count from input file and updates char nodes, creates priority queue for huffman algorithm to work
	read_input(chars, nodes_ascending);

	//takes priority queue and turns it into a tree following the algorithm, then assigns huffman codes to characters
	huffman_tree(nodes_ascending);

	//utility to print table to console
	print_vector(chars);

	//writes same table to output file per project prompt
	write_codetable(chars);

	return 0;
}
//end main function
/*=============================================================================*/

///populates node vector with all char counts starting at 0
void populate(std::vector<Node*>& node_vector)
{
	//iterate through all ascii codes
	for (int i = 0; i < 128; i++)
	{
		Node* temp_node = new Node();
		temp_node->ascii_code = i;
		if( temp_node->ascii_code > 32 && temp_node->ascii_code < 127)
		{
			temp_node->char_name = char(i);
		}
		else
		{
			temp_node->char_name = non_printable[i];
		}
		temp_node->count = 0;

		node_vector.push_back(temp_node);
	}
}

///reads from input file and saves input file data to vector of nodes
void read_input(std::vector<Node*>& node_vector, std::priority_queue<Node*, std::vector<Node*>, Compare_Nodes>& node_queue)
{
	//create file stream and attempt to open input file
	std::fstream in_file;
	in_file.open(INPUT_FILENAME);
	//check for file open errors
	if (in_file.fail())
	{
		std::cout << "Error: input file - " << INPUT_FILENAME << " - not found.\n";
		exit(1);
	}
	else { std::cout << "Reading from input file...\n\n"; }

	//read pairs of input from lines in input
	std::string char_in;
	int char_count;
	while (in_file >> char_in >> char_count)
	{
		if (char_in.length() > 1)
		{
			for (std::pair<const int, std::string> code : non_printable)
			{
				if (code.second == char_in)
				{
					node_vector[code.first]->count = char_count;
					break;
				}
			}
		}
		else
		{
			node_vector[int(char_in[0])]->count = char_count;
		}
	}

	//close file after reading
	in_file.close();

	//create priority queue of nodes based on count and vector of leaves in tree (leaves occur where count > 0)
	create_queue(node_vector, node_queue);
}

///prints out nodes and their data
void print_vector(std::vector<Node*>& node_vector)
{
	for (Node* node : node_vector)
	{
		if (node->count > 0) {
			std::cout << "Code:  " << node->ascii_code << "\tChar: " << node->char_name;
			if(node->char_name.length() < 2)
			{
				std::cout << "\t";
			}
			std::cout << "\tCount: " << node->count << "\tHuff-Code: " << node->huffman_code << std::endl;
		}
	}
	std::cout << std::endl;
}

///prints and pops queue, used for testing
void print_queue(std::priority_queue<Node*, std::vector<Node*>, Compare_Nodes>& node_queue)
{
	while(!node_queue.empty())
	{
		std::cout << "Char: " << node_queue.top()->char_name << "\tCount: " << node_queue.top()->count << std::endl;
		node_queue.pop();
	}
}

///create priority queue of nodes based on count and vector of leaves in tree (leaves occur where count > 0)
void create_queue(std::vector<Node*>& node_vector, std::priority_queue<Node*, std::vector<Node*>, Compare_Nodes>& node_queue)
{
	for (Node* node : node_vector)
	{
		if (node->count > 0)
		{
			node_queue.push(node);
		}
	}
}

///creates the huffman tree from the queue and assigns huffman codes
void huffman_tree(std::priority_queue<Node*, std::vector<Node*>, Compare_Nodes>& node_queue)
{
	//iterate through queue while there is more than one entry creating intermediate nodes between the two minimum values. Remaining item in queue is tree root
	while(node_queue.size() > 1)
	{
		Node* left = new Node();
		Node* right = new Node();
		left = node_queue.top();
		node_queue.pop();
		right = node_queue.top();
		node_queue.pop();
		node_queue.push(create_node(left, right));
	}

	//call to assign codes starting with an empty string
	assign_code(node_queue.top(), "");
}

///traverses huffman tree and assigns code
void assign_code(Node* root, std::string code)
{
	//if there is nothing in passed pointer, there are no leaves
	if (root == NULL) { return; }

	//nodes with positive values for ascii code are leaves of tree
	if(root->ascii_code >= 0)
	{
		root->huffman_code = (code != "") ? code : "1";
	}

	//call recursively to both right and left nodes. Left nodes append a zero to huffman code while right nodes append 1
	assign_code(root->left, code + "0");
	assign_code(root->right, code + "1");
}

//writes all char information for nodes with greater than 0 count to outputfile
void write_codetable(std::vector<Node*> node_vector)
{
	std::ofstream out_file;

	out_file.open(OUTPUT_FILENAME);
	for (Node* node : node_vector)
	{
		if(node->count > 0)
		{
			out_file << "Code:  " << node->ascii_code << "\tChar: " << node->char_name << "\tCount: " << node->count << "\tHuff-Code: " << node->huffman_code << std::endl;
		}
	}
	out_file.close();
}
