import { useState, useEffect } from 'react'
import { deezerApi, DeezerAlbum, DeezerArtist, DeezerTrack } from '../../services/deezer'
import AlbumDetails from '../AlbumDetails'
import MusicPlayer from '../MusicPlayer'

type SearchType = 'artists' | 'albums' | 'tracks'

const MusicTab = () => {
  const [searchQuery, setSearchQuery] = useState('')
  const [searchType, setSearchType] = useState<SearchType>('albums')
  const [artists, setArtists] = useState<DeezerArtist[]>([])
  const [albums, setAlbums] = useState<DeezerAlbum[]>([])
  const [tracks, setTracks] = useState<DeezerTrack[]>([])
  const [selectedAlbum, setSelectedAlbum] = useState<DeezerAlbum | null>(null)
  const [loading, setLoading] = useState(false)
  const [currentTrack, setCurrentTrack] = useState<DeezerTrack | null>(null)
  const [audioElement, setAudioElement] = useState<HTMLAudioElement | null>(null)
  const [isPlaying, setIsPlaying] = useState(false)

  useEffect(() => {
    loadTopContent()
  }, [])

  useEffect(() => {
    // Cleanup audio on unmount
    return () => {
      if (audioElement) {
        audioElement.pause()
        audioElement.src = ''
      }
    }
  }, [audioElement])

  const loadTopContent = async () => {
    setLoading(true)
    try {
      const topAlbums = await deezerApi.getTopAlbums()
      setAlbums(topAlbums)
    } catch (error) {
      console.error('Error loading top content:', error)
    }
    setLoading(false)
  }

  const handleSearch = async (e: React.FormEvent) => {
    e.preventDefault()
    if (!searchQuery.trim()) {
      loadTopContent()
      return
    }

    setLoading(true)
    try {
      if (searchType === 'artists') {
        const results = await deezerApi.searchArtists(searchQuery)
        setArtists(results)
        setAlbums([])
        setTracks([])
      } else if (searchType === 'albums') {
        const results = await deezerApi.searchAlbums(searchQuery)
        setAlbums(results)
        setArtists([])
        setTracks([])
      } else {
        const results = await deezerApi.searchTracks(searchQuery)
        setTracks(results)
        setArtists([])
        setAlbums([])
      }
    } catch (error) {
      console.error('Error searching:', error)
    }
    setLoading(false)
  }

  const playPreview = (track: DeezerTrack) => {
    if (currentTrack?.id === track.id && isPlaying) {
      // Pause current track
      audioElement?.pause()
      setIsPlaying(false)
    } else {
      // Play new track
      if (audioElement) {
        audioElement.pause()
      }
      const audio = new Audio(track.preview)
      audio.play()
      audio.onended = () => {
        setIsPlaying(false)
        setCurrentTrack(null)
      }
      setAudioElement(audio)
      setCurrentTrack(track)
      setIsPlaying(true)
    }
  }

  const handlePlayPause = () => {
    if (audioElement) {
      if (isPlaying) {
        audioElement.pause()
        setIsPlaying(false)
      } else {
        audioElement.play()
        setIsPlaying(true)
      }
    }
  }

  const handleClosePlayer = () => {
    if (audioElement) {
      audioElement.pause()
      audioElement.src = ''
    }
    setCurrentTrack(null)
    setIsPlaying(false)
    setAudioElement(null)
  }

  const handleArtistClick = async (artist: DeezerArtist) => {
    setLoading(true)
    setSearchQuery('') // Clear search
    try {
      const artistAlbums = await deezerApi.getArtistAlbums(artist.id)
      setAlbums(artistAlbums)
      setArtists([])
      setTracks([])
    } catch (error) {
      console.error('Error loading artist albums:', error)
    }
    setLoading(false)
  }

  const handleAlbumClick = (album: DeezerAlbum) => {
    setSelectedAlbum(album)
  }

  return (
    <div className="space-y-6 pb-32">
      {/* Album Details Modal */}
      {selectedAlbum && (
        <AlbumDetails
          album={selectedAlbum}
          onClose={() => setSelectedAlbum(null)}
        />
      )}

      {/* Music Player Bar */}
      <MusicPlayer
        track={currentTrack}
        isPlaying={isPlaying}
        onPlayPause={handlePlayPause}
        onClose={handleClosePlayer}
        audioElement={audioElement}
      />

      <form onSubmit={handleSearch} className="max-w-3xl mx-auto">
        <div className="flex gap-2 mb-4">
          <select
            value={searchType}
            onChange={(e) => setSearchType(e.target.value as SearchType)}
            className="px-4 py-3 bg-netflix-gray border border-gray-600 rounded-lg focus:outline-none focus:border-netflix-red text-white"
          >
            <option value="albums">Albums</option>
            <option value="artists">Artistes</option>
            <option value="tracks">Titres</option>
          </select>
          <input
            type="text"
            value={searchQuery}
            onChange={(e) => setSearchQuery(e.target.value)}
            placeholder={`Rechercher ${
              searchType === 'albums' ? 'un album' : searchType === 'artists' ? 'un artiste' : 'un titre'
            }...`}
            className="flex-1 px-4 py-3 bg-netflix-gray border border-gray-600 rounded-lg focus:outline-none focus:border-netflix-red text-white"
          />
          <button
            type="submit"
            className="px-6 py-3 bg-netflix-red text-white rounded-lg hover:bg-red-700 transition-colors font-medium"
          >
            Rechercher
          </button>
        </div>
      </form>

      {loading ? (
        <div className="text-center py-12">
          <div className="inline-block animate-spin rounded-full h-12 w-12 border-t-2 border-b-2 border-netflix-red"></div>
        </div>
      ) : (
        <>
          {/* Artists Grid */}
          {artists.length > 0 && (
            <div>
              <h2 className="text-2xl font-bold mb-4">Artistes</h2>
              <div className="grid grid-cols-2 md:grid-cols-3 lg:grid-cols-4 xl:grid-cols-6 gap-4">
                {artists.map((artist) => (
                  <div
                    key={artist.id}
                    onClick={() => handleArtistClick(artist)}
                    className="cursor-pointer group"
                  >
                    <div className="relative rounded-full overflow-hidden aspect-square mb-3 hover:scale-105 transition-transform">
                      <img
                        src={artist.picture_medium}
                        alt={artist.name}
                        className="w-full h-full object-cover"
                        onError={(e) => {
                          const target = e.target as HTMLImageElement
                          target.src = 'https://via.placeholder.com/250?text=No+Image'
                        }}
                      />
                    </div>
                    <h3 className="font-bold text-center truncate">{artist.name}</h3>
                    <p className="text-xs text-gray-400 text-center">
                      {artist.nb_fan?.toLocaleString()} fans
                    </p>
                  </div>
                ))}
              </div>
            </div>
          )}

          {/* Albums Grid */}
          {albums.length > 0 && (
            <div>
              <h2 className="text-2xl font-bold mb-4">
                {searchQuery ? 'Résultats' : 'Albums Populaires'}
              </h2>
              <div className="grid grid-cols-2 md:grid-cols-3 lg:grid-cols-4 xl:grid-cols-5 gap-4">
                {albums.map((album) => (
                  <div
                    key={album.id}
                    onClick={() => handleAlbumClick(album)}
                    className="cursor-pointer group"
                  >
                    <div className="relative rounded-lg overflow-hidden aspect-square mb-3 hover:scale-105 transition-transform shadow-lg">
                      <img
                        src={album.cover_medium}
                        alt={album.title}
                        className="w-full h-full object-cover"
                        onError={(e) => {
                          const target = e.target as HTMLImageElement
                          target.src = 'https://via.placeholder.com/250?text=No+Image'
                        }}
                      />
                      <div className="absolute inset-0 bg-black bg-opacity-0 group-hover:bg-opacity-40 transition-opacity flex items-center justify-center">
                        <div className="opacity-0 group-hover:opacity-100 transition-opacity">
                          <div className="w-12 h-12 bg-netflix-red rounded-full flex items-center justify-center">
                            <span className="text-2xl">▶️</span>
                          </div>
                        </div>
                      </div>
                    </div>
                    <h3 className="font-bold text-sm truncate">{album.title}</h3>
                    <p className="text-xs text-gray-400 truncate">{album.artist.name}</p>
                    {album.release_date && (
                      <p className="text-xs text-gray-500">
                        {new Date(album.release_date).getFullYear()}
                      </p>
                    )}
                  </div>
                ))}
              </div>
            </div>
          )}

          {/* Tracks List */}
          {tracks.length > 0 && (
            <div>
              <h2 className="text-2xl font-bold mb-4">Titres</h2>
              <div className="space-y-2">
                {tracks.map((track, index) => (
                  <div
                    key={track.id}
                    className="flex items-center gap-4 p-3 bg-netflix-gray rounded-lg hover:bg-gray-700 transition-colors"
                  >
                    <span className="text-gray-400 w-8 text-center">{index + 1}</span>
                    <img
                      src={track.album.cover_medium}
                      alt={track.album.title}
                      className="w-12 h-12 rounded"
                      onError={(e) => {
                        const target = e.target as HTMLImageElement
                        target.src = 'https://via.placeholder.com/120?text=No+Image'
                      }}
                    />
                    <div className="flex-1 min-w-0">
                      <h3 className="font-semibold truncate">{track.title}</h3>
                      <p className="text-sm text-gray-400 truncate">{track.artist.name}</p>
                    </div>
                    <span className="text-sm text-gray-400">
                      {Math.floor(track.duration / 60)}:{String(track.duration % 60).padStart(2, '0')}
                    </span>
                    <button
                      onClick={() => playPreview(track)}
                      className="px-4 py-2 bg-netflix-red hover:bg-red-700 rounded-full transition-colors"
                    >
                      {currentTrack?.id === track.id && isPlaying ? '⏸️' : '▶️'}
                    </button>
                  </div>
                ))}
              </div>
            </div>
          )}

          {/* Empty State */}
          {!loading && artists.length === 0 && albums.length === 0 && tracks.length === 0 && (
            <div className="text-center py-12">
              <div className="text-6xl mb-4">🎵</div>
              <h2 className="text-2xl font-bold mb-2">Aucun résultat</h2>
              <p className="text-gray-400">
                Essayez une autre recherche ou parcourez les albums populaires
              </p>
            </div>
          )}
        </>
      )}
    </div>
  )
}

export default MusicTab
