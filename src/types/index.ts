export interface Movie {
  id: number
  title: string
  overview: string
  poster_path: string | null
  backdrop_path: string | null
  release_date: string
  vote_average: number
  vote_count: number
  genre_ids?: number[]
}

export interface Series {
  id: number
  name: string
  overview: string
  poster_path: string | null
  backdrop_path: string | null
  first_air_date: string
  vote_average: number
  vote_count: number
  genre_ids?: number[]
}

export interface Video {
  id: string
  key: string
  name: string
  site: string
  type: string
  official: boolean
}

export interface Cast {
  id: number
  name: string
  character: string
  profile_path: string | null
  order: number
}

export interface Favorite {
  id: number
  type: 'movie' | 'series'
  title: string
  poster_path: string | null
  addedAt: string
}
