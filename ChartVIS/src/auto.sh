?
#!/bin/bash

# Setze Variablen
SOURCE_FILE="main.cpp"
CORE_SOURCE="CORE/TradeExec.cpp CORE/Plot.cpp CORE/DataQueue.cpp CORE/GLOBAL.cpp"
OUTPUT_FILE="BackTestEngine"
SFML_INCLUDE_DIR="/opt/homebrew/opt/sfml/include"
SFML_LIB_DIR="/opt/homebrew/opt/sfml/lib"

# Prüfe, ob die Haupt-Quelldatei existiert
if [ ! -f "$SOURCE_FILE" ]; then
  echo "Fehler: $SOURCE_FILE existiert nicht."
  exit 1
fi

# Prüfe, ob jede Datei in CORE_SOURCE existiert
for FILE in $CORE_SOURCE; do
  if [ ! -f "$FILE" ]; then
    echo "Fehler: $FILE existiert nicht."
    exit 1
  fi
done

# Kompiliere die Quelldateien
echo "Kompiliere $SOURCE_FILE und $CORE_SOURCE !!!"
g++ $SOURCE_FILE $CORE_SOURCE -o "$OUTPUT_FILE" \
  -std=c++11 \
  -I"$SFML_INCLUDE_DIR" \
  -L"$SFML_LIB_DIR" \
  -lsfml-graphics -lsfml-window -lsfml-system

# Prüfe auf Kompilierungsfehler
if [ $? -ne 0 ]; then
  echo "Fehler: Kompilierung fehlgeschlagen."
  exit 1
fi

echo "Kompilierung erfolgreich. Führe $OUTPUT_FILE aus..."

# Führe das kompilierte Programm aus
./"$OUTPUT_FILE"

# Prüfe auf Fehler während der Ausführung
if [ $? -ne 0 ]; then
  echo "Fehler: Ausführung von $OUTPUT_FILE fehlgeschlagen."
  exit 1
fi

echo "Ausführung abgeschlossen."
