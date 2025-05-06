#ifndef DATA_QUEUE_H
#define DATA_QUEUE_H

#include <queue>
#include <mutex>
#include "Plot.h" // Enthält die Definition von Candle

// Globale Queue und zugehöriger Mutex
extern std::queue<PlotChart::Candle> DataHandler; // Globale Queue
extern std::mutex QueueLOCK;                     // Mutex für Thread-Sicherheit

#endif // DATA_QUEUE_H
