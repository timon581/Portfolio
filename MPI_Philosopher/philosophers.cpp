#include <iostream>
#include <mpi.h>
#include <string>
#include <vector>
#include<cstdlib>
#include <windows.h>
using namespace std;

struct Fork {
    bool here;
    bool dirty;
};
int lmsg = 0;
int rmsg = 1;
int m = 1;

void SaljiVilicu(int tag, int msg, Fork &left, Fork &right, bool &leftReq, bool &rightReq, int rank, int size, MPI_Status status) {
    if (rank == 0) {
        if (status.MPI_SOURCE == size - 1) {
            if (right.dirty == false) {
                rightReq = true;
            }
            else {
                right.here = false;
                MPI_Send(&rmsg, 1, MPI_INT, size - 1, 1, MPI_COMM_WORLD);
            }
        }
        else if(status.MPI_SOURCE == rank + 1){
            if (left.dirty == false) {
                leftReq = true;
            }
            else {
                left.here = false;
                MPI_Send(&lmsg, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
            }
        }
    }
    else if (rank == size - 1) {
        if (status.MPI_SOURCE == rank - 1) {
            if (right.dirty == false) {
                rightReq = true;
            }
            else {
                right.here = false;
                MPI_Send(&rmsg, 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD);
            }
        }
        else if( status.MPI_SOURCE == 0){
            if (left.dirty == false) {
                leftReq = true;
            }
            else {
                left.here = false;
                MPI_Send(&lmsg, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
            }
        }
    }
    else {
        if (status.MPI_SOURCE == rank - 1) {
            if (right.dirty == false) {
                rightReq = true;
            }
            else {
                right.here = false;
                MPI_Send(&rmsg, 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD);
            }
        }
        else if (status.MPI_SOURCE == rank+1) {
            if (left.dirty == false) {
                leftReq = true;
            }
            else {
                left.here = false;
                MPI_Send(&lmsg, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
            }
        }
    }
}

void SaljiVilicuMisli(int tag, int msg, Fork& left, Fork& right, bool& leftReq, bool& rightReq, int rank, int size, MPI_Status status) {
    if (rank == 0) {
        if (status.MPI_SOURCE == size - 1) {
            right.here = false;
            MPI_Send(&rmsg, 1, MPI_INT, size - 1, 1, MPI_COMM_WORLD);
            
        }
        else if (status.MPI_SOURCE == rank + 1) {
            left.here = false;
            MPI_Send(&lmsg, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
            
        }
    }
    else if (rank == size - 1) {
        if (status.MPI_SOURCE == rank - 1) {
             right.here = false;
             MPI_Send(&rmsg, 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD);
            
        }
        else if (status.MPI_SOURCE == 0) {
            left.here = false;
            MPI_Send(&lmsg, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
            
        }
    }
    else {
        if (status.MPI_SOURCE == rank - 1) {
            right.here = false;
            MPI_Send(&rmsg, 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD);
            
        }
        else if (status.MPI_SOURCE == rank + 1) {
            left.here = false;
            MPI_Send(&lmsg, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
            
        }
    }
}

int getRandomNumber(int min, int max) {
    return min + rand() % (max - min + 1);
}

void obradiZahtjev(int tag, int msg, Fork &left, Fork &right, bool &leftReq, bool &rightReq, int rank, int size, MPI_Status status) {
    if (tag == 1) {
        if (rank == 0) {
            if (status.MPI_SOURCE == size - 1) {
                right.here = true;
                right.dirty = false;
            }
            else if (status.MPI_SOURCE == rank + 1) {
                left.here = true;
                left.dirty = false;
            }
        }
        else if (rank == size - 1) {
            if (status.MPI_SOURCE == 0) {
                left.here = true;
                left.dirty = false;
            }
            else if (status.MPI_SOURCE == rank - 1) {
                right.here = true;
                right.dirty = false;
            }
        }
        else {
            if (status.MPI_SOURCE == rank + 1) {
                left.here = true;
                left.dirty = false;
            }
            else if (status.MPI_SOURCE == rank - 1) {
                right.here = true;
                right.dirty = false;
            }
        }
    }
    else {
        SaljiVilicu(tag, msg, left, right, leftReq, rightReq, rank, size, status);
    }
}

void obradiZahtjevMisli(int tag, int msg, Fork& left, Fork& right, bool& leftReq, bool& rightReq, int rank, int size, MPI_Status status) {
    if (tag == 1) {
        if (rank == 0) {
            if (status.MPI_SOURCE == size - 1) {
                right.here = true;
                right.dirty = false;
            }
            else if (status.MPI_SOURCE == rank + 1) {
                left.here = true;
                left.dirty = false;
            }
        }
        else if (rank == size - 1) {
            if (status.MPI_SOURCE == 0) {
                left.here = true;
                left.dirty = false;
            }
            else if (status.MPI_SOURCE == rank - 1) {
                right.here = true;
                right.dirty = false;
            }
        }
        else {
            if (status.MPI_SOURCE == rank + 1) {
                left.here = true;
                left.dirty = false;
            }
            else if (status.MPI_SOURCE == rank - 1) {
                right.here = true;
                right.dirty = false;
            }
        }
    }
    else {
        SaljiVilicuMisli(tag, msg, left, right, leftReq, rightReq, rank, size, status);
    }
}
int main(int argc, char** argv) {
    
    int rank, size;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    Fork left, right;

    if (rank == 0 ){
        left.here = true;
        right.here = true;
        left.dirty = true;
        right.dirty = true;
    }
    else if (rank == size - 1) {
        left.here = false;
        right.here = false;
        left.dirty = false;
        right.dirty = false;
    }
    else {
        left.here = true;
        right.here = false;
        left.dirty = true;
        right.dirty = false;
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    srand(rank + time(nullptr));
    bool leftReq = false; 
    bool rightReq = false;


    while (1) {
        //Sleep(1000);
        int n = getRandomNumber(1, 4);
        int flag = 0;
        MPI_Status status;
        std::cout << "Filozof " << rank << " misli." << endl;

        for (int i = 0; i < n; i++) {
            Sleep(1000);
           
            MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
            //std::cout << "Provjerio " << flag << endl;
            if (flag) {
                //std::cout << "FIlozof  " << rank << " obraduje zahtjev" << endl;
                int msg;
                MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                int tag = status.MPI_TAG;
                //std::cout << "Primio zahtjev " << tag << endl;

                obradiZahtjevMisli(tag, msg, left, right, leftReq, rightReq, rank, size, status);
            }
        }

        while (left.here != true || right.here != true) {
            //Sleep(1000);
            if (rank == 0) {
                if (left.here != true) {
                    MPI_Send(&rmsg, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                    std::cout << "Filozof " << rank << " trazi desnu vilicu filozofa "<< rank + 1 << endl;
                }
                else if(right.here !=true) {
                    MPI_Send(&lmsg, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD);
                    std::cout << "Filozof " << rank << " trazi lijevu vilicu filozofa " << size - 1 << endl;
                }
            }
            else if (rank == size - 1) {
                if (left.here != true) {
                    MPI_Send(&rmsg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                    std::cout << "Filozof " << rank << " trazi desnu vilicu filozofa " << 0 << endl;
                }
                else if(right.here != true) {
                    MPI_Send(&lmsg, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
                    std::cout << "Filozof " << rank << " trazi lijevu vilicu filozofa " << rank - 1 << endl;
                }
            }
            else {
                if (left.here != true) {
                    MPI_Send(&rmsg, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                    std::cout << "Filozof " << rank << " trazi desnu vilicu filozofa " << rank + 1 << endl;
                }
                else if (right.here != true) {
                    MPI_Send(&lmsg, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
                    std::cout << "Filozof " << rank << " trazi lijevu vilicu filozofa " << rank - 1 << endl;
                }
            }
            bool tempLeft = left.here;
            bool tempRight = right.here;
            do{
                Sleep(1000);
                int msg;
                MPI_Status status;
                MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

                int tag = status.MPI_TAG;
                if (tag == 1) {
                    if (rank == 0) {
                        if (status.MPI_SOURCE == rank + 1) {
                            left.here = true;
                            left.dirty = false;
                            std::cout << "Filozof " << rank << " dobio lijevu vilicu od" << rank + 1 << "  "<< left.here << right.here << endl;
                          
                        }
                        else if(status.MPI_SOURCE == size-1){
                            right.here = true;
                            right.dirty = false;
                            std::cout << "Filozof " << rank << " dobio desnu vilicu od "<< size-1 << "  " << left.here << right.here << endl;
                            
                        }
                    }
                    else if (rank == size - 1) {
                        if (status.MPI_SOURCE == 0) {
                            left.here = true;
                            left.dirty = false;
                            std::cout << "Filozof " << rank << " dobio lijevu vilicu od "<< 0 << "  " << left.here << right.here << endl;
                            
                        }
                        else if(status.MPI_SOURCE == rank - 1){
                            right.here = true;
                            right.dirty = false;
                            std::cout << "Filozof " << rank << " dobio desnu vilicu "<< rank -1 << "  " << left.here << right.here << endl;
                            
                        }
                    }
                    else {
                        if (status.MPI_SOURCE == rank + 1) {
                            left.here = true;
                            left.dirty = false;
                            std::cout << "Filozof " << rank << " dobio lijevu vilicu od "<< rank + 1 << "  " << left.here << right.here << endl;
                            
                        }
                        else if (status.MPI_SOURCE == rank - 1 ) {
                            right.here = true;
                            right.dirty = false;
                            std::cout << "Filozof " << rank << " dobio desnu vilicu od "<< rank - 1 << "L, R: " << left.here << right.here << endl;
                            
                        }
                    }
                }
                else {
                    SaljiVilicu(tag, msg, left, right, leftReq, rightReq, rank, size, status);
                }
            } while ((tempLeft == left.here && tempRight == right.here) && (left.here != true || right.here != true));
        }

        std::cout << "Filozof " << rank << " jede." << endl;
        std::cout << "--------------------------------------------------------------------------------------------------" << endl;
        int sl = getRandomNumber(1, 4);
        Sleep(sl * 1000);
        left.dirty = true;
        right.dirty = true;
        
        if (rightReq == true) {
            
            if (rank == 0) {
                right.here = false;
                MPI_Send(&lmsg, 1, MPI_INT, size - 1, 1, MPI_COMM_WORLD);
                
            }
            else {
                right.here = false;
                MPI_Send(&lmsg, 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD);
            }
            rightReq = false;
        }
        if (leftReq == true) {
            
            if (rank == size -1 ) {
                left.here = false;
                MPI_Send(&rmsg, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
            }
            else {
                left.here = false;
                MPI_Send(&rmsg, 1, MPI_INT, rank + 1, 1, MPI_COMM_WORLD);
            }
            leftReq = false;
        }
        
    }

    //std::cout << "I am process " << rank << " out of " << size << ", Left fork: "<<left.here << ", Right fork: " << right.here << endl;
    MPI_Finalize();

    return 0;
}

