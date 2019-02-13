#ifndef CONSTRUCTORS_H_INCLUDED
#define CONSTRUCTORS_H_INCLUDED

bool checksizes (TMatrix&, TMatrix&, TMatrix&);
void equel (TMatrix&, TMatrix&);
void column_of(TMatrix&, TMatrix&, usint);
void Add (TMatrix&, TMatrix&, TMatrix&);
void Sub (TMatrix&, TMatrix&, TMatrix&);
void Adamar (TMatrix&, TMatrix&, TMatrix&);
void Mult (TMatrix&, TMatrix&, TMatrix&);
void Mult1(TMatrix&, TMatrix&, TMatrix&);
void Mult2(TMatrix&, TMatrix&, TMatrix&);
void Mult (TMatrix&, TMatrix&, D);
void Mult (TMatrix&, D, TMatrix&);
void Divd (TMatrix&, TMatrix&, D);
void Subeq (TMatrix&, TMatrix&);
void Addeq (TMatrix&, TMatrix&);
void Multeq (TMatrix&, D);
void Divdeq (TMatrix&, D);
void Transpon (TMatrix&, TMatrix&);
//дружественные ф-ии
ostream& operator << (ostream&, const TMatrix&);//взятие и всувание в потоккккк

#endif // CONSTRUCTORS_H_INCLUDED
