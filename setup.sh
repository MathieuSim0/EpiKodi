#!/bin/bash

# Script d'installation des dépendances pour EpiKodi

echo "========================================="
echo "  EpiKodi - Installation des dépendances"
echo "========================================="
echo ""

# Créer le dossier external s'il n'existe pas
mkdir -p external

# Installer nlohmann/json
echo "📦 Installation de nlohmann/json..."
if [ -d "external/nlohmann" ]; then
    echo "✓ nlohmann/json déjà installé"
else
    cd external
    git clone https://github.com/nlohmann/json.git nlohmann
    if [ $? -eq 0 ]; then
        echo "✓ nlohmann/json installé avec succès"
    else
        echo "✗ Erreur lors de l'installation de nlohmann/json"
        exit 1
    fi
    cd ..
fi

echo ""

# Vérifier la configuration
echo "🔧 Vérification de la configuration..."
if [ ! -f "config/config.json" ]; then
    echo "⚠️  Fichier config.json manquant"
    echo "   Copie de config.example.json..."
    cp config/config.example.json config/config.json
    echo "✓ config.json créé"
    echo ""
    echo "⚠️  ATTENTION : Vous devez ajouter votre clé API TMDb dans config/config.json"
    echo "   1. Créez un compte sur https://www.themoviedb.org/"
    echo "   2. Obtenez votre clé API dans les paramètres"
    echo "   3. Remplacez YOUR_TMDB_API_KEY_HERE dans config/config.json"
else
    echo "✓ config.json trouvé"
fi

echo ""
echo "========================================="
echo "  Installation terminée !"
echo "========================================="
echo ""
echo "Prochaines étapes :"
echo "1. Configurer votre clé API TMDb dans config/config.json"
echo "2. Compiler le projet :"
echo "   mkdir build && cd build"
echo "   cmake .."
echo "   make"
echo "3. Lancer l'application :"
echo "   ./EpiKodi"
echo ""
