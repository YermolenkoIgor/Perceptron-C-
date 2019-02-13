#include "precompiler.h"
#include "Perceptons.h"
#include "constructors.h"
/*
Perceptons :: ~Perceptons()
{
    for(int i=layers-1; i >=0 ; i--)
    {
        Weights[i][0].~TMatrix();
        Bias[i][0].   ~TMatrix();
    }
    /*
    //delete[] Ns;//something wrong
    cout<<"19."<<endl;
    Weights[0]->~TMatrix();
    Bias[0]->~TMatrix();
    delt[0]->~TMatrix();
    Junc[0]->~TMatrix();
    dW[0]->~TMatrix();
    dB[0]->~TMatrix();
    cout<<"24."<<endl;
    cout<<"Destroid."<<endl;
}*/

Perceptons :: Perceptons()
{
    usint i, j, k;
    layers = 2;
    Weights = new TMatrix*;
    Bias = new TMatrix*;
    
    Ns = new usint[layers+1];
    Ns[0] = 4;//neurons in the input layer
    Ns[1] = 10;
    Ns[2] = 1;//neurons in the output layer

    alfa = 0.1;
    
//alfa - коэффициент инерциальности для сглаживания резких скачков 
//при перемещении по поверхности целевой функции
    
    for(i=0; i<layers; i++)
    {  
        Weights[i] = new TMatrix(Ns[i+1], Ns[i]);
        Bias   [i] = new TMatrix(Ns[i+1],1);
    }
    
    for(k=0; k<layers; k++)
        for(i=0; i<Ns[k+1]; i++)
        {
            for(j=0; j<Ns[k]; j++)
                Weights[k][0].arr[i][j] = Randomer(-0.5/Ns[k], 0.5/Ns[k]);
            Bias[k][0].arr[i][0] = 0.5;
        }
    
    cout<<"Constructed by default"<<endl;
}

Perceptons :: Perceptons(usint lrs, usint *Neurons, D alpha)
{
    usint i, j, k;
    layers = lrs;
    Weights = new TMatrix*;
    Bias = new TMatrix*;
    
    Ns = new usint[layers+1];
    for(i=0; i <= layers; i++)
        Ns[i] = Neurons[i];
    
    alfa = alpha;
    
//alfa - коэффициент инерциальности для сглаживания резких скачков 
//при перемещении по поверхности целевой функции
    
    for(i=0; i<layers; i++)
        Bias[i] = new TMatrix(Ns[i+1],1);
    
    for(i=0; i<layers; i++)
        Weights[i] = new TMatrix(Ns[i+1], Ns[i]);
    
    
    for(k=0; k<layers; k++)
        for(i=0; i<Ns[k+1]; i++)
            for(j=0; j<Ns[k]; j++)
                Weights[k][0].arr[i][j] = Randomer(-0.5/Ns[k], 0.5/Ns[k]);
    
    for(k=0; k<layers; k++)
        for(i=0; i<Ns[k+1]; i++)
            Bias[k][0].arr[i][0] = 0.5;
    
    cout<<"Constructed with parameters"<<endl;
}

Perceptons :: Perceptons(string filename)
{
    usint i, j, k;
    Weights = new TMatrix*;
    Bias = new TMatrix*;
    
    ifstream fr(filename);
    if(!fr)
        cout << "file don't opened for reading\n";
    
    fr >> layers;
    fr >> alfa;
    
    Ns = new usint[layers+1];
    
    for(k=0; k<=layers; k++)
        fr >> Ns[k];
    
    for(i=0; i<layers; i++)
        Weights[i] = new TMatrix(Ns[i+1], Ns[i]);
    
    for(i=0; i<layers; i++)
        Bias   [i] = new TMatrix(Ns[i+1],1);
    
    for(k=0; k<layers; k++)
        for(i=0; i<Ns[k+1]; i++)
            for(j=0; j<Ns[k]; j++)
                fr >> Weights[k][0].arr[i][j];
    
    for(k=0; k<layers; k++)
        for(i=0; i<Ns[k+1]; i++)
            fr >> Bias[k][0].arr[i][0];

    fr.close();
    
    cout << "Constructed from " << filename << endl;
}

void actifun(TMatrix& z)
{
    for(usint i=0; i<z.N; i++)
        z.arr[i][0] = 1./(1 + exp(-z.arr[i][0]));//signumoid       
}

void Perceptons :: distrectution(TMatrix **Junc)//direct distribution
{
    for(usint i=0; i<layers; i++)
    {
        Mult( Junc[i+1][0], Weights[i][0], Junc[i][0] );
        Addeq( Junc[i+1][0], Bias[i][0]);
        actifun( Junc[i+1][0] );
    }
}

void Perceptons :: back_propagation(Neurotask& aTask, usint steps)
{
    usint i, j, k = 0;
    TMatrix evalbox (aTask.Nout, 1);
    TMatrix evalfun (aTask.Nout, 1);
    TMatrix zeros(aTask.Nout, 1);
    TMatrix **Junc = new TMatrix*;
    TMatrix **dfdz = new TMatrix*;
    TMatrix **delt = new TMatrix*;
    TMatrix **dW = new TMatrix*;
    TMatrix **dB = new TMatrix*;
    
    for(i=0; i<=layers; i++)
        Junc[i] = new TMatrix(Ns[i],1);
    
    for(i=0; i<layers; i++)
        dW[i] = new TMatrix(Ns[i+1], Ns[i]);
    
    for(i=0; i<layers; i++)
        dB[i] = new TMatrix(Ns[i+1],1);
    
    for(i=0; i<layers; i++)
        delt[i] = new TMatrix(Ns[layers-i],1);
    
    for(i=0; i<=layers; i++)
        dfdz[i] = new TMatrix(Ns[i],1);//память как на узлы
    
    alfa /= aTask.Nteach;
    
    cout << "Start learning..." << endl;
    
    while(k < steps)
    {
        for(i=0; i<aTask.Nteach; i++)
        {
            column_of( Junc[0][0], aTask.Xtrain, i );//Junc[0][0] = X[i];
            this->distrectution(Junc);
            
            column_of( delt[0][0], aTask.Ytrain, i );
            Sub( delt[0][0], Junc[layers][0], delt[0][0] );
            Adamar( dfdz[layers][0], Junc[layers][0], Junc[layers][0] );
            Sub( dfdz[layers][0], Junc[layers][0], dfdz[layers][0] );
            Adamar( delt[0][0], delt[0][0], dfdz[layers][0] );
            
            for(j=1; j<layers; j++)
            {
                Mult1( delt[j][0], Weights[layers-j][0], delt[j-1][0] );
                Adamar( dfdz[layers-j][0], Junc[layers-j][0], Junc[layers-j][0] );
                Sub( dfdz[layers-j][0], Junc[layers-j][0], dfdz[layers-j][0] );
                Adamar( delt[j][0], delt[j][0], dfdz[layers-j][0] );
            }
                
            for(j=0; j<layers; j++)
            {
                Mult2( dW[j][0], delt[layers-j-1][0], Junc[j][0] );
                Addeq( dB[j][0], delt[layers-j-1][0] );
                
                Divdeq( Weights[j][0], alfa );
                Subeq ( Weights[j][0], dW[j][0] );
                Multeq( Weights[j][0], alfa );
                
                Divdeq( Bias[j][0], alfa );
                Subeq ( Bias[j][0], dB[j][0] );
                Multeq( Bias[j][0], alfa );
            }
            
            column_of( evalbox, aTask.Ytrain, i );
            Subeq ( evalbox, Junc[layers][0] );
            Adamar( evalbox, evalbox, evalbox );
            Addeq ( evalfun, evalbox );
        }
        k++;
        Multeq( evalfun, 0.5/aTask.Nteach );
        
        cout << k << " " << evalfun;
        equel ( evalfun, zeros );
    }
    alfa *= aTask.Nteach;
    cout << "Teached!" << endl;
}

void Perceptons :: neuro_exam(Neurotask& aTask)
{
    usint i;
    TMatrix **Junc = new TMatrix*;
    
    for(i=0; i<=layers; i++)
        Junc[i] = new TMatrix(Ns[i],1);
    
    for(i=0; i < aTask.Ntest; i++)
    {
        column_of( Junc[0][0], aTask.Xtest, i );
        
        this->distrectution(Junc);

        cout << Junc[0][0] << "ANN think:\n" << Junc[layers][0] << "ans from file:" << endl; 
        aTask.Ytest.printline(i);
    }
    cout << "ANN examed" << endl;
}

void Perceptons :: get_weights()
{
    cout << "Weights of the every layer" << '\n';
    for (usint i = 0; i < layers; i++)
        cout << Weights[i][0] << "\n";
    cout << endl;
}

void Perceptons :: get_bias()
{
    cout << "Bias of the every layer" << '\n';
    for (usint i = 0; i < layers; i++)
        cout << Bias[i][0] << '\n';
    cout << endl;
}

void Perceptons :: neuro_to_file(string filename)
{
    usint i, j, k;
    ofstream fp(filename);
    if(!fp)
        cout << "file don't opened for writing\n";
    
    fp << layers << '\n';
    fp << alfa   << '\n';
    
    for(k=0; k<=layers; k++)
        fp << Ns[k] << '\n';
    
    for(k=0; k<layers; k++)
        for(i=0; i<Ns[k+1]; i++)
            for(j=0; j<Ns[k]; j++)
                fp << Weights[k][0].arr[i][j] << '\n';
    
    for(k=0; k<layers; k++)
        for(i=0; i<Ns[k+1]; i++)
            fp << Bias[k][0].arr[i][0] << '\n';
        
    fp << endl;
    fp.close();
}

//Neurotask :: ~Neurotask ()
//{
//    Xtrain.~TMatrix();
//    Ytrain.~TMatrix();
//    Xtest. ~TMatrix();
//    Ytest. ~TMatrix();
//    cout << "Task deleted" << endl;
//}

Neurotask :: Neurotask ()
{
    usint i, j;
    
    Nteach = 1;
    Ntest = 1;
    Ninp = 1;
    Nout = 1;
    
    Xtrain = TMatrix(Nteach, Ninp);
    Ytrain = TMatrix(Nteach, Nout);
    Xtest  = TMatrix(Ntest, Ninp);
    Ytest  = TMatrix(Ntest, Nout);
    
    cout << "Chosen task by default" << endl;
}

Neurotask :: Tobinfile (string filename)
{
    usint i, j;
    ofstream fr(filename);
    if(!fr)
        cout << "file don't opened for readind" << endl;
    
    fr.write( (char*)&Nteach, sizeof(Nteach) );
    fr.write( (char*)&Ntest, sizeof(Ntest) );
    fr.write( (char*)&Ninp, sizeof(Ninp) );
    fr.write( (char*)&Nout, sizeof(Nout) );
    
    for(i=0; i<Nteach; i++)
    {
        for(j=0; j<Ninp; j++)
            fr.write( (char*)&Xtrain.arr[i][j], sizeof(Xtrain.arr[i][j]) );
        for(j=0; j<Nout; j++)
            fr.write( (char*)&Ytrain.arr[i][j], sizeof(Ytrain.arr[i][j]) );
    }
   
    for(i=0; i<Ntest; i++)
    {
        for(j=0; j<Ninp; j++)
            fr.write( (char*)&Xtest.arr[i][j], sizeof(Xtest.arr[i][j]) );
        for(j=0; j<Nout; j++)
            fr.write( (char*)&Ytest.arr[i][j], sizeof(Ytest.arr[i][j]) );
    }

    cout << "Written to " << filename << endl;
    fr.close(); 
}

Neurotask :: Frombinfile (string filename)
{
    usint i, j;
    ifstream fr(filename);
    if(!fr)
        cout << "file don't opened for readind" << endl;
    
    fr.read( (char*)&Nteach, sizeof(Nteach) );
    fr.read( (char*)&Ntest, sizeof(Ntest) );
    fr.read( (char*)&Ninp, sizeof(Ninp) );
    fr.read( (char*)&Nout, sizeof(Nout) );
    
    for(i=0; i<Nteach; i++)
    {
        for(j=0; j<Ninp; j++)
            fr.read( (char*)&Xtrain.arr[i][j], sizeof(Xtrain.arr[i][j]) );
        for(j=0; j<Nout; j++)
            fr.read( (char*)&Ytrain.arr[i][j], sizeof(Ytrain.arr[i][j]) );
    }
   
    for(i=0; i<Ntest; i++)
    {
        for(j=0; j<Ninp; j++)
            fr.read( (char*)&Xtest.arr[i][j], sizeof(Xtest.arr[i][j]) );
        for(j=0; j<Nout; j++)
            fr.read( (char*)&Ytest.arr[i][j], sizeof(Ytest.arr[i][j]) );
    }

    cout << "Read from " << filename << endl;
    fr.close(); 
}

Neurotask :: Neurotask (string filename)
{
    usint i, j;
    ifstream fr(filename);
    if(!fr)
        cout << "file don't opened for readind" << endl;

    fr >> Nteach;
    fr >> Ntest;
    fr >> Ninp;
    fr >> Nout;
    
    Xtrain = TMatrix(Nteach, Ninp);
    Ytrain = TMatrix(Nteach, Nout);
    Xtest  = TMatrix(Ntest , Ninp);
    Ytest  = TMatrix(Ntest , Nout);
    
    for(i=0; i<Nteach; i++)
    {
        for(j=0; j<Ninp; j++)
            fr >> Xtrain.arr[i][j];
        for(j=0; j<Nout; j++)
            fr >> Ytrain.arr[i][j];
    }
   
    for(i=0; i<Ntest; i++)
    {
        for(j=0; j<Ninp; j++)
            fr >> Xtest.arr[i][j];
        for(j=0; j<Nout; j++)
            fr >> Ytest.arr[i][j];
    }

    cout << "Chosen task " << filename << endl;
    fr.close(); 
}

void Neurotask :: get_task()
{
    cout << "Quantity of learn data: " << Nteach << '\n';
    cout << "Quantity of test  data: " << Ntest  << '\n';
    cout << "Learn data:\n" << Xtrain << endl;
    cout << "Learn answers:\n" << Ytrain << endl;
    cout << "Test data:\n" << Xtest << endl;
    cout << "Test answers:\n" << Ytest << endl;
}