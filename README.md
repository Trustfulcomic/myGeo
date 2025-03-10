# MyGeo
Jednoduchá geometrická GeoGebra
## Kompilace (Linux)
Pro kompilaci je potřeba *cmake* a compiler pro c++.
```
mkdir build
cd build
cmake ..
cmake --build . -jN
```
`N` značí počet jader, na kterých bude kompilace probíhat. Zkompilovaný program se nachází v `build/subprojects/Build/myGeo_core/main`.
## Dokumentace
Vývojářská dokumentace je napsána pomocí *doxygen*. Ve složce `doc` je předpřipravený konfigurační soubor, tedy pro vytvoření konfigurace stačí v této složce spustit příkaz `doxygen Doxyfile`.
Uživatelská dokumentace je v souboru `doc/userman.tex`. S nainstalovaným LaTeXem stačí pak spustit příkaz `pdflatex userman.tex`.