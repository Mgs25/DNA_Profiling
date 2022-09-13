//
//  Project 1 Starter Code - DNA Profiling
//  TODO:  Put your header info here.  Update line above.
//
//

#include <iostream>
#include "ourvector.h"
#include <fstream>
#include <sstream>

using namespace std;

typedef ourvector<char> ShortTandemRepeat;
typedef ourvector<char> DNA;

//
// Namespace to store string literal for user command for better readability
//
namespace CMD_CONSTANTS
{
    const string DB_LOAD = "load_db";
    const string DISPLAY = "display";
    const string DNA_LOAD = "load_dna";
    const string PROCESS = "process";
    const string SEARCH = "search";
}

// TODO:  Write your code here.  You should have LOTS of function decomposition.

struct Individual
{
    ourvector<char> Name;
    ourvector<int> StrCount;
};

// Ultility functions
template <typename T>
void DisplayOurVector(ourvector<T>);
template <typename T>
void DisplayOurVector(ourvector<T>, char);
string ReadContentFromFile(const string &);
void PopulateStrs(ourvector<ShortTandemRepeat> &, const string &);
void PopulateIndividuals(ourvector<Individual> &, const string &);
string GetNthLine(const string &, int);

// Command based functions
void loadDb(const string &, ourvector<ShortTandemRepeat> &, ourvector<Individual> &);
void loadDna(const string &, DNA &);
void display(ourvector<Individual>, DNA);

int main()
{
    string dbContent; // read content from DB

    ourvector<ShortTandemRepeat> strList; // List of STRs on the first line
    ourvector<Individual> individuals;    // List of individuals with name and str count
    DNA dna;

    cout << "Welcome to the DNA Profiling Application" << endl;

    string command;
    bool validCmd = true;

    do
    {
        cout << "Enter command or # to exit: ";
        getline(cin, command);

        istringstream ss(command);
        string cmdType, cmdParams;
        ss >> cmdType >> cmdParams;

        if (cmdType == CMD_CONSTANTS::DB_LOAD)
        {
            cout << "Loading database..." << endl;
            // Clear existing DB data
            strList.clear();
            individuals.clear();

            loadDb(cmdParams, strList, individuals); // loads content from db to dbContent string, then populates strList and individuals
        }
        else if (cmdType == CMD_CONSTANTS::DNA_LOAD)
        {
            loadDna(cmdParams, dna);
        }
        else if (cmdType == CMD_CONSTANTS::DISPLAY)
        {
            display(individuals, dna);
        }
        else if (cmdType == CMD_CONSTANTS::PROCESS)
        {
        }
        else if (cmdType == CMD_CONSTANTS::SEARCH)
        {
        }
    } while (command != "#");

    return 0;
}

template <typename T>
void DisplayOurVector(ourvector<T> ov)
{
    for (auto element : ov)
    {
        cout << element;
    }
}

template <typename T>
void DisplayOurVector(ourvector<T> ov, char sep)
{
    for (auto element : ov)
    {
        cout << element << sep;
    }
}

string ReadContentFromFile(const string &fileName)
{
    string content{""};                    // string to store the content of the file
    ifstream file(fileName, ifstream::in); // open file using ifstream class in input (read) mode.

    if (!file.is_open())
    {
        throw runtime_error("read_file: error opening the file"); // throw error message, if file was not opened
    }

    string line{""}; // temporary string to store individual lines of the file

    // loop every line of the file until end of the file (EOF) is reached
    while (getline(file, line))
    {
        content.append(line.append("\n")); // append current line with a newline character to the content string
    }

    file.close(); // close file after accessing the data

    return content;
}

string GetNthLine(const string &dbContent, int lineNumber)
{
    istringstream ss{dbContent};
    string line{};

    while (getline(ss, line, '\n') && lineNumber--)
    {
    }

    return line;
}

void PopulateStrs(ourvector<ShortTandemRepeat> &strList, const string &dbContent)
{
    string firstLine = GetNthLine(dbContent, 0);
    istringstream ss(firstLine);
    string strToken{};

    while (getline(ss, strToken, ','))
    {
        if (strToken != "name")
        {
            ShortTandemRepeat currentStr;
            for (char base : strToken)
            {
                currentStr.push_back(base);
            }
            strList.push_back(currentStr);
        }
    }
}

void PopulateIndividuals(ourvector<Individual> &individuals, const string &dbContent)
{
    int currentLineNumber = 1;
    string currentLine = GetNthLine(dbContent, currentLineNumber);

    while (currentLine != "")
    {
        istringstream ss(currentLine);
        string token;

        // Get name
        ourvector<char> name;
        getline(ss, token, ',');

        for (char c : token)
        {
            name.push_back(c);
        }

        // Get counts
        ourvector<int> strCount;

        while (getline(ss, token, ','))
        {
            strCount.push_back(stoi(token));
        }

        Individual person;
        person.Name = name;
        person.StrCount = strCount;

        individuals.push_back(person);

        currentLine = GetNthLine(dbContent, ++currentLineNumber);
    }
}

void loadDb(const string &fileName, ourvector<ShortTandemRepeat> &strList, ourvector<Individual> &individuals)
{
    try
    {
        string dbContent = ReadContentFromFile(fileName);

        PopulateStrs(strList, dbContent);
        PopulateIndividuals(individuals, dbContent);
    }
    catch (...)
    {
        cout << "Error: unable to open '" << fileName << "'";
        exit(0);
    }
}

void loadDna(const string &fileName, DNA &dna)
{
    try
    {
        string dnaContent = ReadContentFromFile(fileName);

        for (char base : dnaContent)
        {
            dna.push_back(base);
        }
    }
    catch (...)
    {
        cout << "Error: unable to open '" << fileName << "'";
        exit(0);
    }
}

void display(ourvector<Individual> individuals, DNA dna)
{
    if (individuals.size() != 0)
    {
        cout << "Database loaded:" << endl;

        for (auto individual : individuals)
        {
            DisplayOurVector(individual.Name);
            cout << ' ';
            DisplayOurVector(individual.StrCount, ' ');
            cout << endl;
        }

        cout << endl << endl;
    }
    else
    {
        cout << "No database loaded." << endl << endl;
    }

    if (dna.size() != 0)
    {
        cout << "DNA loaded:" << endl;

        DisplayOurVector(dna);

        cout << endl << endl;
    }
    else
    {
        cout << "No DNA loaded." << endl << endl;
    }
}