#include "ReverseMatrix.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <ctime>

double* ReadFromFile(std::string name, int size){

	double* Mat = new double[size * size];
    std::ifstream fin;
    fin.open(name.c_str());
    if (!fin.is_open())
    {
        std::cout << "Can't open file!\n";
        //Exception
    }
    else {
        std::cout << "Open Sucsessfull\n";
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
            	if(fin.eof( )){
            		std::cout << "Incorrect size\n";
        			//Exception
				}
                fin >> Mat[i*size + j];
            }
        }
    }
    return Mat;
}

double* Generate(int k, int size) {
    double* Mat = new double[size * size];
    for (int i = 0; i < size ; i++)
    {
        for (int j = 0; j < size ; j++)
        {
            double a;
            switch (k) 
            {
            case 1:
                a = size - std::max(i, j);
                break;
            case 2:
                a = std::max(i, j) + 1;
                break;
            case 3:
                a = (double)abs(i - j);
                break;
            case 4:
                a = 1 / (double)(i + 1 + j + 1 - 1);
                break;
            }
            Mat[(i) * size + j] = a;
        }
    }
    return Mat;
}

int main(int argc, char* argv[]) {
    int size, show, k;
    size = std::atoi(argv[1]);
    show = std::atoi(argv[2]);
    k = std::atoi(argv[3]);
    unsigned int start_time =  clock();
    double* Mat;
	double* Res = new double[size * size];
    if (k == 0)
        Mat = ReadFromFile(argv[4], size);
    else
        Mat = Generate(k, size);
    Matrix* m = new Matrix(Mat, Res, size);
    std::cout.precision(5);
    //m->PrintStart(show);
    m->Gauss(0);
    m->Print(show);
    std::cout <<"Value of Error : "<< m->ErrorValue()<<std::endl;
	free(Mat);
    free(Res);
    unsigned int end_time = clock(); 
    unsigned int time = end_time - start_time;
    std::cout<<"Reverse time is "<< (double)time/1000<<" seconds";
}
