#include <iostream>
#include "sparsematrix.hpp"
#include "woodpecker.hpp"

TEST("Egyszeru ures"){
SparseMatrix s = SparseMatrix(1000,1000,0);
CHECK_EQ(s(100,10),0);
CHECK_EQ(s(999,42),0);
}
TEST("Meglevo ertek torlese"){
SparseMatrix s = SparseMatrix(1000,1000,2);
CHECK_NOEXC(s.clear(10,10));
CHECK_EQ(s(10,10),2);
}
TEST("Meglevo ertek felulirasa"){
SparseMatrix s = SparseMatrix(1000,1000,2);
s.set(12,12,40);
s.set(12,12,10);
CHECK_EQ(s(12,12), 10);
}
TEST("Kis matrix ertekekkel"){
SparseMatrix s = SparseMatrix(10,10,1);
s.set(2,2,2.2);
s.set(2,1,2.1);
s.set(2,8,2.8);
s.set(2,4,2.4);
s.set(1,4,1.4);
s.set(8,3,8.3);
s.set(6,0,6.0);
s.printMatrix();
CHECK_EQ(s(2,2), 2.2);
CHECK_EQ(s(2,1), 2.1);
CHECK_EQ(s(2,8), 2.8);
CHECK_EQ(s(2,4), 2.4);
CHECK_EQ(s(1,4), 1.4);
CHECK_EQ(s(8,3), 8.3);
CHECK_EQ(s(6,0), 6.0);
CHECK_EQ(s(0,0), 1);
CHECK_EQ(s(0,1), 1);
CHECK_EQ(s(0,2), 1);
CHECK_EQ(s(0,3), 1);
CHECK_EQ(s(1,1), 1);
CHECK_EQ(s(1,2), 1);
CHECK_EQ(s(1,3), 1);
CHECK_EQ(s(6,1), 1);
CHECK_EQ(s(9,9), 1);
CHECK_EQ(s(9,8), 1);
CHECK_EQ(s(9,0), 1);
}
TEST("Nagy matrix ertekekkel"){
SparseMatrix s = SparseMatrix(1000,1000,0);
s.set(885, 12, 42);
s.set(0, 442, 998);
CHECK_EQ(s(100,10),0);
CHECK_EQ(s(999,42),0);
CHECK_EQ(s(885, 12),42);
CHECK_EQ(s(0, 442),998);
}
TEST("Egyszeru ertekekkel - nemnegyzetes"){
SparseMatrix s = SparseMatrix(1000,100,0);
s.set(885, 12, 42);
s.set(0, 42, 998);
CHECK_EQ(s(100,10),0);
CHECK_EQ(s(999,42),0);
CHECK_EQ(s(885, 12),42);
CHECK_EQ(s(0, 42),998);
}

TEST("Egyszeru ertekekkel es eltavolitassal"){
SparseMatrix s = SparseMatrix(1000,1000,0);
for(int i = 0; i < 100; i++){
s.set(200+i, 40+2*i, i+5);
}
s.set(885, 12, 42);
s.set(0, 442, 998);
CHECK_EQ(s(100,10),0);
CHECK_EQ(s(999,42),0);
CHECK_EQ(s(885, 12),42);
CHECK_EQ(s(0, 442),998);
for(int i = 0; i < 100; i++){
CHECK_EQ(s(200+i, 40+2*i),i+5);
CHECK_EQ(s(199, 40+2*i),0);
}

for(int i = 0; i < 100; i++){
s.clear(200+i, 40+2*i);
}
for(int i = 0; i < 100; i++){
CHECK_EQ(s(200+i, 40+2*i),0);
CHECK_EQ(s(199, 40+2*i),0);
}

}

TEST("Egyszeru ertekekkel - kiindexeles"){
SparseMatrix s = SparseMatrix(1000,100,0);
bool ok = true;
try{
s.set(1200, 12, 42);
ok = false;
std::cerr<< " ->Kiindexelesre nem ad hibat: s(1200,12)-nel:" << s(1200,12) << "\n";
}catch(...){
//std::cerr << " ->OK\n";
}
try{
s.set(80, 120, 42);
ok = false;
std::cerr<< " ->Kiindexelesre nem ad hibat: s(1200,12)-nel:" << s(1200,12) << "\n";
}catch(...){
//std::cerr << " ->OK\n";
}
CHECK_EQ(ok,true);
try{
s.set(-80, 80, 42);
ok = false;
std::cerr<< " ->NEGATIV Kiindexelesre nem ad hibat: s(1200,12)-nel:" << s(1200,12) << "\n";
}catch(...){
//std::cerr << " ->OK\n";
}
CHECK_EQ(ok,true);
}
TEST("Matrix nemnulla default ertekkel"){
SparseMatrix s = SparseMatrix(1000,1000,42);
s.set(885, 12, 40);
s.set(0, 442, 998);
CHECK_EQ(s(100,10),42);
CHECK_EQ(s(999,42),42);
CHECK_EQ(s(885, 12),40);
CHECK_EQ(s(0, 442),998);
for(int i = 0; i < 100; i++){
CHECK_EQ(s(200+i, 40+2*i),42);
}
}
TEST("Matrix osszeadas"){
SparseMatrix s = SparseMatrix(1000,1000,0);
s.set(111, 111, 111);
s.set(222, 222, 222);
SparseMatrix s2 = SparseMatrix(1000,1000,0);
s2.set(111, 111, 10);
s2.set(333, 333, 42);
SparseMatrix sum = s+s2;
CHECK_EQ(sum(0,0),0);
CHECK_EQ(sum(100,100),0);
CHECK_EQ(sum(111,111),121);
CHECK_EQ(sum(222,222),222);
CHECK_EQ(sum(333,333),42);
}
TEST("Matrix osszeadas nemnulla default ertekkel"){
SparseMatrix s = SparseMatrix(1000,1000,2);
s.set(111, 111, 111);
s.set(222, 222, 222);
SparseMatrix s2 = SparseMatrix(1000,1000,4);
s2.set(111, 111, 10);
s2.set(333, 333, 42);
SparseMatrix sum = s+s2;
CHECK_EQ(sum(0,0),6);
CHECK_EQ(sum(100,100),6);
CHECK_EQ(sum(111,111),121);
CHECK_EQ(sum(222,222),226);
CHECK_EQ(sum(333,333),44);
}
TEST("Matrix copy konstruktor"){
SparseMatrix s = SparseMatrix(10,10,1);
s.set(2,2,2.2);
s.set(2,1,2.1);
s.set(2,8,2.8);
s.set(2,4,2.4);
s.set(1,4,1.4);
s.set(8,3,8.3);
s.set(6,0,6.0);
SparseMatrix s2(s);
s2.clear(2,2);
s2.clear(2,1);
s2.clear(2,8);
s2.clear(2,4);
s2.clear(1,4);
s2.clear(8,3);
s2.clear(6,0);
CHECK_EQ(s(2,2), 2.2);
CHECK_EQ(s(2,1), 2.1);
CHECK_EQ(s(2,8), 2.8);
CHECK_EQ(s(2,4), 2.4);
CHECK_EQ(s(1,4), 1.4);
CHECK_EQ(s(8,3), 8.3);
CHECK_EQ(s(6,0), 6.0);
CHECK_EQ(s2(2,2), 1);
CHECK_EQ(s2(2,1), 1);
CHECK_EQ(s2(2,8), 1);
CHECK_EQ(s2(2,4), 1);
CHECK_EQ(s2(1,4), 1);
CHECK_EQ(s2(8,3), 1);
CHECK_EQ(s2(6,0), 1);
}
TEST("Matrix assignment operator"){
SparseMatrix s = SparseMatrix(10,10,1);
s.set(2,2,2.2);
s.set(2,1,2.1);
s.set(2,8,2.8);
s.set(2,4,2.4);
s.set(1,4,1.4);
s.set(8,3,8.3);
s.set(6,0,6.0);
SparseMatrix s2 = s;
s2.clear(2,2);
s2.clear(2,1);
s2.clear(2,8);
s2.clear(2,4);
s2.clear(1,4);
s2.clear(8,3);
s2.clear(6,0);
CHECK_EQ(s(2,2), 2.2);
CHECK_EQ(s(2,1), 2.1);
CHECK_EQ(s(2,8), 2.8);
CHECK_EQ(s(2,4), 2.4);
CHECK_EQ(s(1,4), 1.4);
CHECK_EQ(s(8,3), 8.3);
CHECK_EQ(s(6,0), 6.0);
CHECK_EQ(s2(2,2), 1);
CHECK_EQ(s2(2,1), 1);
CHECK_EQ(s2(2,8), 1);
CHECK_EQ(s2(2,4), 1);
CHECK_EQ(s2(1,4), 1);
CHECK_EQ(s2(8,3), 1);
CHECK_EQ(s2(6,0), 1);
}
TEST("Kulonbozo meretu matrixok osszeadasa -> hiba"){
SparseMatrix s = SparseMatrix(1000,1000,2);
s.set(111, 111, 111);
s.set(222, 222, 222);
SparseMatrix s2 = SparseMatrix(100,1000,4);
CHECK_EXC(std::exception, s+s2);
}
WOODPECKER_TEST_MAIN()