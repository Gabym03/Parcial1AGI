#ifndef MINERVA_H
#define MINERVA_H

#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif
/**
* Se encarga de inicializar (si es necesario) su invocacion
* esta llamada se realizara una sola vez al inicio de la ejecucion
*/
void inicializar_minerva(void);
/**
* Dibuja su entrega de la mierva usando unicamente lineas rectas mediante la invocacion del callback
de glut tal y como se mostro en clase.
*/
void dibujar_minerva();

/**
* retorne su numero de carnet en la invocacion, en base a esto se asignara su nota de forma automatica
*/
const char* get_carnet(void);



#endif /* MINERVA_H */
