import { useState, useEffect } from 'react'
import { Series, Video, Cast } from '../../types'
import { tmdbApi } from '../../services/tmdb'
import MediaCard from '../MediaCard'
import MediaDetails from '../MediaDetails'

const SeriesTab = () => {
  const [searchQuery, setSearchQuery] = useState('')
  const [series, setSeries] = useState<Series[]>([])
  const [selectedSeries, setSelectedSeries] = useState<Series | null>(null)
  const [trailer, setTrailer] = useState<Video | null>(null)
  const [cast, setCast] = useState<Cast[]>([])
  const [loading, setLoading] = useState(false)

  useEffect(() => {
    loadPopularSeries()
  }, [])

  const loadPopularSeries = async () => {
    setLoading(true)
    try {
      const results = await tmdbApi.getPopularSeries()
      setSeries(results)
    } catch (error) {
      console.error('Error loading popular series:', error)
    }
    setLoading(false)
  }

  const handleSearch = async (e: React.FormEvent) => {
    e.preventDefault()
    if (!searchQuery.trim()) {
      loadPopularSeries()
      return
    }

    setLoading(true)
    try {
      const results = await tmdbApi.searchSeries(searchQuery)
      setSeries(results)
    } catch (error) {
      console.error('Error searching series:', error)
    }
    setLoading(false)
  }

  const handleSeriesClick = async (show: Series) => {
    setSelectedSeries(show)
    setCast([])
    try {
      const [videos, credits] = await Promise.all([
        tmdbApi.getSeriesVideos(show.id),
        tmdbApi.getSeriesCredits(show.id),
      ])
      const trailerVideo = videos.find(
        (v) => v.type === 'Trailer' && v.site === 'YouTube'
      ) || videos[0]
      setTrailer(trailerVideo || null)
      setCast(credits)
    } catch (error) {
      console.error('Error loading series details:', error)
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
            placeholder="Rechercher une série..."
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

      {selectedSeries && (
        <MediaDetails
          title={selectedSeries.name}
          overview={selectedSeries.overview}
          posterPath={selectedSeries.poster_path}
          backdropPath={selectedSeries.backdrop_path}
          releaseDate={selectedSeries.first_air_date}
          voteAverage={selectedSeries.vote_average}
          trailer={trailer}
          cast={cast}
          mediaId={selectedSeries.id}
          mediaType="series"
          onClose={() => {
            setSelectedSeries(null)
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
          {series.map((show) => (
            <MediaCard
              key={show.id}
              id={show.id}
              title={show.name}
              posterPath={show.poster_path}
              voteAverage={show.vote_average}
              releaseDate={show.first_air_date}
              type="series"
              onClick={() => handleSeriesClick(show)}
            />
          ))}
        </div>
      )}
    </div>
  )
}

export default SeriesTab
