import { useState, useEffect } from 'react'
import { Movie, Video, Cast } from '../../types'
import { tmdbApi } from '../../services/tmdb'
import MediaCard from '../MediaCard'
import MediaDetails from '../MediaDetails'

const MoviesTab = () => {
  const [searchQuery, setSearchQuery] = useState('')
  const [movies, setMovies] = useState<Movie[]>([])
  const [selectedMovie, setSelectedMovie] = useState<Movie | null>(null)
  const [trailer, setTrailer] = useState<Video | null>(null)
  const [cast, setCast] = useState<Cast[]>([])
  const [loading, setLoading] = useState(false)

  useEffect(() => {
    loadPopularMovies()
  }, [])

  const loadPopularMovies = async () => {
    setLoading(true)
    try {
      const results = await tmdbApi.getPopularMovies()
      setMovies(results)
    } catch (error) {
      console.error('Error loading popular movies:', error)
    }
    setLoading(false)
  }

  const handleSearch = async (e: React.FormEvent) => {
    e.preventDefault()
    if (!searchQuery.trim()) {
      loadPopularMovies()
      return
    }

    setLoading(true)
    try {
      const results = await tmdbApi.searchMovies(searchQuery)
      setMovies(results)
    } catch (error) {
      console.error('Error searching movies:', error)
    }
    setLoading(false)
  }

  const handleMovieClick = async (movie: Movie) => {
    setSelectedMovie(movie)
    setCast([])
    try {
      const [videos, credits] = await Promise.all([
        tmdbApi.getMovieVideos(movie.id),
        tmdbApi.getMovieCredits(movie.id),
      ])
      const trailerVideo = videos.find(
        (v) => v.type === 'Trailer' && v.site === 'YouTube'
      ) || videos[0]
      setTrailer(trailerVideo || null)
      setCast(credits)
    } catch (error) {
      console.error('Error loading movie details:', error)
      setTrailer(null)
      setCast([])
    }
  }

  return (
    <div className="space-y-6">
      <form onSubmit={handleSearch} className="max-w-2xl mx-auto">
        <div className="flex gap-2">
          <input
            type="text"
            value={searchQuery}
            onChange={(e) => setSearchQuery(e.target.value)}
            placeholder="Rechercher un film..."
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

      {selectedMovie && (
        <MediaDetails
          title={selectedMovie.title}
          overview={selectedMovie.overview}
          posterPath={selectedMovie.poster_path}
          backdropPath={selectedMovie.backdrop_path}
          releaseDate={selectedMovie.release_date}
          voteAverage={selectedMovie.vote_average}
          trailer={trailer}
          cast={cast}
          mediaId={selectedMovie.id}
          mediaType="movie"
          onClose={() => {
            setSelectedMovie(null)
            setTrailer(null)
            setCast([])
          }}
        />
      )}

      {loading ? (
        <div className="text-center py-12">
          <div className="inline-block animate-spin rounded-full h-12 w-12 border-t-2 border-b-2 border-netflix-red"></div>
        </div>
      ) : (
        <div className="grid grid-cols-2 md:grid-cols-3 lg:grid-cols-4 xl:grid-cols-5 gap-4">
          {movies.map((movie) => (
            <MediaCard
              key={movie.id}
              id={movie.id}
              title={movie.title}
              posterPath={movie.poster_path}
              voteAverage={movie.vote_average}
              releaseDate={movie.release_date}
              type="movie"
              onClick={() => handleMovieClick(movie)}
            />
          ))}
        </div>
      )}
    </div>
  )
}

export default MoviesTab
