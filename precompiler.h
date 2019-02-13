#ifndef PRECOMPILER_H_INCLUDED
#define PRECOMPILER_H_INCLUDED
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <windows.h>
#define Randomer(a,b) (1.*rand())/(RAND_MAX+0.)*(b-a)+a

using namespace std;

typedef double D;
typedef unsigned short int usint;//счетчики работают до 65 535

class TMatrix{
//private:
    
public:
    usint N;  // число строк
    usint M;  // число столбцов
    D **arr;  // двумерный массив
    
    TMatrix ();//конструктор по умолчанию
   ~TMatrix ();//деструктор
    TMatrix (usint, usint);//нулевая матрица
    TMatrix (const TMatrix&); // конструктор копии

    const TMatrix& operator = (const TMatrix&);
    void printline(usint);
    
friend ostream& operator << (ostream&, const TMatrix&);
};

class Neurotask{
public:
    usint Nteach, Ntest;
    usint Ninp, Nout;
    TMatrix Xtrain;
    TMatrix Ytrain;
    TMatrix Xtest;
    TMatrix Ytest;
    
   //~Neurotask();
    Neurotask();
    Neurotask(string);
    Tobinfile(string);
    Frombinfile(string);
    void get_task();
};

class Perceptons{
private:
    usint layers, *Ns;
    D alfa;
    TMatrix **Weights;
    TMatrix **Bias;
    
public:
   //~Perceptons();
    Perceptons();
    Perceptons(string);
    Perceptons(usint,usint*,D);
    void back_propagation(Neurotask&,usint);
    void neuro_exam(Neurotask&);
    void neuro_to_file(string);
    void distrectution(TMatrix**);
    void get_weights();
    void get_bias();
};

#endif // PRECOMPILER_H_INCLUDED
