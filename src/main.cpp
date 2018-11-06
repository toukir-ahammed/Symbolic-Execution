#include <bits/stdc++.h>
#define MAX_NUM_OF_NODES 1000
#define INF 1000000000

using namespace std;

// Function Prototypes
void openinputFile(string inputFileName, ifstream &inFile);
void openOutputFile(string inputFileName, ofstream &outFile);
void closeinputFile(ifstream &inFile);
void closeOutputFile(ofstream &outFile);
void removeComments();
void basicBlockNumbering();
void initialize();
void processBlock(int blocknumber);
int findExit(int blockNumber);
string parseIfCondition(istringstream &iss);
string parseForCondition(istringstream &iss);
string parseWhileCondition(istringstream &iss);
void skipABlock(ifstream &inFile);
string reverseCondition(string condition);
void findAllPaths(int source, int destination);
void findAllPathsUtil(int current, int destination, bool visited[],int path[], int &path_index, string condition);
void printAllPaths();
string NumberToString(int Number);
void printString(string s);
void storeCondition(int index, string condition);
void printAllConditions();
void solve();
void showCFG();

// Global Variables
string inputFileName;
string inputFileName_v_1;
string inputFileName_v_2;
int adjMatrix[MAX_NUM_OF_NODES][MAX_NUM_OF_NODES];
string edges[MAX_NUM_OF_NODES][MAX_NUM_OF_NODES];
queue <int> myQueue;
vector <pair <string, string> >allPaths;
int idx = 0;

typedef struct Constraint
{
    string leftOperand;
    string operatorr;
    int rightOperand;

    Constraint(string leftOperand, string operatorr, int rightOperand)
    {
        this->leftOperand= leftOperand;
        this->operatorr = operatorr;
        this->rightOperand = rightOperand;
    }

    void toString()
    {
        cout << leftOperand << " " << operatorr << " " << rightOperand << ", ";
    }
}constraint;

vector <vector<constraint*> > constraints;
int solver(constraint* c);



int main()
{
    printString("Enter the file name only if in the same directory (e.g. input.c)\nOr, specify the full path (e.g. \"C:\\Users\\touki\\Desktop\\input.c\")");
    do
    {
        initialize();

        cout << "\nEnter the filename:" << endl;
        cin >> inputFileName;

        removeComments();

        basicBlockNumbering();

        myQueue.push(1);

        int lastBlock = 0;
        int choice = 0;

        while(!myQueue.empty())
        {
            int top = myQueue.front();
            myQueue.pop();

            processBlock(top);

            lastBlock = max(lastBlock, top);

        }

        showCFG();

        findAllPaths(1, lastBlock);
        printAllPaths();
        printAllConditions();
        solve();

        printString("");
        printString("Press 1 to continue\t\t Or, Press any other character to exit.");
        cin >> choice;
        if ( choice == 1)
            continue;
        else
            break;


    }while(1);


    return 0;
}



void openinputFile(string inputFileName, ifstream &inFile)
{
    inFile.open(inputFileName.c_str());

    if(!inFile)
    {
        cout << "\nCan't open the specified file." << endl;
        exit(-1);
    }
}


void openOutputFile(string inputFileName, ofstream &outFile)
{
    outFile.open(inputFileName.c_str());

    //outFile << "\nThe output file has been opened successfully." << endl;

}


void closeinputFile(ifstream &inFile)
{
    inFile.close();
}


void closeOutputFile(ofstream &outFile)
{
    outFile.close();
}


void removeComments()
{
    ifstream inFile;
    ofstream outFile;

    openinputFile(inputFileName, inFile);
    inputFileName_v_1 = inputFileName.substr(0, inputFileName.size()-2) + "_v_1.c" ;
    openOutputFile(inputFileName_v_1, outFile);

    char ch;
    ch = inFile.get();

    while(ch != EOF)
    {
        char temp;
        if(ch == '/')
        {
            temp = inFile.get();

            if(temp == '/')
            {
                    ch = inFile.get();
                    while(ch != '\n')
                    {
                        ch = inFile.get();
                    }
                    outFile << ch;
            }
            else if(temp == '*')
            {
                ch = inFile.get();
                while(1)
                {
                    if(ch == '*')
                    {
                        ch = inFile.get();
                        if(ch == '/')
                            break;
                    }
                    ch = inFile.get();
                }
            }
            else
            {
                outFile << ch << temp;
            }
        }
        else
            outFile << ch;

        ch = inFile.get();
    }
    closeinputFile(inFile);
    closeOutputFile(outFile);
}


void basicBlockNumbering()
{
    ifstream inFile;
    ofstream outFile;

    openinputFile(inputFileName_v_1, inFile);
    inputFileName_v_2 = inputFileName.substr(0, inputFileName.size()-2) + "_v_2.c" ;
    openOutputFile(inputFileName_v_2, outFile);

    string line;

    while (getline(inFile,line))
    {

        outFile <<line << endl;
        //cout << line << endl;
        istringstream iss;
        iss.str(line);
        char ch;
        bool start = false;

        while (iss>>ch)
        {
            if ( ch == '{')
            {
                start = true;
                break;
            }
        }

        if(start) break;

    }

    int blockNumber = 1;

    string word;

    while (getline(inFile,line))
    {

        outFile << blockNumber << line << endl;     //read and print line by line
        //cout << line << endl;
        istringstream iss;
        iss.str(line);

        while (iss>>word)
        {
            if (word == "if" || word == "}" || word == "for" || word == "while")
            {
                blockNumber++; // increment blocknumber if "if" or "}" or "for" is found in a line
            }
        }

    }

    cout << "\nNumbering basic block has been finished and stored in \"" << inputFileName_v_2 << "\" successfully."<< endl;

    closeinputFile(inFile);
    closeOutputFile(outFile);

}


void initialize()
{
    inputFileName = "";
    inputFileName_v_1 = "";
    inputFileName_v_2 = "";
    idx = 0;
    allPaths.clear();
    constraints.clear();
    
    for(int i=0; i<MAX_NUM_OF_NODES; i++)
    {
        for(int j=0; j <MAX_NUM_OF_NODES; j++)
        {
            adjMatrix[i][j] = 0;
            edges[i][j] = "";
        }
    }
}


void processBlock(int blockNumber)
{
    //cout << "\nProcessing " << blockNumber << endl;
    ifstream inFile;
    openinputFile(inputFileName_v_2, inFile);

    int currentBlockNumber = 0;
    int exitBlockNumber = findExit(blockNumber);

    string line = "";
    string condition = "";

    int rightBlockNumber = 0;
    int leftBlockNumber = 0;

    while(getline(inFile, line))
    {
        string word = "";
        istringstream iss;
        iss.str(line + "$");

        condition = "";
        iss >> currentBlockNumber;
        iss >> word;
        //cout << line << endl;

        if(currentBlockNumber == blockNumber && word == "if")
        {
            condition = parseIfCondition(iss);

            rightBlockNumber = currentBlockNumber + 1;
            adjMatrix[blockNumber][rightBlockNumber] = 1;
            edges[blockNumber][rightBlockNumber] = condition;
            myQueue.push(rightBlockNumber);

            //inFile.get();
            skipABlock(inFile);

            getline(inFile,line);
            istringstream iss1;
            iss1.str(line+" $");

            iss1 >> currentBlockNumber;
            iss1 >> word;

            if(word == "else")
            {
                leftBlockNumber = currentBlockNumber;
                adjMatrix[blockNumber][leftBlockNumber] = 1;
                edges[blockNumber][leftBlockNumber] = reverseCondition(condition);
                myQueue.push(leftBlockNumber);

                skipABlock(inFile);

                inFile >> currentBlockNumber;

                adjMatrix[rightBlockNumber][currentBlockNumber] = 1;
                adjMatrix[leftBlockNumber][currentBlockNumber] = 1;
                myQueue.push(currentBlockNumber);

            }
            else
            {
                adjMatrix[rightBlockNumber][currentBlockNumber] = 1;
                adjMatrix[blockNumber][currentBlockNumber] = 1;
                edges[blockNumber][currentBlockNumber] = reverseCondition(condition);
                myQueue.push(currentBlockNumber);


            }

            if(exitBlockNumber)
            {
                adjMatrix[blockNumber][exitBlockNumber] = 0;
                adjMatrix[currentBlockNumber][exitBlockNumber] = 1;
            }

            closeinputFile(inFile);

            break;


        }
        else if(currentBlockNumber == blockNumber && word == "for")
        {
            condition = parseForCondition(iss);

            rightBlockNumber = currentBlockNumber + 1;
            adjMatrix[blockNumber][rightBlockNumber] = 1;
            edges[blockNumber][rightBlockNumber] = condition;
            myQueue.push(rightBlockNumber);

            skipABlock(inFile);

            getline(inFile,line);
            istringstream iss1;
            iss1.str(line+" $");

            iss1 >> currentBlockNumber;

            adjMatrix[rightBlockNumber][currentBlockNumber] = 1;
            adjMatrix[blockNumber][currentBlockNumber] = 1;
            edges[blockNumber][currentBlockNumber] = reverseCondition(condition);
            myQueue.push(currentBlockNumber);

            if(exitBlockNumber)
            {
                adjMatrix[blockNumber][exitBlockNumber] = 0;
                adjMatrix[currentBlockNumber][exitBlockNumber] = 1;
            }

            closeinputFile(inFile);

            break;

        }
        else if(currentBlockNumber == blockNumber && word == "while")
        {
            condition = parseWhileCondition(iss);

            rightBlockNumber = currentBlockNumber + 1;
            adjMatrix[blockNumber][rightBlockNumber] = 1;
            edges[blockNumber][rightBlockNumber] = condition;
            myQueue.push(rightBlockNumber);

            skipABlock(inFile);

            getline(inFile,line);
            istringstream iss1;
            iss1.str(line+" $");

            iss1 >> currentBlockNumber;

            adjMatrix[rightBlockNumber][currentBlockNumber] = 1;
            adjMatrix[blockNumber][currentBlockNumber] = 1;
            edges[blockNumber][currentBlockNumber] = reverseCondition(condition);
            myQueue.push(currentBlockNumber);

            if(exitBlockNumber)
            {
                adjMatrix[blockNumber][exitBlockNumber] = 0;
                adjMatrix[currentBlockNumber][exitBlockNumber] = 1;
            }

            closeinputFile(inFile);

            break;


        }

    }
}


int findExit(int blockNumber)
{
    for(int j=0; j<30; j++)
    {
        if(adjMatrix[blockNumber][j] == 1)
            return j;
    }
    return 0;
}


string parseIfCondition(istringstream &iss)
{
    char ch;
    stack <char> myStack;
    ch = iss.get();

    string condition = "";						// read character by character after getting "if"

    while (ch != '$')							// '$' end marker
    {
        if(ch == '(')
        {
            myStack.push(ch);
            ch = iss.get();

            while(!myStack.empty())				// conditional statement reamains untill stack is not empty
            {
                condition += ch;				// append each caharacter in condition string

                ch = iss.get();

                if (ch == '(')
                {
                    myStack.push(ch);
                }

                else if(ch == ')')
                {
                    myStack.pop();
                }

            }
        }
        ch = iss.get();
    }

    return condition;

}


string parseForCondition(istringstream &iss)
{
    char ch;
    stack <char> myStack;
    ch = iss.get();

    string condition = "";						// read character by character after getting "if"

    while (ch != '$')							// '$' end marker
    {
        if(ch == ';')
        {
            myStack.push(ch);
            ch = iss.get();

            while(!myStack.empty())				// conditional statement reamains untill stack is not empty
            {
                condition += ch;				// append each caharacter in condition string

                ch = iss.get();

                if(ch == ';')
                {
                    myStack.pop();
                }

            }
        }
        ch = iss.get();
    }

    return condition;

}


string parseWhileCondition(istringstream &iss)
{
    char ch;
    stack <char> myStack;
    ch = iss.get();							// read character by character after getting "if"

    string condition = "";

    while (ch != '$')						// '$' end marker
    {
        if(ch == '(')
        {
            myStack.push(ch);
            ch = iss.get();

            while(!myStack.empty())				// conditional statement reamains untill stack is not empty
            {
                condition += ch;				// append each caharacter in condition string

                ch = iss.get();
                if(ch == '(')
                {
                    myStack.push(ch);
                }

                else if(ch == ')')
                {
                    myStack.pop();
                }

            }
        }
        ch = iss.get();
    }

    return condition;

}


void skipABlock(ifstream &inFile)
{
    char ch;
    string line;
    stack <char> myStack;
    ch = inFile.get();
    
    while (ch!=EOF)					
    {
        if(ch == '{')
        {
            myStack.push(ch);
            break;
        }
        ch = inFile.get();
    }

    while(!myStack.empty())
    {			
        ch = inFile.get();

        if (ch == '{')
        {
            myStack.push(ch);
        }

        else if(ch == '}')
        {
            myStack.pop();
        }

    }

    getline(inFile, line);

}


string reverseCondition(string condition)
{
    string relationalOperator [] = {">", "<" , "<=", ">=", "==" , "!=" };
    string revRelationalOperator[] = {"<=", ">=" , ">", "<", "!=" , "==" };

    string leftOperand;
    string operatorr;
    string rightOperand;

    istringstream iss;
    iss.str(condition);

    iss >> leftOperand;
    iss >> operatorr;
    iss >> rightOperand;

    string revCondition = "";

    for (int i = 0; i < 6; ++i)
    {
        if(operatorr == relationalOperator[i])
        {
            operatorr = revRelationalOperator[i];
            break;
        }
    }

    revCondition += leftOperand;
    revCondition += " " + operatorr + " ";
    revCondition += rightOperand;

    return " " +revCondition+ " ";


}



void showCFG()
{
    printString("Control Flow Graph");
    cout << "Edges\t\t\tLabel" << endl;
    cout << "========\t\t========" << endl;
    for(int i=0; i<MAX_NUM_OF_NODES; i++)
    {
        for(int j=0; j<MAX_NUM_OF_NODES; j++)
        {
            if(adjMatrix[i][j] == 1 )
            {
                cout << "(" << setw(2) << setfill('0') << i << ", " << setw(2) << setfill('0') << j << ")\t\t" << edges[i][j] << endl;
            }

        }
    }
}


void findAllPaths(int source, int destination)
{
    bool *visited = new bool[MAX_NUM_OF_NODES];

    int *path = new int[MAX_NUM_OF_NODES];
    int path_index = 0;

    // Initialize all nodes as not visited
    for (int i = 0; i < MAX_NUM_OF_NODES; i++)
        visited[i] = false;

    constraints.assign(MAX_NUM_OF_NODES, vector<constraint*>());	//initialize constraints vector

    // Call the recursive to find all paths
    findAllPathsUtil(source, destination, visited, path, path_index, "");

    delete [] visited;
    delete [] path;
}


void findAllPathsUtil(int current, int destination, bool visited[],
                              int path[], int &path_index, string condition)
{
    // Mark the current node and store it in path[]
    visited[current] = true;
    path[path_index] = current;
    path_index++;

    // If current node is the exit node, path is finished
    if (current == destination)
    {
        string tempPath = "";
        for (int i = 0; i<path_index; i++)
        {
            if(i!=0)
                tempPath += " ==> ";
            tempPath += NumberToString(path[i]);
        }
        //cout << condition << endl;
        //s = "";
        //cout << endl;
        allPaths.push_back(make_pair(tempPath, condition));
        storeCondition(idx++,condition);
    }
    else // If current node is not the exit node recur for all adjacent node
    {
        //list<int>::iterator i;
        for (int i = 0; i < MAX_NUM_OF_NODES; ++i)
        {
            if(adjMatrix[current][i] == 1 && !visited[i])
            {
                string newCondition = "";
                if(edges[current][i] != "")
                    newCondition = condition + edges[current][i] + + ",";
                else
                    newCondition = condition;
                findAllPathsUtil(i, destination, visited, path, path_index, newCondition);
            }

        }

    }

    // Remove current node from path[] and mark it as unvisited
    path_index--;
    visited[current] = false;
}


void printAllPaths()
{
    printString("All Paths");
    for(int i=0; i<allPaths.size(); i++)
    {
        cout << "Path " << i+1 << ": ";
        cout << allPaths[i].first << endl;
    }

    cout << "\nTotal Paths: " <<allPaths.size() << endl;
}


string NumberToString(int Number)
{
	stringstream ss;
	ss << Number;
	return ss.str();
}


void printString(string s)
{
    cout << "\n================================================================================================" << endl;
    cout << s << endl;
    cout << "================================================================================================" << endl;
}


void storeCondition(int index, string conditon)
{
    istringstream iss;
    string leftOperand;
    string operatorr;
    int rightOperand;
    string comma;

    iss.str(conditon);

    while(iss >> leftOperand >> operatorr >> rightOperand >> comma)
    {
        //cout << leftOperand << " " << operatorr << " " << rightOperand << endl;

        constraint* c = new constraint(leftOperand,operatorr,rightOperand);
        constraints[index].push_back(c);
        //cout << c << endl;;
    }


}


void printAllConditions()
{
    printString("All Paths Constraints");
    for(int i=0; i<allPaths.size(); i++)
    {
        cout << "Path " << i+1 << ": ";
        //cout << "size: " << constraints[i].size() << endl;
        for(int j=0; j<constraints[i].size(); j++)
        {
            //cout << constraints[i][j] << endl;
            constraint* c = constraints[i][j];
            //cout << c->leftOperand << " " << c->operatorr << " " << c-><<endl;
            c->toString();
            //cout << ", ";
        }
        cout << endl;
    }
}


void solve()
{
    printString("Solution for all paths:");
    map <string, constraint*> myMap;
    map <string, constraint*> :: iterator it;

    for(int i=0; i<allPaths.size(); i++)
    {
        for(int j=0; j<constraints[i].size(); j++)
        {

            constraint* c = constraints[i][j];

            it = myMap.find(c->leftOperand);
            if(it == myMap.end())
            {
                myMap[c->leftOperand] = c;
            }
            else
            {
                string previous = it->second->operatorr;
                string current = c->operatorr;

                if(previous == ">")
                {
                    if(current == ">")
                    {
                        it->second->rightOperand = max(it->second->rightOperand, c->rightOperand);

                    }
                    else if (current == ">=")
                    {
                        it->second->rightOperand = max(it->second->rightOperand, c->rightOperand);

                    }
                    else if (current == "<")
                    {
                        if((c->rightOperand - it->second->rightOperand) >= 2)
                        {
                            it->second->rightOperand = (it->second->rightOperand + c->rightOperand) / 2;
                            it->second->operatorr = "==";
                        }
                        else
                        {
                            it->second->rightOperand = INF;
                            it->second->operatorr = "==";
                        }

                    }
                    else if (current == "<=")
                    {
                        if((c->rightOperand - it->second->rightOperand) >=1 )
                        {
                            it->second->rightOperand = c->rightOperand;
                            it->second->operatorr = "==";
                        }
                        else
                        {
                            it->second->rightOperand = INF;
                            it->second->operatorr = "==";
                        }


                    }
                    else if (current == "==")
                    {
                        int x = c->rightOperand;
                        if(x > it->second->rightOperand)
                        {
                            it->second->operatorr = c->operatorr;
                            it->second->rightOperand = x;
                        }
                        else
                        {
                            it->second->operatorr = c->operatorr;
                            it->second->rightOperand = INF;
                        }


                    }
                    else if (current == "!=")
                    {
                        it->second->rightOperand = max(it->second->rightOperand, c->rightOperand);
                    }

                }
                else if (previous == ">=")
                {
                    if(current == ">")
                    {
                        it->second->rightOperand = max(it->second->rightOperand, c->rightOperand);

                    }
                    else if (current == ">=")
                    {
                        it->second->rightOperand = max(it->second->rightOperand, c->rightOperand);

                    }
                    else if (current == "<")
                    {
                        if((c->rightOperand - it->second->rightOperand) >=1 )
                        {
                            it->second->rightOperand = it->second->rightOperand;
                            it->second->operatorr = "==";
                        }
                        else
                        {
                            it->second->rightOperand = INF;
                            it->second->operatorr = "==";
                        }

                    }
                    else if (current == "<=")
                    {
                        if((c->rightOperand - it->second->rightOperand) >=0 )
                        {
                            it->second->rightOperand = it->second->rightOperand;
                            it->second->operatorr = "==";
                        }
                        else
                        {
                            it->second->rightOperand = INF;
                            it->second->operatorr = "==";
                        }


                    }
                    else if (current == "==")
                    {
                        int x = c->rightOperand;
                        if(x >= it->second->rightOperand)
                        {
                            it->second->operatorr = c->operatorr;
                            it->second->rightOperand = x;
                        }
                        else
                        {
                            it->second->operatorr = c->operatorr;
                            it->second->rightOperand = INF;
                        }

                    }
                    else if (current == "!=")
                    {
                        it->second->rightOperand = max(it->second->rightOperand, c->rightOperand);
                    }

                }
                else if (previous == "<")
                {
                    if(current == ">")
                    {
                        if((it->second->rightOperand - c->rightOperand) >= 2)
                        {
                            it->second->rightOperand = (it->second->rightOperand + c->rightOperand) / 2;
                            it->second->operatorr = "==";
                        }
                        else
                        {
                            it->second->rightOperand = INF;
                            it->second->operatorr = "==";
                        }

                    }
                    else if (current == ">=")
                    {
                        if((it->second->rightOperand - c->rightOperand) >= 1)
                        {
                            it->second->rightOperand =  c->rightOperand;
                            it->second->operatorr = "==";
                        }
                        else
                        {
                            it->second->rightOperand = INF;
                            it->second->operatorr = "==";
                        }


                    }
                    else if (current == "<")
                    {
                        it->second->rightOperand = min(it->second->rightOperand, c->rightOperand);
                    }
                    else if (current == "<=")
                    {
                        it->second->rightOperand = min(it->second->rightOperand, c->rightOperand);
                    }
                    else if (current == "==")
                    {
                        int x = c->rightOperand;
                        if(x < it->second->rightOperand)
                        {
                            it->second->operatorr = c->operatorr;
                            it->second->rightOperand = x;
                        }
                        else
                        {
                            it->second->operatorr = c->operatorr;
                            it->second->rightOperand = INF;
                        }
                    }
                    else if (current == "!=")
                    {
                        it->second->rightOperand = min(it->second->rightOperand, c->rightOperand);
                    }

                }
                else if (previous == "<=")
                {
                    if(current == ">")
                    {
                        if((it->second->rightOperand - c->rightOperand) >= 1)
                        {
                            it->second->rightOperand = it->second->rightOperand;
                            it->second->operatorr = "==";
                        }
                        else
                        {
                            it->second->rightOperand = INF;
                            it->second->operatorr = "==";
                        }
                    }
                    else if (current == ">=")
                    {
                        if((it->second->rightOperand - c->rightOperand) >= 0)
                        {
                            it->second->rightOperand = it->second->rightOperand;
                            it->second->operatorr = "==";
                        }
                        else
                        {
                            it->second->rightOperand = INF;
                            it->second->operatorr = "==";
                        }
                    }
                    else if (current == "<")
                    {
                        it->second->rightOperand = min(it->second->rightOperand, c->rightOperand);
                    }
                    else if (current == "<=")
                    {
                        it->second->rightOperand = min(it->second->rightOperand, c->rightOperand);
                    }
                    else if (current == "==")
                    {
                        int x = c->rightOperand;
                        if(x <= it->second->rightOperand)
                        {
                            it->second->operatorr = c->operatorr;
                            it->second->rightOperand = x;
                        }
                        else
                        {
                            it->second->operatorr = c->operatorr;
                            it->second->rightOperand = INF;
                        }
                    }
                    else if (current == "!=")
                    {
                        it->second->rightOperand = min(it->second->rightOperand, c->rightOperand);
                    }

                }
                else if (previous == "==")
                {
                    if(current == ">")
                    {
                        if(!(it->second->rightOperand > c->rightOperand))
                        {
                            it->second->operatorr = "==";
                            it->second->rightOperand = INF;
                        }

                    }
                    else if (current == ">=")
                    {
                        if(!(it->second->rightOperand >= c->rightOperand))
                        {
                            it->second->operatorr = "==";
                            it->second->rightOperand = INF;
                        }
                    }
                    else if (current == "<")
                    {
                        if(!(it->second->rightOperand < c->rightOperand))
                        {
                            it->second->operatorr = "==";
                            it->second->rightOperand = INF;
                        }
                    }
                    else if (current == "<=")
                    {
                        if(!(it->second->rightOperand <= c->rightOperand))
                        {
                            it->second->operatorr = "==";
                            it->second->rightOperand = INF;
                        }
                    }
                    else if (current == "==")
                    {
                        if(!(it->second->rightOperand == c->rightOperand))
                        {
                            it->second->operatorr = "==";
                            it->second->rightOperand = INF;
                        }

                    }
                    else if (current == "!=")
                    {
                        if(!(it->second->rightOperand != c->rightOperand))
                        {
                            it->second->operatorr = "==";
                            it->second->rightOperand = INF;
                        }
                    }

                }
                else if (previous == "!=")
                {
                    if(current == ">")
                    {
                        it->second->rightOperand = max(it->second->rightOperand, c->rightOperand);
                    }
                    else if (current == ">=")
                    {
                        it->second->rightOperand = max(it->second->rightOperand, c->rightOperand);
                    }
                    else if (current == "<")
                    {
                        it->second->rightOperand = min(it->second->rightOperand, c->rightOperand);
                    }
                    else if (current == "<=")
                    {
                        it->second->rightOperand = min(it->second->rightOperand, c->rightOperand);
                    }
                    else if (current == "==")
                    {
                        int x = c->rightOperand;
                        //changed
                        if(x == it->second->rightOperand)
                        {
                            /*it->second->operatorr = c->operatorr;
                            it->second->rightOperand = x;*/
                            it->second->operatorr = c->operatorr;
                            it->second->rightOperand = INF;
                        }
                        else
                        {
                            /*it->second->operatorr = c->operatorr;
                            it->second->rightOperand = INF;*/
                            it->second->operatorr = c->operatorr;
                            it->second->rightOperand = x;
                        }


                    }
                    else if (current == "!=")
                    {
                        if(!(it->second->rightOperand != c->rightOperand))
                        {
                            it->second->operatorr = "==";
                            it->second->rightOperand = INF;
                        }
                        // changed
                        else
                        {
                            it->second->operatorr = "==";
                            it->second->rightOperand = c->rightOperand * it->second->rightOperand + 5  ;
                        }

                    }

                }
            }


        }

        cout << "Path " << i+1 << ": ";
        for(it = myMap.begin(); it!=myMap.end(); it++)
        {
            constraint* c = it->second;
            int soln = solver(c);

            if(soln != INF)
                cout << it->first << " = " << soln <<", ";
            else
                cout <<  it->first << " = " << "No solution exist, ";
        }
        
        cout << endl;
        myMap.clear();
    }

}


int solver(constraint* c)
{
    int solution = 0;
    string relationalOperator [] = {">", "<" , "<=", ">=", "==" , "!=" };
    int solutionMaker[] = {+1, -1 , -1, +1, 0 , +5 };

    string leftOperand = c->leftOperand;
    string operatorr = c->operatorr;
    int rightOperand = c->rightOperand;

    for (int i = 0; i < 6; ++i)
    {
        if(operatorr == relationalOperator[i])
        {
            solution = rightOperand + solutionMaker[i];
            break;
        }
    }

    return solution;

}

