#include "Plot.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

// Eigene Libaries
#include "GLOBAL.hpp"




PlotChart::Candle::Candle(float o, float h, float l, float c, float v) 
    : open(o), high(h), low(l), close(c), volume(v) {}

std::vector<PlotChart::Candle> PlotChart::loadCandlesFromCSV(const std::string& filePath, size_t maxCandles) {
    std::vector<Candle> candles;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Fehler beim Öffnen der Datei: " << filePath << std::endl;
        return candles;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;

        getline(ss, cell, '\t'); // Timestamp
        getline(ss, cell, '\t'); // Unused column
        float open = std::stof(cell);
        getline(ss, cell, '\t');
        float high = std::stof(cell);
        getline(ss, cell, '\t');
        float low = std::stof(cell);
        getline(ss, cell, '\t');
        float close = std::stof(cell);
        getline(ss, cell, '\t');
        float volume = std::stof(cell);

        candles.emplace_back(open, high, low, close, volume);
    }

    file.close();
    return candles;
}

void PlotChart::drawGrid(sf::RenderWindow& window, float xOffset, float yOffset, float chartWidth, float chartHeight, size_t numColumns, size_t numRows) {
    float columnWidth = chartWidth / numColumns;
    for (size_t i = 0; i <= numColumns; ++i) {
        float x = xOffset + i * columnWidth;
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x, yOffset), sf::Color(100, 100, 100)),
            sf::Vertex(sf::Vector2f(x, yOffset + chartHeight), sf::Color(100, 100, 100))
        };
        window.draw(line, 2, sf::Lines);
    }

    float rowHeight = chartHeight / numRows;
    for (size_t i = 0; i <= numRows; ++i) {
        float y = yOffset + i * rowHeight;
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(xOffset, y), sf::Color(100, 100, 100)),
            sf::Vertex(sf::Vector2f(xOffset + chartWidth, y), sf::Color(100, 100, 100))
        };
        window.draw(line, 2, sf::Lines);
    }
}

void PlotChart::plotChart(sf::RenderWindow& window, float xOffset, float yOffset, float chartHeight, float chartWidth, const std::vector<Candle>& candles, sf::Font& font) {
    if (candles.empty()) {
        std::cerr << "Keine Kerzen zum Anzeigen." << std::endl;
        return;
    }

    float candleWidth = chartWidth / (candles.size() * 1.5f);
    float minValue = candles[0].low, maxValue = candles[0].high;

    for (const auto& c : candles) {
        minValue = std::min(minValue, c.low);
        maxValue = std::max(maxValue, c.high);
    }

    float yScale = chartHeight / (maxValue - minValue);
    drawYAxis(window, xOffset + chartWidth + 60, yOffset, chartHeight, minValue, maxValue, 10, font);

    for (size_t i = 0; i < candles.size(); ++i) {
        const Candle& c = candles[i];
        float x = xOffset + i * candleWidth * 1.5f;

        float yOpen = yOffset + chartHeight - (c.open - minValue) * yScale;
        float yClose = yOffset + chartHeight - (c.close - minValue) * yScale;
        float yHigh = yOffset + chartHeight - (c.high - minValue) * yScale;
        float yLow = yOffset + chartHeight - (c.low - minValue) * yScale;

        sf::RectangleShape body(sf::Vector2f(candleWidth, std::abs(yClose - yOpen)));
        body.setPosition(x, std::min(yClose, yOpen));
        body.setFillColor(c.close >= c.open ? sf::Color::Green : sf::Color::Red);

        sf::RectangleShape wick(sf::Vector2f(2, yHigh - yLow));
        wick.setPosition(x + candleWidth / 2 - 1, yLow);
        wick.setFillColor(sf::Color::White);

        window.draw(wick);
        window.draw(body);
    }
}

void PlotChart::drawVolume(sf::RenderWindow& window, float xOffset, float yOffset, float chartWidth, float chartHeight, const std::vector<Candle>& candles, sf::Font& font) {
    if (candles.empty()) return;

    float maxVolume = candles[0].volume;
    for (const auto& c : candles) {
        maxVolume = std::max(maxVolume, c.volume);
    }

    drawYAxis(window, xOffset - 17 + chartWidth + 60, yOffset, chartHeight, 0, maxVolume, 5, font);

    float xStep = chartWidth / candles.size();
    float yScale = chartHeight / maxVolume;

    sf::VertexArray volumeBars(sf::LineStrip, candles.size());
    for (size_t i = 0; i < candles.size(); ++i) {
        float x = xOffset + i * xStep + xStep / 2;
        float y = yOffset + chartHeight - (candles[i].volume * yScale);
        volumeBars[i].position = sf::Vector2f(x, y);
        volumeBars[i].color = sf::Color::Blue;
    }
    window.draw(volumeBars);
}

void PlotChart::drawStats(sf::RenderWindow& window, sf::Font& font, float x, float y, float width, float height) {
    sf::RectangleShape statsPanel(sf::Vector2f(width, height));
    statsPanel.setFillColor(sf::Color::Black);
    statsPanel.setPosition(x, y);
    window.draw(statsPanel);

    sf::Text statsText;
    statsText.setFont(font);
    statsText.setCharacterSize(16);
    statsText.setFillColor(sf::Color::White);
    statsText.setPosition(x + 10, y + 10);

    std::stringstream stats;

    //stats << "FPS = 60\nTPS = 120\nTicks = 1000\n";
    stats << "Date (US) = 2024-01-01\nTokyo = 09:00\nLondon = 01:00\n";
    stats << "Win % = 55\nProfit factor = 1.5\n";
    stats << "Balance: " << std::fixed << std::setprecision(2) << BALANCE << " EURO\n";
    statsText.setString(stats.str());

    window.draw(statsText);
}

void PlotChart::drawYAxis(sf::RenderWindow& window, float xOffset, float yOffset, float chartHeight, float minValue, float maxValue, int steps, sf::Font& font) {
    float stepSize = (maxValue - minValue) / steps;
    float yStep = chartHeight / steps;

    for (int i = 0; i <= steps; ++i) {
        float y = yOffset + chartHeight - i * yStep;
        float value = minValue + i * stepSize;

        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(14);
        text.setFillColor(sf::Color::White);
        text.setString(std::to_string(value));
        text.setPosition(xOffset - 60, y - 10);
        window.draw(text);
    }
}

void PlotChart::drawTitle(sf::RenderWindow& window, const std::string& title, float xOffset, float yOffset, sf::Font& font) {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);
    text.setString(title);
    text.setPosition(xOffset, yOffset);
    window.draw(text);
}
