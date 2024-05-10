#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Tabla de puntuaciones (Máximo 5 Jugadores)
char *Tabla[13][6] = {{"           ", "", "", "", "", ""}, {"Suma 1     ", "", "", "", "", ""}, {"Suma 2     ", "", "", "", "", ""}, 
{"Suma 3     ", "", "", "", "", ""}, {"Suma 4     ", "", "", "", "", ""}, {"Suma 5     ", "", "", "", "", ""}, 
{"Suma 6     ", "", "", "", "", ""}, {"Escalera   ", "", "", "", "", ""}, {"Full       ", "", "", "", "", ""}, {"Poker      ", "", "", "", "", ""},
{"Generala   ", "", "", "", "", ""}, {"Generala D.", "", "", "", "", ""}, {"Total      ", "", "", "", "", ""}};

// Verifica inputs numéricos y si están dentro de un intervalo.
int verificar(int num, int lim_inf, int lim_sup){
    while (scanf("%d", &num) != 1 || getchar() != '\n'){
        printf("Ingrese un numero: ");
        while (getchar() != '\n');
    }
    while (num < lim_inf || num > lim_sup){
        printf("Ingrese un numero entre %d y %d: ", lim_inf, lim_sup);
        scanf("%d", &num);
        while (getchar() != '\n');
    }
    return num;
}

int lanzar_dado(){
    return (rand() % 6) + 1;
}

void mostrar_dados(int *dados){
    printf("\n");
    for (int i = 0; i < 5; i ++)
        printf("[ %d ] ", dados[i]);
}

void ordenar_array(int array[], int indice1, int indice2){
    int aux;
    aux = array[indice1];
    array[indice1] = array[indice2];
    array[indice2] = aux;
} 

// Función para elegir el orden de los turnos y desempatar (si se da el caso)
int empezar(int cant_jugadores, int dados[], char Names[][100], int c_empates, int empates[], int IDs[], int indices[5]){
    int suma_puntos[5] = {0, 0, 0, 0, 0};
    c_empates = 0;
    for (int i = 0; i < cant_jugadores; i ++){ // Calcular los puntos de cada jugador
        if (! empates[i]){
            printf("\n%s:", Names[indices[i] - 1]);
            for (int j = 0; j < 5; j ++)
                dados[j] = lanzar_dado();
            suma_puntos[i] = dados[0] + dados[1] + dados[2] + dados[3] + dados[4];
            mostrar_dados(dados);
            printf("\t%d", suma_puntos[i]);
        }
    }
    for (int i = 0; i < cant_jugadores - 1; i ++){ // Ordenar suma_puntos y los demás arrays con el método burbuja de mayor a menor
        for (int j = 0; j < cant_jugadores - i - 1; j ++){
            if ((suma_puntos[j + 1] > suma_puntos[j]) && (! empates[j] && ! empates[j + 1])){ 
                ordenar_array(suma_puntos, j, j + 1);
                ordenar_array(empates, j, j + 1);
                ordenar_array(indices, j, j + 1);
                ordenar_array(IDs, j, j + 1);
            }
        }
    }
    for (int i = 0; i < cant_jugadores; i ++){ // Comparar los puntajes
        int c_empates_aux = 0;
        for (int j = 0; j < cant_jugadores; j ++){
            if ((suma_puntos[i] == suma_puntos[j] && i != j) && (! empates[i] && ! empates[j]))
                c_empates_aux ++;
        }
        c_empates += c_empates_aux;
        if (! c_empates_aux && ! empates[i]){ // Si no hay empates se asigna el nombre en la posición correcta
            Tabla[0][i + 1] = Names[indices[i] - 1];
            empates[i] = 1;
        }
    }
    return c_empates;
}

void mostrar_tabla(){
    printf("\n");
    for (int i = 0; i < 13; i++){
        for (int j = 0; j < 6; j++)
            printf("%s\t", Tabla[i][j]);
        printf("\n");
    }
}

// Relanzar los dados.
void redice(int *dados, int cambios, int cambios_bot[], int BOT){
    if (cambios != 5){
        if (! BOT){ // Si es una persona el jugador:
            printf("\nCuales dados?\t(Del 1 al 5)\n");
            for (int i = 0; i < cambios; i ++){
                int opcion = verificar(opcion, 1, 5);
                dados[opcion - 1] = lanzar_dado();
            }
        }
        else{ // Si es un Bot:
            for (int i = 0; i < 5; i ++){
                if (cambios == 7){ // Si está en dificultad fácil
                    if ((rand() % 2) + 1 == 1)
                        dados[i] = lanzar_dado();
                }
                else{ // Si está en dificultad dificil
                    if (cambios_bot[i])
                        dados[i] = lanzar_dado();
                }
            }
        }
    }
    else{ // Si se cambian todos los dados
        for (int i = 0; i < 5; i++)
            dados[i] = lanzar_dado();
    }
    mostrar_dados(dados);
}

// Verifica si hay dados iguales.
void check_iguales(int *dados, int jugadas[5][12], int jugador){
    int c, escalera = 0, full = 0, marcador = 0;    //marcador uso para calcular la escalera debido a que los números deben ser consecutivos pero no puede ser 1 y 6 juntos
    for (int i = 1; i < 7; i ++){
        c = 0;
        for (int j = 0; j < 5; j++){
            if (i == dados[j]){
                c += 1;
                i == 1 || i == 6 ? marcador ++ : 0;
            }
        }
        c == 1 ? escalera ++ : 0;
        c == 2 ? full ++ : 0;
        c == 3 ? full += 2 : 0;
        jugadas[jugador][i] = c * i;              // Suma de Números
        c >= 4 ? jugadas[jugador][9] = 40 : 0;    // Poker
        if (c == 5){    // Generala y Doble Generala
            jugadas[jugador][10] = 50;
            strcmp(Tabla[10][jugador + 1], "") && strcmp(Tabla[10][jugador + 1], "-") ? jugadas[jugador][11] = 100 : 0; // Solo si Generala ya se jugó se puede jugar Doble Generala
        }   
    }
    marcador == 2 ? escalera -- : 0;
    escalera == 5 ? jugadas[jugador][7] = 20 : 0; // Escalera
    full == 3 ? jugadas[jugador][8] = 30 : 0;     // Full
}

// Verifica si hay una categoria para anotar, pregunta al jugador y la almacena en la tabla.
void anotar_categorias(int jugadas[5][12], int jugador, char *str_puntos, int cambios, int BOT, int best_move){
    str_puntos = malloc(100); // Asigno un espacio en la memoria para cada iteración de los puntos sumados.
    int jugada_elegida = 0, jugada_borrar = 0, c = 0, contador = 0, opciones[12] = {0}, opciones_bot[12] = {0}; // jugada_elegida almacena la posición en el array opciones de la categoría elegida ||| jugada_borrar borra una categoría
    ! BOT ? printf("\n\nTus opciones son:\n") : 0;
    for (int j = 1; j < 12; j ++){
        if (! strcmp(Tabla[j][jugador + 1], "") && jugadas[jugador][j]){
            (! cambios || cambios == 5) && j > 6 && j < 11 ? jugadas[jugador][j] += 5 : 0;
            ! BOT ? printf("\n%d\t%s\t%d puntos", c, Tabla[j][0], jugadas[jugador][j]) : 0;
            opciones[c] = j;
            opciones_bot[j] = c;
            c ++;
        }
    }
    if (c != 0){    // Si el jugador puede anotar en alguna categoría:
        if (! BOT){ // Si es una persona:
            printf("\nQue jugada elegis?\n");
            jugada_elegida = verificar(jugada_elegida, 0, c);
        }
        else{ // Si es un Bot:
            if (BOT == 2) // Si está en dificultad fácil:
                jugada_elegida = rand() % c;
            else{ // Si está en dificil:
                if (best_move == 7) // Si hay jugada mayor, se elige la opcion más alta
                    jugada_elegida = c - 1;
                if (best_move != 7){ // Si no hay una jugada mayor, se elige la jugada menor que tenga mas repetidos (si hay empate, se elige el de menor valor)
                    for (int i = 1; i < 7; i++){
                        if (!strcmp(Tabla[i][jugador + 1], "") && jugadas[jugador][i] / i > 1){
                            jugada_elegida = opciones_bot[i];
                            break;
                        }
                    }
                }
            }
            
        }
        sprintf(str_puntos, "%d", jugadas[jugador][opciones[jugada_elegida]]);
        Tabla[opciones[jugada_elegida]][jugador + 1] = str_puntos;
    }
    else{   // Si el jugador no puede anotar en ninguna categoría:
        printf("\nNo se puede hacer ninguna jugada.");
        ! BOT ? printf("Cual categoria queres borrar?") : 0;
        for (int j = 1; j < 12; j++){
            if (! strcmp(Tabla[j][jugador + 1], "")){
                ! BOT ? printf("\n%d\t%s", contador, Tabla[j][0]) : 0;
                opciones[contador] = j;
                contador ++;
            }
        }
        jugada_borrar = ! BOT ? verificar(jugada_borrar, 0, contador) : 0;
        Tabla[opciones[jugada_borrar]][jugador + 1] = "-";
    }
}

int main(){
    srand(time(NULL));  // Genero la semilla para rand
    int dados[5], cant_jugadores = 0, rondas = 1, bot_humano = 1, choice, c_empates = 0, win = 0; // choice = decisión de volver a tirar || cambios = cantidad de dados a cambiar
    int IDs[5] = {0}, empates[5] = {0}, total_puntos[5] = {0}, indices[5] = {1, 2, 3, 4, 5}, bot_dif[5]; // IDs = 0 si es una persona, 1 si es bot dificil y 2 si es bot facil || indices es el orden de los jugadores
    char Names[5][100] = {"", "", "", "", ""}, BOTS[5][100] = {"BOT 1", "BOT 2", "BOT 3", "BOT 4", "BOT 5"}, *str_puntos = "", *str_total_puntos; // str_puntos se usa para almacenar el valor numérico del puntaje elegido para una categoría
    
    printf("Bienvenido al juego de Generala\nCuantos jugadores? (2 - 5)\n");
    cant_jugadores = verificar(cant_jugadores, 2, 5);
    for (int i = 1, num_bots = 0, choice2; i < cant_jugadores + 1; i++){ // Preguntar si es un bot o una persona e ingresar los nombres de los jugadores de la partida
        printf("Persona o Bot?\n1 -- Persona\t\t0 -- Bot\n");
        bot_humano = verificar(bot_humano, 0, 1);
        if (bot_humano){
            printf("Introduzca el nombre del jugador %d\n", i);
            scanf("%s", Names[i - 1]);
        }
        else{
            printf("Que dificultad desea para el %s?\n1 -- Facil\t\t0 -- Dificil\n", BOTS[num_bots]);
            choice2 = verificar(choice2, 0, 1);
            IDs[i - 1] = choice2 + 1;
            strcpy(Names[i - 1], BOTS[num_bots]);
            num_bots ++; 
        }
    }
    c_empates = empezar(cant_jugadores, dados, Names, c_empates, empates, IDs, indices);
    while (c_empates){
        printf("\n\nEmpate");
        c_empates = empezar(cant_jugadores, dados, Names, c_empates, empates, IDs, indices);
    }
    while (rondas != 12){   // Loop del juego. Cada turno es una iteración del for para cada jugador, al terminar el for vuelve a jugar el jugador 1 y así
        for (int Jugador = 1; Jugador < cant_jugadores + 1; Jugador ++){
            int jugadas[5][12] = {{0}, {0}, {0}, {0}, {0}}, cambios = 0, best_move, BOT;
            printf("\nRonda %d\nTurno de %s\nTus dados son...\n", rondas, Tabla[0][Jugador]);
            BOT = IDs[Jugador - 1];
            for (int i = 0; i < 5; i ++) // Se lanzan los dados
                dados[i] = lanzar_dado();
            mostrar_dados(dados);

            for (int tiros_disponibles = 2; tiros_disponibles > 0; tiros_disponibles --){   // El jugador decide si vuelve a tirar o no sus dados
                best_move = 0;
                int cambios_bot[5] = {0, 0, 0, 0, 0};
                if (! BOT){  // Si es el turno de una persona:
                    printf("\n\nDesea volver a tirar? (quedan %d tiros disponibles)\n1 -- Si\t\t0 -- No\n", tiros_disponibles);
                    choice = verificar(choice, 0, 1);
                    if (choice){
                        printf("Cuantos dados desea volver a tirar?\n");
                        cambios = verificar(cambios, 0, 5);
                        redice(dados, cambios, cambios_bot, BOT);
                    }
                    else
                        tiros_disponibles --;
                }
                else{   //Si es el turno de un Bot:
                    check_iguales(dados, jugadas, Jugador - 1);
                    for (int i = 7; i < 11; i ++)   // Verifico si hay alguna jugada mayor disponible para el Bot, si hay, será utilizada
                        jugadas[Jugador - 1][i] != 0 && ! strcmp(Tabla[i][Jugador], "") ? best_move = 7 : 0;
                    if (best_move) break;   // Si hay una jugada mayor se rompe el for para añadirla
                    cambios = 0;    // Reseteamos los cambios
                    if (! bot_dif[Jugador - 1]){   // Si es el Bot de dificultad dificil:
                        for (int i = 0; i < 5; i ++){   // Se deciden que dados relanzar
                            int contador = 0;
                            for (int j = 0; j < 5; j ++)
                                dados[i] == dados[j] && i != j && ! strcmp(Tabla[dados[i]][Jugador], "") ? contador ++ : 0;
                            if (contador == 0){
                                cambios_bot[i] = 1;
                                cambios ++;
                            }
                        }
                    }
                    else    // Si el bot está en fácil: Se elige aleatoriamente que dados relanzar
                        cambios = 7;        
                }
                if (cambios){
                    printf("\n\nSe vuelven a lanzar los dados del %s\n", Tabla[0][Jugador]);
                    redice(dados, cambios, cambios_bot, BOT);
                }
            }
            check_iguales(dados, jugadas, Jugador - 1); // Verificamos las jugadas posibles con las elecciones finales de dados
            for (int i = 7; i < 11; i++)    // Verifico otra vez si hay alguna jugada mayor disponible para el Bot después de volver a lanzar los dados por 3ra vez
                jugadas[Jugador - 1][i] != 0 && !strcmp(Tabla[i][Jugador], "") ? best_move = 7 : 0;
            anotar_categorias(jugadas, Jugador - 1, str_puntos, cambios, BOT, best_move); // Anotamos en la categoría que el jugador desee
            if (! strcmp(Tabla[10][Jugador], "55")){    // Si un jugador hace Generala Servida gana la partida
                printf("\nGano %s", Tabla[0][Jugador]);
                win = 1;
                break;
            }
            printf("\n");
            mostrar_tabla();
        }
        rondas ++; // Cuando todos han jugado un turno, se pasa a la siguiente ronda
        if (win) break; //Si alguien ganó se rompe el while
    }
    for (int i = 0; i < cant_jugadores; i ++){  // Calcular los totales
        str_total_puntos = malloc(100);
        for (int j = 1; j < 12; j++)
            total_puntos[i] += atoi(Tabla[j][i + 1]);
        sprintf(str_total_puntos, "%d", total_puntos[i]);
        Tabla[12][i + 1] = str_total_puntos;
    }
    for (int i = 0; i < cant_jugadores - 1; i ++){  // Ordenar los totales y los indices para saber quién ganó o si hubo empate
        for (int j = 0; j < cant_jugadores - i - 1; j ++){ // Se ordenan los arrays con el método burbuja
            if (total_puntos[j + 1] > total_puntos[j]){
                ordenar_arrays(total_puntos, j, j + 1);
                ordenar_arrays(indices, j, j + 1);
            }
        }
    }
    if (! win)
        total_puntos[0] == total_puntos[1] ? printf("\nEmpate.") : printf("\nGano %s", Names[indices[0] - 1]);
    mostrar_tabla();
    return 0;
}