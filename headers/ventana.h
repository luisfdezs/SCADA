/*
    DECLARACION DE LA CLASE PRINCIPAL VENTANA QUE HEREDA DE QMAINWINDOW
*/
//*********************************************************************************
#pragma once                       //Guardian para evitar multiples declaraciones
//*********************************************************************************
#include "tratamiento.h"           //Clases definidas por nosotros en este proyecto
#include "variasgraficas.h"
#include "controlador.h"
#include "planta.h"
//*********************************************************************************
#include <QMainWindow>             //Clases predefinidas por Qt
#include <QTimer>
//*********************************************************************************
#define Tmuestreo 0.1f             //Periodo de muestreo de 0.1seg
#define UMAX 100.0f                //Actuacion maxima (Quemador al 100%)
#define UMIN -100.0f               //Actuacion minima (Ventilador al 100%)
#define RETARDO 1.2f               //Retardo debido al sensor de temperatura
#define Kquemador 65.0f            //Ganancia del sistema del quemador
#define Tquemador 5.0f             //Cte tiempo del sistema del quemador
#define Kventilador 65.0f
//Ganancia del sistema del ventilador cuando el horno esta a 1000 grados celsious
#define Tventilador 5.0f           //Cte tiempo del sistema del ventilador
#define TAMB    20.0f              //Temperatura inicial del horno
//*********************************************************************************
QT_BEGIN_NAMESPACE
namespace Ui {class VentanaPpal;}  //Espacio de nombres Ui donde tenemos nuestra
QT_END_NAMESPACE                   //clase principal
//*********************************************************************************
class Ventana : public QMainWindow{
    Q_OBJECT
//----------------------------------------------------------------//METODOS PUBLICOS
public:
    Ventana(QWidget *parent = nullptr);                         //Constructor
    ~Ventana();                                                 //Destructor
//--------------------------------------------------------//METODOS Y DATOS PRIVADOS
private:
    Ui::VentanaPpal *ui;
    //Puntero 'ui', que apunta a esta misma clase ventana para manejo de widgets
    float consigna,error,actuacion,temperatura;
    //Valores discretos de consigna, error, actuacion y temperatura con los que
    //construiremos las senales de consigna,error,actuacion y temperatura.
    float tiempo_transcurrido;
    //Variable 'tiempo' que usaremos para discretizar las senales
    Tratamiento *tt;
    Controlador *pid;
    Planta *horno;
    //Instancias tt (tratamiento termico), pid y horno que usaremos para armar
    //el sistema de control
    VariasGraficas *grafica;
    //Grafica donde visualizaremos las senales de consigna, temperatura y actuacion
    QTimer *Tm;
    //Temporizador con el que llamaremos al metodo encargado de muestrear las senales
    //a cada periodo de muestreo
    bool enableWindup;
    //booleano con el que permitimos al usuario habilitar y deshabilitar el metodo
    //anti-windup, basado en limitar error acumulado

//------------------------------------------//METODOS ACCIONADOS POR EVENTOS, 'SLOTS'
private slots:
    void ComenzarTratamiento(); //Para comentar el tratamiento
    void MuestrearSenales();    //Para muestrear las senales
    void CambiarKpTiTd();       //Para cambiar los parametros del controlador
};
//*********************************************************************************
