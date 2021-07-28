/*
    DECLARACION DE LA CLASE PLANTA

    En realidad con esta clase simulamos los dos sistemas diferentes que se dan
    con dos actuadores diferentes:
        -El quemador de gas
        -El ventilador
*/
//*********************************************************************************
#pragma once//Guardian para evitar multiples declaraciones
//*********************************************************************************
class Planta{
//----------------------------------------------    DATOS ENCAPSULADOS    ---------
private:
    float K1,K2_1000ºC,K2,        //ganancias
          T1,T2,                  //constantes de tiempo
          T1aux,T2aux,
          //constantes de tiempo para tratamientos con el horno vacio (masa cero)
          tReal,
          //Temperatura real del horno, que es distinta de la que detecta el sensor
          retardo_s,              //retardo en segundos
          Tm,                     //periodo de muestreo
          Yant,                   //valor anterior de la salida (senal de temperatura)
          Uant;                   //valor anterior de la entrada (senal de actuacion)
    float *fifoTemperaturas;      //Fifo de temperaturas para gestionar el retardo
    unsigned char periodosRetardo;//Para conocer el tamaño de dicha fifo
//------------------------------------------------    METODOS PUBLICOS    ----------
public:
    Planta(float,float,float,float,float,float,float);//Constructor
    void PesoPieza(float);         //Relaciona la masa con las constantes de tiempo
    float Temperatura(float,float);//Funcion de transferencia
};
//---------------------------------------------------------------------------------
