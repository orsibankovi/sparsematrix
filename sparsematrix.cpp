//
// Created by banko on 2022. 10. 07..
//

#include "sparsematrix.hpp"
#include "OutOfBoundException.h"

SparseMatrix::SparseMatrix(int size_x, int size_y, double defaultValue): x_size(size_x), y_size(size_y), default_value(defaultValue), row_head(nullptr), row_tail(nullptr) {}

SparseMatrix::SparseMatrix(const SparseMatrix &other) noexcept: x_size(other.x_size), y_size(other.y_size), default_value(other.default_value), row_head(nullptr), row_tail(nullptr) {
    if (other.isEmpty()){ return; }
    for (Row *i = other.row_head; i != nullptr; i = i->next_row){
        for (Node *j = i->node_head; j != nullptr; j = j->next_node){
            set(j->x, j->y, j->value_);
        }
    }
}

SparseMatrix &SparseMatrix::operator=(const SparseMatrix &other) noexcept {
    // Ha a kapott lista megegyezik az eredetivel, akkor keszen vagyunk
    if (&other == this){ return *this; }

    // Egyebkent ki kell uritenunk a mar meglevo listankat, ha nem ures
    if (!isEmpty()) {
        for (Row *r = row_head; r != nullptr;) {
            Row *tmp = r;
            for (Node *n = r->node_head; n != nullptr;) {
                Node *tmp_ = n;
                n = n->next_node;
                delete tmp_;
            }
            r = r->next_row;
            delete tmp;
        }
    }
    row_tail = row_head = nullptr;
    x_size = other.x_size;
    y_size = other.y_size;
    default_value = other.default_value;

    if (other.isEmpty()){
        return *this;
    }

    for (Row *i = other.row_head; i != nullptr; i = i->next_row){
        for (Node *j = i->node_head; j != nullptr; j = j->next_node){
            set(j->x, j->y, j->value_);
        }
    }
    return *this;
}

bool SparseMatrix::isEmpty() const {
    return (row_head == nullptr);
}

double SparseMatrix::operator()(int x, int y) const {
    if (x>x_size or y>y_size or x < 0 or y < 0){ throw OutOfBoundException(); }

    double val_ = default_value;
    if (isEmpty()){ return val_; }
    else {
        for (Row *i = row_head; i != nullptr; i = i->next_row) {
            if (i->row_num == y) {
                for (Node *j = i->node_head; j != nullptr; j = j->next_node) {
                    if (j->x == x) { val_ = j->value_; }
                }
            }
        }
    }
    return val_;
}

void SparseMatrix::set(int x, int y, double value) {
    if (x > x_size or y > y_size or x < 0 or y < 0) { throw OutOfBoundException(); }

    Node *n = new Node(x, y, value, nullptr);
    if (isEmpty()) {
        Row *r = new Row(y, n, n, nullptr);
        row_head = row_tail = r;
        return;
    }
    else if (y < row_head->row_num) { //elso sor ele szurjuk be, es meg nincs ilyen sor
        Row *new_row = new Row(y, n, n, row_head);
        row_head = new_row;
        return;
    }
    else if (y > row_tail->row_num) { //utolso sor utanra szurjuk be, es meg nincs ilyen sor
        Row *new_row = new Row(y, n, n, nullptr);
        row_tail->next_row = new_row;
        row_tail = new_row;
        return;
    }
    for (Row *i = row_head; i != nullptr; i = i->next_row) {
        if (i->row_num == y) { //van ilyen sor mar a matrixban
            //itt kell a sor lancolatba befuzni az elemet
            for (Node *j = i->node_head; j != nullptr; j = j->next_node) {
                if (j->x == x) { //van mar ilyen elem a matrixban, csak felul kell irni
                    j->value_ = value;
                    delete n;
                    return;
                } else if (x > i->node_tail->x) { //adott sorban az utolso utan
                    i->node_tail->next_node = n;
                    i->node_tail = n;
                    return;
                } else if (x < i->node_head->x) { //adott sorban az elso ele
                    n->next_node = i->node_head;
                    i->node_head = n;
                    return;
                } else if (j->next_node->x > x) { // nincs az adott indexen elem, kozbulso elem
                    n->next_node = j->next_node;
                    j->next_node = n;
                    return;
                }
            }
        }  else if (i->next_row->row_num > y) { //nincs meg ilyen sor, sorok listájának kozepere akarjuk beszurni
            Row *r = new Row(y, n, n, nullptr);
            r->next_row = i->next_row; //a nodedal egyutt beepitjuk a sorok lancaba
            i->next_row = r;
            return;
        }
    }
}

void SparseMatrix::clear(int x, int y) {
    if (x > x_size or y > y_size or x < 0 or y < 0) { throw OutOfBoundException(); }

    if (isEmpty()){
        return;
    }

    if (!isEmpty() and y == row_head->row_num){ //elso sor torlese
        if (row_head->node_head == row_head->node_tail and row_head->node_head->x == x){ //egyetlen elem van a sorban, sort is torolni kell
            if (row_head == row_tail) { //csak ez az egy sor van
                delete row_head->node_head;
                delete row_head;
                row_head = row_tail = nullptr;
                return;
            } else {
                delete row_head->node_head;
                Row *tmp = row_head;
                row_head = row_head->next_row;
                delete tmp;
                return;
            }
        } else { //tobb elem van az elso sorban
            if (x == row_head->node_head->x) { //elso elemet toroljuk
                Node *tmp = row_head->node_head;
                row_head->node_head = row_head->node_head->next_node;
                delete tmp;
                return;
            } else {
                for (Node *n = row_head->node_head; n != row_head->node_tail; n =n->next_node) {
                    if (x == row_head->node_tail->x) { //utolso elem
                        delete n->next_node;
                        n->next_node = nullptr;
                        return;
                    } else if (x == n->next_node->x) {
                        Node *tmp = n->next_node;
                        n->next_node = n->next_node->next_node;
                        delete tmp;
                        return;
                    }
                }
            }
        }
    }
    else { //utolso, vagy kozbulso sorbol torlunk
        for (Row *r = row_head; r != row_tail or r != nullptr; r = r->next_row) {
            if (r->next_row->node_head == r->next_row->node_tail and r->next_row->node_head->x == x) { //csak egy elem van a sorban, sort is torolni kell
                if (y == row_tail->row_num) { //utolso sor
                    delete r->next_row->node_head;
                    delete r->next_row;
                    r->next_row = nullptr;
                    return;
                } else if (r->next_row->row_num == y and r->next_row->node_head->x == x) { //nem utolso sor
                    delete r->next_row->node_head;
                    Row *tmp = r->next_row;
                    r->next_row = r->next_row->next_row;
                    delete tmp;
                    return;
                }

            } else if (y == r->next_row->row_num) { //tobb elem van a sorban
                if (x == r->next_row->node_head->x) { //elso elemet toroljuk
                    Node *tmp = r->next_row->node_head;
                    r->next_row->node_head = r->next_row->node_head->next_node;
                    delete tmp;
                    return;
                } else {
                    for (Node *n = r->next_row->node_head; n != r->next_row->node_tail; n =n->next_node) {
                        if (x == r->next_row->node_tail->x) { //utolso elem
                            delete n->next_node;
                            n->next_node = nullptr;
                            return;
                        } else if (x == n->next_node->x) {
                            Node *tmp = n->next_node;
                            n->next_node = n->next_node->next_node;
                            delete tmp;
                            return;
                        }
                    }
                }
            }
        }
    }
}

SparseMatrix SparseMatrix::operator+(const SparseMatrix &other) const {
    if (x_size != other.x_size or y_size != other.y_size){ throw OutOfBoundException();}

    SparseMatrix s(x_size, y_size, default_value + other.default_value);

    int row_num1;
    int row_num2;
    int x1;
    int x2;

    for (Row *r1 = row_head, *r2 = other.row_head; r1 != nullptr or r2 != nullptr;){
        if (r1 == nullptr){
            row_num1 = y_size + 1; //az egyik matrix sorain vegigertunk, ezt sosem fogja elerni a masik matrix, azzal addig lepkedunk, amig vegig nem erunk a sorokon (nullptr) -> ekkor vagyunk kesz
            row_num2 = r2->row_num;
        }
        else if (r2 == nullptr){
            row_num1 = r1->row_num;
            row_num2 = y_size + 1;
        } else {
            row_num1 = r1->row_num;
            row_num2 = r2->row_num;
        }

        if (row_num1 == row_num2){
            //elemeken vegig megyunk
            for (Node *n1 = r1->node_head, *n2 = r2->node_head; n1 != nullptr or n2 != nullptr; ){
                if (n1 == nullptr){
                    x1 = x_size + 1;
                    x2 = n2->x;
                }
                else if (n2 == nullptr){
                    x1 = n1->x;
                    x2 = y_size + 1;
                } else {
                    x1 = n1->x;
                    x2 = n2->x;
                }

                if (x1 == x2){
                    s.set(n1->x, n1->y, n1->value_ + n2->value_),
                            n1 = n1->next_node;
                    n2 = n2->next_node;
                }
                else if (x1 < x2){
                    s.set(n1->x, n1->y, n1->value_ + other.default_value);
                    n1 = n1->next_node;
                }
                else if (x1 > x2){
                    s.set(n2->x, n2->y, n2->value_ + default_value);
                    n2 = n2->next_node;
                }
            }
            r1 = r1->next_row;
            r2 = r2->next_row;
        }
        else if (row_num1 < row_num2){
            //r1-en vegig megyunk, + other df
            for (Node *n1 = r1->node_head; n1 != nullptr; n1 = n1->next_node){
                s.set(n1->x, n1->y, n1->value_ + other.default_value);
            }
            r1 = r1->next_row;
        }
        else if (row_num1 > row_num2){
            //r2-n vegig megyunk, + df
            for (Node *n2 = r2->node_head; n2 != nullptr; n2 = n2->next_node){
                s.set(n2->x, n2->y, n2->value_ + default_value);
            }
            r2 = r2->next_row;
        }
    }

    return s;
}



void SparseMatrix::printMatrix() const {
    double val_;
    for (int j =0; j<y_size; j++){
        for (int i = 0; i<x_size; i++){
            val_ = default_value;
            for (Row *r = row_head; r != nullptr; r = r->next_row){
                for (Node *n = r->node_head; n != nullptr; n = n->next_node){
                    if (n->x == i and n->y == j){val_ = n->value_; }
                }
            }
            std::cout << val_ << "  ";
        }
        std::cout << std::endl;
    }
}

SparseMatrix::~SparseMatrix() {
    if (!isEmpty()) {
        for (Row *r = row_head; r != nullptr;) {
            Row *tmp = r;
            for (Node *n = r->node_head; n != nullptr;) {
                Node *tmp_ = n;
                n = n->next_node;
                delete tmp_;
            }
            r = r->next_row;
            delete tmp;
        }
    }
    row_head = row_tail = nullptr;
}