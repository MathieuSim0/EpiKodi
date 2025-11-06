import { useFavorites } from '../../context/FavoritesContext'
import MediaCard from '../MediaCard'

const FavoritesTab = () => {
  const { favorites } = useFavorites()

  if (favorites.length === 0) {
    return (
      <div className="text-center py-12">
        <div className="text-6xl mb-4">⭐</div>
        <h2 className="text-2xl font-bold mb-2">Aucun favori</h2>
        <p className="text-gray-400">
          Ajoutez des films ou séries à vos favoris pour les retrouver ici.
        </p>
      </div>
    )
  }

  return (
    <div className="space-y-6">
      <h2 className="text-2xl font-bold">Mes Favoris ({favorites.length})</h2>
      <div className="grid grid-cols-2 md:grid-cols-3 lg:grid-cols-4 xl:grid-cols-5 gap-4">
        {favorites.map((fav) => (
          <MediaCard
            key={`${fav.type}-${fav.id}`}
            id={fav.id}
            title={fav.title}
            posterPath={fav.poster_path}
            voteAverage={0}
            releaseDate=""
            type={fav.type}
            onClick={() => {}}
          />
        ))}
      </div>
    </div>
  )
}

export default FavoritesTab
