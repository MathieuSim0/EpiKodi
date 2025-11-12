# StreamFlow - Plateforme de Streaming Moderne 🎬📺⭐

Application web moderne de gestion multimédia avec intégration TMDb pour films et séries. Interface style Netflix développée avec React et TypeScript.

## ✨ Interface Moderne

- 🎨 **Design Netflix-like** avec palette de couleurs signature
- 🃏 **Grille de cartes responsive** pour les films et séries
- 🖼️ **Posters haute qualité** via TMDb API
- 🎥 **Bandes-annonces YouTube** intégrées directement dans les détails
- 🎯 **Navigation par onglets** fluide et intuitive
- ⭐ **Système de favoris** avec persistance locale

## ✨ Fonctionnalités

- � **Films** : Recherche instantanée et affichage de films populaires via TMDb
- 📺 **Séries** : Découvrez des séries TV avec détails complets
- � **Bandes-annonces** : Visionnez les trailers YouTube intégrés
- ⭐ **Favoris** : Ajoutez et gérez vos contenus préférés (LocalStorage)
- 🔍 **Recherche rapide** : Trouvez facilement films et séries
- 📱 **Design responsive** : Interface adaptée à tous les écrans

## 🛠️ Technologies

- **React 18** - Library UI moderne avec hooks
- **TypeScript** - Typage statique pour plus de sécurité
- **Vite** - Build tool ultra-rapide avec HMR
- **Tailwind CSS** - Framework CSS utilitaire
- **React Player** - Lecteur vidéo YouTube intégré
- **Axios** - Client HTTP pour appels API
- **TMDb API v3** - Base de données de films et séries

## 📋 Prérequis

- **Node.js 18+** et npm
- Clé API TMDb (gratuite - déjà configurée dans le projet)
- Navigateur web moderne (Chrome, Firefox, Edge, Safari)

## 🚀 Installation

### 1. Cloner le dépôt

```bash
git clone https://github.com/MathieuSim0/EpiKodi.git
cd EpiKodi
```

### 2. Installer les dépendances

```bash
npm install
```

### 3. Configurer l'API TMDb (Optionnel)

La clé API est déjà configurée dans `src/services/tmdb.ts`. Pour utiliser votre propre clé :

1. Créer un compte sur [TheMovieDB](https://www.themoviedb.org/)
2. Obtenir votre clé API dans Paramètres > API
3. Remplacer la clé dans `src/services/tmdb.ts` :

```typescript
const API_KEY = 'VOTRE_CLE_API'
```

### 4. Lancer le serveur de développement

```bash
npm run dev
```

L'application sera accessible sur **http://localhost:3000**

### 5. Build de production

```bash
npm run build
npm run preview
```

## 📁 Structure du Projet

```
EpiKodi/
├── index.html                  # Template HTML
├── package.json                # Dépendances npm
├── vite.config.ts              # Configuration Vite
├── tailwind.config.js          # Configuration Tailwind CSS
├── tsconfig.json               # Configuration TypeScript
├── src/
│   ├── main.tsx                # Point d'entrée React
│   ├── App.tsx                 # Composant racine
│   ├── index.css               # Styles globaux + Tailwind
│   ├── components/             # Composants React
│   │   ├── Layout.tsx          # Layout principal
│   │   ├── Navigation.tsx      # Barre de navigation
│   │   ├── MediaCard.tsx       # Carte film/série
│   │   ├── MediaDetails.tsx    # Détails avec trailer
│   │   └── tabs/               # Onglets
│   │       ├── MoviesTab.tsx   # Section Films
│   │       ├── SeriesTab.tsx   # Section Séries
│   │       ├── MusicTab.tsx    # Section Musique
│   │       └── FavoritesTab.tsx # Section Favoris
│   ├── services/               # Services API
│   │   └── tmdb.ts             # Client TMDb API
│   ├── context/                # React Context
│   │   └── FavoritesContext.tsx # Gestion favoris
│   └── types/                  # Types TypeScript
│       └── index.ts            # Définitions types
├── config/                     # Configuration
└── public/                     # Assets statiques
```

## 🎯 Architecture

### Composants Principaux

1. **Services** (`src/services/`)
   - `tmdb.ts` : Client API pour films/séries, gestion images

2. **Context** (`src/context/`)
   - `FavoritesContext` : State management des favoris avec LocalStorage

3. **Components** (`src/components/`)
   - `Layout` : Structure principale de l'app
   - `Navigation` : Barre de navigation avec onglets
   - `MediaCard` : Carte réutilisable pour afficher un média
   - `MediaDetails` : Modal avec détails et lecteur trailer
   - `tabs/*` : Composants spécialisés par section

4. **Types** (`src/types/`)
   - Interfaces TypeScript pour Movie, Series, Video, Favorite

## 🔧 Utilisation

### Navigation

- **Films 🎬** : Découvrez les films populaires ou recherchez par titre
- **Séries 📺** : Explorez les séries TV tendance
- **Musique 🎵** : Section à venir (MusicBrainz API)
- **Favoris ⭐** : Retrouvez tous vos contenus favoris

### Rechercher du Contenu

1. Cliquez sur l'onglet Films ou Séries
2. Utilisez la barre de recherche
3. Parcourez les résultats avec posters haute qualité
4. Laisser vide pour voir les contenus populaires

### Voir les Détails et Trailers

1. Cliquez sur une affiche pour ouvrir les détails
2. La bande-annonce YouTube se charge automatiquement
3. Consultez le synopsis, la note et l'année de sortie
4. Utilisez le bouton ❌ en haut à droite pour fermer

### Gérer les Favoris

- **Ajouter** : Cliquez sur 🤍 sur une carte ou dans les détails
- **Retirer** : Cliquez sur ❤️ pour retirer des favoris
- **Voir tous** : Onglet Favoris pour voir votre collection
- **Persistance** : Les favoris sont sauvegardés localement (LocalStorage)

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
