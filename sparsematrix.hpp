#ifndef NHF1_SKELETON_SPARSEMATRIX_HPP
#define NHF1_SKELETON_SPARSEMATRIX_HPP
#include <iostream>

class SparseMatrix{
private:
    struct Node{
        int x; //sor
        int y; //oszlop
        double value_;
        Node* next_node; //kovetkezo nem default elemre mutat

        Node(): x(0), y(0), value_(0), next_node(nullptr){}
        Node(int x_, int y_, double v_, Node* next_): x(x_), y(y_), value_(v_), next_node(next_){}
    };
private:
    struct Row{
        int row_num;
        Node* node_head; //sorban az elso nem default node
        Node* node_tail; //sorban az utolso nem default node
        Row* next_row ; //a kovetkezo nem csak default elemet tartalmazo sorra mutat

        Row(): row_num(0), node_head(nullptr), node_tail(nullptr), next_row(nullptr){}
        Row(int y_, Node* node_head_, Node* node_tail_, Row* next_): row_num(y_), node_head(node_head_), node_tail(node_tail_), next_row(next_) {}
    };
    int x_size;
    int y_size;
    double default_value;
    Row* row_head; //az elso nem csak default elemet tartalmazo sorra mutat
    Row* row_tail; //az utolso nem csak default elemet tartalmazo sorra mutat

    SparseMatrix(): x_size(0), y_size(0), default_value(0), row_head(nullptr), row_tail(nullptr){}

public:
    //Konstruktor
    SparseMatrix(int size_x, int size_y, double defaultValue);

    bool isEmpty() const; //segedfv

    //Copy konstruktor
    SparseMatrix(const SparseMatrix &other) noexcept;

    //Assignment operator
    SparseMatrix &operator=(const SparseMatrix &other) noexcept;

    //Destruktor
    ~SparseMatrix();

    //Indexeles - kiindexeles eseten hibat dob
    double operator()(int x, int y) const;

    //Teljes matrix kiiratasa consolera
    void printMatrix() const;

    //Matrix elem ertekadas - ha letezik az ertek, felulirja
    //Kiindexeles eseten hibat dob
    void set(int x, int y, double value);

    //Matrix elem torlese, ha nem letezik nem default ertek
    // az adott koordiniatan, akkor nem csinal semmit
    //Kiindexeles eseten hibat dob
    void clear(int x, int y);

    //Osszeadas operator - hibat dob ha nem azonos dimenzioju a 2 matrix
    SparseMatrix operator+(const SparseMatrix &other) const;

};
#endif //NHF1_SKELETON_SPARSEMATRIX_HPP
