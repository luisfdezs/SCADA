/*

    DEFINICION DE LA CLASE CONTROLADOR

*/
//**************************************************************************************************
#include "controlador.h"
//**************************************************************************************************
#include <math.h>
//*******************************************************   CONSTRUCTOR   **************************
Controlador::Controlador(float _Tm,float _Kp,float _Ti,float _Td,float _Umax,float _Umin){
    Tm=_Tm;
    Kp=_Kp;
    Ti=_Ti;
    Td=_Td;
    Umax=_Umax;
    Umin=_Umin;
    Eantant=0.0f;
    Eant=0.0f;
    Uantant=0.0f;
    Uant=0.0f;
}
//*******************************************************   setKPTiTd   ****************************
void Controlador::setKpTiTd(float _Kp,float _Ti, float _Td){
    Kp=_Kp;
    Ti=_Ti;
    Td=_Td;
}
//*******************************************************   WIND-UP   ******************************
void Controlador::WindUp(float & _Eacum,float _Uantant,float _Umax,float _Umin){
    if((_Uantant+_Eacum)>=_Umax || _Uantant+_Eacum<=(_Umin))
        _Eacum=0.0f;
    //Cuando el sistema se satura ponemos a cero el error acumulado
}
//*******************************************************   FUNCION DE TRANSFERENCIA   *************
float Controlador::Actuacion(float _e,bool _windup){
    //----------------------------------------------------------------------------------------------
    float U;    //Manejaremos el valor de actuacion que tenemos que devolver en esta variable local
    //----------------------------------------------------------------------------------------------
    float Ki=Kp/Ti; //Calculo de Ki en base a Kp y Ti
    float Kd=Td*Kp; //Calculo de Kd en base a Kp y Td
    //----------------------------------------------------------------------------------------------
    float c1=2*Tm*Kp+Ki*pow(Tm,2)+4*Kd;
    float c2=2*Ki*pow(Tm,2)-8*Kd;
    float c3=-2*Tm*Kp+Ki*pow(Tm,2)+4*Kd;
    float Eacum=(c1*_e+c2*Eant+c3*Eantant)/(2*Tm);
    //Calculo del error acumulado en base a el metodo de la aproximacion mediante discretizacion
    //integral, trapezoidal o de Tustin
    //----------------------------------------------------------------------------------------------
    U=Uantant+Eacum;   
    //Calculo de la actuacion en base a la actuacion ante-anterior y el error acumulado que acabamos
    //de calcular con este metodo de Tustin
    //----------------------------------------------------------------------------------------------
    if(U>Umax)
        U=Umax;   //No permitimos que la actuacion del quemador supere su maxima
    if(U<Umin)
        U=Umin;   //No permitimos que la actuacion del ventilador supere su maxima
    if(_windup)
        WindUp(Eacum,Uantant,Umax,Umin);
    //Solamente aplicamos el metodo anti-windup si el operario lo habilita mediante su interfaz
    //----------------------------------------------------------------------------------------------
    Eantant=Eant;
    Eant=_e;
    Uantant=Uant;
    Uant=U;             //Refrescamos los valores
    //----------------------------------------------------------------------------------------------
    return U;           //Devolvemos la actuacion
}
