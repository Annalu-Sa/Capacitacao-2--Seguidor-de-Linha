/*Arquivo responsável pela calibração dos sensores, leitura dos valores calibrados,
pela identificação da posição do robô e leitura das marcações da pista*/

#ifndef IRSENSORS_HPP
#define IRSENSORS_HPP

#include "funcoes_uteis.hpp"
#include "C:/Program Files/Webots/include/controller/cpp/webots/DistanceSensor.hpp"
#include "C:/Program Files/Webots/include/controller/cpp/webots/Robot.hpp"
#include <iostream>
#include "stdio.h"

#define QTDE_SENSORES 8 //presto possuim 8 sensores
#define DIST_TRIGGER 3 //cte para processamento da distancia

using namespace webots;
using namespace std;

string sensor_name[] = {"frontal_sensors", "right_sensor", "left_right_sensor", "left_left_sensor"};

class IRsensor
{
    private:
    public:

    //vetores que armazenam os valores max e min para o calibrate:
        double max[20];
        double min[20];

    
}
//ANTES DA CORRIDA
    //Calibrar Sensores
    void CALIBRATE(){
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
//DURANTE A CORRIDA
    //Qual a posição do Robô?
    //Ler Marcações da Pista

#endif
