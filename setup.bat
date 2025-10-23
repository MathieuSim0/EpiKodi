@echo off
REM Script d'installation des dépendances pour EpiKodi (Windows)

echo =========================================
echo   EpiKodi - Installation des dependances
echo =========================================
echo.

REM Créer le dossier external s'il n'existe pas
if not exist "external" mkdir external

REM Installer nlohmann/json
echo Installation de nlohmann/json...
if exist "external\nlohmann" (
    echo nlohmann/json deja installe
) else (
    cd external
    git clone https://github.com/nlohmann/json.git nlohmann
    if %ERRORLEVEL% EQU 0 (
        echo nlohmann/json installe avec succes
    ) else (
        echo Erreur lors de l'installation de nlohmann/json
        exit /b 1
    )
    cd ..
)

echo.

REM Vérifier la configuration
echo Verification de la configuration...
if not exist "config\config.json" (
    echo Fichier config.json manquant
    echo Copie de config.example.json...
    copy config\config.example.json config\config.json
    echo config.json cree
    echo.
    echo ATTENTION : Vous devez ajouter votre cle API TMDb dans config/config.json
    echo 1. Creez un compte sur https://www.themoviedb.org/
    echo 2. Obtenez votre cle API dans les parametres
    echo 3. Remplacez YOUR_TMDB_API_KEY_HERE dans config/config.json
) else (
    echo config.json trouve
)

echo.
echo =========================================
echo   Installation terminee !
echo =========================================
echo.
echo Prochaines etapes :
echo 1. Configurer votre cle API TMDb dans config/config.json
echo 2. Compiler le projet :
echo    mkdir build ^&^& cd build
echo    cmake .. -G "Visual Studio 17 2022"
echo    cmake --build . --config Release
echo 3. Lancer l'application :
echo    Release\EpiKodi.exe
echo.

pause
