//Arquivo que contém funções que são utilizadas em outros arquivos

#ifndef FUNCOES_UTEIS_HPP
#define FUNCOES_UTEIS_HPP

double map(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


double constrain (double x, double min, double max)
{
    if (x > max) x = max;
    if (x < min) x = min;
    return x;
}

float max(float a, float b)
{
    if(a > b)
        return a;
    else
        return b;
}

double getMedian(int n, double arr[])
{
    for(int i = 0, i < n, i++){
        double numerador += arr[i] ;
    }
    double value = numerador / n ;
}

#endif