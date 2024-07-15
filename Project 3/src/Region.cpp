#include "Headers\Region.h"

Region::Region() {
    ideal_index = 0.0
}

void Region::calculateIdealIndex() {
    ideal_index = 1.0;
}

double Region::getIdealIndex() {
    return ideal_index;
}