cmake_minimum_required(VERSION 3.16)
project(EpiKodi VERSION 1.0.0 LANGUAGES CXX)

# Configuration C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Trouver Qt 6
find_package(Qt6 REQUIRED COMPONENTS 
    Core 
    Widgets 
    Network 
    Multimedia 
    MultimediaWidgets
)

# Inclure nlohmann/json (header-only library)
# Télécharger depuis: https://github.com/nlohmann/json
include_directories(${CMAKE_SOURCE_DIR}/external/nlohmann)

# Inclure les répertoires headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Sources principales
set(SOURCES
    src/main.cpp
    src/api/TMDbApiClient.cpp
    src/api/OMDbApiClient.cpp
    src/api/MusicBrainzApiClient.cpp
    src/models/Movie.cpp
    src/models/Series.cpp
    src/models/Album.cpp
    src/models/Artist.cpp
    src/managers/FavoritesManager.cpp
    src/player/MediaPlayer.cpp
    src/ui/MainWindow.cpp
    src/ui/MovieWidget.cpp
    src/ui/MovieCard.cpp
    src/ui/SeriesWidget.cpp
    src/ui/SeriesCard.cpp
    src/ui/MusicWidget.cpp
    src/ui/FavoritesWidget.cpp
    src/ui/PlayerWidget.cpp
)

# Headers
set(HEADERS
    include/api/TMDbApiClient.h
    include/api/OMDbApiClient.h
    include/api/MusicBrainzApiClient.h
    include/models/Movie.h
    include/models/Series.h
    include/models/Album.h
    include/models/Artist.h
    include/managers/FavoritesManager.h
    include/player/MediaPlayer.h
    include/ui/MainWindow.h
    include/ui/MovieWidget.h
    include/ui/MovieCard.h
    include/ui/SeriesWidget.h
    include/ui/SeriesCard.h
    include/ui/MusicWidget.h
    include/ui/FavoritesWidget.h
    include/ui/PlayerWidget.h
)

# Ressources Qt
set(RESOURCES
    resources/resources.qrc
)

# Créer l'exécutable
add_executable(${PROJECT_NAME} 
    ${SOURCES} 
    ${HEADERS}
    ${RESOURCES}
)

# Lier les bibliothèques Qt 6
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
    Qt6::Multimedia
    Qt6::MultimediaWidgets
)

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Copier le fichier de configuration exemple
install(FILES config/config.example.json
    DESTINATION ${CMAKE_INSTALL_PREFIX}/config
)
