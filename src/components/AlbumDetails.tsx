import { DeezerAlbum, DeezerTrack, deezerApi } from '../services/deezer'
import { useState, useEffect } from 'react'

interface AlbumDetailsProps {
  album: DeezerAlbum
  onClose: () => void
}

const AlbumDetails = ({ album, onClose }: AlbumDetailsProps) => {
  const [tracks, setTracks] = useState<DeezerTrack[]>([])
  const [loading, setLoading] = useState(true)
  const [currentTrack, setCurrentTrack] = useState<DeezerTrack | null>(null)
  const [audioElement, setAudioElement] = useState<HTMLAudioElement | null>(null)
  const [isPlaying, setIsPlaying] = useState(false)

  useEffect(() => {
    loadTracks()
    return () => {
      if (audioElement) {
        audioElement.pause()
        audioElement.src = ''
      }
    }
  }, [album.id])

  const loadTracks = async () => {
    setLoading(true)
    try {
      const albumTracks = await deezerApi.getAlbumTracks(album.id)
      setTracks(albumTracks)
    } catch (error) {
      console.error('Error loading tracks:', error)
    }
    setLoading(false)
  }

  const playPreview = (track: DeezerTrack) => {
    if (currentTrack?.id === track.id && isPlaying) {
      audioElement?.pause()
      setIsPlaying(false)
    } else {
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

  const formatDuration = (seconds: number) => {
    const mins = Math.floor(seconds / 60)
    const secs = seconds % 60
    return `${mins}:${String(secs).padStart(2, '0')}`
  }

  const totalDuration = tracks.reduce((acc, track) => acc + track.duration, 0)

  return (
    <div className="fixed inset-0 bg-black bg-opacity-90 z-50 overflow-y-auto pb-32">
      <div className="min-h-screen p-4 md:p-8">
        <button
          onClick={onClose}
          className="fixed top-4 right-4 bg-netflix-gray hover:bg-gray-600 rounded-full p-3 transition-colors z-10"
        >
          <span className="text-2xl">✕</span>
        </button>

        <div className="max-w-6xl mx-auto">
          {/* Album Header */}
          <div className="flex flex-col md:flex-row gap-8 mb-8">
            {/* Album Cover */}
            <div className="flex-shrink-0">
              <img
                src={album.cover_big || album.cover_medium}
                alt={album.title}
                className="w-64 h-64 md:w-80 md:h-80 rounded-lg shadow-2xl"
                onError={(e) => {
                  const target = e.target as HTMLImageElement
                  target.src = 'https://via.placeholder.com/500?text=No+Image'
                }}
              />
            </div>

            {/* Album Info */}
            <div className="flex-1">
              <div className="mb-2">
                <span className="text-sm text-gray-400 uppercase tracking-wide">Album</span>
              </div>
              <h1 className="text-4xl md:text-5xl font-bold mb-4">{album.title}</h1>
              <div className="flex items-center gap-2 mb-4">
                <img
                  src={album.artist.picture_medium}
                  alt={album.artist.name}
                  className="w-8 h-8 rounded-full"
                  onError={(e) => {
                    const target = e.target as HTMLImageElement
                    target.src = 'https://via.placeholder.com/100?text=Artist'
                  }}
                />
                <span className="text-lg font-semibold">{album.artist.name}</span>
              </div>
              <div className="flex items-center gap-3 text-sm text-gray-300">
                {album.release_date && (
                  <span>{new Date(album.release_date).getFullYear()}</span>
                )}
                {tracks.length > 0 && (
                  <>
                    <span>•</span>
                    <span>{tracks.length} titre{tracks.length > 1 ? 's' : ''}</span>
                  </>
                )}
                {totalDuration > 0 && (
                  <>
                    <span>•</span>
                    <span>{formatDuration(totalDuration)}</span>
                  </>
                )}
              </div>
              {album.record_type && (
                <div className="mt-4">
                  <span className="inline-block px-3 py-1 bg-netflix-gray rounded-full text-xs uppercase">
                    {album.record_type}
                  </span>
                </div>
              )}
            </div>
          </div>

          {/* Tracks List */}
          <div className="bg-netflix-gray bg-opacity-30 rounded-lg p-6">
            <h2 className="text-2xl font-bold mb-4">Titres</h2>
            {loading ? (
              <div className="text-center py-12">
                <div className="inline-block animate-spin rounded-full h-12 w-12 border-t-2 border-b-2 border-netflix-red"></div>
              </div>
            ) : (
              <div className="space-y-2">
                {tracks.map((track, index) => (
                  <div
                    key={track.id}
                    className={`flex items-center gap-4 p-3 rounded-lg transition-colors ${
                      currentTrack?.id === track.id && isPlaying
                        ? 'bg-netflix-red bg-opacity-20'
                        : 'hover:bg-netflix-gray hover:bg-opacity-50'
                    }`}
                  >
                    <button
                      onClick={() => playPreview(track)}
                      className="w-10 h-10 flex items-center justify-center bg-netflix-red hover:bg-red-700 rounded-full transition-colors flex-shrink-0"
                    >
                      {currentTrack?.id === track.id && isPlaying ? (
                        <span className="text-xl">⏸️</span>
                      ) : (
                        <span className="text-xl">▶️</span>
                      )}
                    </button>
                    <span className="text-gray-400 w-8 text-right">{index + 1}</span>
                    <div className="flex-1 min-w-0">
                      <h3 className="font-semibold truncate">{track.title}</h3>
                      {track.title !== track.title_short && (
                        <p className="text-sm text-gray-400 truncate">{track.title_short}</p>
                      )}
                    </div>
                    <span className="text-sm text-gray-400 flex-shrink-0">
                      {formatDuration(track.duration)}
                    </span>
                  </div>
                ))}
              </div>
            )}
            <div className="mt-6 p-4 bg-blue-900 bg-opacity-30 border border-blue-700 rounded-lg">
              <p className="text-blue-200 text-sm">
                🎧 Cliquez sur ▶️ pour écouter un aperçu de 30 secondes
              </p>
            </div>
          </div>
        </div>
      </div>
    </div>
  )
}

export default AlbumDetails
