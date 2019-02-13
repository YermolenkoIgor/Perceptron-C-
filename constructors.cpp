#include "precompiler.h"
#include "constructors.h"
//деструктор
TMatrix :: ~TMatrix()
{
    for (usint i=0; i<this->N; i++)
        delete[]this->arr[i];
    delete[]this->arr;
    //cout<<"destroid"<<endl;
}
//конструктор по-умолчанию
TMatrix::TMatrix()
{
    N = 2;
    M = 2;
    arr = new D* [N];
    for (usint i = 0; i < N; i++)
        arr[i] = new D [M];

    for(usint i=0;i<N;i++)
        for(usint j=0;j<M;j++)
            arr[i][j] = 0.;
    //cout<<"default constr"<<endl;//отслеживаем срабатывание дефолтного конструктора 
}
//нулевая матрица (или рандомная)
TMatrix::TMatrix(usint N, usint M)
{
    this-> M = M;
    this-> N = N;

    arr = new D* [N];
    for (usint i = 0; i < N; i++)
        arr[i] = new D [M];

    for(usint i=0;i<N;i++)
        for(usint j=0;j<M;j++)
            arr[i][j] = 0.; //Randomer(0,10);
}

TMatrix::TMatrix(const TMatrix &matrixToCopy) // конструктор копии
{
    this-> M = matrixToCopy.M;
    this-> N = matrixToCopy.N;
    //старую arr удалять?
    arr = new D* [N];
    for (usint i = 0; i < N; i++)
        arr[i] = new D [M];
 
    for(usint i=0; i<this->N; i++)
        for(usint j=0; j<this->M; j++)
            arr[i][j] = matrixToCopy.arr[i][j];// заполняем матрицу значениями матрицы matrixToCopy
    //cout << "Matrix coppied" << endl;
}

ostream &operator << (ostream &t, const TMatrix &obj)
{
    for (usint i=0;i<obj.N;i++)
    {
        for (usint j= 0;j<obj.M;j++)
            t << obj.arr[i][j] << " ";
        t << endl;
    }
    return t;
}

void TMatrix :: printline(usint num)
{
    for(usint i=0; i < M; i++)
        cout << arr[num][i] << '\n';
    cout << endl;
}

//ищeт несоответствие в размерах матриц
bool checksizes (TMatrix& res, TMatrix& obj1, TMatrix& obj2)
{
    return (( obj1.N != obj2.N) | (obj1.M != obj2.M) | 
            ( res.N != obj1.N) | (res.M != obj1.M) | 
            ( res.N != obj2.N) | (res.M != obj2.M) );
}

void equel (TMatrix& res, TMatrix& obj)
{
    if(res.N != obj.N | res.M != obj.M)
        cout << "Matrixes must have same size for M1 = M2" << endl;
    else
        for (usint i=0;i<res.N;i++)
            for (usint j= 0;j<res.M;j++)
                res.arr[i][j] = obj.arr[i][j];
}

const TMatrix& TMatrix :: operator = (const TMatrix& obj)
{
    if(&obj != this)
    {
        if(this->N != obj.N | this->M != obj.M)
        {
            for (usint i=0; i<this->N; i++)
                delete[]this->arr[i];
            delete[]this->arr;
                
            this->N = obj.N;
            this->M = obj.M;
            
            this->arr = new D* [N];
            for (usint i = 0; i < N; i++)
                this->arr[i] = new D [M];
        }
        for (usint i=0;i<obj.N;i++)
            for (usint j= 0;j<obj.M;j++)
                this-> arr[i][j] = obj.arr[i][j];    
    }
    return *this;
}

void column_of(TMatrix& res, TMatrix& obj, usint num)
{
    if( res.N != obj.M | res.M != 1 )
        cout << "Error in column_of" << endl;
    else
        for(usint i=0; i < res.N; i++)
            res.arr[i][0] = obj.arr[num][i];
}

void Add (TMatrix& res, TMatrix& obj1, TMatrix& obj2)
{
    if ( checksizes(res, obj1, obj2) )
        cout << "Matrixes must have same size for +" << endl;
    else
        for (usint i=0; i < res.N; i++)
            for (usint j=0; j < res.M; j++)
                res.arr[i][j] = obj1.arr[i][j] + obj2.arr[i][j];
}

void Sub (TMatrix& res, TMatrix& obj1, TMatrix& obj2)
{
    if ( checksizes(res, obj1, obj2) )
        cout << "Matrixes must have same size for -" << endl;
    else
        for (usint i=0; i < res.N; i++)
            for (usint j=0; j < res.M; j++)
                res.arr[i][j] = obj1.arr[i][j] - obj2.arr[i][j];
}

void Mult (TMatrix& res, TMatrix& obj1, TMatrix& obj2)
{
    //Результатом умножения матриц A n×m и B m×k будет матрица C n×k
    if ( ( obj1.M != obj2.N) | ( res.N != obj1.N) | (res.M != obj2.M) )
        cout << "Matrixes must have same size for *" << endl;
    else
    {
        usint i, j, k;
        for( i = 0; i < res.N; i++)
            for( j = 0; j < res.M; j++)
                res.arr[i][j] = 0;
                
        for( i = 0; i < obj1.N; i++)
            for( j = 0; j < obj2.M; j++)
                for( k = 0; k < obj1.M; k++)
                    res.arr[i][j] += obj1.arr[i][k]*obj2.arr[k][j];
    }
}

//умножение с транспонированным первам множителем
void Mult1 (TMatrix& res, TMatrix& obj1, TMatrix& obj2)
{
    if ( ( obj1.N != obj2.N) | ( res.N != obj1.M) | (res.M != obj2.M) )
        cout << "Matrixes must have same size for * transpon 1" << endl;
    else
    {
        usint i, j, k;
        for( i = 0; i < res.N; i++)
            for( j = 0; j < res.M; j++)
                res.arr[i][j] = 0;
                
        for( i = 0; i < obj1.M; i++)
            for( j = 0; j < obj2.M; j++)
                for( k = 0; k < obj1.N; k++)
                    res.arr[i][j] += obj1.arr[k][i]*obj2.arr[k][j];
    }
}

void Mult2 (TMatrix& res, TMatrix& obj1, TMatrix& obj2)//без зануления res
{
    if ( ( obj1.M != obj2.M) | ( res.N != obj1.N) | (res.M != obj2.N) )
        cout << "Matrixes must have same size for * transpon 2" << endl;
    else
        for( usint i = 0; i < obj1.N; i++)
            for( usint j = 0; j < obj2.N; j++)
                for( usint k = 0; k < obj1.M; k++)
                    res.arr[i][j] += obj1.arr[i][k]*obj2.arr[j][k];
}

void Mult (TMatrix& res, TMatrix& obj1, D obj2)
{
    if (res.N != obj1.N | res.M != obj1.M)
        cout << "Matrixes must have same size for M*d" << endl;
    else
        for (usint i=0; i < res.N; i++)
            for (usint j=0; j < res.M; j++)
                res.arr[i][j] = obj1.arr[i][j] * obj2;
}

void Mult (TMatrix& res, D obj1, TMatrix& obj2)
{
    if (res.N != obj2.N | res.M != obj2.M)
        cout << "Matrixes must have same size for d*M" << endl;
    else
        for (usint i=0; i < res.N; i++)
            for (usint j=0; j < res.M; j++)
                res.arr[i][j] = obj1 * obj2.arr[i][j];
}

void Adamar (TMatrix& res, TMatrix& obj1, TMatrix& obj2)//Произведение Адамара (Шура)
{
    if ( checksizes(res, obj1, obj2) )
        cout << "Matrixes must have same size for *.a" << endl;
    else
        for (usint i=0; i < res.N; i++)
            for (usint j=0; j < res.M; j++)
                res.arr[i][j] = obj1.arr[i][j] * obj2.arr[i][j];
}

void Subeq (TMatrix& res, TMatrix& obj)
{
    if(res.N != obj.N | res.M != obj.M)
        cout << "Matrixes must have same size for -=" << endl;
    else
        for (usint i=0; i < res.N; i++)
            for (usint j=0; j < res.M; j++)
                res.arr[i][j] -= obj.arr[i][j];
}

void Addeq (TMatrix& res, TMatrix& obj)
{
    if(res.N != obj.N | res.M != obj.M)
        cout << "Matrixes must have same size for +=" << endl;
    else
        for (usint i=0; i < res.N; i++)
            for (usint j=0; j < res.M; j++)
                res.arr[i][j] += obj.arr[i][j];
}

void Divd (TMatrix& res, TMatrix& obj1, D& obj2)
{
    if (res.N != obj1.N | res.M != obj1.M)
        cout << "Matrixes must have same size for M/d" << endl;
    else
        for (usint i=0; i < res.N; i++)
            for (usint j=0; j < res.M; j++)
                res.arr[i][j] = obj1.arr[i][j] / obj2;
}

void Multeq (TMatrix& res, D obj)
{
    for (usint i=0; i < res.N; i++)
        for (usint j=0; j < res.M; j++)
            res.arr[i][j] *= obj;
}

void Divdeq (TMatrix& res, D obj)
{
    if(obj == 0)
        cout << "divided by zero :(" << endl;
    else
        for (usint i=0; i<res.N; i++)
            for (usint j=0; j<res.M; j++)
                res.arr[i][j] /= obj;
}

void Transpon (TMatrix& res, TMatrix& obj)
{
    if(res.N != obj.M | res.M != obj.N)
        cout << "Matrixes must have same size for transformation" << endl;
    else
        for (usint i=0; i < res.N; i++)
            for (usint j=0; j < res.M; j++)
                res.arr[i][j] = obj.arr[j][i];
}