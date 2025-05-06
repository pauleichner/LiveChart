#ifndef PLOTCHART_H
#define PLOTCHART_H

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class PlotChart {
public:
    struct Candle {
        float open, high, low, close, volume;
        Candle(float o, float h, float l, float c, float v);
    };

    // Methoden
    std::vector<Candle> loadCandlesFromCSV(const std::string& filePath, size_t maxCandles = 30);
    void drawGrid(sf::RenderWindow& window, float xOffset, float yOffset, float chartWidth, float chartHeight, size_t numColumns, size_t numRows);
    void plotChart(sf::RenderWindow& window, float xOffset, float yOffset, float chartHeight, float chartWidth, const std::vector<Candle>& candles, sf::Font& font);
    void drawVolume(sf::RenderWindow& window, float xOffset, float yOffset, float chartWidth, float chartHeight, const std::vector<Candle>& candles, sf::Font& font);
    void drawStats(sf::RenderWindow& window, sf::Font& font, float x, float y, float width, float height);
    void drawYAxis(sf::RenderWindow& window, float xOffset, float yOffset, float chartHeight, float minValue, float maxValue, int steps, sf::Font& font);
    void drawTitle(sf::RenderWindow& window, const std::string& title, float xOffset, float yOffset, sf::Font& font);
};

#endif // PLOTCHART_H
