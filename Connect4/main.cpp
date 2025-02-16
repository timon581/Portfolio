#include <iostream>
#include <vector>
#include <mpi.h>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <limits>
#include "board.h"

using namespace std;



typedef struct {
    Board board; 
    int pMove; 
} sendData;

typedef struct {
    int pMove;
    double result;
} resultData;

int mpi_rank, processorNum;

double Evaluate(Board Current, player LastMover, int iLastCol, int iDepth) {
    double dResult, dTotal;
    player NewMover;
    bool bAllLose = true, bAllWin = true;
    int iMoves;
    Board B;

    if(Current.GameEnd(iLastCol))
        if(LastMover == CPU)
            return 1;
        else 
            return -1;
    if(iDepth == 0)
        return 0;
    iDepth--;
    if(LastMover == CPU)
        NewMover = HUMAN;
    else
        NewMover = CPU;
    dTotal = 0;
    iMoves = 0;
    for(int iCol=0; iCol<Current.Columns(); iCol++)
    {if(Current.MoveLegal(iCol))
        {iMoves++;
            Current.Move(iCol, NewMover);
            dResult = Evaluate(Current, NewMover, iCol, iDepth);
            Current.UndoMove(iCol);
            if(dResult > -1)
                bAllLose = false;
            if(dResult != 1)
                bAllWin = false;
            if(dResult == 1 && NewMover == CPU)
                return 1;
            if(dResult == -1 && NewMover == HUMAN)
                return -1;
            dTotal += dResult;
        }
    }
    if(bAllWin == true)
        return 1;
    if(bAllLose == true)
        return -1;
    dTotal /= iMoves;
    return dTotal;
}



double mEvaluate(Board Current, player LastMover, int iLastCol, int iDepth) {
    double dResult, dTotal;
    player NewMover;
    bool bAllLose = true, bAllWin = true;
    int iMoves;

    if(Current.GameEnd(iLastCol))
        if(LastMover == CPU)
            return 1;
        else 
            return -1;

    if(LastMover == CPU)
        NewMover = HUMAN;
    else
        NewMover = CPU;
    dTotal = 0;
    iMoves = 0;
    if(iDepth <= 0) {
        for(int i = 0; i < 7; i++) {
            if(Current.MoveLegal(i)) {
                iMoves++;
                Current.Move(i, NewMover);
                sendData data;
                data.board = Current;
                data.pMove = i;
                
               Current.UndoMove(i);
                MPI_Send(&data, sizeof(sendData), MPI_BYTE, (i % (processorNum - 1) + 1), 1, MPI_COMM_WORLD);
            }
        }
        for(int i = 0; i < iMoves; i++) {
            resultData data;
            MPI_Recv(&data, sizeof(resultData), MPI_BYTE,  (i % (processorNum - 1) + 1), 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
           
            dResult = data.result;
            
            if(dResult > -1)
                bAllLose = false;
            if(dResult != 1)
                bAllWin = false;
            if(dResult == 1 && NewMover == CPU)
                return 1; 
            if(dResult == -1 && NewMover == HUMAN)
                return -1; 
            dTotal += dResult;
        }

    } else {
        for(int iCol=0; iCol<Current.Columns(); iCol++) {
            if(Current.MoveLegal(iCol)) {
                iMoves++;
                Current.Move(iCol, NewMover);
                dResult = mEvaluate(Current, NewMover, iCol, iDepth-1);
                
                Current.UndoMove(iCol);
                if(dResult > -1)
                    bAllLose = false;
                if(dResult != 1)
                    bAllWin = false;
                if(dResult == 1 && NewMover == CPU)
                    return 1;
                if(dResult == -1 && NewMover == HUMAN) {
                    return -1;
                }

                dTotal += dResult;
            }
        }
    }
    if(bAllWin == true)
        return 1;
    if(bAllLose == true) {
        return -1;
    }

    dTotal /= iMoves;
    return dTotal;
}



int best_move(Board board, int depth) {
    double maxValue = -2;
    int index = -2;
    for(int i = 0; i < 7; i++) {
        if(board.MoveLegal(i)) {
            board.Move(i, CPU);
            double value = Evaluate(board, CPU, i, depth);
            board.UndoMove(i);
            if(maxValue < value) {
                maxValue = value;
                index = i;
            }
        }
    }
    return index;
}


int parallelMovePick(Board board, int mdepth, int sdepth, int rank, int size) {

    double best_value = -2;
    int index = -2;

    if (mdepth != 0) {
        for (int i = 0; i < 7; i++) {
            if(board.MoveLegal(i)) {
                board.Move(i, CPU);
                double move_value = mEvaluate(board, CPU, i, mdepth-1);
                board.UndoMove(i);
                if (move_value > best_value) {
                    best_value = move_value;
                    index = i;
                }
            }
        }
    }

    return index;
}

bool check_win(Board board) {
    for(int iCol=0; iCol<board.Columns(); iCol++)
        if(board.GameEnd(iCol)) {
            return true;
        }
    return false;
}


int main(int argc, char** argv) {


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &processorNum);

    if (argc != 3) {
        if (mpi_rank == 0) {
            cout << "Usage: mpiexec -n <NUM_PROCESSES> ./connect4 <MASTER_DEPTH> <SLAVE_DEPTH>" << endl;
        }
        MPI_Finalize();
        return 1;
    }

    int MASTER_DEPTH = atoi(argv[1]);
    int SLAVE_DEPTH = atoi(argv[2]);

    Board game_board;
    int stop_signal = 0;
    double total_time = 0;
    int num_moves = 0;

    if (mpi_rank == 0) {
        while (stop_signal==0) {
            game_board.printBoard();
            int player_move;
            cout << "Enter your move (1-7): ";
            cin >> player_move;
            player_move -= 1;
            if (game_board.MoveLegal(player_move)) {
                game_board.Move(player_move, HUMAN);
                if (check_win(game_board)) {
                    game_board.printBoard();
                    cout << "Player wins!" << endl;
                    break;
                }

                clock_t start_time = clock();
                int computer_move = (processorNum == 1 || MASTER_DEPTH == 0) ? best_move(game_board, MASTER_DEPTH) : parallelMovePick(game_board, MASTER_DEPTH, SLAVE_DEPTH, mpi_rank, processorNum);
                clock_t end_time = clock();
                double move_time = double(end_time - start_time) / CLOCKS_PER_SEC;
                total_time += move_time;
                num_moves++;

                cout << "Time: " << move_time << " seconds" << endl;
                game_board.Move(computer_move, CPU);
                if (check_win(game_board)) {
                    game_board.printBoard();
                    cout << "Computer wins!" << endl;
                    break;
                }
            }
        }
        cout << game_board.is_full() << endl;
        for (int i = 1; i < processorNum; i++) {
            sendData data;
            data.board = game_board;
            data.pMove = -2;
            stop_signal = 1;
            MPI_Send(&data, sizeof(sendData), MPI_BYTE, i, 1, MPI_COMM_WORLD);
        }
        if (num_moves > 0) {
            double average_time = total_time / num_moves;
            cout << "Average time per computer move: " << average_time << " seconds" << endl;
        }
    } else {
        while (true) {
            int move;
            sendData data;

            MPI_Recv(&data, sizeof(sendData), MPI_BYTE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (data.pMove == -2) {
                break;     
            }
            double eval = Evaluate(data.board, data.board.getLastMover(), data.pMove, SLAVE_DEPTH);
            resultData result;
            result.pMove = data.pMove;
            result.result = eval;
            MPI_Send(&result, sizeof(resultData), MPI_BYTE, 0, 1, MPI_COMM_WORLD);

        }
    }
    MPI_Finalize();
    return 0;
}