import { tmdbApi } from '../services/tmdb'
import { useFavorites } from '../context/FavoritesContext'

interface MediaCardProps {
  id: number
  title: string
  posterPath: string | null
  voteAverage: number
  releaseDate: string
  type: 'movie' | 'series'
  onClick: () => void
}

const MediaCard = ({ id, title, posterPath, voteAverage, releaseDate, type, onClick }: MediaCardProps) => {
  const { isFavorite, addFavorite, removeFavorite } = useFavorites()
  const favorite = isFavorite(id, type)

  const handleFavoriteClick = (e: React.MouseEvent) => {
    e.stopPropagation()
    if (favorite) {
      removeFavorite(id, type)
    } else {
      addFavorite({ id, type, title, poster_path: posterPath })
    }
  }

  return (
    <div
      onClick={onClick}
      className="relative group cursor-pointer rounded-lg overflow-hidden bg-netflix-gray hover:scale-105 transition-transform duration-200"
    >
      <img
        src={tmdbApi.getPosterUrl(posterPath)}
        alt={title}
        className="w-full aspect-[2/3] object-cover"
        onError={(e) => {
          const target = e.target as HTMLImageElement
          target.src = 'https://via.placeholder.com/342x513?text=No+Image'
        }}
      />
      <div className="absolute inset-0 bg-gradient-to-t from-black via-transparent to-transparent opacity-0 group-hover:opacity-100 transition-opacity duration-200">
        <div className="absolute bottom-0 left-0 right-0 p-4">
          <h3 className="font-bold text-sm mb-1 line-clamp-2">{title}</h3>
          {releaseDate && (
            <p className="text-xs text-gray-300">
              {new Date(releaseDate).getFullYear()}
            </p>
          )}
          {voteAverage > 0 && (
            <div className="flex items-center mt-1">
              <span className="text-yellow-400 text-xs">⭐</span>
              <span className="text-xs ml-1">{voteAverage.toFixed(1)}</span>
            </div>
          )}
        </div>
      </div>
      <button
        onClick={handleFavoriteClick}
        className="absolute top-2 right-2 bg-black bg-opacity-70 hover:bg-opacity-90 rounded-full p-2 transition-all opacity-0 group-hover:opacity-100"
      >
        <span className="text-xl">{favorite ? '❤️' : '🤍'}</span>
      </button>
    </div>
  )
}

export default MediaCard
