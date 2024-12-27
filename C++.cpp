/**
 * @file C++.cpp
 * @brief Barra de carga en consola que muestra el tiempo transcurrido.
 * 
 * Es una muestra de cómo podemos presentar en una bonita barra de carga, 
 * el porcentage de progreso de una tarea.
 * 
 * @details Este programa utiliza tres hilos:
 * - Uno para manejar la cuenta regresiva (`countdown`).
 * - Uno para mostrar la barra de progreso (`printProgressBar`).
 * - Uno para incrementar un contador (`increment`).
 * 
 * La interacción entre estos hilos permite que la barra de progreso
 * sea actualizada en tiempo real mientras se ejecutan otras tareas.
 * 
 * @note Si solo quieres ver la creación de la barra de progreso, puede checar
 * el método @link printProgressBar @endlink. @line 135.
 * 
 * @author J-Xander
 * @date 2020-07-22
 * @version 1.0
 */
// -----: lIBRERIA CON SUS RESPECTIVAS fUNCIONES EN EL CODIGO :------
#include <iostream>
/* @method
 * cout -> Para imprimir en consola
 *///----------------------------------------------------------------
#include <atomic>
/**
 * @Data Types
 * atomic<Temolate> -> Para trabajar con variables atómicas
 *    atomic<[type]> [name]([value]) -> Inicialización
 *    [name].store([value]) -> Asignar un valor
 *    [name].load() -> Obtener el valor que almacena.
 *    [name].fetch_add([value]) -> Añade el [value] a la variable
 *///----------------------------------------------------------------
#include <thread>
/**
 * @method
 * this_thread::sleep_for([time]) -> Pausa la ejecución del hilo
 * thearead [name]([function], [args]) -> Crear un hilo de ejecución
 *///----------------------------------------------------------------
#include <chrono>
/**
 * @method
 * chrono::milliseconds([time]) -> Crear un objeto de tiempo en
 *    milisegundos
 *///----------------------------------------------------------------
#include <cstdint>
/**
 * @Data Types
 * uint64_t -> Entero sin signo de 64 bits
 *///----------------------------------------------------------------
#include <string>
/**
 * @method
 * string([size], [char]) -> Crear una cadena de texto de
 *     tamaño [size], con el caracter [char]
 *///----------------------------------------------------------------

std::atomic<bool> stopFlag(true);         // Bandera para indicar que el conteo ha terminado.
std::atomic<std::uint64_t> number(0);     // Número a incrementar
std::atomic<uint> percentage(0);          // Porcentaje de tiempo transcurrido

/**
 * @brief Cuenta en tiempo real, sera la encargada de enviar el 
 * porcentaje de avance.
 *
 * @param finish Tiempo (en segundos) para la cuenta. 
 * El valor se multiplica por 2 para controlar la precisión.
 * 
 * @note Esta función actualiza una bandera de parada (`stopFlag`) 
 * y el porcentaje de progreso (`percentage`) mientras ejecuta.
 */
void countdown(int finish);

/**
 * @brief Imprime una barra de progreso animada.
 *
 * @param size Tamaño de la barra de carga.
 * @param fillChar Carácter usado para representar el progreso.
 * @param spaceChar Carácter usado para el espacio restante.
 * 
 * @note Actualiza la barra en tiempo real basado en 
 * el porcentaje almacenado en `percentage`.
 */
void printProgressBar(int size, char fillChar, char spaceChar);

/**
 * @brief Incrementa un contador de forma indefinida.
 *
 * @note Este contador (`number`) incrementa mientras `stopFlag` sea verdadero.
 */
void increment();

// ---------------------------: MAIN :-------------------------------
int main(){
    const int SECONDS = 10;          // Tiempo de ejecución
    const int BAR_SIZE = 30;         // Tamaño de la barra de progreso
    const char FILL_CHAR = '#';      // Carácter de progreso
    const char SPACE_CHAR = '-';     // Carácter de espacio

    // Ejecutar las funciones en paralelo
    std::thread t2(printProgressBar, BAR_SIZE, FILL_CHAR, SPACE_CHAR);
    std::thread t1(countdown, SECONDS);
    std::thread t3(increment);

    t1.join(); 
    t2.join();
    t3.join();
    // El programa no continuará hasta que los tres hilos hayan terminado.

    std::cout << "====: Total de Aumentos :====" << std::endl;
    std::cout << number.load() << std::endl;
    std::cout << "=============================" << std::endl;

    return 0;
}

// ---------------------------: FUNCIONES :-------------------------------
void countdown(int finish){
    int calPercentage = 0;
    finish = finish * 2;

    for (int i = 1; i <= finish; i++){
        calPercentage = (i * 100) / finish;     // Calcula el porcentaje
        percentage.store(calPercentage);        // de avance y lo guarda

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    stopFlag.store(false);     // Cambiando el estado de (`stopFlag`) a falso
}

void printProgressBar(int size, char fillChar, char spaceChar){
    while (stopFlag.load()){
        
        std::cout << std::flush
                  << "\033[?25l"     // Ocultar el cursor
                  << "\r";

        std::cout << "[";
        
        int auxPercentage = percentage.load();
        int avance = (size * auxPercentage) / 100;     // Calcula el avance
        
        // Imprime la barra de progreso
        std::cout << "\033[32m"     // Cambia el color a verde
                  << std::string(avance, fillChar)
                  << "\033[0m"      // Restaura el color
                  << std::string(size - avance, spaceChar)
                  << "] "
                  << (auxPercentage == 100 ? "\033[32m" : "")
                  << auxPercentage << "%"
                  << std::flush;

        // Retardo para que solo se actualice cada 100 ms
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  

    }
    std::cout << "\033[0m" 
              << "\033[?25h"     // Mostrar el cursor
              << std::endl;
}

void increment(){
    // Incrementa el número mientras `stopFlag` sea verdadero
    while (stopFlag.load())
        number.fetch_add(1);
}      
