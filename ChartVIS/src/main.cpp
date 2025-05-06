#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <thread>

#include <queue>
#include <mutex>
#include <condition_variable>

// -- OWN Classes
#include "CORE/TradeExec.h"
#include "CORE/Plot.h"
#include "CORE/DataQueue.h"
#include "CORE/GLOBAL.hpp"

using namespace std;


void AddDatatQUEUE(const PlotChart::Candle& candle){
    std::lock_guard<std::mutex> lock(QueueLOCK);
    if (DataHandler.size() > 3){
        DataHandler.pop();
    }
    DataHandler.push(candle);
}




int main() {
    //#################################################################################################
    //################################ GUI ############################################################
    TradeExecution *ptrTradeExec = new TradeExecution();
    ptrTradeExec->sayHello();
    


    sf::RenderWindow window(sf::VideoMode(1700, 900), "Trading Chart");
    sf::Font font;
    if (!font.loadFromFile("/System/Library/Fonts/SFNSMono.ttf")) {
        cerr << "Fehler beim Laden der Schriftart." << endl;
        return -1;
    }
    PlotChart plotter;
    //vector<PlotChart::Candle> allCandles = plotter.loadCandlesFromCSV("../data/Apple_1MD.csv");
    vector<PlotChart::Candle> allCandles = plotter.loadCandlesFromCSV("../data/EURUSD1.csv");
    vector<PlotChart::Candle> candles; // Dynamisches Fenster der angezeigten Kerzen
    size_t windowSize = 30; // Anzahl der Kerzen im Fenster
    if (allCandles.size() <= windowSize) {
        cerr << "Nicht genügend Kerzen in der CSV-Datei." << endl;
        return -1;
    }
    cout << "Total candles in CSV: " << allCandles.size() << endl;
    // Starte mit den ersten 30 Kerzen
    candles.insert(candles.end(), allCandles.begin(), allCandles.begin() + windowSize);
    size_t currentIndex = windowSize; // Position der nächsten Kerze
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);
        plotter.drawStats(window, font, 0, 0, 300, 900);
        plotter.drawGrid(window, 320, 30, 1300, 450, 30, 10);
        plotter.drawTitle(window, "Price Chart", 320, 10, font);
        plotter.plotChart(window, 320, 30, 450, 1300, candles, font);
        plotter.drawTitle(window, "Volume", 320, 500, font);
        plotter.drawVolume(window, 320, 600, 1300, 200, candles, font);
        window.display();

        // Wartezeit von 500ms (0,5 Sekunden)
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        //#################################################################################################       
        ptrTradeExec->EntityExecute();

        // Neue Candle hinzufügen und die älteste entfernen
        if (currentIndex < allCandles.size()) {
            candles.erase(candles.begin());
            candles.push_back(allCandles[currentIndex]); // Newest Candle !!!
            // Write  Newest !Finished! Candle in Queue (not the First one because in RL this Candle would now just be forming)
            AddDatatQUEUE(*(candles.end() - 3));         // Use Iterator

            //ptrTradeExec->sayQueue();

            
            ++currentIndex;

        } else {
            cerr << "Keine weiteren Kerzen in der CSV-Datei. Programm pausiert." << endl;
            std::this_thread::sleep_for(std::chrono::seconds(1)); // 1 Sekunde warten
            continue; // Gehe zurück zum Anfang der Schleife und warte weiter
        }



    }

    return 0;
}
