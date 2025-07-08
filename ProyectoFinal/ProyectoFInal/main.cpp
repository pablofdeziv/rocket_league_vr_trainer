#include "CGApplication.h"
#include <iostream>
#include <stdexcept>

//
//// PROYECTO: Project11b
//// DESCRIPCI�N: Aplicaci�n gr�fica que incluye el c�lculo de sombras
////              con la t�cnica de percentage-closer filtering (PCF).
//
int main()
{
    CGApplication app;

    try
    {
        app.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}