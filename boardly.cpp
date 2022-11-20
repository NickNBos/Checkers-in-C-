//
// Created by NBosl on 9/10/2022.
//

#include "boardly.h"

// Default (Sole) Constructor
Boardly::Boardly() {
    size = 8;
    vector<unique_ptr<Shapely>> tempVec;
    // Establish the primary color
    Color mainColor;
    mainColor.red = 1;
    mainColor.blue = 0;
    mainColor.green = 0;
    int alternator = 0;
    // Fill out the Boardly in a checkerboard pattern
    for(int row = 0; row < size; row++){
        for(int col = 0; col < size; col++){
            if(alternator == 0) {
                tempVec.push_back(make_unique<Shapely>(Shapely('~',mainColor, forbidden)));
            }
            else
                tempVec.push_back(make_unique<Shapely>(Shapely('-', mainColor, neutral)));
            alternator = (alternator+1)%2;
        }
        shapes.push_back(move(tempVec));
        alternator = (alternator+1)%2;
        tempVec.clear();
    }
}
// Getter of the boards size
int Boardly::getSize() const{
    return size;
}

// 'Getter' that retrieves a Shapely at a given index
Shapely& Boardly::getShape(int row,int col) const{
    return *shapes[row][col];
}
// 'Setter' that sets a given Shapely at a given index
void Boardly::setShape(int row, int col, Shapely &newShape) {
    shapes[row][col] = make_unique<Shapely>(newShape);
}
void Boardly::setPiece(int row, int col, Piecely &newPiece) {
    shapes[row][col] = make_unique<Piecely>(newPiece);
}

// Overloaded ostream operator, properly prints Boardly
ostream& operator << (ostream& outs, const Boardly& board){
    // Space for the top left corner
    cout << "   ";
    // Put the numbers on top
    for(int col = 0; col < board.getSize(); col++) {
        cout << setw(3) << col+1;
    }
    cout << endl;
    // Fill out the bulk of the board
    for(int row = 0; row < board.getSize(); row++){
        // Put the letters on the left
        cout << setw(3) << char('A'+row);
        // Put the Shapelys in the bulk
        for(int col = 0; col < board.getSize(); col++) {
            outs << " " << board.getShape(row,col);
        }
        cout << endl;
    }

    return outs;
}

