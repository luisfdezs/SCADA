/*
    DECLARACION DE LA CLASE TRATAMIENTO
*/
//*********************************************************************************
#pragma once                        //Guardian para evitar multiples declaraciones
//*********************************************************************************
class Tratamiento{
//----------------------------------------------    DATOS ENCAPSULADOS    ---------
private:
    unsigned char numFases;         //Numero de fases del tratamiento
    float Tinicial;                 //Temperatura del horno antes de comenzar
    float *Tf;                      //Temperaturas finales de cada fase
    float *t_s;                     //Tiempos (en segundos) de cada fase
//----------------------------------------------    METODOS PUBLICOS    -----------
public:
    Tratamiento(float,unsigned char,float[],float[]);
    bool FIN(float);                //Devuelve true si el tratamiento ha finalizado
    float Consigna(float);          //Funcion de transferencia
};
//---------------------------------------------------------------------------------
