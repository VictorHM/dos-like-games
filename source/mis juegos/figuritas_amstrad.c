#include <math.h>
#include <stdlib.h>
#include "..\dos.h"

// TODO necesito configurar texto y posibilidad de interaccion por parte del usuario
#define texWidth 256
#define texHeight 256
#define screenWidth 320
#define screenHeight 200

int main (int argc, char *argv[]) {
  setvideomode( videomode_320x200 );
  setdoublebuffer(1);
  int w = screenWidth;
  int h = screenHeight;

  // TODO teclas disponibles
  // KEY_R para dibujar un rectangulo
  // KEY_E para dibujar una espiral
}