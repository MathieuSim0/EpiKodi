import { useState } from 'react'
import Layout from './components/Layout'
import MoviesTab from './components/tabs/MoviesTab'
import SeriesTab from './components/tabs/SeriesTab'
import MusicTab from './components/tabs/MusicTab'
import FavoritesTab from './components/tabs/FavoritesTab'
import { FavoritesProvider } from './context/FavoritesContext'

export type TabType = 'movies' | 'series' | 'music' | 'favorites'

function App() {
  const [activeTab, setActiveTab] = useState<TabType>('movies')

  const renderTab = () => {
    switch (activeTab) {
      case 'movies':
        return <MoviesTab />
      case 'series':
        return <SeriesTab />
      case 'music':
        return <MusicTab />
      case 'favorites':
        return <FavoritesTab />
      default:
        return <MoviesTab />
    }
  }

  return (
    <FavoritesProvider>
      <Layout activeTab={activeTab} setActiveTab={setActiveTab}>
        {renderTab()}
      </Layout>
    </FavoritesProvider>
  )
}

export default App
