#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "monopoly.h" // NEMENIT !!!

// TODO vlastne funkcie

// DOLEZITE HLAVNE PREMENNE PRE PROGRAM
int cislo_tahu_celkovo = 1;  // celkovy pocet tahov ktore boli vykonane - potrebne pre zakladny vypis
char aktualny_hrac_na_tahu_index = 0;  // MAX je 4, index hraca z nazvov hracov ktory je akurat na tahu
char *nazvy_hracov[] = {  // pole hracov odkial printujem nazov hraca podla indexu vyssie
        "P1",
        "P2",
        "P3",
        "P4"
    };
int vitaz_index = 5;  // ak niekto vyhral tak index hraca ktory vyhral, ak nie tak 5 pretoze max hracov je 4


// ----------------------------------------- VYPISY -----------------------------------------


// zakladny vypis (opakuje sa stale v kazdom tahu)

void zakladny_vypis(int hod_cislo, int cislo_tahu, char *pole_hracov[], int index_hraca){
    printf("R: %d\n", hod_cislo);
    printf("Turn: %d\n", cislo_tahu);
    printf("Player on turn: %s\n", pole_hracov[index_hraca]);
}

// vypis hracej plochy, prepínač -s

void vypis_hracieho_planu_s(SPACE doska[]){
    printf("Game board:\n");
    for(int i=0;i<NUM_SPACES;i++){
        if(i<10){
            printf("%d.   ", i);
        }
        else{
            printf("%d.  ", i);
        }
        switch (doska[i].type){
            case Start:
                printf("START\n");
                break;
            case Jail_pass:
                printf("JAIL PASS\n");
                break;
            case In_jail:
                printf("IN JAIL\n");
                break;
            case Go_to_jail:
                printf("GO TO JAIL\n");
                break;
            case Free_parking:
                printf("FREE PARKING\n");
                break;
            case Property:
                printf("%-17s %d\n", game_board[i].property->name, game_board[i].property->price);
        }
    }
}


// vypis hracov, prepínač -p

void vypis_hracov_p(PLAYER hraci[], int pocet_hracov){
    printf("Players:\n");
    for (int i = 0; i < pocet_hracov; i++) {
        printf("%d. S: %d, C: %d, JP: %d, IJ: %d\n", hraci[i].number, hraci[i].space_number, hraci[i].cash, hraci[i].num_jail_pass, hraci[i].is_in_jail);
        for (int j = 0; j < hraci[i].num_properties; j++) {
            // Predpokladám, že hraci[i].owned_properties[j] obsahuje nejaké informácie o vlastnených pozemkoch
            printf("- Pozemok %d\n", j + 1);
            // Tu môžete vypísať ďalšie informácie o vlastnených pozemkoch
        }
    }
}

void vypis_stavu_g(SPACE doska[], PLAYER hraci[], int pocet_hracov, int vitaz_i, char *pole_hracov[], int index_hraca){
    //vypis hracov
    printf("Players:\n");
    for (int i = 0; i < pocet_hracov; i++) {
        printf("%d. S: %d, C: %d, JP: %d, IJ: %d\n", hraci[i].number, hraci[i].space_number, hraci[i].cash, hraci[i].num_jail_pass, hraci[i].is_in_jail);
        for (int j = 0; j < hraci[i].num_properties; j++) {
            // Predpokladám, že hraci[i].owned_properties[j] obsahuje nejaké informácie o vlastnených pozemkoch
            printf("- Pozemok %d\n", j + 1);
            // Tu môžete vypísať ďalšie informácie o vlastnených pozemkoch
        }
    }
    //vypis hracieho planu
    printf("Game board:\n");
    for(int i=0;i<NUM_SPACES;i++){
        if(i<10){
            printf("%d.   ", i);
        }
        else{
            printf("%d.  ", i);
        }
        switch (doska[i].type){
            case Start:
                printf("START\n");
                break;
            case Jail_pass:
                printf("JAIL PASS\n");
                break;
            case In_jail:
                printf("IN JAIL\n");
                break;
            case Go_to_jail:
                printf("GO TO JAIL\n");
                break;
            case Free_parking:
                printf("FREE PARKING\n");
                break;
            case Property:
                printf("%-17s %d\n", game_board[i].property->name, game_board[i].property->price);
        }
    }
    if(vitaz_i==5){
        printf("WINNER: -\n");
    }
    if(vitaz_i!=5){
        printf("WINNER: %s\n", pole_hracov[index_hraca]);
    }
}


void vypis_stavu_hry(){
    // vypis stavu hry ktory sa uskutocni na zaciatku a na konci
}


// ---------------------------------------------------------------------------------------------------
// --------------------------------------- MAIN FUNKCIA ----------------------------------------------
// ---------------------------------------------------------------------------------------------------

int main(int argc, char *argv[]) {

    // ------------------------------------- CMD ARGUMENTY -------------------------------------

    // pocet hracov, po prejdeni argumentov sa nastavi na pozadovanu hodnotu
    int pocet_hracov = 2;
    // premenne ktore zistia ci su zadane prepinace
    int prep_s = 0, prep_p = 0, prep_g = 0;
    int opt;
    int n_argument = 2; // Predvolená hodnota pre -n, reprezentuje pocet hracov

    while ((opt = getopt(argc, argv, "spgn:")) != -1) {
        switch (opt) {
            case 's':
                prep_s = 1;
                break;
            case 'p':
                prep_p = 1;
                break;
            case 'g':
                prep_g = 1;
                break;
            case 'n':
                sscanf(optarg, "%d", &pocet_hracov);  // zmenime pocet hracov na zadany parametrom
                break;
            default:
                break;
        }
    }


    // ----------------- ZADEFINOVANIE STRUKTUR HRACOV, INICIALIZACIA HRY -----------------

    // rozdelenie penazi mezi hracov
    int suma_penazi = 0;
    if(pocet_hracov == 2){
        suma_penazi = 20;
    }
    if(pocet_hracov == 3){
        suma_penazi = 18;
    }
    if(pocet_hracov == 4){
        suma_penazi = 16;
    }

    // samotna inicializacia hracov
    PLAYER hraci[MAX_PLAYERS];
    for (int i = 0; i < pocet_hracov; i++) {
        hraci[i].number = i + 1;
        hraci[i].space_number = 0;
        hraci[i].cash = suma_penazi;
        hraci[i].num_jail_pass = 0;;
        hraci[i].is_in_jail = 0;
        for (int j = 0; j < NUM_PROPERTIES; j++) {  // inicializujeme kazdy item pola na NULL, kedze hrac nevlastni ziadny pozemok
            hraci[i].owned_properties[j] = NULL;
        }
        hraci[i].num_properties = 0;
    }

    vypis_hracov_p(hraci, pocet_hracov);
    vypis_stavu_g(game_board, hraci, pocet_hracov, vitaz_index, nazvy_hracov, vitaz_index);
    vypis_hracieho_planu_s(game_board);




    // ----------------- START HRY, HODY KOCKOU - INPUT a ZAKLADNY GAME LOOP -----------------
    int hod;

    while (1) {
        if (scanf("%d", &hod) != 1) {
            break; // načítana bola nečíselna hodnota, program končí
        }
        printf("%d\n", hod);
        // kod s hodom ktory sme nacitali - ZAKLADY GAME LOOP
    }





    return 0;
}
