#include "precompiler.h"
#include "constructors.h"
#include "Perceptons.h"

int main()
{
    usint Players = 1;//phantom layers
    
    Neurotask nt1("Outpoot.txt");
    //Neurotask nt1("Segmentation.txt");
    //nt1.Tobinfile("Tendention.bin");
    //nt1.Frombinfile("Tendention.bin");
    //nt1.get_task();
    
    usint NIEL[Players+2] = { nt1.Ninp, 8, nt1.Nout };//neurons in every layer
    
    //Perceptons p1;
    Perceptons p1(Players+1, NIEL, 0.1);
    //Perceptons p1("newf0.txt");
    
    //p1.get_weights();
    //p1.get_bias();
    
    p1.back_propagation(nt1, 50);
    //p1.neuro_exam(nt1);
    
    //p1.get_weights();
    //p1.get_bias();
    //p1.neuro_to_file("newf1.txt");

    return 0;
}
