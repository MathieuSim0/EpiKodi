import { createContext, useContext, useState, useEffect, ReactNode } from 'react'
import { Favorite } from '../types'

interface FavoritesContextType {
  favorites: Favorite[]
  addFavorite: (favorite: Omit<Favorite, 'addedAt'>) => void
  removeFavorite: (id: number, type: 'movie' | 'series') => void
  isFavorite: (id: number, type: 'movie' | 'series') => boolean
}

const FavoritesContext = createContext<FavoritesContextType | undefined>(undefined)

export const FavoritesProvider = ({ children }: { children: ReactNode }) => {
  const [favorites, setFavorites] = useState<Favorite[]>([])

  useEffect(() => {
    const stored = localStorage.getItem('epikodi-favorites')
    if (stored) {
      setFavorites(JSON.parse(stored))
    }
  }, [])

  useEffect(() => {
    localStorage.setItem('epikodi-favorites', JSON.stringify(favorites))
  }, [favorites])

  const addFavorite = (favorite: Omit<Favorite, 'addedAt'>) => {
    const newFavorite: Favorite = {
      ...favorite,
      addedAt: new Date().toISOString(),
    }
    setFavorites((prev) => [...prev, newFavorite])
  }

  const removeFavorite = (id: number, type: 'movie' | 'series') => {
    setFavorites((prev) => prev.filter((fav) => !(fav.id === id && fav.type === type)))
  }

  const isFavorite = (id: number, type: 'movie' | 'series') => {
    return favorites.some((fav) => fav.id === id && fav.type === type)
  }

  return (
    <FavoritesContext.Provider value={{ favorites, addFavorite, removeFavorite, isFavorite }}>
      {children}
    </FavoritesContext.Provider>
  )
}

export const useFavorites = () => {
  const context = useContext(FavoritesContext)
  if (!context) {
    throw new Error('useFavorites must be used within a FavoritesProvider')
  }
  return context
}
