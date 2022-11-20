//
// Created by NBosl on 9/10/2022.
//

#ifndef CHECKERS_IN_CPP_SHAPELY_H
#define CHECKERS_IN_CPP_SHAPELY_H

#include <iomanip>
#include <iostream>
#include <optional>
#include <string>
using namespace std;

struct Color{
    // Primary colors, 0-1
    double red;
    double blue;
    double green;

    // Constructors
    // Default
    Color() {
        red = 0;
        blue = 0;
        green = 0;
    }
    // All parameters
    Color(double r, double b, double g){
        if(r > 1.0)
            red = 1.0;
        else if(r < 0.0)
            red = 0;
        else
            red = r;

        if(b > 1.0)
            blue = 1.0;
        else if(b < 0.0)
            blue = 0;
        else
            blue = b;

        if(g > 1.0)
            green = 1.0;
        else if(g < 0.0)
            green = 0;
        else
            green = g;
    }
    // Gives the inverse of the color
    Color inverseColor() const{
        return Color(1 - red,1 - blue,1 - green);
    }
};

// Enum type that dictates the team of a given Shapely
enum team{self, opponent, neutral, forbidden};
// Enum type for use in Piecely
enum prestige{normal, king};
class Shapely {
private:
    // Fields
    char itemShape;
    team shapeTeam;
    Color shapeColor;
    int position;
protected:
    // Method to change itemShape when prestiged in child class
    void setItemShape(char newItemShape);
public:
    // Constructors
    Shapely(char itemShape, double red, double blue, double green, team side);
    Shapely(char shape, Color shapeColor, team side);
    Shapely();
    // Virtual Destructor
    virtual ~Shapely() = default;
    // Getters
    char getItemShape() const;
    team getShapeTeam() const;
    Color getShapeColor() const;
    int getPosition() const;
    // Getter for overriding that always returns nullopt - circumvents abstraction
    virtual optional<prestige> getRank() const;
    // Setters
    void setPosition(int givenPosition);
    // Extras
    void reShape(Shapely newShape);
    void inverseColor();
    // Overloaded ops
    friend ostream& operator << (ostream& outs, const Shapely& shape);
    friend bool operator == (const Shapely& shape, const Shapely& otherShape);
};

#endif //CHECKERS_IN_CPP_SHAPELY_H
