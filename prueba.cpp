#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "genann.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <string.h>
#include <string>
#include <map>
#include <vector>
using namespace std;

struct IMAGEN {
    int ancho;
    int alto;
    int canales; // r,g,b,a
    vector<int> pixeles;
};

IMAGEN letraA;
IMAGEN letraB;
IMAGEN letraB2;

bool cargarImagen(char *ruta,IMAGEN &v) {
    
    int x,y,n;
    unsigned char *data = stbi_load(ruta, &x, &y, &n, 0);
    
    if (data) {
        
        v.ancho = x;
        v.alto = y;
        v.canales = n;
        int total = x*y*n;
        
        for(int i = 0; i < total; i++) {
            v.pixeles.push_back(data[i]);
        }
        
        stbi_image_free(data);
        return true;        
        
    }else{
        stbi_image_free(data);
        return false;
    }   
}

void pintarImagen(HWND &hwnd,IMAGEN &v,float x,float y) {
    
  PAINTSTRUCT ps;
  RECT r;

  HDC hdc = BeginPaint(hwnd, &ps);

  GetClientRect(hwnd, &r);

    for(int yy = 0; yy < v.alto; yy++) {
        
        for(int xx = 0; xx < v.ancho; xx++) {
        
            int r = v.pixeles[yy*v.ancho*v.canales+xx*v.canales+0];
            int g = v.pixeles[yy*v.ancho*v.canales+xx*v.canales+1];
            int b = v.pixeles[yy*v.ancho*v.canales+xx*v.canales+2];
            
            SetPixel(hdc, x+xx, y+yy, RGB(r, g, b));
        
        }
    }
    
    EndPaint(hwnd, &ps);
}

int prepararInputs(double* &inputs,IMAGEN &letraA) {
                
                inputs = new double [letraA.alto*letraA.ancho*letraA.canales];
                
                for(int yy = 0; yy < letraA.alto; yy++) {
                    
                    for(int xx = 0; xx < letraA.ancho; xx++) {
                    
                        double r = letraA.pixeles[yy*letraA.ancho*letraA.canales+xx*letraA.canales+0] / 255;
                        double g = letraA.pixeles[yy*letraA.ancho*letraA.canales+xx*letraA.canales+1] / 255;
                        double b = letraA.pixeles[yy*letraA.ancho*letraA.canales+xx*letraA.canales+2] / 255;
                        
                        inputs[yy*letraA.ancho*letraA.canales+xx*letraA.canales+0] = r;
                        inputs[yy*letraA.ancho*letraA.canales+xx*letraA.canales+1] = g;
                        inputs[yy*letraA.ancho*letraA.canales+xx*letraA.canales+2] = b;
                        
                    }
                }
                
    return letraA.ancho;
}

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    switch(Message) {
        case WM_PAINT:
        {
             //pintarImagen(hwnd,letraA,50,50);
              pintarImagen(hwnd,letraB2,100,250);
        }       
        break;
        case WM_CREATE: 
        {
            
            if( cargarImagen((char*)"0.jpg",letraA) == false ) {
                printf("0.jpg no cargo.\n");
            }
            
            if( cargarImagen((char*)"1.jpg",letraB) == false ) {
                printf("1.jpg no cargo.\n");
            }
            
            if( cargarImagen((char*)"3.jpg",letraB2) == false ) {
                printf("3.jpg no cargo.\n");
            }
            
                printf("GENANN example 1.\n");
                printf("Train a small ANN to the XOR function using backpropagation.\n");
            
                /* This will make the neural network initialize differently each run. */
                /* If you don't get a good result, try again for a different result. */
                srand(time(0));
                
                int CapasOcultas = letraA.ancho*2;
                int Neuronas = 100;
                

                const double output[][2] = { // a,b
                    {1,0}, 
                    {0,1}  
                };
                int i;
            
                double *LETRA_A_INPUT = NULL;
                prepararInputs(LETRA_A_INPUT,letraA);
                
                double *LETRA_B_INPUT = NULL;
                prepararInputs(LETRA_B_INPUT,letraB);  
            
                double *LETRA_B2_INPUT = NULL;
                prepararInputs(LETRA_B2_INPUT,letraB2);  
                /* New network with 2 inputs,
                 * 1 hidden layer of 2 neurons,
                 * and 1 output. */
                genann *ann = genann_init(letraA.pixeles.size(), CapasOcultas, Neuronas, 2);
                
                /* Train on the four labeled data points many times. */
                
                for (i = 0; i < 500; ++i) {
                    genann_train(ann, LETRA_A_INPUT, output[0], 3);
                    genann_train(ann, LETRA_B_INPUT, output[1], 3);

                }
            
                /* Run the network and see what it predicts. */
                
                double *r = NULL;
                prepararInputs(r,letraB2);
                
                double *resultado = (double*)genann_run(ann, r);
                
                printf("Output for Letra A is [%1.f,%1.f].\n", resultado[0], resultado[1] );
            
                genann_free(ann);
                
                if(r!=NULL) {
                    delete[] r;
                    r = NULL;
                }
                
                if(LETRA_A_INPUT!=NULL) {
                    delete[] LETRA_A_INPUT;
                    LETRA_A_INPUT = NULL;
                }
                
                if(LETRA_B_INPUT!=NULL) {
                    delete[] LETRA_B_INPUT;
                    LETRA_B_INPUT = NULL;
                }
                
                if(LETRA_B2_INPUT!=NULL) {
                    delete[] LETRA_B2_INPUT;
                    LETRA_B2_INPUT = NULL;
                }
                
        }
        break;
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hwnd, Message, wParam, lParam);
    }
    return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc; /* A properties struct of our window */
    HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
    MSG msg; /* A temporary location for all messages */

    /* zero out the struct and set the stuff we want to modify */
    memset(&wc,0,sizeof(wc));
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.lpfnWndProc   = WndProc; /* This is where we will send messages to */
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    
    /* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszClassName = "WindowClass";
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

    if(!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Caption",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, /* x */
        CW_USEDEFAULT, /* y */
        640, /* width */
        480, /* height */
        NULL,NULL,hInstance,NULL);

    if(hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        return 0;
    }

    /*
        This is the heart of our program where all input is processed and 
        sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
        this loop will not produce unreasonably high CPU usage
    */
    while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
        TranslateMessage(&msg); /* Translate key codes to chars if present */
        DispatchMessage(&msg); /* Send it to WndProc */
    }
    return msg.wParam;
}
