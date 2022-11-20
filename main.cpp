//
// Created by NBosl on 9/10/2022.
//


#include <fstream>
#include <iostream>
#include <string>
#include "boardly.h"
using namespace std;
/*
 * This program demos the Shapely and Boardly classes through a game of Checkers
 */

// Helper methods with input validation

// Asks the user a yes or no question, returns true or false accordingly
bool askUserToPlay();

// Checks the possible moves for a given piece, taking into account rank, team, and board positions
vector<vector<int>> checkMoves(const vector<Piecely> &checkMe,const Boardly &board);

// Asks the user for coordinates in the Letter-Number form
string askUserCoord();

// Translates letter-number coordinates to an integer for computer reading
int translatePosition(string stringPos);

// Prints out letter-number coordinates when given an integer for user-reading
void translatePosition(int intPos);

// Returns a vector with movement directions when given a vector of possible moves
vector<int> computerMove(vector<vector<int>> movables);

int main() {

    // Initialize overall, unchanging values and random seed
    srand(time(nullptr));
    Shapely emptyBoardShape('-', 1,0,0, neutral);
    Piecely playerPiece('o', Color(0, 0.5, 1), self);
    Piecely opponentPiece('x', playerPiece.getShapeColor().inverseColor(), opponent);

    int gameWinCount = 0;
    int gameLossCount = 0;

    // Introduce rules
    cout << "Welcome to Checkers!" << endl;
    cout << "This is the meant to be a similar game to actual checkers, though without multi-jumps," << endl;
    cout << "so the rules are similar: each piece can be moved to either of the open front" << endl;
    cout << "diagonals, or it can jump over an opposing piece if the space beyond is empty." << endl;
    cout << "Beware though, if no moves are possible or a player's pieces run out," << endl;
    cout << "the game will end with the movable player marked the winner." << endl;
    cout << "You will be o's, and your opponent will be x's" << endl;

    cout << endl << "Would you like to continue your last record?";

    ifstream inFile("../Record.txt");
    if(askUserToPlay() && inFile) {
        inFile >> gameWinCount;
        inFile >> gameLossCount;
        cout << "Currently " << gameWinCount << " wins and " << gameLossCount << " losses" << endl;
    }
    inFile.close();

    cout << endl << "Would you like to play?";

    // Primary Game Loop
    while(askUserToPlay()) {
        // Initialize game variables
        bool playerCanMove = true;
        bool opponentCanMove = true;
        bool cancel;
        bool validInput;
        int jumpedPosition;
        int placementTemp;
        int lastMovedIndex;
        string lastMovedFrom;
        string lastMovedTo;
        int lastMovedFromInt;
        int lastMovedToInt;

        vector<Piecely> playerPieceVec;
        vector<Piecely> opponentPieceVec;
        vector<vector<int>> playerMovables;
        vector<vector<int>> opponentMovables;
        vector<int> opponentMove;

        Boardly checkerBoard;
        // Fill in the game board
        // Place opponent pieces on top of board
        for(int i = 1; i < 13; i++) {
            placementTemp = 2*i-1;
            if(placementTemp/8%2 == 1)
                placementTemp--;
            opponentPieceVec.push_back(Piecely(opponentPiece));
            opponentPieceVec[i-1].setPosition(placementTemp);
            checkerBoard.setPiece(placementTemp/8,placementTemp%8,opponentPiece);
        }
        // Place player pieces on bottom of board
        for(int i = 1; i < 13; i++) {
            placementTemp = (2*i-2) + 40;
            if(placementTemp/8%2 == 0)
                placementTemp++;
            playerPieceVec.push_back(Piecely(playerPiece));
            playerPieceVec[i-1].setPosition(placementTemp);
            checkerBoard.setPiece(placementTemp/8,placementTemp%8,playerPiece);
        }

        // Actual game loop

        while(playerCanMove && opponentCanMove) {
            // Print Board For Player Phase
            cout << checkerBoard;

            // Second line is the original while loop

            // Player Move Phase
            playerMovables = checkMoves(playerPieceVec, checkerBoard);
            if (playerMovables.empty())
                playerCanMove = false;
            // Get player move input
            cancel = true;
            while(cancel && playerCanMove) {
                // Ask Which Piece to move
                validInput = false;
                while(!validInput) {
                    validInput = false;
                    cout <<"Select one of your movable pieces to move!"<< endl;
                    lastMovedFrom = askUserCoord();
                    for (int i = 0; i < playerMovables.size(); i++)
                        if (translatePosition(lastMovedFrom) == playerMovables[i][4]) {
                            lastMovedIndex = i;
                            validInput = true;
                        }
                }
                // Ask if user is sure, display moves
                cout << "You can move to ";
                for(int i = 0; i < 4; i ++) {
                    if(playerMovables[lastMovedIndex][i] != -1) {
                        translatePosition(playerMovables[lastMovedIndex][i]);
                        cout << " ";
                    }
                }

                cout << endl << "Are you sure you want to move " << lastMovedFrom << "?";
                cancel = !askUserToPlay();

                // Ask where to move
                validInput = false;
                while(!cancel && !validInput) {
                    validInput = false;
                    cout << "Select a valid place to move to!" << endl;
                    lastMovedTo = askUserCoord();
                    for(int i = 0; i < 4; i++) {
                        if (translatePosition(lastMovedTo) == playerMovables[lastMovedIndex][i]) {
                            validInput = true;
                        }
                    }
                }
            }
            // Update Board
            // Remove piece from where it was
            lastMovedFromInt = translatePosition(lastMovedFrom);
            checkerBoard.setShape(lastMovedFromInt/8, lastMovedFromInt%8, emptyBoardShape);
            // Put piece where it should be
            lastMovedToInt = translatePosition(lastMovedTo);
            // Update piece position
            checkerBoard.getShape(lastMovedToInt/8, lastMovedToInt%8).setPosition(lastMovedToInt);
            for(int i = 0; i < playerPieceVec.size(); i++)
                if(lastMovedFromInt == playerPieceVec[i].getPosition()) {
                    if((lastMovedToInt - 1)/8 == 0) {
                        playerPieceVec[i].rankUp();
                        }
                    playerPieceVec[i].setPosition(lastMovedToInt);
                    checkerBoard.setPiece(lastMovedToInt / 8, lastMovedToInt % 8, playerPieceVec[i]);
                }
            // Check if the play was a jump, remove enemy piece if so
            if(abs(lastMovedFromInt - lastMovedToInt) > 10) {
                jumpedPosition = (lastMovedToInt + lastMovedFromInt) / 2;
                for(int i = 0; i < opponentPieceVec.size(); i++) {
                    if(opponentPieceVec[i].getPosition() == (lastMovedToInt + lastMovedFromInt)/2) {
                        opponentPieceVec.erase(opponentPieceVec.begin()+i);
                        checkerBoard.setShape(jumpedPosition / 8, jumpedPosition % 8, emptyBoardShape);
                        i = opponentPieceVec.size();
                    }
                }
            }

            // Print Board for Opponent Phase
            cout << checkerBoard;

            // Opponent Move Phase
            opponentMovables = checkMoves(opponentPieceVec, checkerBoard);
            if (opponentMovables.empty())
                opponentCanMove = false;
            if (opponentCanMove) {
                opponentMove = computerMove(opponentMovables);
                lastMovedFromInt = opponentMove[0];
                lastMovedToInt = opponentMove[1];

                // Remove piece from where it was
                checkerBoard.setShape(lastMovedFromInt/8, lastMovedFromInt%8, emptyBoardShape);
                // Update piece position
                checkerBoard.getShape(lastMovedToInt/8, lastMovedToInt%8).setPosition(lastMovedToInt);
                for(int i = 0; i < opponentPieceVec.size(); i++)
                    if(lastMovedFromInt == opponentPieceVec[i].getPosition()) {
                        if((lastMovedToInt + 1) / 8 >= 7) {
                            opponentPieceVec[i].rankUp();
                        }
                        opponentPieceVec[i].setPosition(lastMovedToInt);
                        // Put piece where it should be
                        checkerBoard.setPiece(lastMovedToInt/8, lastMovedToInt%8, opponentPieceVec[i]);
                    }
            }
            // Check if the play was a jump, remove player piece if so
            if(abs(lastMovedFromInt - lastMovedToInt) > 10) {
                jumpedPosition = (lastMovedToInt + lastMovedFromInt) / 2;
                for(int i = 0; i < playerPieceVec.size(); i++) {
                    if(playerPieceVec[i].getPosition() == (lastMovedToInt + lastMovedFromInt)/2) {
                        playerPieceVec.erase(playerPieceVec.begin() + i);
                        checkerBoard.setShape(jumpedPosition / 8, jumpedPosition % 8, emptyBoardShape);
                        i = playerPieceVec.size();
                    }
                }
            }

        }


    if(playerCanMove) {
        cout << endl << "Nice Win!" << endl;
        gameWinCount++;
    }
    else {
        cout << endl << "A loss! Argh!" << endl;
        gameLossCount++;
    }
    cout << "Would you like to play again?";
}

    if(gameWinCount + gameLossCount == 0)
        cout << "That's fair";
    else
        cout << "That's " << gameWinCount << " wins, and " << gameLossCount << " losses!" << endl;

    cout << endl << "Would you like to save these results?";

    ofstream outFile("../Record.txt");
    if(askUserToPlay()) {
        outFile << gameWinCount << endl << gameLossCount;
    }
    outFile.close();

    cout << "See ya!";
    return 0;
}


// Ask the user for a yes/no question
bool askUserToPlay(){
    string inputLine;
    while(true){
        cout << endl << "Enter (y)es or (n)o:";
        getline(cin, inputLine);
        if(inputLine == "yes" || inputLine == "YES" || inputLine == "Yes" || inputLine == "y" || inputLine == "Y" ) {
            return true;
        }
        else if(inputLine == "no" || inputLine == "NO" || inputLine == "No" || inputLine == "n" || inputLine == "N" ){
            return false;
        }
        else
            cout << "Sorry, invalid input";
    }
}

// The output here is a vector of vectors: in each, the first is the left side move, the second is the right side,
// and the third is the movable piece
vector<vector<int>> checkMoves(const vector<Piecely>& checkMe, const Boardly& board ){
    vector<vector<int>> movables;
    vector<int> tempVec;
    int positionHolder;
    int rowHolder;
    int colHolder;
    for(int i = 0; i < checkMe.size(); i++) {
        // Player Checker
        if (checkMe[i].getShapeTeam() == self || checkMe[i].getRank() == 1) {
                positionHolder = checkMe[i].getPosition();
                // Check up left
                rowHolder = (positionHolder - 9) / 8;
                colHolder = (positionHolder - 9) % 8;
                if (positionHolder - 9 > 0) {
                    // Check if move available
                    if (board.getShape(rowHolder, colHolder).getShapeTeam() == neutral)
                        tempVec.push_back(positionHolder - 9);
                    // Check if jump available
                    else if (board.getShape(rowHolder, colHolder).getShapeTeam() != checkMe[0].getShapeTeam()
                                            && positionHolder - 18 > 0 && board.getShape(rowHolder, colHolder).getShapeTeam() != forbidden
                                            && rowHolder > 0 && colHolder > 0) {
                        rowHolder -= 1; // Check up-er 1
                        colHolder -= 1; // Check left-er 1
                        if(board.getShape(rowHolder, colHolder).getShapeTeam() == neutral)
                            tempVec.push_back(positionHolder-18);
                        else
                            tempVec.push_back(-1);
                    }
                    else
                        tempVec.push_back(-1);
                } else
                    tempVec.push_back(-1);
                // Check up right
                rowHolder = (positionHolder - 7) / 8;
                colHolder = (positionHolder - 7) % 8;
                if (positionHolder - 7 > 0) {
                    if (board.getShape(rowHolder, colHolder).getShapeTeam() == neutral)
                        tempVec.push_back(positionHolder - 7);
                    // Check if jump available
                    else if (board.getShape(rowHolder, colHolder).getShapeTeam() != checkMe[0].getShapeTeam()
                                            && positionHolder - 14 > 0 && board.getShape(rowHolder, colHolder).getShapeTeam() != forbidden
                                            && rowHolder > 0 && colHolder < 7) {
                        rowHolder -= 1; // Check up-er 1
                        colHolder += 1; // Check right-er 1
                        if(board.getShape(rowHolder, colHolder).getShapeTeam() == neutral)
                            tempVec.push_back(positionHolder-14);
                        else
                            tempVec.push_back(-1);
                    }
                    else
                        tempVec.push_back(-1);
                } else
                    tempVec.push_back(-1);
            }
        else {
            tempVec.push_back(-1);
            tempVec.push_back(-1);
        }
            // Computer Checker
        if(checkMe[i].getShapeTeam() == opponent || checkMe[i].getRank() == 1) {
            positionHolder = checkMe[i].getPosition();

            // Check down left
            rowHolder = (positionHolder + 7) / 8;
            colHolder = (positionHolder + 7) % 8;
            if (positionHolder + 7 < 63) {
                if (board.getShape(rowHolder, colHolder).getShapeTeam() == neutral)
                    tempVec.push_back(positionHolder + 7);
                    // Check if jump available
                else if (board.getShape(rowHolder, colHolder).getShapeTeam() != checkMe[0].getShapeTeam()
                         && positionHolder + 14 > 0 && board.getShape(rowHolder, colHolder).getShapeTeam() != forbidden
                         && rowHolder < 7 && colHolder > 0) {
                    rowHolder += 1; // Check down-er 1
                    colHolder -= 1; // Check left-er 1
                    if(board.getShape(rowHolder, colHolder).getShapeTeam() == neutral)
                        tempVec.push_back(positionHolder+14);
                    else
                        tempVec.push_back(-1);
                }
                else
                    tempVec.push_back(-1);
            } else
                tempVec.push_back(-1);

            // Check down right
            rowHolder = (positionHolder + 9) / 8;
            colHolder = (positionHolder + 9) % 8;
            if (positionHolder + 9 < 63) {
                if (board.getShape(rowHolder, colHolder).getShapeTeam() == neutral)
                    tempVec.push_back(positionHolder + 9);
                    // Check if jump available
                else if (board.getShape(rowHolder, colHolder).getShapeTeam() != checkMe[0].getShapeTeam()
                         && positionHolder + 18 < 63 && board.getShape(rowHolder, colHolder).getShapeTeam() != forbidden
                         && rowHolder < 7 && colHolder < 7) {
                    rowHolder += 1; // Check down-er 1
                    colHolder += 1; // Check right-er 1
                    if(board.getShape(rowHolder, colHolder).getShapeTeam() == neutral)
                        tempVec.push_back(positionHolder + 18);
                    else
                        tempVec.push_back(-1);
                }
                else
                    tempVec.push_back(-1);
            } else
                tempVec.push_back(-1);

        }
        else {
            tempVec.push_back(-1);
            tempVec.push_back(-1);
        }
        if (tempVec[0] != -1 || tempVec[1] != -1 || tempVec[2] != -1 || tempVec[3] != -1) {
            tempVec.push_back(checkMe[i].getPosition());
            movables.push_back(tempVec);
        }
        tempVec.clear();
    }
    return movables;
}

// Ask the user for an in-bound input of coordinates
string askUserCoord() {
    string inputLine;
    while (true) {
        cout << "Input coordinates in form between A1 and H8:";
        getline(cin, inputLine);

        if (inputLine.length() != 2) {
            cout << "Sorry, invalid input" << endl;
        } else if (inputLine[0] >= 'a' && inputLine[0] <= 'h' && inputLine[1] >= '1' && inputLine[1] <= '8') {
            inputLine[0] = inputLine[0] - 'a' + 'A';
            return inputLine;
        } else if (inputLine[0] >= 'A' && inputLine[0] <= 'H' && inputLine[1] >= '1' && inputLine[1] <= '8') {
            return inputLine;
        } else
            cout << "The first digit must be between A and H, and the second must be between 1 and 8" << endl;
    }
}

// Translate the letter-number position to a 0-63 value
int translatePosition(string stringPos){
    return int(stringPos[0]-'A')*8 + int(stringPos[1]-'1');
}

// Print the 0-63 position as a user readable letter-number
void translatePosition(int intPos){
    string stringPos;
    char firstDigit = char('A' + (intPos/8));
    char secondDigit = char(intPos%8+49);
    cout << firstDigit << secondDigit;
}

// The output here is a vector containing which positions the computer will move from and to
vector<int> computerMove(vector<vector<int>> movables){
    // TODO: Reimplement opportunity index to always jump when possible
    vector<int> fromAndTo;
    vector<int> moveOptions;
    int moveFromIndex = rand() % movables.size();
    fromAndTo.push_back(movables[moveFromIndex][4]);


    for(int i = 0; i < 4; i++) {
        if(movables[moveFromIndex][i] != -1) {
            moveOptions.push_back(i);
        }
    }

    int moveToIndex = moveOptions[rand() % moveOptions.size()];
    fromAndTo.push_back(movables[moveFromIndex][moveToIndex]);
    return fromAndTo;
}

