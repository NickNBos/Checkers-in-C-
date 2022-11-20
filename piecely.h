//
// Created by NBosl on 10/1/2022.
//

#ifndef CHECKERS_IN_CPP_PIECELY_H
#define CHECKERS_IN_CPP_PIECELY_H

#include "shapely.h"

class Piecely : public Shapely {
private:
    // Fields
    prestige rank;
public:
    // Constructors
    Piecely();

    Piecely(char itemShape, double red, double blue, double green, team side);

    Piecely(char shape, Color shapeColor, team side);

    // Getters
    optional<prestige> getRank() const override;
    // Setters
    void rankUp();
};


#endif //CHECKERS_IN_CPP_PIECELY_H
