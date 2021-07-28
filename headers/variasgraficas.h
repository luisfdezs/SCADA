#pragma once
#include <QWidget>
#include <QChartView>
#include <QLineSeries>
#include <QChart>
#include <QColor>
#include <QValueAxis>
class VariasGraficas : public QWidget {
    // Clase para visualización de varias gráficas superpuestas en una ventana. Hay gráficas asociadas
    // a un rango de valores que se muestra en el eje vertical izquierdo y hay gráficas asociadas a
    // un rango de valores que se muestra en el eje vertical derecho.
    // Derivada de QWidget, que es clase base también para el resto de widgets QPushButton, QLabel, etc
    QtCharts::QLineSeries ** puntosIzquierda, ** puntosDerecha;
    // Punteros a matrices de punteros a objetos, cada un de ellos con la colección de puntos de
    // cada gráfica que se va a representar uniéndolos mediante líneas
    int nGraficasIzquierda, nGraficasDerecha;
    // Número de gráficas asociadas al eje de la izquierda y al de la derecha
    QtCharts::QChart * grafica;
    // Puntero a un objeto de la clase QChart donde se pueden mostrar varias
    // colecciones de puntos
    QtCharts::QChartView * visor;
    // Puntero a un objeto que permite visualizar las gráficas en la ventana
    QtCharts::QValueAxis * ejeX, * ejeYIzquierdo, * ejeYDerecho;
    // Punteros a objetos para manejar los ejes
public://Metodos publicos
    explicit VariasGraficas(QWidget *parent = nullptr);
    // Constructor al que se le pasa la dirección del objeto que representa al widget que contiene a
    // este componente
    void setNGraficas(int nGraficasIzquierda, int nGraficasDerecha);
    // Para establecer cuántas gráficas están asociadas al eje izquierdo y cuántas al derecho
    void setColorGraficaIzquierda(int nGrafica, QColor * pColor);
    void setColorGraficaDerecha(int nGrafica, QColor * pColor);
    // Establece el color de cada gráfica. Se numeran a partir de 0.
    void setColorEjeIzquierdo(QColor * pColor);
    void setColorEjeDerecho(QColor * pColor);
    // Establece el color de cada eje
    void setRangoX(double xMin, double xMax);
    void setRangoYIzquierda(double yMin, double yMax);
    void setRangoYDerecha(double yMin, double yMax);
    // Establece el rango de valores utilizado en el eje horizontal y en los verticales
    void anadePuntos(double x, double * valoresIzquierda, double * valoresDerecha);
    // Añade un nuevos puntos, todos con la misma coordenada X. En valoresIzquierda se pasa la
    // dirección de memoria donde están todos los valores para las gráficas asociadas al eje izquierdo.
    // En valoresDerecha para las del eje derecho.
    void setGeometry(const QRect & rectangulo);
    // Redefine el mismo método de la clase base para que la gráfica se inscriba dentro del rectángulo
    // representado por el objeto pasado por referencia
    void borrar();//Para borrar la grafica
    void elegirSenales(bool,bool,bool,bool,bool);//Para elegir que senales queremos visualizar
};
