#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <iomanip>
#include <climits>
#include <vector>
using namespace std;
/*
 Name: Zachary West
 Email: zwest2563@gmail.com
 Course Section: Fall 2024 CS 101
 Project #: 1
 Instructions to Compile:
    I used the following statement to compile my code: "g++ p1.cpp"
 Instructions to Execute:
    To execute the code properly, use the following format: 
        "./a.out ex.csv 15 (filename) (col/row)"
        the (col/row) value references how wide (# of characters) each column is allotted in the print
    
   NOTE: the search function has the notation "*|Col_Name". 
   This means that you can EITHER use * (searches across all columns) or enter a column Name.
   If you search within a column that is more than one word, include quotes such as "First Name"

*/

vector <string> FindCol(string colName, vector <vector <string> > data, int rowSize, int colSize);
vector<string> ReadLine(string tempLine);
vector<string> CommandToWords(string commandLine);
int main(int argc,char* argv[]){
    // Usage: ./a.out ex.csv 15 (filename) (col/row)
    
    int colWidth;
    int height; // vertical size
    int length;
    string tempLine;
    vector<string> vectorRow;
    vector<string> colNames;

    //FILES & INPUTS

    if (argc != 3){
        cout <<"Usage: ./a database.csv #col_width" <<endl;
        return -1;
    }
    string fName = argv[1];
    colWidth = stoi(argv[2]);
    ifstream fin(fName);
    if (!(fin.is_open())){//if false
        cout << "Failed to open \"" <<fName << "\"" << endl;
        return -1;
    }

    getline(fin,tempLine); // first line of the file, provides column names
    colNames = ReadLine(tempLine);
    vectorRow = colNames;
    length = vectorRow.size();
    
    vector< vector <string> > data; //2D vector... RESIZING MAY NEED ADDRESSED
    data.push_back(vectorRow); // adds columns to the database
    
    while (getline(fin,tempLine)){  // input a line from CSV, clean the data, assign the data, add that data to database,
        vectorRow = ReadLine(tempLine);
        data.push_back(vectorRow); // for every added row, the height increments
    }
    fin.close();

    length = vectorRow.size();
    vectorRow.resize(length); 

    height = data.size();
    data.resize(data.size());

    //cout << "checkpoint! Data initialized into the 2D vector!" << endl;


    //MENU FUNCTION
    
    string quit = "quit";
    string help = "help";
    string print = "print";
    string cols = "cols";
    vector<string> column(height-1); // initialized with spots for all rows except the title of a col

    cout << endl << "Enter a command or \"help\" for a command list:" << endl;
    string command = "test";
    vector<string> words(3);
    int numWords;
    int targetFlag = 0;
    int asteriskFlag = 0;
    int quoteFlag = 0;
    while (command != quit){ //MENU
        getline(cin,command);

        words = CommandToWords(command);
        numWords = words.size();
        //cout << words[0] << " " << numWords<< endl;
        if (numWords == 1){ // 1 word commands
        if (words[0] == quit){
            break;
        }
        else if (words[0] == help){
            string justify = "        ";
            cout << "command list:" << endl << justify << print << endl << justify << cols << endl << justify << "search *|col_name val" << endl << justify << "min col_name" << endl << justify << "max col_name" << endl << justify << "avg col_name" <<endl << endl;
            continue;
        }
        else if (words[0] == print){
            for (int i = 0; i < height; i ++){
                for (int j = 0 ; j < length; j ++){
                    cout << setw(colWidth) <<data[i][j];
                }
                cout << endl;
            }
            continue;
        }
        else if (words[0] == cols){
            for (int i = 0; i < length; i ++){
                cout << colNames.at(i) << endl;
            }
            continue;
        }
        }

        //2 WORD COMMANDS
        targetFlag = 0;
        asteriskFlag = 0; // if a column is an asterisk
        
        string target = words.at(1);
        string targetQuotes = target; //targetQuotes equals the original word unless its quoted, then it equals whats inside the quotes WILL COLUMNS INCLUDE QUOTES?
        bool valFound = false;
        if (numWords == 2) // checks to ensure word 2 is relevant
        {
            if (target.at(0) == '"'){ //if the first thing of the word is a quote, then we should search WITHIN the quotes
                targetQuotes = target.substr(1,target.size()-2);
                quoteFlag = 1;
            }
            for (int i = 0;i < colNames.size();i++){ // search the colNames for what you are searching for, grab that column
                if (colNames.at(i) == targetQuotes){ //if the current colName is = the target
                    targetFlag = 1; // bool for if column is found
                    column = FindCol(targetQuotes,data,height,length);
                }
                if (colNames.at(i) == "*") asteriskFlag = 1; // asteriskFlag tells us if a column is an asterisk! if quoteFlag AND asteriskFlag,
            }

            if (!targetFlag){ // if the target  is not found... continue to the next command
                cout << "invalid column \"" << targetQuotes << "\"" << endl;
                continue;
            }


        if (words[0] =="min" && targetFlag){
            double minimum = INT_MAX;
            for(int a = 1; a < height; a++){
                if (column.at(a) == "") continue;
                if (stod(column.at(a)) <= minimum){
                    minimum = stod(column.at(a));
                }
            }
            if (quoteFlag){
                target = targetQuotes;    
                }
            if (minimum == INT_MAX){
                
              cout << "The min for col \""<<target<<"\" = " <<" N/A"<< endl;  
            }
            else{
            cout << "The min for col \""<<target<<"\" = " << minimum << endl;
            }
            continue;
        }
        else if (words[0]=="max" && targetFlag){
            double maximum = INT_MIN;
            for(int a = 1; a < height; a++){
                if (column.at(a) == "") continue;
                if (stod(column.at(a)) >= maximum){
                    maximum = stod(column.at(a));
                }
            }
            if (quoteFlag){
                target = targetQuotes;    
                }
            if (maximum == INT_MAX){
                
              cout << "The max for col \""<<target<<"\" = " <<" N/A"<< endl;  
            }
            else{
            cout << "The max for col \""<<target<<"\" = " << maximum << endl;
            }
            continue;
        }
        else if (words[0] =="avg" && targetFlag){
            double sum = 0;
            int flag = 0;
            double average;
            double denom = 0.0;
            for(int a = 1; a < height; a++){
                if (column.at(a) == "") continue;
                sum += stod(column.at(a));
                denom++;
                flag = 1;
            }
            if (quoteFlag){
                target = targetQuotes;    
                }
            average = sum/denom;
            if (flag){
                 cout << "The avg for col \""<<target<<"\" = " << average << endl;  
            }
            else{
                cout << "The avg for col \""<<target<<"\" = " << "N/A" << endl;  
            }
            continue;
        }
        }
        
        vector < vector <string> > searchBase;
        vector <string> column1(length);
        vector <string> temporary(length);
        if (numWords == 3 && words.at(0) == "search"){
            bool valFound = false;
            bool colFound = false;
            bool quoted2 = false;
            bool quoted3 = false;

            string word2 = words.at(1);
            string word3 = words.at(2);
            
            if (word2.at(0) == '"'){
                quoted2 = true;
                word2 = word2.substr(1,word2.size()-2);
            }
            if (word3.at(0) == '"'){
                quoted3 = true;
                word3 = word3.substr(1,word3.size()-2);
            }

            if (word2 == "*"){ // searching entire database
            //cout << "Searching entire database..." << endl;
                for (int row = 0; row < height; row++){
                    for (int col = 0; col < length; col++){
                        if (row == 0){
                            temporary = data.at(0);
                            searchBase.push_back(temporary);
                            break;
                        }
                        else if (data[row][col] == word3){ // if the database finds the object value
                            temporary = data.at(row); // adds the entire row of the found value;
                            searchBase.push_back(temporary);
                            valFound = true;
                            continue; //skip to next line to avoid adding same line twice...
                        }
                    }
                }
                if (valFound){
                    for (int row = 0; row < searchBase.size(); row++){ // prints the found rows
                        for (int col = 0; col < length; col++){
                            cout << setw(colWidth) << searchBase[row][col];
                        }
                        cout << endl;
                    }
                }
                else{
                    cout << "No results" << endl;
                }
                continue;
            }
            /*
            else if (quoted2 && word2 == "*"){
                //cout << "searching the * col..." << endl;
                for (int col = 0; col < length; col++){ 
                if (word2 == colNames.at(col)){// if a column name is equal to the word
                    searchBase.push_back(colNames);
                    colFound = true;
                    column1 = FindCol(word2, data, height, length); //returns the column, the word passed will not have quotes either way
                    break;
                }
                if (!colFound){
                cout << "Wrong col name..." << endl;
                continue;
                }
                for (int row = 1 ; row < length ; row++){
                    if (column1.at(row) == word3){ // if the value in the column is found...
                    temporary = data.at(row); 
                    valFound = true;
                    searchBase.push_back(temporary);
                    continue;
                    }
                }
                }
                if (valFound && colFound){
                    for (int row = 0; row < searchBase.size(); row++){ // prints the found rows
                        for (int col = 0; col < length; col++){
                            cout << setw(colWidth) << searchBase[row][col];
                        }
                        cout << endl;
                    }
                    continue;
                }
                
                else{
                    cout << "No values found" << endl;
                }
                continue;
            }
            */
            else{
               // cout << "searching specified col..." << endl;
                 for (int col = 0; col < length; col++){ 
                    if (word2 == colNames.at(col)){// if a column name is equal to the word
                        colFound = true;
                        //cout << "FOUND COL: " << colNames.at(col) << endl;
                        searchBase.push_back(colNames);
                        column1 = FindCol(word2, data, height, length); //returns the column, the word passed will not have quotes either way
                        break;
                    }
                }

                if (!colFound){
                //cout << "Wrong col name..." << endl;
                cout << "Invalid command" << endl;
                continue;
                }
                for (int row = 0; row < height; row++){
                    if (column1.at(row) == word3){ // if the current col val is equal to the specified val
                        searchBase.push_back(data.at(row));
                        valFound = true;
                        continue;
                    }
                }
               // cout << "data added to Searchbase..." << endl;
            if (valFound && colFound){
                    for (int row = 0; row < searchBase.size(); row++){ // prints the found rows
                        for (int col = 0; col < length; col++){
                            cout << setw(colWidth) << searchBase[row][col];
                        }
                        cout << endl;
                    }
                }
    
            else{
                    cout << "No results" << endl;
                }
                continue;
            }
    }
    else{
        cout<<"Invalid command" << endl;
        continue;
    }
    }
return 0;
}





vector<string> ReadLine(string line){
    //cout <<"ReadLine executed" << endl;
    vector<string> product;

    bool quoted = false;
    string element;
    char comma = ',';
    char quote = '"';
    line +=",";
    int size =0;
    char letter;
    for (int i = 0; i <line.size();i++){
        letter = line.at(i);
        if (letter == comma && !quoted){//end of an element | separating comma found
            product.push_back(element); // adds element to the returnable row 
            element = ""; // resets the element
            size++;
        }
        else if (letter == quote && !quoted){ //opening quote found
            quoted = true;
            continue;
        }
        else if (line.at(i) == quote && quoted){ //closing quote found
            quoted = false;
        continue;
        }
        else{ // NOT quote, NOT comma

            if (true){ // if quoted anything goes
                element += line.at(i);
            }
            else if (letter != comma && line.at(i) != ' ' && letter != quote){ //if NOT quoted, anything but spaces
                element += line.at(i);
            }
        }
    
    }

    return product;
}
vector <string> FindCol(string colName, vector <vector <string> > data, int height, int length){
    //cout << "FindCol executed" << endl;
    int index;
    vector<string> column(height);
    for (int j = 0; j < length; j ++){//finds the column to traverse
        if (data[0][j] == colName){
            index = j;
        }
    }
    for(int i = 0; i < height; i++){
        column.at(i) = data[i][index];
        // cout << data[i][index] << " | " <<column.at(i-1) << endl; // TESTER!
        
    }
    return column;
}
vector<string> CommandToWords(string commandLine){
    vector<string> entity;
    string word = "";
    bool quoted = false;
    
    char space = ' ';
    char quote = '"';
    char letter;
    int wordCount = 0;
    int strlen = commandLine.size();

    for (int i = 0; i < strlen; i ++){
        letter = commandLine.at(i);
        
        if (letter == quote ){
            if (quoted){ // its a closing quote
                quoted = false;
            }
            else{ 
                quoted = true;
            }

            word += letter;
        }

        else if (letter == space){
            if (quoted){
                word += letter;
            }
            else{ 

                entity.push_back(word);
                word = "";
            
                continue;
            }
        }
        else{
            word += letter;
        }
        }
    entity.push_back(word); // regardless at the end, the last thing entered will be the final world
    return entity;
}


