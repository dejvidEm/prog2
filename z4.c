#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "monopoly.h" // NEMENIT !!!

// TODO vlastne funkcie


// DOLEZITE HLAVNE PREMENNE PRE PROGRAM
int cislo_tahu_celkovo = 1;  // celkovy pocet tahov ktore boli vykonane - potrebne pre zakladny vypis
int aktualny_hrac_na_tahu_index = 0;  // MAX je 4, index hraca z nazvov hracov ktory je akurat na tahu
char *nazvy_hracov[] = {  // pole hracov odkial printujem nazov hraca podla indexu vyssie
        "P1",
        "P2",
        "P3",
        "P4"
    };

int penaze[4] = {0, 0, 0, 0};

int vitaz_index = 5;  // ak niekto vyhral tak index hraca ktory vyhral, ak nie tak 5 pretoze max hracov je 4

int pole[24] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

// ------------------------------------ POMOCNÉ PREMENNÉ ------------------------------------

// funkcia na zmenu tahu
void zmena_tahu(int *premenna, int pocet_hracov) {
    if (*premenna < pocet_hracov-1) {
        (*premenna)++;
    }else{
        *premenna = 0;
    }
}

// ----------------------------------------- VYPISY -----------------------------------------


// zakladny vypis (opakuje sa stale v kazdom tahu)

void zakladny_vypis(int hod_cislo, int cislo_tahu, char *pole_hracov[], int index_hraca){
    printf("R: %d\n", hod_cislo);
    printf("Turn: %d\n", cislo_tahu);
    printf("Player on turn: %s\n", pole_hracov[index_hraca]);
    printf("\n");
}

// vypis hracej plochy, prepínač -s

void vypis_hracieho_planu_s(SPACE doska[], int pole_indexov_hracov[]){
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
                printf("%-17s %d  ", game_board[i].property->name, game_board[i].property->price);
                switch (doska[i].property->color){
                    case Brown:
                        printf("%-9s  ", "Brown");
                        break;
                    case Skyblue:
                        printf("%-9s  ", "Skyblue");
                        break;
                    case Purple:
                        printf("%-9s  ", "Purple");
                        break;
                    case Orange:
                        printf("%-9s  ", "Orange");
                        break;
                    case Red:
                        printf("%-9s  ", "Red");
                        break;
                    case Yellow:
                        printf("%-9s  ", "Yellow");
                        break;
                    case Green:
                        printf("%-9s  ", "Green");
                        break;
                    case Blue:
                        printf("%-9s  ", "Blue");
                        break;
                }
                if(pole_indexov_hracov[i]==0){
                    printf("P1");
                }
                if(pole_indexov_hracov[i]==1){
                    printf("P2");
                }
                if(pole_indexov_hracov[i]==2){
                    printf("P3");
                }
                if(pole_indexov_hracov[i]==3){
                    printf("P4");
                }
                printf("\n");
        }
    }
}


// vypis hracov, prepínač -p

void vypis_hracov_p(PLAYER hraci[], int pocet_hracov){
    int index_na_ploche = 0;
    printf("Players:\n");
    for (int i = 0; i < pocet_hracov; i++) {
        printf("%d. S: %d, C: %d, JP: %d, IJ: %d\n", hraci[i].number, hraci[i].space_number, hraci[i].cash, hraci[i].num_jail_pass, hraci[i].is_in_jail);
        for (int j = 0; j < hraci[i].num_properties; j++) {
            // Predpokladám, že hraci[i].owned_properties[j] obsahuje nejaké informácie o vlastnených pozemkoch
            printf("     %-17s  %d  ", hraci[i].owned_properties[j]->name, hraci[i].owned_properties[j]->price);
            switch (hraci[i].owned_properties[j]->color){
                case Brown:
                    printf("%-9s", "Brown");
                    break;
                case Skyblue:
                    printf("%-9s", "Skyblue");
                    break;
                case Purple:
                    printf("%-9s", "Purple");
                    break;
                case Orange:
                    printf("%-9s", "Orange");
                    break;
                case Red:
                    printf("%-9s", "Red");
                    break;
                case Yellow:
                    printf("%-9s", "Yellow");
                    break;
                case Green:
                    printf("%-9s", "Green");
                    break;
                case Blue:
                    printf("%-9s", "Blue");
                    break;
            }
            for (int in = 0; in < NUM_SPACES; in++) {
                if(game_board[in].type == Property){
                    if (strcmp(game_board[in].property->name, hraci[i].owned_properties[j]->name) == 0) {
                        index_na_ploche = in+1; // Vráti index, ak nájde zhodu
                    }
                }
            }
            printf("S%d\n",index_na_ploche);
            // Tu môžete vypísať ďalšie informácie o vlastnených pozemkoch
        }
    }
    printf("\n");
}

void vypis_stavu_g(SPACE doska[], PLAYER hraci[], int pocet_hracov, int vitaz_i, char *pole_hracov[], int pole_indexov[]){
    //vypis hracov
    printf("Players:\n");
    for (int i = 0; i < pocet_hracov; i++) {
        printf("%d. S: %d, C: %d, JP: %d, IJ: %d\n", hraci[i].number, hraci[i].space_number, hraci[i].cash, hraci[i].num_jail_pass, hraci[i].is_in_jail);
        for (int j = 0; j < hraci[i].num_properties; j++) {
            // Predpokladám, že hraci[i].owned_properties[j] obsahuje nejaké informácie o vlastnených pozemkoch
            printf("     %-17s  %d  ", hraci[i].owned_properties[j]->name, hraci[i].owned_properties[j]->price);
            switch (hraci[i].owned_properties[j]->color){
                case Brown:
                    printf("%-9s", "Brown");
                    break;
                case Skyblue:
                    printf("%-9s", "Skyblue");
                    break;
                case Purple:
                    printf("%-9s", "Purple");
                    break;
                case Orange:
                    printf("%-9s", "Orange");
                    break;
                case Red:
                    printf("%-9s", "Red");
                    break;
                case Yellow:
                    printf("%-9s", "Yellow");
                    break;
                case Green:
                    printf("%-9s", "Green");
                    break;
                case Blue:
                    printf("%-9s", "Blue");
                    break;
            }
            printf("\n");
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
                printf("%-17s %d  ", game_board[i].property->name, game_board[i].property->price);
                switch (doska[i].property->color){
                    case Brown:
                        printf("%-9s", "Brown");
                        break;
                    case Skyblue:
                        printf("%-9s", "Skyblue");
                        break;
                    case Purple:
                        printf("%-9s", "Purple");
                        break;
                    case Orange:
                        printf("%-9s", "Orange");
                        break;
                    case Red:
                        printf("%-9s", "Red");
                        break;
                    case Yellow:
                        printf("%-9s", "Yellow");
                        break;
                    case Green:
                        printf("%-9s", "Green");
                        break;
                    case Blue:
                        printf("%-9s", "Blue");
                        break;
                }
                if(pole_indexov[i]==0){
                    printf("P1");
                }
                if(pole_indexov[i]==1){
                    printf("P2");
                }
                if(pole_indexov[i]==2){
                    printf("P3");
                }
                if(pole_indexov[i]==3){
                    printf("P4");
                }
                printf("\n");
        }
    }
    if(vitaz_i==5){
        printf("WINNER: -\n");
    }
    if(vitaz_i!=5){
        printf("WINNER: %s\n", pole_hracov[vitaz_i]);
    }
}


void vypis_stavu_hry(){
    // vypis stavu hry ktory sa uskutocni na zaciatku a na konci
}







// ---------------------------------------------------------------------------------------------------
// --------------------------------------- MAIN FUNKCIA ----------------------------------------------
// ---------------------------------------------------------------------------------------------------



int main(int argc, char *argv[]) {
    int hra_bezi = 1;  // ak nemame vitaza a hra neskoncila = 1, ak hra skoncila = 0

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
        hraci[i].space_number = 1;
        hraci[i].cash = suma_penazi;
        hraci[i].num_jail_pass = 0;;
        hraci[i].is_in_jail = 0;
        for (int j = 0; j < NUM_PROPERTIES; j++) {  // inicializujeme kazdy item pola na NULL, kedze hrac nevlastni ziadny pozemok
            hraci[i].owned_properties[j] = NULL;
        }
        hraci[i].num_properties = 0;
    }

    //hraci[0].owned_properties[0] = &properties[0];
    // hraci[0].owned_properties[0] = &properties[0];  // FOOD TRUCK
    // hraci[0].owned_properties[1] = &properties[1];  // PIZZA
//    hraci[0].owned_properties[2] = &properties[2];  // DOUGHNUT
    // hraci[0].num_properties = 2;
//
   // hraci[1].owned_properties[0] = &properties[3];  // ICE CREAM
//    hraci[1].owned_properties[1] = &properties[4];  // MUSEUM
//    hraci[1].owned_properties[2] = &properties[5];  // LIBRARY
    // hraci[1].num_properties = 1;
//
//    hraci[2].owned_properties[0] = &properties[6];  // THE PARK
//    hraci[2].owned_properties[1] = &properties[7];  // THE BEACH
//    hraci[2].owned_properties[2] = &properties[8];  // POST OFFICE
//    hraci[2].num_properties = 3;
    //hraci[0].owned_properties[2] = &properties[2];
    //hraci[0].owned_properties[3] = &properties[3];
    //hraci[0].num_properties = 4;

    // vypis_hracov_p(hraci, pocet_hracov);

    // vypis_hracieho_planu_s(game_board);
    // zakladny_vypis(hod, cislo_tahu_celkovo, nazvy_hracov, aktualny_hrac_na_tahu_index);
    // printf("\n");printf("\n");




    // ----------------- START HRY, HODY KOCKOU - INPUT a ZAKLADNY GAME LOOP -----------------
    int hod; // premenna hodu kocky
    int bankrot = 0;
    vypis_stavu_g(game_board, hraci, pocet_hracov, vitaz_index, nazvy_hracov, pole);
    printf("\n");
    while (hra_bezi) { // loop dokym budu zadane ciselne hodnoty ako hody kociek alebo nemame vyhercu GAME LOOP
        if (scanf("%d", &hod) != 1) {
            hra_bezi = 0;
            break; // načítana bola nečíselna hodnota, program končí
        }

        // kod s hodom ktory sme nacitali - LOGIKA PROGRAMHU

        // 1. vypis ktory sa uskutocni stale

        // 2. kontrola ci hrac neopusta vazenie, ak ano plati 1€
        //             ci hrac nepresiel startom, ak ano dostane 2€
        //              - kontrolujeme aj bankrot hraca (nema € na vazenie)

        // tu budu vsetky vpisy ak  boli zadane dane prepinace
        // ZAKLADNY VYPIS - vypise sa stale
        zakladny_vypis(hod, cislo_tahu_celkovo, nazvy_hracov, aktualny_hrac_na_tahu_index);

        // VYPIS - P
        if(prep_p==1){
            // vypis P
        }

        // VYPIS - S
        if(prep_s==1){
            // vypis S
        }

        // VYPIS - G
        if(prep_g==1){
            // vypis G
        }

        // kontrola ci je hrac vo vazeni, ak je strhneme mu 1 euro a dalej kontrolujeme bankrot
        if(hraci[aktualny_hrac_na_tahu_index].is_in_jail == 1){
            if(hraci[aktualny_hrac_na_tahu_index].cash>=1){
                hraci[aktualny_hrac_na_tahu_index].cash-=1;
                hraci[aktualny_hrac_na_tahu_index].is_in_jail = 0;  // hrac uz nieje vo vazeni a posuva sa dalej
            }
            // bankrot, vymysliet funkciu na bankrot
        }

        // ------------------------------------ POSUN HRACA ------------------------------------

        // kontrola ci hrac nieje na konci hracej plochy a posunieme ho na start ak je a dáme mu 2 eura ak prejde startom
        if((hraci[aktualny_hrac_na_tahu_index].space_number+hod)<=24){
            hraci[aktualny_hrac_na_tahu_index].space_number+=hod;

        }else{
            hraci[aktualny_hrac_na_tahu_index].space_number=(hraci[aktualny_hrac_na_tahu_index].space_number+=hod)-24;
            hraci[aktualny_hrac_na_tahu_index].cash+=2; // hrac dostane 2 eura za prechod startom
        }

        int vlastnik_index = -1;
        int index_pozemku = 3;  // index ktory zistujeme ci sa nachadza vo vlastnictve niektoreho z hracov, menime pri kazdom pohybe hraca na tahu

        int cena_nehnutelnosti = 0;


        switch (game_board[hraci[aktualny_hrac_na_tahu_index].space_number-1].type){
            case Start:
                printf("policko START\n");
                break;


            case Jail_pass:
                hraci[aktualny_hrac_na_tahu_index].num_jail_pass++;
                printf("policko JAIL PASS\n");
                break;


            case In_jail:
                printf("policko IN JAIL\n");
                break;


            case Go_to_jail:
                // kontrola ci ma hrac priepustku z vazenia, ak ano znizime ich pocet o jednu a hrac ostava stat na mieste
                // ak nie hrac pojde na pole vazenie
                if(hraci[aktualny_hrac_na_tahu_index].num_jail_pass>0){
                    hraci[aktualny_hrac_na_tahu_index].num_jail_pass--;
                }else{
                    hraci[aktualny_hrac_na_tahu_index].space_number = 7;
                    hraci[aktualny_hrac_na_tahu_index].is_in_jail = 1;
                    printf("%s\n", game_board[hraci[aktualny_hrac_na_tahu_index].space_number-1].property->name);
                }
                printf("policko GO TO JAIL\n");
                break;


            case Free_parking:
                printf("policko FREE PARKING\n");
                break;


            // zistim ci hrac vlastni pozemok
            case Property:
                for(int i_hrac = 0;i_hrac<pocet_hracov;i_hrac++){
                    for(int i_prop = 0;i_prop<hraci[i_hrac].num_properties;i_prop++){
                        if(strcmp(hraci[i_hrac].owned_properties[i_prop]->name, game_board[hraci[aktualny_hrac_na_tahu_index].space_number-1].property->name) == 0){
                            if(i_hrac == aktualny_hrac_na_tahu_index){
                                // vlastnik je aj hrac na tahu, nic sa nedeje
                            }
                            else{
                                cena_nehnutelnosti = game_board[hraci[aktualny_hrac_na_tahu_index].space_number-1].property->price;
                                hraci[aktualny_hrac_na_tahu_index].cash-=cena_nehnutelnosti;
                                if(hraci[aktualny_hrac_na_tahu_index].cash<0){
                                    bankrot = 1;
                                }
                                if(hraci[aktualny_hrac_na_tahu_index].cash>0){
                                    hraci[i_hrac].cash+=cena_nehnutelnosti;
                                }
                                // hrac plati za nehnutelnost inemu hracovi
                            }
                        }
                    }
                }
                // kupa nehnutelnosti ak ju nikto nevlastni
                if(pole[hraci[aktualny_hrac_na_tahu_index].space_number-1] == -1){
                    cena_nehnutelnosti = game_board[hraci[aktualny_hrac_na_tahu_index].space_number-1].property->price;
                    hraci[aktualny_hrac_na_tahu_index].cash-=cena_nehnutelnosti;
                    if(hraci[aktualny_hrac_na_tahu_index].cash<0){
                        bankrot = 1;
                    }
                    if(hraci[aktualny_hrac_na_tahu_index].cash>0){
                        hraci[aktualny_hrac_na_tahu_index].owned_properties[hraci[aktualny_hrac_na_tahu_index].num_properties] = game_board[hraci[aktualny_hrac_na_tahu_index].space_number-1].property;
                        hraci[aktualny_hrac_na_tahu_index].num_properties++;
                        pole[hraci[aktualny_hrac_na_tahu_index].space_number-1] = aktualny_hrac_na_tahu_index;
                    }
                }

        }


        // vypis_hracov_p(hraci, pocet_hracov);
        // vypis_hracieho_planu_s(game_board, pole);
        // vypis_stavu_g(game_board, hraci, pocet_hracov, vitaz_index, nazvy_hracov, pole);



        // ziskanie nazvu nehnutelnosti na ktoru sa postavil
        if(bankrot == 0){
            zmena_tahu(&aktualny_hrac_na_tahu_index, pocet_hracov);
            cislo_tahu_celkovo++;
        }
        if(bankrot == 1){
            vitaz_index = 0;
            for (int i_pen = 1; i_pen < pocet_hracov; i_pen++) {
                if (hraci[i_pen].cash > hraci[vitaz_index].cash) {
                    vitaz_index = i_pen;
                }
            }
            vypis_stavu_g(game_board, hraci, pocet_hracov, vitaz_index, nazvy_hracov, pole);
            hra_bezi = 0;
            break;
        }




        // implementacia pohybu hracov, aby sme vedeli previest kontroly ktore su spomenute vyssie

        // 3. posun, ak hrac nezbankrotoval a akcia podla typu pola (nakup, platba vlastnikovi, presun do vazenia)
        //              - kontrolujeme aj bankrot hraca (nema € na najomne alebo nakup)

        // 3.B Bankrot - urcite vitaza podla:
        //              - hrac ktory ma najviac penazi
        //              - ak maju rovnaky pocet, predaju nehnutelnosti ktore maju
        //              - ak stale maju rovnaky pocet € tak nastava remiza
        // 3.B - vypis stavu hry aj s vitazom, iny vypis sa neuskutocni

        // 4. Vypis po tahu podla prepinaca (-s, -p, -g)
        //     - navrat na bod cislo 1.
    }



    return 0;
}
