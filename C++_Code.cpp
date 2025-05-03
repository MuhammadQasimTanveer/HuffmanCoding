#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <iomanip>
using namespace std;

// Node structure for Huffman Tree
class Node 
{
public:
    char ch;
    int frequency;
    Node* left;
    Node* right;

    Node(char ch, int frequency) 
	{
        this->ch = ch;
        this->frequency = frequency;
        this->left = this->right = nullptr;
    }
};


// Comparator to prioritize nodes with lower frequencies in the priority queue
class Compare 
{
public:
    bool operator()(Node* left, Node* right) 
	{
        return left->frequency > right->frequency;
    }
};


// Recursive function to build Huffman Codes from the Huffman Tree
void buildCodes(Node* root, string str, unordered_map<char, string>& huffmanCode) 
{
    if(!root) 
	        return;
	        
	        
    // If we reach a leaf node, store its Huffman code
    if(!root->left && !root->right) 
	{
        huffmanCode[root->ch] = str.empty() ? "0" : str;
    }
    buildCodes(root->left, str + "0", huffmanCode); // Traverse left subtree with added '0'
    buildCodes(root->right, str + "1", huffmanCode); // Traverse right subtree with added '1'
}

// Function to delete the tree and free memory
void freeTree(Node* root) 
{
    if(!root) 
	       return;
	       
    freeTree(root->left);  // Free left subtree
    freeTree(root->right); // Free right subtree
    delete root;           // Delete the current node
}

int main() 
{
    string text;

    // User prompt for text input
    cout<<"\n\n                                        ==========================================\n";
    cout<<"                                                Welcome to Huffman Coding         \n";
    cout<<"                                        ==========================================\n\n";
    cout<<"Enter text to encode: ";
    getline(cin, text); // Get the input text

    // Check if the input text is empty
    if(text.empty())
	{
        cout<<"\nInput text is empty. Please enter some text.\n";
        return 0;
    }

    unordered_map<char, int> freq;  // Map to store character frequencies
    for(char ch : text)
	{
	    freq[ch]++; // Calculate frequency of each character
	}

    priority_queue<Node*, vector<Node*>, Compare> pq; // Min-heap for building the Huffman Tree
    for(auto pair : freq) 
	{
        pq.push(new Node(pair.first, pair.second)); // Push all nodes into the priority queue
    }

    // Build the Huffman Tree by combining the two least frequent nodes
    while(pq.size() > 1) 
	{
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        Node* merged = new Node('\0', left->frequency + right->frequency); // New internal node
        merged->left = left;  // Set the left child
        merged->right = right; // Set the right child
        pq.push(merged); // Push the merged node back into the priority queue
    }

    Node* root = pq.top(); // Root of the Huffman Tree

    unordered_map<char, string> huffmanCode; // Map to store the Huffman codes for each character
    buildCodes(root, "", huffmanCode); // Generate the Huffman codes

    string encodedStr;
    // Create the encoded string using the Huffman codes
    for(char ch : text) 
	{
        encodedStr += huffmanCode[ch];
    }

    string decodedStr;
    Node* curr = root;
    
    // Decode the encoded string
    for(char bit : encodedStr) 
	{
        curr = (bit == '0') ? curr->left : curr->right; // Traverse the tree based on the bit
        if (!curr->left && !curr->right) 
		{ 
		    // If we reach a leaf node
            decodedStr += curr->ch; // Append the decoded character
            curr = root; // Reset the current node to the root
        }
    }

    // Main menu for user interaction
    while(true) 
	{
        cout<<"\n\n         *********    HUFFMAN CODING MENU   **********\n\n      \n";

        cout<<"1. Show Character Frequencies\n";
        cout<<"2. Show Huffman Codes\n";
        cout<<"3. Show Original vs Compressed Size\n";
        cout<<"4. Show Encoded String\n";
        cout<<"5. Show Decoded String\n";
        cout<<"6. Find Frequency and Code of Specific Character\n";
        cout<<"7. Exit\n\n\n";
        cout<<"==========================================\n";
        cout<<"Choose an option: ";

        int choice;
        cin>>choice;  // Get user choice

        // Option 1: Show character frequencies
        if(choice == 1) 
		{
            cout<<"\n--- Character Frequencies ---\n";
            for(auto pair : freq) 
			{
                cout<<"'"<< pair.first<<"' : "<< pair.second<<"\n";
            }
        }
        
        // Option 2: Show Huffman codes
        else if(choice == 2) 
		{
            cout<<"\n--- Huffman Codes ---\n";
            for(auto pair : huffmanCode) 
			{
                cout<<"'"<< pair.first<<"' : "<< pair.second<<"\n";
            }
        }
        
        // Option 3: Show original vs compressed size
        else if(choice == 3) 
		{
            int originalSize = text.size() * 8; // Original size in bits
            int compressedSize = encodedStr.size(); // Compressed size in bits
            cout<<"\n--- Size Comparison ---\n";
            cout<<"Original Size:   "<<originalSize<<" bits\n";
            cout<<"Compressed Size: "<<compressedSize<<" bits\n";
            cout<<"Compression Ratio: "<<fixed << setprecision(2)
                << (100.0 * compressedSize / originalSize)<<"%\n";
        }
        
        // Option 4: Show the encoded string
        else if(choice == 4) 
		{
            cout<<"\n--- Encoded String ---\n";
            cout<<encodedStr<<"\n";
        }
        
        // Option 5: Show the decoded string
        else if(choice == 5) 
		{
            cout<<"\n--- Decoded String ---\n";
            cout<< decodedStr<<"\n";
        }

        // Option 6: Find frequency and Huffman code of a specific character
        else if(choice == 6) 
		{
            char searchChar;
            cout<<"\nEnter character to search for: ";
            cin>>searchChar;  // Get the character to search for

            if(freq.find(searchChar) != freq.end()) 
			{
                cout<< "\nFrequency of '"<<searchChar<<"': "<<freq[searchChar]<<"\n";
                cout<< "Huffman Code of '"<<searchChar<<"': "<<huffmanCode[searchChar]<<"\n";
            }
            else 
			{
                cout<<"Character not found in the input text.\n";
            }
        }
        
        // Option 7: Exit the program
        else if(choice == 7) 
		{
            cout<<"\nExiting...\n";
            break;
        }
        
        // Invalid option handling
        else 
		{
            cout<<"\nInvalid option. Please try again.\n";
        }

        cout<<"\n------------------------------------------\n";
    }

    // Free memory by deleting the Huffman tree
    freeTree(root);
    return 0;
}

