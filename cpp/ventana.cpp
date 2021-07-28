/*

    DEFINICION DE LA CLASE PRINCIPAL

*/
//****************************************************************************
#include "ventana.h"
#include "ui_ventana.h"
//****************************************************************************
#include <QtMath>
//***************************************************   CONSTRUCTOR   ********
Ventana::Ventana(QWidget *parent):QMainWindow(parent),ui(new Ui::VentanaPpal){
    //-----------------------------------------------------------------------
    ui->setupUi(this);
    //Inicializacion del puntero ui
    //-----------------------------------------------------------------------
    Tm = new QTimer(this);
    //Inicializacion del timer Tm para realizar los muestreos de las senales
    //-----------------------------------------------------------------------
    connect(ui->pushButtonStart,SIGNAL(released()),this,SLOT(ComenzarTratamiento()));
    connect(Tm,SIGNAL(timeout()),this,SLOT(MuestrearSenales()));
    connect(ui->pushButtonKs,SIGNAL(released()),this,SLOT(CambiarKpTiTd()));
    //Conexiones entre los desencadenantes de los eventos, las acciones que los
    //desencadenan y quienes reciben o sufren dicho evento (que en nuestro caso
    //sera siempre la ventana)
    //-----------------------------------------------------------------------
    ui->graficas->setNGraficas(4, 1);
    //Asociadas al eje izquierdo graficaremos dos senales: consigna y temperatura
    //Asociada al eje derecho graficaremos la senal de actuacion
    ui->graficas->setRangoYIzquierda(-150,2100);
    //En principio establecemos un rango de valores entre 0 y 2100 para el eje de
    //las temperaturas real y de consigna, suponiendo que el horno siempre se
    //mantendra entre ambos limites (valores que representan grados celsious)
    ui->graficas->setRangoYDerecha(-110, 110);
    //Para el eje de la actuacion, que en principio esta limitada por los valores
    //maximos que pueden dar los actuadores (quemador y ventilador), expresaremos
    //dichas actuaciones como porcentajes, dejando un margen para apreciar la senal
    QColor* pAzul = new QColor("blue");
    QColor* pAzulClaro = new QColor("cyan");
    QColor* pRojo = new QColor("red");
    QColor *pVerde = new QColor("green");
    QColor *pNegro = new QColor("black");
    //Colores para distingir las tres senales
    ui->graficas->setColorGraficaDerecha(0, pVerde);
    ui->graficas->setColorEjeDerecho(pVerde);
    //Pintaremos la senal de actuacion y su respectivo eje de color verde
    ui->graficas->setColorGraficaIzquierda(0, pNegro);
    //Pintaremos la senal horizontal de referencia cero de negro
    ui->graficas->setColorGraficaIzquierda(1, pRojo);
    //Pintaremos la senal de error de rojo
    ui->graficas->setColorGraficaIzquierda(2, pAzul);
    //Pintaremos la senal de referencia o consigna de azul oscuro
    ui->graficas->setColorGraficaIzquierda(3, pAzulClaro);
    //Pintaremos la senal de temperatura de azul claro
    ui->graficas->setColorEjeIzquierdo(pAzulClaro);
    //Pintaremos el eje de las temperaturas de azul claro
    //-----------------------------------------------------------------------
}
//***************************************************   DESTRUCTOR   *********
Ventana::~Ventana(){
    delete ui;
    //Cuando se cierre la ventana se destruira, y el puntero asociado ui
    //que apunta a ella se borrara
}
//***************************************************   COMENZAR TT   ********
void Ventana::ComenzarTratamiento(){
    //-----------------------------------------------------------------------
    ui->graficas->borrar();
    tiempo_transcurrido=0.0f;
    //Damos la posibilidad de reiniciar el tratamiento poniendo el tiempo a
    //cero y borrando la grafica si se vuelve a pulsar el boton de start
    //una vez que el tratamiento esta en marcha
    //-----------------------------------------------------------------------
    QString texto=ui->fases->text();
    QStringList fases = texto.split(";");
    int numFases = fases.length();
    float T[numFases];
    float t[numFases];
    for(unsigned char i=0; i<numFases; i++){
        QStringList T_t=fases[i].split(",");
        T[i]=T_t[0].toFloat();
        t[i]=T_t[1].toFloat();
    }
    //Para poder especificar todas las fases del tratamiento con sus
    //correspondientes valores de temperatura y tiempo en un solo lineEdit.

    //Se supone que el operario conoce como especificar el tratamiento
    //separando las fases con el caracter ';'

    //Tambien se supone que el operario conoce la forma de especificar cada
    //una de las fases:
    //Primero se escribe el valor de temperatura en grados celsious y a
    //continuacion el tiempo en segundos, separando ambos valores con una coma ','
    //-----------------------------------------------------------------------
    tt = new Tratamiento(TAMB,numFases,T,t);
    //Llamamos al constructor del tratamiento, pasandole la temperatura
    //ambiente (que es la temperatura inicial del horno),el numero de fases
    //de las que se compone y las posiciones de inicio de los vectores de
    //temperaturas y tiempos
    pid = new Controlador(Tmuestreo,
                          ui->Kp->text().toFloat(),
                          ui->Ti->text().toFloat(),
                          ui->Td->text().toFloat(),
                          UMAX,UMIN);
    //Llamamos al constructor del controlador pid, pasandole el periodo de
    //muestreo y los parametros del controlador (Kp,Ti y Td), aunque
    //dichos valores podran ser modificados durante el tratamiento
    //Tambien le pasamos los limites de actuacion
    horno = new Planta(TAMB,Tmuestreo,RETARDO,Kquemador,Tquemador,Kventilador,Tventilador);
    //Llamamos al constructor del horno, pasandole el periodo de muestreo, los
    //parametros de los dos sistemas (ganancias y ctes tiempo) y el retardo
    //debido al sensor.
    horno->PesoPieza(ui->peso->text().toFloat());
    //Indicamos el peso de la pieza al metodo de la clase Planta para que ajuste las
    //constantes de tiempo de los dos sistemas proporcionalmente a dicha masa.
    //-----------------------------------------------------------------------
    Tm->start(1000*Tmuestreo);
    //Todo listo, arrancamos el temporizador y comienza el tratamiento
    //-----------------------------------------------------------------------
}
//***************************************************   CAMBIAR KpTiTd   *****
void Ventana::CambiarKpTiTd(){
    pid->setKpTiTd(ui->Kp->text().toFloat(),
                   ui->Ti->text().toFloat(),
                   ui->Td->text().toFloat());
    //Permitimos el cambio de los parametros Kp,Ti y Td incluso durante la
    //ejecucion del tratamiento.
}
//***************************************************   MUESTREAR SENALES   **
void Ventana::MuestrearSenales(){
    //-----------------------------------------------------------------------
    tiempo_transcurrido+=Tmuestreo;
    //Aumentamos el tiempo transcurrido en un periodo de muestreo
    //-----------------------------------------------------------------------
    ui->graficas->setRangoX(tiempo_transcurrido-10, tiempo_transcurrido+1);
    //Actualizamos el rango de vision de la grafica conforme a los valores
    //actuales de tiempo y temperatura de consigna para mayor comodidad
    //de visualizacion.
    //-----------------------------------------------------------------------
    if(ui->checkWup->isChecked())
        enableWindup=true;
    else
        enableWindup=false;
    //Permitimos que el operario pueda habilitar y deshabilitar los distintos
    //metodos anti-windup incluso durante la ejecucion del tratamiento
    //-----------------------------------------------------------------------
    if(!tt->FIN(tiempo_transcurrido)){
        //Mientras no haya finalizado el tratamiento...
        consigna=tt->Consigna(tiempo_transcurrido);
        error=consigna-temperatura;
        actuacion=pid->Actuacion(error,enableWindup);
        temperatura=horno->Temperatura(actuacion,TAMB);
        //Calculamos los valores de las senales en este instante
        double valoresIzquierda[4], valoresDerecha;
        valoresIzquierda[0] = 0.0f;
        //linea horizontal con valor cero para tener una referencia visual del error
        valoresIzquierda[1] = error;                        //senal de error
        valoresIzquierda[2] = consigna;                     //senal de consigna
        valoresDerecha = actuacion;                         //senal de actuacion
        valoresIzquierda[3] = temperatura;                  //senal de temperatura
        ui->graficas->elegirSenales(ui->Senal0->isChecked(),ui->SenalE->isChecked(),
                                    ui->SenalR->isChecked(),ui->SenalU->isChecked(),
                                    ui->SenalY->isChecked());
        //Permitimos que el operario elija en tiempo de ejecucion que senales visualizar
        ui->graficas->anadePuntos(tiempo_transcurrido, valoresIzquierda, &valoresDerecha);
        //Anadimos dichos valores a sus correspondientes lineas de puntos para
        //visualizar las senales
    }
    else
        Tm->stop();
        //Paramos el temporizador cuando finaliza el tratamiento para no seguir
        //muestreando.
}
//****************************************************************************
