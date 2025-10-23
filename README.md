# EpiKodi - Media Center Moderne 🎬🎵📺

EpiKodi est un media center complet développé en C++ avec Qt 6, avec une **interface moderne inspirée de Netflix** permettant de rechercher et gérer films, séries et musique via des APIs publiques.

## ✨ Interface Moderne

- 🎨 **Design Netflix-like** avec dégradés rouges signature
- 🃏 **Grille de cartes interactives** pour les films et séries
- 🖼️ **Posters haute qualité** chargés automatiquement
- ✨ **Animations fluides** au survol et à la sélection
- 🎯 **Navigation intuitive** avec onglets et recherche optimisée

## ✨ Fonctionnalités

- 🎥 **Films & Séries** : Recherche et affichage via TheMovieDB (TMDb)
- 🎵 **Musique** : Recherche d'artistes et albums via MusicBrainz + Cover Art Archive
- ▶️ **Lecteur Multimédia** : Lecture de vidéos (trailers) et audio
- ⭐ **Favoris** : Gestion locale des favoris (fichier JSON)
- 🎨 **Interface Moderne** : Interface fluide et esthétique avec Qt 6

## 🛠️ Technologies

- **Langage** : C++17
- **Framework** : Qt 6 (QtWidgets, QtNetwork, QtMultimedia)
- **APIs** : TheMovieDB API, MusicBrainz API, Cover Art Archive
- **JSON** : nlohmann/json
- **Build System** : CMake

## 📋 Prérequis

- **Qt 6.2+** (avec modules Core, Widgets, Network, Multimedia)
- **CMake 3.16+**
- **Compilateur C++17** (GCC 7+, Clang 5+, MSVC 2017+)
- **Git**

## 🚀 Installation

### 1. Cloner le dépôt

```bash
git clone https://github.com/MathieuSim0/EpiKodi.git
cd EpiKodi
```

### 2. Installer nlohmann/json

```bash
mkdir -p external
cd external
git clone https://github.com/nlohmann/json.git nlohmann
cd ..
```

### 3. Configurer les clés API

Copier le fichier de configuration exemple et ajouter votre clé TMDb :

```bash
cp config/config.example.json config/config.json
```

Éditer `config/config.json` et remplacer `YOUR_TMDB_API_KEY_HERE` par votre clé API TMDb.

**Obtenir une clé TMDb** :
1. Créer un compte sur [TheMovieDB](https://www.themoviedb.org/)
2. Aller dans Paramètres > API
3. Demander une clé API (gratuite)

### 4. Compiler le projet

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### 5. Lancer l'application

```bash
./EpiKodi
```

## 📁 Structure du Projet

```
EpiKodi/
├── CMakeLists.txt              # Configuration CMake principale
├── README.md                   # Documentation
├── .gitignore                  # Fichiers à ignorer
├── config/
│   └── config.example.json     # Configuration exemple
├── include/                    # Headers (.h)
│   ├── api/                    # Clients API
│   ├── models/                 # Modèles de données
│   ├── managers/               # Gestionnaires (favoris, etc.)
│   ├── player/                 # Lecteur multimédia
│   └── ui/                     # Interface utilisateur
├── src/                        # Sources (.cpp)
│   ├── main.cpp                # Point d'entrée
│   ├── api/
│   ├── models/
│   ├── managers/
│   ├── player/
│   └── ui/
├── resources/                  # Ressources Qt
│   ├── icons/                  # Icônes
│   ├── styles/                 # Feuilles de style
│   └── resources.qrc           # Fichier de ressources Qt
└── external/                   # Bibliothèques externes
    └── nlohmann/               # nlohmann/json
```

## 🎯 Architecture

### Modules Principaux

1. **API Clients** (`src/api/`)
   - `TMDbApiClient` : Recherche de films et séries
   - `MusicBrainzApiClient` : Recherche d'artistes et albums

2. **Modèles** (`src/models/`)
   - `Movie`, `Series`, `Album`, `Artist` : Structures de données

3. **Managers** (`src/managers/`)
   - `FavoritesManager` : Gestion des favoris (persistence JSON)

4. **Lecteur** (`src/player/`)
   - `MediaPlayer` : Lecture audio/vidéo avec QtMultimedia

5. **Interface** (`src/ui/`)
   - `MainWindow` : Fenêtre principale avec navigation
   - `MovieWidget`, `SeriesWidget`, `MusicWidget` : Vues spécialisées
   - `FavoritesWidget` : Liste des favoris
   - `PlayerWidget` : Contrôles du lecteur

## 🔧 Utilisation

### Rechercher des Films

1. Cliquer sur l'onglet **Films**
2. Entrer un titre dans la barre de recherche
3. Parcourir les résultats avec posters et descriptions
4. Cliquer sur un film pour voir les détails et le trailer

### Rechercher de la Musique

1. Cliquer sur l'onglet **Musique**
2. Rechercher un artiste ou un album
3. Voir les pochettes d'albums
4. Lire des extraits audio

### Gérer les Favoris

- Cliquer sur l'icône ⭐ pour ajouter aux favoris
- Accéder à tous vos favoris dans l'onglet **Favoris**
- Les favoris sont sauvegardés dans `favorites.json`

## 🤝 Contribution

Les contributions sont les bienvenues ! N'hésitez pas à :

1. Fork le projet
2. Créer une branche (`git checkout -b feature/AmazingFeature`)
3. Commit vos changements (`git commit -m 'Add AmazingFeature'`)
4. Push vers la branche (`git push origin feature/AmazingFeature`)
5. Ouvrir une Pull Request

## 📄 Licence

Ce projet est un projet éducatif. Respectez les conditions d'utilisation des APIs (TMDb, MusicBrainz).

## 📧 Contact

Mathieu Sim - [@MathieuSim0](https://github.com/MathieuSim0)

Lien du projet : [https://github.com/MathieuSim0/EpiKodi](https://github.com/MathieuSim0/EpiKodi)

## 🙏 Remerciements

- [TheMovieDB](https://www.themoviedb.org/) pour l'API de films/séries
- [MusicBrainz](https://musicbrainz.org/) pour l'API musicale
- [Qt Framework](https://www.qt.io/) pour le framework UI
- [nlohmann/json](https://github.com/nlohmann/json) pour le parsing JSON
