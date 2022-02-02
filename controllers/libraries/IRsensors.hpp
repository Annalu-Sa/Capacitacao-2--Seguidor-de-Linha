/*Arquivo responsável pela calibração dos sensores, leitura dos valores calibrados,
pela identificação da posição do robô e leitura das marcações da pista*/

#ifndef IRSENSORS_HPP
#define IRSENSORS_HPP

#include "funcoes_uteis.hpp"
#include "C:/Program Files/Webots/include/controller/cpp/webots/DistanceSensor.hpp"
#include "C:/Program Files/Webots/include/controller/cpp/webots/Robot.hpp"
#include <iostream>
#include "stdio.h"

#define SENSORES_SETUP 20 
#define DIST_TRIGGER 3 //cte para processamento da distancia

using namespace webots;
using namespace std;

string sensor_name[] = {"frontal_sensors", "right_sensor", "left_right_sensor", "left_left_sensor"};

class IRsensor{
    private:
    public:

    int qtde_sensores;
    int index_da_leitura = 0;
    int ultima_leitura = 0;
    //vetores que armazenam os valores max e min para o calibrate:
        double max[20];
        double min[20];

    DistanceSensor *sensores[20];

    IRsensor(int _quantidade_sensores, string _nome_dos_sensores[], Robot *meu_robo)
    {

        qtde_sensores = _quantidade_sensores;
    //instanciação dos sensores
        if(qtde_sensores = 4){
            
            DistanceSensor *right_sensor = robot->getDistanceSensor("ir_right");
            right_sensor -> enable(TIMESTEP);
            sensores[0] = right_sensor;

            DistanceSensor *left_right_sensor = robot->getDistanceSensor("ir_left_right");
            left_right_sensor -> enable(TIMESTEP);
            sensores[1] = left_right_sensor;

            DistanceSensor *left_left_sensor = robot->getDistanceSensor("ir_left_left");
            left_left_sensor -> enable(TIMESTEP);
            sensores[2] = left_left_sensor;

            DistanceSensor *frontal_sensor = robot->getDistanceSensor("ir4");
            frontal_sensor -> enable(TIMESTEP);
            sensores[3] = frontal_sensor;


            maximo[4]= {0, 0, 0, 0};
            minimo[4]={100000, 100000, 100000, 100000};
        }
        else{
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
            void CALIBRARSENSORS(){
                while (robot->step(TIMESTEP) != -1){

                    double time = robot->getTime();

                    while (time < 11){
                        robot->(TIMESTEP);
                        right_sensor.calibrateSensors(time);
                        left_right_sensor.calibrateSensors(time);
                        left_left_sensor.calibrateSensors(time);
                        frontal_sensor.calibrateSensors(time);

                        time = robot->getTime();
                    }
                    cout << "Fim da calibracao" << endl;
                }
            }
            //Ler valores calibrados
            void LERSENSORS{
                for(int ind_sensor; ind_sensor < qtde_sensores; ind_sensor++){
                    sensores[ind_sensor] -> getValue();
                   int medias[ind_sensor] = getMedian(qtde_sensores, sensores[qtde_sensores]);
                }
                //atualiza para última posição de leitura
                index_da_leitura ++;

                //loop da leitura de 0 a 19
                if(index_da_leitura == 20)
                    index_da_leitura = 0;
                            
            }
        //DURANTE A CORRIDA
            //Qual a posição do Robô?
            //Ler Marcações da Pista
        
    }

} 



#endif
