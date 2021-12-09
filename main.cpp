#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <queue>

using namespace std;

double DBL_MAX = 999999999;


void floyd(vector<vector<double>> g, vector<string> names)
{
    int n = g.size();
    //creates container with same size and value as graph to store distance
    vector<vector<double>>path (n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (g.at(i).at(j) == 0 && i != j)
            {
                path.at(i).push_back(DBL_MAX);
            }
            else
            {
                path.at(i).push_back(g.at(i).at(j));
            }
        }
    }

    //container to store shortest paths
    vector<vector<int>> calc(n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (path.at(i).at(j) == DBL_MAX)
            {
                calc.at(i).push_back(-1);
            }
            else
            {
                if(i != j)
                    calc.at(i).push_back(i);
                else
                    calc.at(i).push_back(0);
            }
        }
    }

    //floyd-warshall algorithm
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                if (path.at(j).at(k) > path.at(j).at(i) + path.at(i).at(k))
                {
                    path.at(j).at(k) = (path.at(j).at(i) + path.at(i).at(k));
                    calc.at(j).at(k) = calc.at(i).at(k);
                }

            }
        }
    }

    //print results

    string from = names.at(0);
    int toIndex = names.size() - 1;
    string to = names.at(toIndex);

    vector<int> temp;
    int ind = calc.at(0).at(toIndex);
    while (ind != 0)
    {
        temp.push_back(ind);
        ind = calc.at(0).at(ind);
    }
    cout << "The train will start in " << from << ".\n";
    cout << "It will stop at " << temp.size() << " station(s).\n";
    if (temp.size() > 0)
    {
        cout << "The stops will be: \n";
        for (int i = temp.size() - 1; i >= 0; i--)
        {
            cout << "\t" << names.at(temp.at(i)) << "\n";
        }
    }
    cout << "The final destination will be " << to << ".\n";
    cout << "Trip distance: " << path.at(0).at(toIndex) << " miles.\n";
}

void dijkstra(vector<vector<double>> g, vector<string> names)
{
    string from = names.at(0);
    int toIndex = names.size() - 1;
    string to = names.at(toIndex);

    int n = g.size();
    int s = 0;
    //visited vertices
    set<int> S;
    //not visited vertices
    set<int> VS;

    for (int i = 0; i < n; i++)
    {
        VS.insert(i);
    }

    vector<double> d(n, DBL_MAX);
    d.at(s) = 0;

    vector<int> p(n, -1);
    p.at(0) = s;

    S.insert(s);
    VS.erase(s);

    //change initial distance
    for (int v : VS)
    {
        p.at(v) = s;
        if (g.at(s).at(v) != 0 || s == v)
        {
            d.at(v) = (g.at(s).at(v));
        }
        else
        {
            d.at(v) = DBL_MAX;
        }
    }

    //go through all non-visited vertices
    //dijkstra algorithm
    while (VS.size() != 0)
    {
        double min = DBL_MAX;
        int u = 0;
        //find next minimum distance
        for (int i : VS)
        {
            if (d[i] <= min)
            {
                min = d[i];
                u = i;
            }
        }
        VS.erase(u);
        S.insert(u);

        //queue stores adjacent vertices
        queue<int> q;
        for (int i = 0; i < n; i++)
        {
            if (g.at(u).at(i) != 0 && u != i)
            {
                q.push(i);
            }
        }
        //go through all adjacent vertices
        while (!q.empty())
        {
            int v = q.front();
            q.pop();
            if (d.at(u) + g.at(u).at(v) < d.at(v))
            {
                d.at(v) = d.at(u) + g.at(u).at(v);
                p.at(v) = u;
            }
        }
    }

    //print results

    vector<int> calc;
    int temp = p.at(toIndex);
    while (temp != 0)
    {
        calc.push_back(temp);
        temp = p.at(temp);
    }
    cout << "The train will start in " << from << ".\n";
    cout << "It will stop at " << calc.size() << " station(s).\n";
    if (calc.size() > 0)
    {
        cout << "The stops will be: \n";
        for (int i = calc.size() - 1; i >= 0; i--)
        {
            cout << "\t" << names.at(calc.at(i)) << "\n";
        }
    }
    cout << "The final destination will be " << to << ".\n";
    cout << "Trip distance: " << d.at(toIndex) << " miles.\n";
}


//Global variable for entering desired menu option
int menuChoice = 0;

//Two vectors, userInput is the vector filled with the user's desired locations
// loc is the vector with all data from .csv file
vector<vector<pair<string, string>>> userInput;
vector<vector<pair<string, string>>> loc;

//Function that returns index of state in loc vector
int getStateIndex(string stateName, vector<vector<pair<string, string>>> &locations)
{
    for(int i = 0; i < locations.size(); i++){
        if(locations.at(i).at(0).second.compare(stateName) == 0){
            return i;
        }
    }
    return -1;
}

pair<string, string> getStatevec(string stateName, vector<vector<pair<string, string>>> &locations)
{
    for (int i = 0; i < locations.size(); i++)
    {
        if (locations.at(i).at(0).second.compare(stateName) == 0)
        {
            return locations.at(i).at(0);
        }
    }
}

pair<string, string> getCity(string cityName, vector<vector<pair<string, string>>> &locations, int statePos)
{
    for(int i = 0; i < locations.at(statePos).size(); i++){
        if(locations.at(statePos).at(i).second.compare(cityName) == 0){
            return locations.at(statePos).at(i);
        }
    }
    pair<string, string> empty = make_pair("", "");
    return empty;
}

void outputStates(vector<vector<pair<string, string>>> &locations){
    cout << "List of States" << endl;
    cout << "************************" << endl;
    for(int i = 0; i < locations.size(); i++){
        cout << locations.at(i).at(0).first << " " << locations.at(i).at(0).second << endl;
    }
    cout << "************************" << endl;
}

//Function that outputs cities based on desired state
void outputCities(vector<vector<pair<string, string>>> &locations, string state){
    int stateCheck = 0;
    for(int i = 0; i < locations.size(); i++){
        if(locations.at(i).at(0).second.compare(state) == 0){
            stateCheck = 1;
            for(int j = 0; j < locations.at(i).size(); j++){
                cout << locations.at(i).at(j).second << endl;
            }
            break;
        }
    }
    if(stateCheck == 0){
        cout << "You entered an invalid state. Please try again." << endl;
    }
}

//Function to output initial menu options
void menuOptions(){
    cout << "1. Start a route" << endl;
    cout << "0. Quit program" << endl;
}

void routeOptions(){
    cout << "1. View state list" << endl;
    cout << "2. View city list" << endl;
    cout << "3. Enter a state and city" << endl;
    cout << "4. Finish location input" << endl;
    cout << "0. Quit program" << endl;
}

//Add location to vector based on routeChoice input
void addLocation(pair<string, string> stateName, pair<string, string> cityName){
    int stateCheck = 0;
    int cityCheck = 0;

    //for loop checks to see if state and/or city is already in list
    for(int i = 0; i < userInput.size(); i++){
        if(stateName.second.compare(userInput.at(i).at(0).second) == 0){
            stateCheck = -1;

            for(int j = 0; j < userInput.at(i).size(); j++){
                if(cityName.second.compare(userInput.at(i).at(j).second) == 0){
                    cityCheck = -1;
                }
            }
            if(cityCheck == 0){
                userInput.at(i).push_back(cityName);
            }
            break;
        }
    }
    if(stateCheck == 0){
        vector<pair<string, string>> input;
        input.push_back(stateName);
        input.push_back(cityName);
        userInput.push_back(input);
    }
}

//Main function for entering desired locations
void routeChoice(vector<vector<pair<string, string>>> &locations)
{
    string stateChoice;
    if(menuChoice == 1){ //Option 1, output list of all states
        outputStates(locations);
    }
    else if(menuChoice == 2){ //Option 2, output list of cities in a desired state
        cout << "Please enter a state you would like to see the cities of: ";
        getline(cin, stateChoice);
        getline(cin, stateChoice);
        cout << endl;
        outputCities(locations, stateChoice);
    }
    else if(menuChoice == 3){ //Option 3, input state and city to be added to userInput vector
        cout << "Please enter the state you would like to add to the route: ";
        getline(cin, stateChoice);
        getline(cin, stateChoice);
        pair<string, string> stateInput = getStatevec(stateChoice, locations); //gets state pair from loc vector and inputs it into a temporary pair
        int stateIndex = getStateIndex(stateChoice, locations); //gets state index to help with searching for city
        if(stateIndex == -1){  //Invalid state entry
            cout << "Invalid state, please try again." << endl;
        }
        else{ //Valid state entry
            cout << "Please enter a city within the state you would like to add." << endl;
            cout << "Enter 2 to see list of cities within state." << endl;
            string gettingCity = "";
            getline(cin, gettingCity);
            if(gettingCity.compare("2") == 0){ //If user inputs 2, show list of cities within city
                outputCities(locations, stateChoice); //Outputs cities list
                cout << "Please enter desired city: " << endl;
                getline(cin, gettingCity); //Input desired city
            }
            pair<string, string> cityInput;
            cityInput = getCity(gettingCity, locations, stateIndex); //Put city pair data into a temporary pair
            if (cityInput.first.compare("") == 0){
                cout << "Invalid city" << endl;
            }
            else{
                addLocation(stateInput, cityInput); //Add location to vector
            }
        }
    }
    else if(menuChoice == 0){
        cout << "Exiting." << endl;

    }
    else if(menuChoice == 4){
        cout << "Compiling locations..." << endl;
    }
    else{
        cout << "Invalid input! Please try again" << endl;
    }
}


//get distance from one place to another
double getDist(string state1, string city1, string state2, string city2){
    ifstream inFile("sf12010placedistance100miles.csv");

    if (state1.compare(state2) == 0){
        if (city1.compare(city2) == 0){
            return 0;
        }
    }

    if (stoi(state1) > stoi(state2)){
        string tempState1 = state2;
        string tempCity1 = city2;
        state2 = state1;
        city2 = city1;
        state1 = tempState1;
        city1 = tempCity1;
    }

    //skip the first line
    string line;
    getline(inFile, line);

    while (getline(inFile, line, '\n')){
        line.erase(remove(line.begin(), line.end(), '\"'), line.end()); //remove all quotations from the line
        string stateFrom, cityFrom, miles, stateTo, cityTo;

        stringstream ss(line);

        getline(ss, stateFrom, ',');

        if (stateFrom == state1){
            getline(ss, cityFrom, ',');

            if (cityFrom == city1){
                getline(ss, miles, ',');
                getline(ss, stateTo, ',');

                if (stateTo == state2){

                    getline(ss, cityTo, '\n');
                    if (cityTo == city2){
                        return stod(miles);
                    }
                }
            }
        }
        else if (stoi(stateFrom) > stoi(state1)){
            return 0;
        }

    }

    return 0;
}


int main() {

    ifstream inFile("sf12010placename.csv");

    //skip the first line
    string line;
    getline(inFile, line);

    while (getline(inFile, line, '\n')){

        line.erase(remove(line.begin(), line.end(), '\"'), line.end()); //remove all quotations from the line

        stringstream ss(line);
        string stateID, stateName, cityID, cityName;

        getline(ss, stateID, ',');
        getline(ss, stateName, ',');
        getline(ss, cityID, ',');
        getline(ss, cityName, '\n');

        pair<string, string> state = make_pair(stateID, stateName);
        pair<string, string> city = make_pair(cityID, cityName);

        bool containsState = false;
        for (int i = 0; i < loc.size(); i++){
            if (loc[i][0] == state){
                containsState = true;
                loc[i].push_back(city);
            }
        }
        if (!containsState){
            vector<pair<string, string>> vec;
            vec.push_back(state);
            vec.push_back(city);
            loc.push_back(vec);
        }


    }


    //Intro to program
    cout << "Welcome to the High Speed National Railway!" << endl << endl;
    cout << "The first city you chose will be the starting location" << endl;
    cout << "and the last city you chose will be the destination." << endl << endl;
    cout << "Stations within 100 miles will be connected, while stations" << endl;
    cout << "greater than 100 miles will not." << endl << endl;
    cout << "Please choose from the following options: " << endl;

    menuOptions();
    cin >> menuChoice;

    //while loop that runs as long as the user still wants to input locations or view location lists
    while(menuChoice != 0){
        routeOptions();
        cin >> menuChoice;
        routeChoice(loc);

        if (menuChoice == 4){
            break;
        }
    }

    if(menuChoice == 0){
        return 0;
    }



    vector<pair<string, string>> temp;
    vector<vector<double>> adjMatrix;
    vector<string> names;



    for (int i = 0; i < userInput.size(); i++){
        for (int j = 1; j < userInput[i].size(); j++){
            pair <string, string> ID = make_pair(userInput.at(i)[0].first, userInput.at(i)[j].first);
            temp.push_back(ID);
            names.push_back(userInput[i][j].second);
        }
    }

    for (int i = 0; i < temp.size(); i++){
        cout << temp[i].first << " " << temp[i].second << endl;
    }


    for (int i = 0; i < temp.size(); i++){
        vector<double> initialize;
        for (int j = 0; j < temp.size(); j++){
            initialize.push_back(0);
        }
        adjMatrix.push_back(initialize);
    }



    for (int i = 0; i < temp.size(); i++){
        for (int j = i; j < temp.size(); j++){
            if (i == j){
                adjMatrix[i][j] = 0;
                continue;
            }
            double dist = getDist(temp[i].first, temp[i].second, temp[j].first, temp[j].second);
            adjMatrix[i][j] = dist;
            adjMatrix[j][i] = dist;
        }
    }




    //printing to test
    /*

    for (int i = 0; i < temp.size(); i++){
        for (int j = 0; j < temp.size(); j++){
            cout << "dist [" << i << "][" << j << "]: " << adjMatrix[i][j] << endl;
        }
    }

    for (int i = 0; i < names.size(); i++){

        cout << "name[" << i << "]: " << names[i] << endl;

    }
     */


    cout << "outputting dijkstra's" << endl;
    dijkstra(adjMatrix, names);

    cout << endl << endl << endl;
    cout << "outputting floyd's" << endl;
    floyd(adjMatrix, names);



    return 0;
}