/*
    DECLARACION DE LA CLASE CONTROLADOR
*/
//**********************************************************************************
#pragma once                          //Guardian para evitar multiples declaraciones
//**********************************************************************************
class Controlador{
//----------------------------------------------    DATOS ENCAPSULADOS    ----------
private:
    float Tm,                         //Periodo muestreo
          Kp,Ti,Td,                   //Parametros del controlador
          Umax,Umin,                  //Limites de actuacion
          Eant,Eantant,               //Estados anteriores de la senal de error
          Uant,Uantant;               //Estados anteriores de la senal de actuacion

//----------------------------------------------    METODOS PUBLICOS    ------------
public://Metodos publicos
    Controlador(float,float,float,float,float,float);//Constructor
    void setKpTiTd(float,float,float);//Cambia parametros en tiempo de ejecucion
    void WindUp(float&,float,float,float);  //Habilita o deshabilita el windup
    float Actuacion(float,bool); //Funcion de transferencia
};
//----------------------------------------------------------------------------------
