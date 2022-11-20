//
// Created by NBosl on 10/1/2022.
//

#include "piecely.h"

// Constructors
Piecely::Piecely() : Shapely(), rank(normal) {
}

Piecely::Piecely(char itemShape, double red, double blue, double green, team side) : Shapely(itemShape, red, blue, green, side), rank(normal){
}

Piecely::Piecely(char shape, Color shapeColor, team side) : Shapely(shape, shapeColor, side), rank(normal) {
}

// Getter
optional<prestige> Piecely::getRank() const{
    return rank;
}

// Extra/Setter, ranks up and shifts to uppercase
void Piecely::rankUp() {
    if(rank == king)
        return;
    rank = king;
    setItemShape(char(getItemShape()+'A' - 'a'));
}
