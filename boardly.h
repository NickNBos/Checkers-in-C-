//
// Created by NBosl on 9/10/2022.
//

#ifndef CHECKERS_IN_CPP_BOARDLY_H
#define CHECKERS_IN_CPP_BOARDLY_H

#include "piecely.h"
#include "shapely.h"
#include <vector>
#include <memory>

using namespace std;

class Boardly {
private:
    // Fields
    vector<vector<unique_ptr<Shapely>>> shapes;
    int size;

public:
    Boardly();
    // Getter
    int getSize() const;
    // Pseudo-getter
    Shapely& getShape(int row,int col) const;
    // Pseudo-setter
    void setShape(int row, int col, Shapely &newShape);
    void setPiece(int row, int col, Piecely &newPiece);
    // Overloaded ostream op
    friend ostream& operator << (ostream& outs, const Boardly& board);
};

#endif //CHECKERS_IN_CPP_BOARDLY_H
