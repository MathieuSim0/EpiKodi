import ReactPlayer from 'react-player/youtube'
import { Video, Cast } from '../types'
import { tmdbApi } from '../services/tmdb'
import { useFavorites } from '../context/FavoritesContext'

interface MediaDetailsProps {
  title: string
  overview: string
  posterPath: string | null
  backdropPath: string | null
  releaseDate: string
  voteAverage: number
  trailer: Video | null
  cast: Cast[]
  mediaId: number
  mediaType: 'movie' | 'series'
  onClose: () => void
}

const MediaDetails = ({
  title,
  overview,
  posterPath,
  backdropPath,
  releaseDate,
  voteAverage,
  trailer,
  cast,
  mediaId,
  mediaType,
  onClose,
}: MediaDetailsProps) => {
  const { isFavorite, addFavorite, removeFavorite } = useFavorites()
  const favorite = isFavorite(mediaId, mediaType)

  const handleFavoriteClick = () => {
    if (favorite) {
      removeFavorite(mediaId, mediaType)
    } else {
      addFavorite({ id: mediaId, type: mediaType, title, poster_path: posterPath })
    }
  }

  return (
    <div className="fixed inset-0 bg-black bg-opacity-90 z-50 overflow-y-auto">
      <div className="min-h-screen p-4 md:p-8">
        <button
          onClick={onClose}
          className="fixed top-4 right-4 bg-netflix-gray hover:bg-gray-600 rounded-full p-3 transition-colors z-10"
        >
          <span className="text-2xl">✕</span>
        </button>

        <div className="max-w-6xl mx-auto">
          {/* Backdrop/Trailer Section */}
          <div className="relative w-full aspect-video bg-netflix-gray rounded-lg overflow-hidden mb-6">
            {trailer ? (
              <ReactPlayer
                url={`https://www.youtube.com/watch?v=${trailer.key}`}
                width="100%"
                height="100%"
                controls
                playing
              />
            ) : (
              <img
                src={tmdbApi.getBackdropUrl(backdropPath)}
                alt={title}
                className="w-full h-full object-cover"
                onError={(e) => {
                  const target = e.target as HTMLImageElement
                  target.src = 'https://via.placeholder.com/1280x720?text=No+Image'
                }}
              />
            )}
          </div>

          {/* Details Section */}
          <div className="flex flex-col md:flex-row gap-6">
            {/* Poster */}
            <div className="flex-shrink-0">
              <img
                src={tmdbApi.getPosterUrl(posterPath, 'w342')}
                alt={title}
                className="w-48 rounded-lg shadow-lg"
                onError={(e) => {
                  const target = e.target as HTMLImageElement
                  target.src = 'https://via.placeholder.com/342x513?text=No+Image'
                }}
              />
            </div>

            {/* Info */}
            <div className="flex-1">
              <div className="flex items-start justify-between mb-4">
                <h1 className="text-4xl font-bold">{title}</h1>
                <button
                  onClick={handleFavoriteClick}
                  className="bg-netflix-gray hover:bg-gray-600 rounded-full p-3 transition-colors ml-4"
                >
                  <span className="text-2xl">{favorite ? '❤️' : '🤍'}</span>
                </button>
              </div>

              <div className="flex items-center gap-4 mb-4 text-gray-300">
                {releaseDate && (
                  <span>{new Date(releaseDate).getFullYear()}</span>
                )}
                {voteAverage > 0 && (
                  <div className="flex items-center">
                    <span className="text-yellow-400 mr-1">⭐</span>
                    <span>{voteAverage.toFixed(1)}/10</span>
                  </div>
                )}
              </div>

              <div className="prose prose-invert max-w-none">
                <h3 className="text-xl font-semibold mb-2">Synopsis</h3>
                <p className="text-gray-300 leading-relaxed">
                  {overview || 'Aucune description disponible.'}
                </p>
              </div>

              {/* Casting */}
              {cast.length > 0 && (
                <div className="mt-6">
                  <h3 className="text-xl font-semibold mb-4">Casting</h3>
                  <div className="grid grid-cols-2 sm:grid-cols-3 md:grid-cols-4 lg:grid-cols-5 gap-4">
                    {cast.map((actor) => (
                      <div key={actor.id} className="text-center">
                        <img
                          src={tmdbApi.getProfileUrl(actor.profile_path)}
                          alt={actor.name}
                          className="w-full aspect-[2/3] object-cover rounded-lg mb-2"
                          onError={(e) => {
                            const target = e.target as HTMLImageElement
                            target.src = 'https://via.placeholder.com/185x278?text=No+Photo'
                          }}
                        />
                        <p className="font-semibold text-sm text-white truncate">{actor.name}</p>
                        <p className="text-xs text-gray-400 truncate">{actor.character}</p>
                      </div>
                    ))}
                  </div>
                </div>
              )}

              {!trailer && (
                <div className="mt-6 p-4 bg-yellow-900 bg-opacity-30 border border-yellow-700 rounded-lg">
                  <p className="text-yellow-200">
                    ℹ️ Aucune bande-annonce disponible pour ce contenu.
                  </p>
                </div>
              )}
            </div>
          </div>
        </div>
      </div>
    </div>
  )
}

export default MediaDetails
