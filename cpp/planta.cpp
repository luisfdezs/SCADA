/*

    DEFINICION DE LA CLASE PLANTA

*/
//**************************************************************************************************
#include "planta.h"
//**************************************************************************************************
#include <math.h>
//************************************************************   CONSTRUCTOR   *********************
Planta::Planta(float _Tamb,float _Tm,float _retardo,
               float _k1,float _T1,
               float _k2_1000ºC,float _T2){
    //----------------------------------------------------------------------------------------------
    Uant=0;//Antes de comenzar el tratamiento todavia no se ha actuado sobre el horno
    Yant=_Tamb;//Y la temperatura del mismo se supone que es la temperatura ambiente
    Tm=_Tm;//Periodo de muestreo
    retardo_s=_retardo;//Retardo en segundos
    K1=_k1;//Ganancia del quemador de gas
    T1aux=_T1;//Cte de tiempo del quemador de gas para masa=0Kg
    K2_1000ºC=_k2_1000ºC;//Ganancia del ventilador cuando la temperatura del horno es de 1000ºC
    T2aux=_T2;//Cte de tiempo del sistema del ventilador para masa=0Kg
    tReal=_Tamb;//Temperatura real del horno inicialmente igual a la temperatura ambiente
    //----------------------------------------------------------------------------------------------
    periodosRetardo=(unsigned char)(retardo_s/Tm);
    fifoTemperaturas = new float[periodosRetardo];
    //Creamos un vector de temperaturas que usaremos como una memoria fifo para simular el retardo,
    //almacenando las temperaturas reales antes de que estas sean detectadas por el sensor
    for(unsigned char i=0; i<periodosRetardo; i++){
        fifoTemperaturas[i]=0.0f;
    }
    //Inicializamos a cero, limpliamos, dicha memoria fifo
    //----------------------------------------------------------------------------------------------
}
//*********************************************************   RELACION ENTRE MASA Y CTES TIEMPO   **
void Planta::PesoPieza(float _masa){
    T1=0.0001*_masa+T1aux;
    T2=0.0002*_masa+T2aux;
}
//*********************************************************   FUNCION DE TRANSFERENCIA   ***********
float Planta::Temperatura(float _act, float _Tamb){
    //----------------------------------------------------------------------------------------------
    float tSensor=0.0f;
    //Distinguimos entre la temperatura real del horno y la detectada por el sensor
    //----------------------------------------------------------------------------------------------
    if(_act>=0)
        tReal=Yant*exp(-Tm/T1)+Uant*K1*(1-exp(-Tm/T1));
    else{
        K2=K2_1000ºC/(1000-_Tamb)*(tReal-_Tamb);
        tReal=Yant*exp(-Tm/T2)+Uant*K2*(1-exp(-Tm/T2));
    }
    //Aplicamos la ecuacion en diferencias que resulta de discretizar la funcion de transferencia
    //de un sistema de primer orden sin retardo por el metodo del mantenedor de orden cero.

    //Si la actuacion es negativa (estamos ventilando), ajustamos la ganancia del ventilador
    //en funcion de la temperatura del horno para simular un sistema de ventilacion mas realista,
    //donde el ventilador es mas eficiente cuando la temperatura del horno es mas alta con respecto
    //a la Tamb y menos eficiente cuanto mas parecidas son estas temperaturas.
    //----------------------------------------------------------------------------------------------
    tSensor=fifoTemperaturas[periodosRetardo-1];
    //Guardamos el ultimo valor de nuestra memoria fifo como la temperatura que detecta el sensor
    for(unsigned char i=1; i<periodosRetardo; i++){
        fifoTemperaturas[periodosRetardo-i]=fifoTemperaturas[periodosRetardo-i-1];
    }
    //Avanzamos todos los valores una posicion
    fifoTemperaturas[0]=tReal;
    //E introducimos en la posicion inicial el valor de temperatura real del horno
    //----------------------------------------------------------------------------------------------
    Yant=tSensor;   //Refrescamos el valor de temperatura para la proxima llamada a este metodo
    Uant=_act;      //Refrescamos el valor de actuacion para la proxima llamada a este metodo
    //----------------------------------------------------------------------------------------------
    return tSensor; //Devolvemos la temperatura captada por el sensor
    //----------------------------------------------------------------------------------------------
}
