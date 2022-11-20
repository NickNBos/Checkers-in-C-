//
// Created by NBosl on 9/10/2022.
//

#include "shapely.h"

// setItemShape for child class
void Shapely::setItemShape(char newItemShape) {
    itemShape = newItemShape;
}
// Constructor with parts of Color given
Shapely::Shapely(char shape, double red, double blue, double green, team side) : itemShape(shape), shapeTeam(side), shapeColor(red, blue, green), position(0) {
};
// Constructor with Color given
Shapely::Shapely(char shape, Color otherColor, team side) : itemShape(shape), shapeTeam(side), shapeColor(otherColor), position(0) {

};
// Default Constructor
Shapely::Shapely() : itemShape('.'), shapeTeam(neutral), shapeColor(Color()), position(0) {

};
// Getters
char Shapely::getItemShape() const{
    return itemShape;
};

team Shapely::getShapeTeam() const{
    return shapeTeam;
}

Color Shapely::getShapeColor() const{
    return shapeColor;
};

int Shapely::getPosition() const {
    return position;
}

optional<prestige> Shapely::getRank() const {
    return nullopt;
}
// Setter
void Shapely::setPosition(int givenPosition) {
    position = givenPosition;
}
// Change the Shapely into a given Shapely
void Shapely::reShape(Shapely newShape){
    this->itemShape = newShape.getItemShape();
    this->shapeColor = newShape.getShapeColor();
    this->shapeTeam = newShape.getShapeTeam();
    this->position = newShape.getPosition();
}
// Inverse the color of the Shapely
void Shapely::inverseColor() {
    shapeColor = shapeColor.inverseColor();
}
// ostream op overload
ostream& operator << (ostream& outs, const Shapely& shape){
    outs << " " << shape.getItemShape();
    return outs;
}
// == op overload, Shapely's are equal if their team is the same
bool operator == (const Shapely& shape, const Shapely& otherShape){
    if(shape.getShapeTeam() == otherShape.getShapeTeam())
        return true;
    return false;
};