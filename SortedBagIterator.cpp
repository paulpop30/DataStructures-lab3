#include "SortedBagIterator.h"
#include "SortedBag.h"
#include <exception>

using namespace std;

SortedBagIterator::SortedBagIterator(const SortedBag& b) : bag(b) {
    this->current = this->bag.slla->head;
    this->freq = this->bag.slla->elems[current].frequency;;

}//BC=WC=TOTAL=Theta(1)

TComp SortedBagIterator::getCurrent() {

    if (this->valid())
        return this->bag.slla->elems[current].element;
    else
        throw exception();
}//BC=WC=TOTAL=Theta(1)

bool SortedBagIterator::valid() const {
    if (this->current == -1)
        return false;
    return true;

}//BC=WC=TOTAL=Theta(1)

void SortedBagIterator::next() {
    if (this->valid())
    {
        this->freq--;
        if (this->freq == 0) {
            this->freq = this->bag.slla->elems[current].frequency;
            this->current = this->bag.slla->next[this->current];

        }
    }
    else
        throw exception();
}
//BC=WC=TOTAL=Theta(1)

void SortedBagIterator::first() {
    this->current = this->bag.slla->head;
    this->freq = this->bag.slla->elems[current].frequency;
}
//BC=WC=TOTAL=Theta(1)
