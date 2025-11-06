import axios from 'axios'

// Utiliser le proxy CORS pour contourner les restrictions
const BASE_URL = 'https://api.deezer.com'
const CORS_PROXY = 'https://corsproxy.io/?' // Proxy CORS gratuit

export interface DeezerArtist {
  id: number
  name: string
  picture: string
  picture_small: string
  picture_medium: string
  picture_big: string
  picture_xl: string
  nb_album: number
  nb_fan: number
  type: string
}

export interface DeezerAlbum {
  id: number
  title: string
  cover: string
  cover_small: string
  cover_medium: string
  cover_big: string
  cover_xl: string
  release_date: string
  record_type: string
  nb_tracks: number
  artist: {
    id: number
    name: string
    picture_medium: string
  }
  type: string
}

export interface DeezerTrack {
  id: number
  title: string
  title_short: string
  duration: number
  preview: string // URL audio 30 secondes
  artist: {
    id: number
    name: string
  }
  album: {
    id: number
    title: string
    cover_medium: string
  }
  type: string
}

export const deezerApi = {
  // Search
  searchArtists: async (query: string): Promise<DeezerArtist[]> => {
    try {
      const url = `${CORS_PROXY}${encodeURIComponent(BASE_URL)}/search/artist`
      const response = await axios.get(url, {
        params: { q: query },
      })
      return response.data.data || []
    } catch (error) {
      console.error('Error searching artists:', error)
      return []
    }
  },

  searchAlbums: async (query: string): Promise<DeezerAlbum[]> => {
    try {
      const url = `${CORS_PROXY}${encodeURIComponent(BASE_URL)}/search/album`
      const response = await axios.get(url, {
        params: { q: query },
      })
      return response.data.data || []
    } catch (error) {
      console.error('Error searching albums:', error)
      return []
    }
  },

  searchTracks: async (query: string): Promise<DeezerTrack[]> => {
    try {
      const url = `${CORS_PROXY}${encodeURIComponent(BASE_URL)}/search/track`
      const response = await axios.get(url, {
        params: { q: query },
      })
      return response.data.data || []
    } catch (error) {
      console.error('Error searching tracks:', error)
      return []
    }
  },

  // Get details
  getArtist: async (artistId: number): Promise<DeezerArtist | null> => {
    try {
      const url = `${CORS_PROXY}${encodeURIComponent(BASE_URL)}/artist/${artistId}`
      const response = await axios.get(url)
      return response.data
    } catch (error) {
      console.error('Error getting artist:', error)
      return null
    }
  },

  getArtistAlbums: async (artistId: number): Promise<DeezerAlbum[]> => {
    try {
      const url = `${CORS_PROXY}${encodeURIComponent(BASE_URL)}/artist/${artistId}/albums`
      const response = await axios.get(url)
      return response.data.data || []
    } catch (error) {
      console.error('Error getting artist albums:', error)
      return []
    }
  },

  getAlbum: async (albumId: number): Promise<DeezerAlbum | null> => {
    try {
      const url = `${CORS_PROXY}${encodeURIComponent(BASE_URL)}/album/${albumId}`
      const response = await axios.get(url)
      return response.data
    } catch (error) {
      console.error('Error getting album:', error)
      return null
    }
  },

  getAlbumTracks: async (albumId: number): Promise<DeezerTrack[]> => {
    try {
      const url = `${CORS_PROXY}${encodeURIComponent(BASE_URL)}/album/${albumId}/tracks`
      const response = await axios.get(url)
      return response.data.data || []
    } catch (error) {
      console.error('Error getting album tracks:', error)
      return []
    }
  },

  // Charts
  getTopArtists: async (): Promise<DeezerArtist[]> => {
    try {
      const url = `${CORS_PROXY}${encodeURIComponent(BASE_URL)}/chart/0/artists`
      const response = await axios.get(url)
      return response.data.data || []
    } catch (error) {
      console.error('Error getting top artists:', error)
      return []
    }
  },

  getTopAlbums: async (): Promise<DeezerAlbum[]> => {
    try {
      const url = `${CORS_PROXY}${encodeURIComponent(BASE_URL)}/chart/0/albums`
      const response = await axios.get(url)
      return response.data.data || []
    } catch (error) {
      console.error('Error getting top albums:', error)
      return []
    }
  },
}
