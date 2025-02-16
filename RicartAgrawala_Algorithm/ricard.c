#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define NUM_FILOZOFA 5

#define ZAHTJEV 1
#define ODGOVOR 2

int pipe_fds[NUM_FILOZOFA][2]; // Cjevovodi za komunikaciju
int cl;

typedef struct {
    int filozof_id;
    int tip;
    int c;
} poruka;

void misliti(int filozof_id) {
    printf("Filozof %d misli...\n", filozof_id);
    int rand_broj = rand() % 3 + 1;
    sleep(rand_broj); // misli između 1 i 3 sekunde
    cl+=rand_broj;
}

void jedi(int filozof_id) {
    printf("Filozof %d jede...\n", filozof_id);
    sleep(rand() % 3 + 1); // jede između 1 i 3 sekunde
    printf("Filozof %d zavrsio jelo...\n", filozof_id);
}

int update_clock(int c1, int c2) {
    return (c1 > c2) ? c1 + 1 : c2 + 1;
}

void posalji_poruku(int fd, poruka *p) {
    if (write(fd, p, sizeof(poruka)) == -1) {
        perror("Greška pri slanju poruke");
    }
}
void primi_poruku(int fd, poruka *p) {
    if(read(fd, p, sizeof(poruka)) == -1)perror("Greška pri primanju poruke");
}

// Funkcija za slanje zahtjeva za jelo lijevom i desnom filozofu
void posalji_zahtjev(int sender_filozof_id, int reciever_filozof_id) {
    poruka p = {sender_filozof_id, ZAHTJEV, cl};  
    printf("Filozof %d šalje ZAHTJEV filozofu %d s clockom %d\n", sender_filozof_id, reciever_filozof_id, cl);
    //posalji_poruku(pipe_fds[reciever_filozof_id][1], &p);
    if (write(pipe_fds[reciever_filozof_id][1], &p, sizeof(poruka)) == -1) {
        perror("Greška pri slanju poruke");
    }
    cl++;
}

// Funkcija za slanje odgovora
void posalji_odgovor(int sender_filozof_id, int reciever_filozof_id) {
    poruka p = {sender_filozof_id, ODGOVOR, cl};  
    printf("Filozof %d šalje ODGOVOR filozofu %d s clockom %d\n", sender_filozof_id, reciever_filozof_id, cl);
    posalji_poruku(pipe_fds[reciever_filozof_id][1], &p);
    cl++;
}

void filozof_proces(int id) {
    int odgovor_lijevi = 0;
    int odgovor_desni = 0;
    poruka msg;
    int zahtjev_cl = cl;

    while (1) {
        misliti(id);
        printf("Filozof %d je gladan\n", id);

        odgovor_lijevi = 0;
        odgovor_desni = 0;
        zahtjev_cl = cl;
        int za_odgovoriti_lijevi = 0;
        int za_odgovoriti_desni = 0;

        posalji_zahtjev(id, (id + NUM_FILOZOFA - 1) % NUM_FILOZOFA); 
        posalji_zahtjev(id, (id + 1) % NUM_FILOZOFA); 

        while (odgovor_desni + odgovor_lijevi < 2) {
            // Čitanje od lijevog susjeda
            primi_poruku(pipe_fds[id][0], &msg);
            cl = update_clock(cl, msg.c);

            if (msg.tip == ZAHTJEV) {
                printf("Filozof %d primio ZAHTJEV od filozofa %d s clockom %d\n", id, msg.filozof_id, msg.c);
                if (zahtjev_cl < msg.c) {
                    if (msg.filozof_id == (id + NUM_FILOZOFA - 1) % NUM_FILOZOFA)
                        za_odgovoriti_lijevi = 1;
                    else if (msg.filozof_id == (id + 1) % NUM_FILOZOFA)
                        za_odgovoriti_desni = 1;
                } else {
                    posalji_odgovor(id, msg.filozof_id);
                }
            } else if (msg.tip == ODGOVOR) {
                printf("Filozof %d primio ODGOVOR od filozofa %d s clockom %d\n", id, msg.filozof_id, msg.c);
                if (msg.filozof_id == (id + NUM_FILOZOFA - 1) % NUM_FILOZOFA)
                    odgovor_lijevi = 1;
                else if (msg.filozof_id == (id + 1) % NUM_FILOZOFA)
                    odgovor_desni = 1;
            }
        }

        jedi(id);

        if (za_odgovoriti_lijevi == 1)
            posalji_odgovor(id, (id + NUM_FILOZOFA - 1) % NUM_FILOZOFA);
        if (za_odgovoriti_desni == 1)
            posalji_odgovor(id, (id + 1) % NUM_FILOZOFA);
    }
}

int main() {
    pid_t pid;

    // Kreiranje cjevovoda za komunikaciju
    for (int i = 0; i < NUM_FILOZOFA; i++) {
        if (pipe(pipe_fds[i]) == -1) {
            perror("Greška pri kreiranju cjevovoda");
            exit(1);
        }
        
       
    }

    // Kreiranje filozof procesa koristeći fork()
    for (int i = 0; i < NUM_FILOZOFA; i++) {
        pid = fork();
        cl = i;
        srand(time(NULL) + i);
        if (pid == 0) {
            // Dijete proces: svaki filozof ima svoj proces
            // Zatvorite neiskorištene dijelove cjevovoda
            for (int j = 0; j < NUM_FILOZOFA; j++) {
                if (j != i) { // Zatvori desni dio cjevovoda (za pisanje)
                    close(pipe_fds[j][0]);
                }
            }
            close(pipe_fds[i][1]);

            filozof_proces(i);
            exit(0);  // Nakon završetka simulacije, proces izlazi
        } else if (pid < 0) {
            perror("Greška pri fork() pozivu");
            exit(1);
        }
    }

    // Roditeljski proces čeka sve filozofe
    for (int i = 0; i < NUM_FILOZOFA; i++) {
        wait(NULL);  // Čeka dok svi procesi ne završe (nikada neće završiti zbog beskonačne petlje)
    }

    return 0;
}