import axios from 'axios'
import { Movie, Series, Video, Cast } from '../types'

const API_KEY = '03781b82d379a5acfb9e6336c65baf8e'
const BASE_URL = 'https://api.themoviedb.org/3'
const IMAGE_BASE_URL = 'https://image.tmdb.org/t/p'

export const tmdbApi = {
  // Movies
  searchMovies: async (query: string): Promise<Movie[]> => {
    const response = await axios.get(`${BASE_URL}/search/movie`, {
      params: {
        api_key: API_KEY,
        query,
        language: 'fr-FR',
      },
    })
    return response.data.results
  },

  getPopularMovies: async (): Promise<Movie[]> => {
    const response = await axios.get(`${BASE_URL}/movie/popular`, {
      params: {
        api_key: API_KEY,
        language: 'fr-FR',
      },
    })
    return response.data.results
  },

  getMovieVideos: async (movieId: number): Promise<Video[]> => {
    const response = await axios.get(`${BASE_URL}/movie/${movieId}/videos`, {
      params: {
        api_key: API_KEY,
        language: 'fr-FR',
      },
    })
    return response.data.results
  },

  getMovieCredits: async (movieId: number): Promise<Cast[]> => {
    const response = await axios.get(`${BASE_URL}/movie/${movieId}/credits`, {
      params: {
        api_key: API_KEY,
        language: 'fr-FR',
      },
    })
    return response.data.cast.slice(0, 10) // Top 10 acteurs
  },

  // Series
  searchSeries: async (query: string): Promise<Series[]> => {
    const response = await axios.get(`${BASE_URL}/search/tv`, {
      params: {
        api_key: API_KEY,
        query,
        language: 'fr-FR',
      },
    })
    return response.data.results
  },

  getPopularSeries: async (): Promise<Series[]> => {
    const response = await axios.get(`${BASE_URL}/tv/popular`, {
      params: {
        api_key: API_KEY,
        language: 'fr-FR',
      },
    })
    return response.data.results
  },

  getSeriesVideos: async (seriesId: number): Promise<Video[]> => {
    const response = await axios.get(`${BASE_URL}/tv/${seriesId}/videos`, {
      params: {
        api_key: API_KEY,
        language: 'fr-FR',
      },
    })
    return response.data.results
  },

  getSeriesCredits: async (seriesId: number): Promise<Cast[]> => {
    const response = await axios.get(`${BASE_URL}/tv/${seriesId}/credits`, {
      params: {
        api_key: API_KEY,
        language: 'fr-FR',
      },
    })
    return response.data.cast.slice(0, 10) // Top 10 acteurs
  },

  // Image helpers
  getPosterUrl: (path: string | null, size: 'w185' | 'w342' | 'w500' = 'w342'): string => {
    return path ? `${IMAGE_BASE_URL}/${size}${path}` : '/placeholder.png'
  },

  getBackdropUrl: (path: string | null, size: 'w780' | 'w1280' | 'original' = 'w1280'): string => {
    return path ? `${IMAGE_BASE_URL}/${size}${path}` : '/placeholder.png'
  },

  getProfileUrl: (path: string | null, size: 'w185' = 'w185'): string => {
    return path ? `${IMAGE_BASE_URL}/${size}${path}` : '/placeholder.png'
  },
}
