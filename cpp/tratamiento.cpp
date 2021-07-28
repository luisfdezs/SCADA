/*

    DEFINICION DE LA CLASE TRATAMIENTO

*/
//**************************************************************************************************
#include "tratamiento.h"
//*************************************************************************   CONSTRUCTOR   ********
Tratamiento::Tratamiento(float _Tamb,unsigned char _n,float* _Tf,float* _t){
    //----------------------------------------------------------------------------------------------
    numFases=_n;
    //Indicamos el numero de fases del tratamiento
    //----------------------------------------------------------------------------------------------
    Tinicial=_Tamb;
    //Indicamos la temperatura inicial del horno antes de comenzar el tratamiento
    //----------------------------------------------------------------------------------------------
    Tf = new float[numFases];
    t_s = new float[numFases];
    //Reservamos espacio para guardar los vectores de temperaturas finales y tiempos (en segundos)
    //del tratamiento
    //----------------------------------------------------------------------------------------------
    for (unsigned char i=0; i<numFases; i++){
        Tf[i]=_Tf[i];
        t_s[i]=_t[i];
    }
    //Guardamos los respectivos valores de temperaturas finales y tiempos (en segundos)
    //del tratamiento
}
//*************************************************************************   FDT   ****************
float Tratamiento::Consigna(float _t){
    //----------------------------------------------------------------------------------------------
    unsigned char fase=0;
    //Para conocer la temperatura consigna en un determinado instante _t averiguamos primero la fase
    //del tratamiento donde nos encontramos.
    float t0=0.0f;
    float t=0.0f;
    //Tambien hallamos los valores de tiempo de inicio y fin de dicha fase.
    float T0=0.0f;
    float T=0.0f;
    //Y los valores de temperatura de inicio y fin de dicha fase.
    //----------------------------------------------------------------------------------------------
    while(t<_t){
        t+=t_s[fase];
        fase++;
    }
    //Capturamos el instante de finalizacion de la fase en la que nos encontramos.
    fase--;
    //Capturamos la fase en la que nos encontramos.
    t0=t-t_s[fase];
    //Restamos la duracion de esta fase al instante final para obtener el instante inicial.
    if(!fase)   T0=Tinicial;
    else        T0=Tf[fase-1];
    //La temperatura inicial de esta fase sera la temperatura final de la fase anterior. En el caso
    //de que la fase actual sea la primera, la temperatura inicial sera la temperatura ambiente,
    //entendida como temperatura inicial del horno antes de comenzar el tratamiento.
    T=Tf[fase];
    //Guardamos el valor de temperatura final
    //----------------------------------------------------------------------------------------------
    return T0+(T-T0)/(t-t0)*(_t-t0);
    //Ahora si, devolvemos la temperatura consigna en el instante _t sabiendo que las fases
    //consignadas son lineales (rectas)
}
//*************************************************************************   ¿FIN?   **************
bool Tratamiento::FIN(float _t){
    float duraciontt=0.0f;
    for(unsigned char i=0; i<numFases; i++){
        duraciontt+=t_s[i];
    }
    //Calculamos la duracion total del tratamiento como la suma de la duracion particular de cada fase
    if(_t>duraciontt)
        return true;
    //Devolvemos true si el tiempo que se nos indica por parametro supera la duracion del tratamiento
    else
        return false;
    //Devolvemos false en caso contrario
}
//**************************************************************************************************
