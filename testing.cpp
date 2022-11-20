//
// Created by NBosl on 10/6/2022.
//

#include <iostream>
#include <string>
#include "boardly.h"

using namespace std;

bool testBoardly();
bool testShapely();
bool testPiecely();

int main() {
    if(testBoardly()) {
        cout << "Passed all Boardly test cases" << endl;
    }
    if(testShapely()) {
        cout << "Passed all Shapely test cases" << endl;
    }
    if(testPiecely()) {
        cout << "Passed all Piecely test cases" << endl;
    }
    return 0;
}

bool testBoardly() {
    bool passed = true;
    Boardly b;
    Piecely p;

    b.setPiece(0,0,p);
    b.setShape(0,1,p);

    if(b.getShape(0,0).getRank() != normal) {
        cout << "FAILED polymorphism test case" << endl;
        passed = false;
    }
    if(b.getShape(0,1).getRank() != nullopt) {
        cout << "FAILED conversion test case" << endl;
        passed = false;
    }

    if(b.getSize() != 8) {
        cout << "FAILED size test case" << endl;
        passed = false;
    }

    return passed;
}

bool testShapely() {
    bool passed = true;
    Shapely shape;
    Color color(1,.25,.75);
    Shapely shape2('c', color, neutral);
    Color inverseColor1(0,.75,.25);
    Color inverseColor2 = color.inverseColor();
    if(!(shape == shape2)) {
        cout << "FAILED comparison of team test case" << endl;
        passed = false;
    }
    if(inverseColor1.red != inverseColor2.red && inverseColor1.blue != inverseColor2.blue && inverseColor1.green != inverseColor2.green) {
        cout << "FAILED inverseColor enum test case" << endl;
        passed = false;
    }
    shape2.inverseColor();
    if(shape2.getShapeColor().red != inverseColor1.red && shape2.getShapeColor().blue != inverseColor1.blue && shape2.getShapeColor().green != inverseColor1.green) {
        cout << "FAILED inverseColor Shapely method test case" << endl;
        passed = false;
    }

    if(shape2.getItemShape() != 'c') {
        cout << "FAILED custom character test case" << endl;
        passed = false;
    }

    shape2.reShape(shape);
    if(shape2.getItemShape() != '.') {
        cout << "FAILED reShape to default test case" << endl;
        passed = false;
    }

    shape2.setPosition(5);
    if(shape2.getPosition() != 5) {
        cout << "FAILED set position test case" << endl;
        passed = false;
    }

    return passed;
}
bool testPiecely() {
    bool passed = true;
    vector<unique_ptr<Shapely>> pieces;
    Piecely p;
    Shapely s;
    pieces.push_back(make_unique<Piecely>(p));
    pieces.push_back(make_unique<Shapely>(s));

    if(pieces[1]->getRank()) {
        cout << "FAILED getRank from Shapely test case" << endl;
        passed = false;
    }

    if(pieces[0]->getRank() != normal) {
        cout << "FAILED getRank thru polymorphism test case" << endl;
        passed = false;
    }

    p.rankUp();
    pieces.push_back(make_unique<Piecely>(p));
    if(pieces[2]->getRank() != king) {
        cout << "FAILED rank up test case" << endl;
        passed = false;
    }

    return passed;
}