/*Arquivo responsável pela calibração dos sensores, leitura dos valores calibrados,
pela identificação da posição do robô e leitura das marcações da pista*/

#ifndef IRSENSORS_HPP
#define IRSENSORS_HPP
//CAPINAR O CODIGO IRSENSORS
#include "funcoes_uteis.hpp"
#include "C:/Program Files/Webots/include/controller/cpp/webots/DistanceSensor.hpp"
#include "C:/Program Files/Webots/include/controller/cpp/webots/Robot.hpp"
#include <iostream>
#include "stdio.h"

#define QTDE_SENSORES_SETUP 15 
#define DIST_TRIGGER 3 //cte para processamento da distancia

using namespace webots;
using namespace std;

string sensor_name[] = {"frontal_sensors", "right_sensor", "left_right_sensor", "left_left_sensor"};

class IRsensor
{
    private:
    public:

    int qtde_sensores;
    double leituras_calibradas[QTDE_SENSORES_SETUP];
    int index_da_leitura = 0;
    int ultima_leitura = 0;
    double medias[QTDE_SENSORES_SETUP];

    //Criacao dos vetores que armazenam os valores max e min para o calibrate:
    double maximo[20] = VECcreator(20; maximo[20]);
    double minimo[20] = VECcreator(20; minimo[20]);

    DistanceSensor *sensores[20];

    IRsensor(int _quantidade_sensores, string _nome_dos_sensores[], Robot *meu_robo)
    {

        qtde_sensores = _quantidade_sensores;
    //instanciação dos sensores
        for(int i = 0; i < qtde_sensores; i++)
        {
            sensores[i] = meu_robo->getDistanceSensor(_nome_dos_sensores[i]);
            sensores[i]->enable(TIMESTEP);
            maximo[i]=0;
            minimo[i]=100000; 
        } 
    }
    //ANTES DA CORRIDA
    
    //Calibrar Sensores por 11seg
    void CALIBRARSENSORS(double time)
    {
                
        //Esses 3seg é um delay para evitar ruídos no Webots
        if(time > 3)
        {
            while (robot->step(TIMESTEP) != -1)
            {
                double time = robot->getTime();

                while (time < 11)
                {
                    for(int i = 0; i < qtde_sensores; i++)
                    {
                        time = robot->getTime();
                        sensores[i].calibrateSensors(time);    
                    }
                }
                cout << "Fim da calibracao" << endl;
            }
        }
    }
    //Leitura dos sensores sem a limitacao 'constrain'
    void LERSENSORS_puro()
    {

        //Lendo os valores capturados por cada sensor
        for(int ind_sensor = 0; ind_sensor < qtde_sensores; ind_sensor++)
        {
            sensores[ind_sensor] -> getValue();
                
            //Fazendo a media desses valores capturados
            int VECintermedio[ind_sensor][index_da_leitura] = getMedian(qtde_sensores, sensores[qtde_sensores]);

            medias[ind_sensor] = VECintermedio[ind_sensor][index_da_leitura];
        }
               
        //Atualiza para ultima posicao de leitura
        index_da_leitura ++;

        //loop da leitura de 0 a 19
        if(index_da_leitura == 20)
            index_da_leitura = 0;
                            
    }

    //função auxiliar pro calibrate sensors
    void AUX_CALIBRATE_SENSORS()
    {
        //Comparar valor_medio,que pegamos no LERSENSORS_puro, e definir um maximo/minimo
        //pra cada sensor. 
        for (int i = 0; i < qtde_sensores; i++)
        { 
            if (medias[i] < minimo[i]) minimo[i] = medias[i];
            if (medias[i] > maximo[i]) maximo[i] = medias[i];
        }
    }

    //Leitura dos Sensores com a limitação do 'constrain', que servira para termos certeza que
    //os valores que queremos, foram, de fato, limitados
    double LERSENSORS_lim()
    {
        for (int i = 0; i<qtde_sensores; i++)
        { 
            leituras_calibradas[i] = constrain(map(medias[i], minimo[i], maximo[i], 0, 1000), 0, 1000);
        }
        if (qtde_sensores == 1) 
            return leituras_calibradas[0];
        else 
            return -1;

    }
    //DURANTE A CORRIDA
    
    //Qual a posicao do Robo?(pegar posicao com metodo ReadLine)
    double readLine()
    {
        unsigned char i;
        unsigned long avg; // Isso é para o peso total, que é long
                            // Antes da divisão
        unsigned int sum; // Isso é para o denominador, que é <= 64000

        readCalibrated();   //readLine trabalha com as leituras calibradas de 0 a 1000

        avg = 0;
        sum = 0;
        int on_line = 0;

        for(i=0; i<qtde_sensores; i++)
        {
            double leitura = leituras_calibradas[i];
            if(leitura > 600) on_line = 1;        //Valor arbitrario, verificar durante simulacao!
            if(leitura > 250)
            {
                avg += (long)(leitura) * (i * 1000);
                sum += leitura;
            }
        }
        if (!on_line) //Se o robo nao estiver vendo a linha 
        {
            //Se ele sair da linha pela direita, retorna o valor minimo pra reencontrar a linha na esquerda
             if(ultima_leitura < (qtde_sensores-1)*1000/2)
                return 0;
            //Se ele sair da linha pela esquerda, retorna o valor máximo pra reencontrar a linha na direita
             else
                return (qtde_sensores-1)*1000;
        }
        if(sum != 0) 
            ultima_leitura = avg/sum;
        else 
            ultima_leitura = 0;
        return ultima_leitura;
    }      
};
//Ler Marcações Direita e Esquerda da Pista

double last_giro_esq, last_giro_dir = 0;

//Usado para ler as marcações no lado esquerdo da pista e sinalizar as curvas.
bool readLeft(double giro_esq_atual, double giro_dir_atual, double leitura_esq_dir,  double leitura_esq_esq, int mapping_path)
{

    if (((leitura_esq_dir > 700) || (leitura_esq_esq > 700))      // está vendo a marcação
        && giro_esq_atual - last_giro_esq > DIST_TRIGGER && giro_dir_atual - last_giro_dir > DIST_TRIGGER)
    {   
        // Passou pelo menos 3 cm da marcação anterior
        last_giro_esq = giro_esq_atual;
        last_giro_dir = giro_dir_atual;
        mapping_path += 1;
        return true;
    }
    return false;
};

bool readRight(double giro_esq_atual, double giro_dir_atual, double leitura_dir, int mapping_path)
{
    if ((leitura_dir > 700) && (giro_esq_atual - last_giro_esq) > DIST_TRIGGER && (giro_dir_atual - last_giro_dir) > DIST_TRIGGER)
    {
        last_giro_esq = giro_esq_atual;
        last_giro_dir = giro_dir_atual;
        mapping_path += 1;
        return true;
    }
    return false;
};

#endif
