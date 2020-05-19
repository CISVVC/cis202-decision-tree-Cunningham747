/*
   File: Binary Tree Project
   Description: Creates a binary tree,from a command line argumented file, for an animal guessing game. Writes 
                additional nodes/subtrees to file for future games.
   Author: Christopher Cunningham
   Email:
*/

#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include "binary_tree.h"

/*
    Reads the input file and constructs Binary_tree from data in file
    @param in, istream object that is open onto file specified in command-line arguments
    @return Binary_tree, complete Binary_tree populated with data from input file
*/
Binary_tree read(istream &in)
{

    // Note the stream is called in

    string data; // Declare data as string
    int level;   // Declare level as integer

    in >> level; // Get level from stream

    // Determine if at end of path
    if (level == -1)
    {
        return Binary_tree();
    }

    getline(in >> std::ws, data); // Get line from stream, assign to data

    Binary_tree left(read(in)); // Declare left as Binary_tree, assign it read(in)

    Binary_tree right(read(in)); // Declare right as Binary_tree, assign it read(in)

    return Binary_tree(data, left, right); //Return complete Binary_tree
}

/*
    Writes user modified Binary_tree to original file, to be used for future iterations of this program
    @param out, ostream object that will write output to input file
    @param tree, referenced Binary_tree that will have its data written to input file
    @param level, integer denoting the height of the root node, usually 0
    @return no return, void function
*/
void write(ostream &out, const Binary_tree &tree, int level)
{

    // Determine if tree is empty
    if (tree.empty())
    {
        out << -1 << "\n";
        return;
    }

    out << level << ' ' << tree.data() << std::endl; // Formatted line in output file
    write(out, tree.left(), level + 1);              // Output left tree
    write(out, tree.right(), level + 1);             // Output right tree
}

/*
 * helper function that will help with definite or indefinite 
 * articles in a string
 * @param entry, input string to be manipulated
 * @return string, modified to include new article
 */
std::string article(const std::string &entry)
{
    std::string article = "a";
    if (std::string("aeiou").find(entry[0]) != std::string::npos)
    {
        article = "an";
    }
    return article;
}
/*
    Main driver function
    @param argc, integer denoting number of command-line arguments
    @param argv, character array containing user-input command-line argument
    @return integer, denotes whether execution was successful
*/
int main(int argc, char **argv)
{
    string filename; // String for holding command line argument filename

    // Check for command-line arguments and process
    if (argc > 1)
    {
        filename = argv[1]; // Get filename from command line arguments
    }
    else
    {
        std::cout << "No filename specified. Closing program."; // Close program if no filename specified
        return 0;
    }

    std::fstream in;             // Init filestream
    in.open(filename);           // Open filestream
    Binary_tree root = read(in); // Initilize root Binary_tree with file data
    in.close();                  // Close filestream

    Binary_tree question_tree = root;      // Declare a Binary_tree question_tree and set it to root
    string response, n_answer, n_question; // Declare strings for response and possible user input tree
    bool done = false;                     //Boolean to control loop

    while (!done)
    {
        Binary_tree left = question_tree.left();   // Declare Binary_tree left,set to question_tree.left()
        Binary_tree right = question_tree.right(); // Declare Binary_tree right,set to question_tree.right()

        if (left.empty() && right.empty()) // If there are no more questions
        {
            do
            {
                std::cout << "Is it " << article(question_tree.data()) << " " << question_tree.data() << std::endl;
                std::cin >> response;                     // Dump input into response string
            } while (response != "y" && response != "n"); // Loop to iterate if non-valid response

            if (response == "y")
            {
                std::cout << "I guessed it!\n";
            }
            else
            {
                // Ask what the user was thinking of

                std::cout << "I give up. What is it?" << std::endl;
                cin.clear();   //Clear input
                cin.ignore();  //Ignore last input
                getline(std::cin, response); // Dump input into response string
                n_answer = response;         // Process articles from input

                // Ask user for question for their given animal

                std::cout << "Please give me a question that is true for " << article(n_answer) << " " << n_answer
                          << " and false for a(n) " << question_tree.data() << std::endl;
                cin.clear();    //Clear input
                getline(std::cin, response); // Dump input into response string
                n_question = response;       // Dump response string into new question string

                //Insert new node into question tree

                string temp = question_tree.data(); //Store current data

                Binary_tree n_left(n_answer);              //New "no" binary tree
                Binary_tree n_right(question_tree.data()); //New "yes" binary tree

                question_tree.replace(n_question, n_left, n_right); // Replaces current node with new input binary tree
            }
            
            //Ask if user wants to play again

            do
            {
                std::cout << "May I try again?";
                std::cin >> response; // Dump input into response string
                if (response != "y") {done = true;} // Ends game
                else {question_tree = root;} // reset question tree to starting position}
            } while (response != "y" && response != "n");
        }
        else
        {
            do
            {
                std::cout << question_tree.data() << std::endl; // Output question
                std::cin >> response;                           // Dump input into response string
            } while (response != "y" && response != "n");       // Loop to iterate if non-valid response

            if (response == "y") {question_tree = left;} // Move to left tree if yes
            else {question_tree = right;} // Move to right tree if no
        }
    }

    //Write question tree to file
    std::cout << "Writing to file...";
    in.open(filename);  // Open file
    write(in, root, 0); // Write data to file
    in.close();         // Close file
    return 0;
}
