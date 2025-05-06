// Hier findet die eigentliche Logik des Algo statt
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include "TradeExec.h"
#include "DataQueue.h"
#include "GLOBAL.hpp"


std::mutex MUTBalance; 



void TradeExecution::sayHello() {
    std::cout << "Hello from Trade Execution CPP" << '\n';
}

void TradeExecution::sayQueue() {
    std::lock_guard<std::mutex> lock(QueueLOCK); // Schütze die Queue

    if (DataHandler.empty()) {
        std::cout << "Die Queue ist leer." << std::endl;
        return;
    }

    // Iteriere über die Queue und gib die relevanten Daten aus
    std::queue<PlotChart::Candle> tempQueue = DataHandler; // Kopiere die Queue
    while (!tempQueue.empty()) {
        const auto& candle = tempQueue.front();
        std::cout << "High: " << candle.high << ", Close: " << candle.close  << ", Low : " << candle.low << std::endl;
        tempQueue.pop();
    }
}

// Hier müssen noch Eingaben stattfinden
void TradeExecution::updatePerformanceStats(double wp, double pf, double avp, double b, int n) {
    std::lock_guard<std::mutex> lock(MUTBalance);
    // Nur zu Testzwecken
    BALANCE = std::rand();
}


// MAIN FUNCTION Execute the ALGO
void TradeExecution::EntityExecute() {
    // Immer dann ausführen wenn neue Candle in der Queue angekommen ist
    double winperc = 0;
    double pfactor = 0;
    double avgprofit = 0;
    double balance = 5000.0;
    int numofopenpos = 0;


    std::thread UpdatePERF(&TradeExecution::updatePerformanceStats, this ,winperc, pfactor, avgprofit, balance, numofopenpos);
    UpdatePERF.join();
    





}
