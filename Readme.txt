Rules of making task file:

fr >> Nteach;
fr >> Ntest;
fr >> Ninp;
fr >> Nout;
    
for(i=0; i<Nteach; i++)
{
    for(j=0; j<Ninp; j++)
        fr >> xtrain[i]->column[j];
    for(j=0; j<Nout; j++)
        fr >> ytrain[i]->column[j];
}
for(i=0; i<Ntest; i++)
{
    for(j=0; j<Ninp; j++)
        fr >> xtest[i]->column[j];
    for(j=0; j<Nout; j++)
       fr >> ytest[i]->column[j];
}