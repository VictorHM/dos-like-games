#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../dos.h"

struct color_val {
  int r;
  int g;
  int b;
};
struct color_val colores[] = {
  {0, 0, 0},
  {255, 255, 255},
  // Rango del rojo puro al verde puro.
  {255, 0, 0},  // Rojo [2]
  {0, 255, 0}, // Azul [3]
  {255, 255, 0} // Amarillo [4]
};
struct pointXY {
  int x;
  int y;
};

enum COORD {
  H,
  V,
};

void setPaletteToColor(void) {
  // Set first colors to black and white
  struct color_val base = colores[0];
  setpal(0, base.r, base.g, base.b);
  base = colores[1];
  setpal(1, base.r, base.g, base.b);
  // Set the rest of ranges based on index
  struct color_val rojo = colores[2];
  struct color_val azul = colores[3];
  struct color_val color_to_set;
  for(int i = 0; i < 255; ++i){
    color_to_set.r = (rojo.r-i) % 255;
    color_to_set.g = (rojo.r - i) % 255;
    setpal(i+2, color_to_set.r, color_to_set.g, color_to_set.b);
  } 
}
// firstCorner: lower left corner of the square
// endCorner: the right upper corner of the square
// TODO poner en letras en la pantalla las coordenadas en cada dibujo del
// cuadrado. Ahora mismo solo dibuja la H no el V. Quiero ver que coordenadas
// tiene cuando no aparece dibujado.
void drawStripSquare(struct pointXY firstCorner, struct pointXY endCorner, enum COORD hv) {
  struct pointXY point = firstCorner;
  int color = 100;
  int colorShifting = 0;

  // Portion of the square to define stripes size.
  int stripSizeX = (endCorner.x - firstCorner.x) % 4;
  int stripSizeY = (endCorner.y - firstCorner.y) % 4;
  int stripSize;

  int firstCornerCoord2;
  
  // For selecting where the stripes go
  int endpoint1; // by default this is endCorner.x
  int endpoint2; // by default this is point.y

  endpoint1 = endCorner.x;
  endpoint2 = endCorner.y;
  int coord1 = point.x;
  int coord2 = point.y;

  if (hv == (H)) {
    firstCornerCoord2 = firstCorner.y;
    stripSize = stripSizeX;
  } else {
    endpoint1 = endCorner.y;
    endpoint2 = endCorner.x;
    firstCornerCoord2 = firstCorner.x;
    stripSize = stripSizeY;
  }

  // TODO esto deberia usar las coords del cuadrado, y solo las de
  // color cuando haya que pintar. El loop va con coords normales, y dentro el
  // pintar en color o no, es un chequeo dentro.
  for(; coord1 <= endpoint1; ++coord1) {
    for (; coord2 <= endpoint2; ++coord2) {
      putpixel(coord1, coord2, color);
    }
    // Reinit the Y position
    coord2 = firstCornerCoord2;
    ++colorShifting;

    
    // This is to check how it is the XY set in the system. Each line with 
    // the same X will have the same color. After Dx == 10 we shift the colors
    if (colorShifting > stripSize) {
      colorShifting = 0;
      color += 30;
      if(color > 255) color = 0;
    }
  }
}

void drawMySquare(struct pointXY firstCorner, struct pointXY endCorner) {
  struct pointXY point = firstCorner;
  int color = 100;
  for(; point.x <= endCorner.x; ++point.x) {
    for (; point.y <= endCorner.y; ++point.y) {
      putpixel(point.x, point.y, color);
    }
    // Reinit the Y position
    point.y = firstCorner.y;
  }
}

void refreshPalette(int shift) {
  // Loop goes through every entry of the palette to set the
  // color for each one. Later we can use the index to select that color.
  // Definir rango de colores
  // Gama amarillo: desde (255, 255, 0) hasta verde (0, 255, 0)
  int colorVal = shift;
  for( int i = 0; i < 256; ++i ) {
    // The values for color have to be between 0 and 63.
    // If bigger, we reset it.
    if (colorVal == 63) colorVal = 0;
    setpal( i, colorVal, 63-colorVal, colorVal );
    ++colorVal;
  } 
}

// Reset palette to default value for shift = 0
void resetPalette(void) {
  refreshPalette(0);
}

int main(int argc, char *argv[]) {
  setvideomode( videomode_320x200 );
  // Initialize the videobuffer setting the screen palette.
  if(argc == 2) {
    int shift = atoi(argv[1]);
    refreshPalette(shift);
  } else {
    setPaletteToColor();
  }
  
  int w = 320;
  int h = 200;

  struct pointXY initPos = {150, 50};
  struct pointXY endPos = {200, 100};

  // TODO let's try using double buffer. To see how it works.
  setdoublebuffer( 1 );
  uint8_t* screen = screenbuffer();
  // Let's draw a cube and rotate it
  enum COORD VorH = (H);
  printf("Init point: (%d, %d)\n", initPos.x, initPos.y);
  printf("End point: (%d, %d)\n", endPos.x, endPos.y);
  // Main loop
  while(!shuttingdown()) {
    clearscreen();
    //drawStripSquare(initPos, endPos, VorH);
    drawMySquare(initPos, endPos);

    if (keystate(KEY_UP)) {
      initPos.x -= 1;
      endPos.x -= 1;
      // TODO circle back to 255 if less than 0
      {
        if (initPos.x < 0) initPos.x = 255;
        if (endPos.x < 0) endPos.x = 255;
      }
      printf("KEY_UP\n");
      printf("Init point: (%d, %d)\n", initPos.x, initPos.y);
      printf("End point: (%d, %d)\n", endPos.x, endPos.y);
      printf("\n");
    }

    if (keystate(KEY_DOWN)) {
      initPos.x += 1;
      endPos.x += 1;
      // TODO circle back to 0 if bigger than 255
      {
        if (initPos.x > 255) initPos.x = 0;
        if (endPos.x > 255) endPos.x = 0;
      }
      printf("KEY_DOWN\n");
      printf("Init point: (%d, %d)\n", initPos.x, initPos.y);
      printf("End point: (%d, %d)\n", endPos.x, endPos.y);
      printf("\n");
    }

    if (keystate(KEY_LEFT)) {
      initPos.y -= 1;
      endPos.y -= 1;
      // TODO circle back to 255 if less than 0
      {
        if (initPos.y < 0) initPos.y = 255;
        if (endPos.y < 0) endPos.y = 255;
      }
      printf("KEY_LEFT\n");
      printf("Init point: (%d, %d)\n", initPos.x, initPos.y);
      printf("End point: (%d, %d)\n", endPos.x, endPos.y);
      printf("\n");
    }
    
    if (keystate(KEY_RIGHT)) {
      initPos.y += 1;
      endPos.y += 1;
      // TODO circle back to 255 if less than 0
      {
        if (initPos.y > 255) initPos.y = 0;
        if (endPos.y > 255) endPos.y = 0;
      }
      printf("KEY_DOWN\n");
      printf("Init point: (%d, %d)\n", initPos.x, initPos.y);
      printf("End point: (%d, %d)\n", endPos.x, endPos.y);
      printf("\n");
    }

    screen = swapbuffers();
    if (keystate( KEY_ESCAPE )) {
      break;
    }
  }

  return(0);
}
