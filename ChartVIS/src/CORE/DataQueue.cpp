#include "DataQueue.h"

// Definition der globalen Queue und des Mutex
std::queue<PlotChart::Candle> DataHandler;
std::mutex QueueLOCK;
