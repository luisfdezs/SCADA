#include "ventana.h"
#include <QApplication>
int main(int n, char **m){
    QApplication aplicacion(n, m);
    Ventana ventana;
    ventana.show();
    return aplicacion.exec();
}
