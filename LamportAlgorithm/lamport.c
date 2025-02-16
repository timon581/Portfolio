#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

#define NUM_FILOZOFA 5

#define ZAHTJEV 1
#define ODGOVOR 2
#define IZLAZAK 3

int red_poruka[NUM_FILOZOFA];
int cl;

typedef struct {
    long msgtyp;  // Message type for msgrcv
    int filozof_id;
    int tip;
    int c;
} poruka;

void misliti(int filozof_id) {
    printf("Filozof %d misli...\n", filozof_id);
    sleep(rand() % 3 + 1); //misli izmedu 1 i 3 sekunde
}

void jedi(int filozof_id){
     printf("Filozof %d jede...\n", filozof_id);
     sleep(rand() % 3 + 1); //misli izmedu 1 i 3 sekunde
     printf("Filozof %d zavrsio jelo\n", filozof_id);
}

int update_clock(int c1, int c2){
    if(c1 < c2){
        return c2 + 1;
    }else if ( c1 > c2){
        return c1 + 1;
    }else{
        return c1 + 1;
    }
}

// Funkcija za slanje poruka
void posalji_poruku(int red_id, int primatelj_id, int sender_id, int type) {
    poruka msg;
    msg.msgtyp = primatelj_id + 1;  
    msg.tip = type;
    msg.filozof_id = sender_id;
    msg.c = cl;
    int result = msgsnd(red_id, &msg, sizeof(poruka) - sizeof(long), 0);
    if (result == -1) {
        perror("Greška pri slanju poruke");
        exit(1);
    }
    if (msg.tip == ZAHTJEV) {
        printf("Filozof %d šalje poruku ZAHTJEV filozofu %d\n", sender_id, primatelj_id);
    } else if (msg.tip == ODGOVOR) {
        printf("Filozof %d šalje poruku ODGOVOR filozofu %d\n", sender_id, primatelj_id);
    } else if (msg.tip == IZLAZAK) {
        printf("Filozof %d šalje poruku IZLAZAK filozofu %d\n", sender_id, primatelj_id);
    } else {
        printf("Greška s tipom poslane poruke: %d\n", msg.tip);
    }
}

// Funkcija za primanje poruka
poruka primi_poruku(int red_id, int moj_id) {
    poruka msg;
    int result = msgrcv(red_id, &msg, sizeof(poruka) - sizeof(long), moj_id + 1, 0);
    if (result == -1) {
        perror("Greška pri primanju poruke");
        exit(1);
    }
    if (msg.tip == ZAHTJEV) {
        printf("Filozof %d prima poruku ZAHTJEV od filozofa %d s clockom %d, a njegov clock je %d\n", moj_id, msg.filozof_id, msg.c, cl);
    } else if (msg.tip == ODGOVOR) {
        printf("Filozof %d prima poruku ODGOVOR od filozofa %d s clockom %d, an njegov clock je %d\n", moj_id, msg.filozof_id, msg.c, cl);
    } else if (msg.tip == IZLAZAK) {
        printf("Filozof %d prima poruku IZLAZAK od filozofa %d s clockom %d, a njegov clock je %d\n", moj_id, msg.filozof_id, msg.c, cl);
    } else {
        printf("Greška s tipom primljene poruke: %d\n", msg.tip);
    }
    return msg;
}

// Funkcija za uzimanje štapića 
void uzmi_stapice(int id) {
    int odgovor_lijevi = 0;
    int odgovor_desni = 0;
    int zahtjev_lijevi = 0;
    int zahtjev_desni = 0;
    int izlazak_lijevi = 0;
    int izlazak_desni = 0;
    int zahtjev_clock = cl;

    printf("Filozof %d je gladan\n", id);
    
     
    posalji_poruku(red_poruka[id], (id + NUM_FILOZOFA - 1) % NUM_FILOZOFA, id, ZAHTJEV);
    cl++;
    posalji_poruku(red_poruka[id], (id + 1) % NUM_FILOZOFA, id, ZAHTJEV);
    cl++;
    while(odgovor_lijevi + odgovor_desni < 2 || izlazak_lijevi == 1 || izlazak_desni==1){
        //blokirajuci pozivi
        //printf("Stanja filozofa %d: %d %d %d %d %d %d\n", id, odgovor_lijevi, odgovor_desni, zahtjev_lijevi, zahtjev_desni, izlazak_lijevi, izlazak_desni);
        poruka msg = primi_poruku(red_poruka[id], id);
        //printf("Stanja filozofa %d: %d %d %d %d %d %d\n", id, odgovor_lijevi, odgovor_desni, zahtjev_lijevi, zahtjev_desni, izlazak_lijevi, izlazak_desni);
        if(msg.tip == ODGOVOR){
            if(msg.filozof_id == ((id + NUM_FILOZOFA - 1) % NUM_FILOZOFA)){
                odgovor_lijevi++;
                
            }else{
                odgovor_desni++;
            }
        }else if(msg.tip == ZAHTJEV){
            if(msg.filozof_id == ((id + NUM_FILOZOFA - 1) % NUM_FILOZOFA)){
                zahtjev_lijevi++;
                if(msg.c <= zahtjev_clock){
                    izlazak_lijevi = 1;
                }
                posalji_poruku(red_poruka[id], (id + NUM_FILOZOFA - 1) % NUM_FILOZOFA, id, ODGOVOR);
                cl++;
            }else{
                zahtjev_desni++;
                if(msg.c <= zahtjev_clock){
                    izlazak_desni = 1;
                }
                posalji_poruku(red_poruka[id], (id + 1) % NUM_FILOZOFA, id, ODGOVOR);
                cl++;
            }
        }else{
            if(msg.filozof_id == ((id + NUM_FILOZOFA - 1) % NUM_FILOZOFA)){
                izlazak_lijevi = 0;
            }else{
                izlazak_desni = 0;
            }
            
        }
        //printf("Filozof %d ima odgovora %d\n", id, odgovor_desni+odgovor_lijevi);
        cl = update_clock(cl, msg.c);
    }
    jedi(id);
        
    // Obavještava susjede da su štapići slobodni
    posalji_poruku(red_poruka[id], (id + NUM_FILOZOFA - 1) % NUM_FILOZOFA, id, IZLAZAK);
    cl++;
    posalji_poruku(red_poruka[id], (id + 1) % NUM_FILOZOFA, id, IZLAZAK);
    cl++;
    odgovor_lijevi = 0;
    odgovor_desni = 0;
    zahtjev_lijevi = 0;
    zahtjev_desni = 0;
    izlazak_lijevi = 0;
    izlazak_desni = 0;
    zahtjev_clock = cl;
}

// Funkcija filozofa
void filozof_proces(int id) {
    while (1) {
        misliti(id);
        uzmi_stapice(id);
    }
}

// Glavna funkcija
int main() {
    int i;
    pid_t pid;
    key_t key;
    
    // Kreiranje redova poruka za svaki proces-filozof
    for (i = 0; i < NUM_FILOZOFA; i++) {
        key = ftok("program", i + 1);
        red_poruka[i] = msgget(key, 0666 | IPC_CREAT);
    }
    
    // Kreiranje procesa za svakog filozofa
    for (i = 0; i < NUM_FILOZOFA; i++) {
        pid = fork();
        cl = i;
        if (pid < 0) {
            perror("Greška pri kreiranju procesa");
            exit(1);
        } else if (pid == 0) {
            // Djetetov proces izvršava funkciju filozof_proces
            filozof_proces(i);
            exit(0);
        }
    }
    
    // Čekanje da svi procesi završe (što se ovdje neće dogoditi jer je petlja beskonačna)
    for (i = 0; i < NUM_FILOZOFA; i++) {
        wait(NULL);
    }
    
    // Brisanje redova poruka
    for (i = 0; i < NUM_FILOZOFA; i++) {
        msgctl(red_poruka[i], IPC_RMID, NULL);
    }
    
    return 0;
}