#include "SortedBag.h"
#include "SortedBagIterator.h"
#include <exception>

SortedBag::SortedBag(Relation r) {
    this->rel = r;
    this->length = 0;
    this->slla = new SLLA;
    this->slla->capacity = 1;
    this->slla->elems = new Pair[1];
    this->slla->next = new int[1];
    this->slla->next[0] = -1;
    this->slla->head = -1;
    this->slla->first_empty = 0;

}
//Theta(1)

void SortedBag::resize() {
    Pair* new_elems = new Pair[2 * this->slla->capacity];
    int* new_next = new int[2 * this->slla->capacity];

    for (int i = 0;i < this->slla->capacity; i++) {
        new_elems[i] = this->slla->elems[i];
        new_next[i] = this->slla->next[i];
    }
    for (int i = this->slla->capacity; i < this->slla->capacity * 2 - 1; i++)
        new_next[i] = i + 1;
    new_next[this->slla->capacity * 2 - 1] = -1;

    delete[] this->slla->elems;
    delete[] this->slla->next;

    this->slla->elems = new_elems;
    this->slla->next = new_next;
    this->slla->first_empty = this->slla->capacity;
    this->slla->capacity = this->slla->capacity * 2;

}//BC=WC=TOTAL=Theta(n)


void SortedBag::add(TComp e) {
    Pair new_elem{};
    new_elem.frequency = 1;
    new_elem.element = e;
    if (this->slla->head == -1) //insert when slla is empty
    {
        this->slla->elems[0] = new_elem;
        this->slla->next[0] = this->slla->head;
        this->slla->head = 0;
        this->slla->first_empty = this->slla->next[this->slla->first_empty];
        this->length++;
        return;
    }
    else if (this->rel(e, this->slla->elems[this->slla->head].element)) //insert on the first position when slla not empty
    {
        if (this->slla->elems[this->slla->head].element == e)//if it s equal to the elem on the first pos
        {
            this->slla->elems[this->slla->head].frequency++;//increase frequency
            this->length++;
            return;
        }
        if (this->slla->first_empty == -1)
            this->resize();


        int new_position = this->slla->first_empty;
        this->slla->elems[new_position] = new_elem;
        this->slla->first_empty = this->slla->next[this->slla->first_empty];
        this->slla->next[new_position] = this->slla->head;
        this->slla->head = new_position;

        this->length++;
        return;
    }
    else
    {
        //search for the position where the new element should be
        int current = this->slla->head;
        while (this->rel(this->slla->elems[this->slla->next[current]].element, e) && this->slla->next[current] != -1)
            current = this->slla->next[current];
        //The new element is already in the bag and we need to update the frequency
        if (this->slla->elems[current].element == e)
        {
            this->slla->elems[current].frequency++;
            this->length++;
            return;
        }

        //The new element is not in the bag so we have to add it after the current elem
        if (this->slla->first_empty == -1)
            this->resize();
        int new_position = this->slla->first_empty;
        int next_position = this->slla->next[current];
        this->slla->elems[new_position] = new_elem;
        this->slla->first_empty = this->slla->next[this->slla->first_empty];
        this->slla->next[new_position] = next_position;
        this->slla->next[current] = new_position;
        this->length++;


    }

}
//BC- Theta(1), when we don t have to resize or search for the position
//WC-Theta(n), when we do have to resize in order to add the element
//Total-O(n)

bool SortedBag::remove(TComp e) {
    int current_node = this->slla->head;
    int prev_node = -1;
    while (current_node != -1 && this->slla->elems[current_node].element != e)
    {
        prev_node = current_node;
        current_node = this->slla->next[current_node];
    }
    if (current_node != -1) //if we found the element
    {
        if (this->slla->elems[current_node].frequency > 1) //if the element has a frequency bigger than 1
            this->slla->elems[current_node].frequency--;
        else
        {
            if (current_node == this->slla->head)
                this->slla->head = this->slla->next[slla->head];
            else
                this->slla->next[prev_node] = this->slla->next[current_node];
            this->slla->next[current_node] = this->slla->first_empty;
            this->slla->first_empty = current_node;

        }
        this->length--;
        return true;
    }
    return false; //the element doesn't exist
}
//BC-Theta(1)- the element to be deleted is on the first position
//WC-Theta(nr of pairs)- the element to be deleted is not in the bag
//TC-O(nr of pairs)


bool SortedBag::search(TComp elem) const {
    int current = this->slla->head;
    while (current != -1 && this->slla->elems[current].element != elem)
        current = this->slla->next[current];
    if (current != -1)
        return true;
    return false;
}
//BC-Theta(1)- the searched element is on the first position
//WC-Theta(nr of pairs)- the searched element is not in the bag
//TC-O(nr of pairs)


//adds nr occurrences of elem into the SortedBag
// throws an exception if nr is negative
void SortedBag::addOccurences(int nr, TComp elem) {
    if (nr < 0)
        throw std::exception();
    else {
        for (int i = 0; i < nr;i++) {
            this->add(elem);
        }
    }

}//BC- Theta(nr), when we don t have to resize or search for the position
//WC-Theta(nr^2 + nr*size of the bag), when we do have to resize in order to add the element
//Total-O(nr * capacity)

//WC-Theta(nr * capacity), when we do have to resize in order to add the element



int SortedBag::nrOccurrences(TComp elem) const {


    int current = this->slla->head;
    while (current != -1 && this->slla->elems[current].element != elem)
        current = this->slla->next[current];

    if (current != -1)
        return this->slla->elems[current].frequency;
    return 0;

}//BC-Theta(1)- the searched element is on the first position
//WC-Theta(nr of pairs)- the searched element is not in the bag
//TC-O(nr of pairs)

int SortedBag::size() const {
    return this->length;
}
//BC=WC=TOTAL=Theta(1)


bool SortedBag::isEmpty() const {
    if (this->length == 0)
        return true;
    return false;
}
//BC=WC=TOTAL=Theta(1)

SortedBagIterator SortedBag::iterator() const {
    return SortedBagIterator(*this);
}
//BC=WC=TOTAL=Theta(1)

SortedBag::~SortedBag() {

    delete[] this->slla->elems;
    delete[] this->slla->next;
}
//BC=WC=TOTAL=Theta(1)