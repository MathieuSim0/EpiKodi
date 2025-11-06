import { TabType } from '../App'

interface NavigationProps {
  activeTab: TabType
  setActiveTab: (tab: TabType) => void
}

const Navigation = ({ activeTab, setActiveTab }: NavigationProps) => {
  const tabs = [
    { id: 'movies' as TabType, label: 'Films', icon: '🎬' },
    { id: 'series' as TabType, label: 'Séries', icon: '📺' },
    { id: 'music' as TabType, label: 'Musique', icon: '🎵' },
    { id: 'favorites' as TabType, label: 'Favoris', icon: '⭐' },
  ]

  return (
    <nav className="bg-netflix-gray shadow-lg sticky top-0 z-50">
      <div className="container mx-auto px-4">
        <div className="flex items-center justify-between h-16">
          <div className="flex items-center space-x-2">
            <span className="text-netflix-red text-2xl font-bold">EpiKodi</span>
          </div>
          <div className="flex space-x-1">
            {tabs.map((tab) => (
              <button
                key={tab.id}
                onClick={() => setActiveTab(tab.id)}
                className={`px-4 py-2 rounded-lg transition-all duration-200 flex items-center space-x-2 ${
                  activeTab === tab.id
                    ? 'bg-netflix-red text-white'
                    : 'text-gray-300 hover:bg-gray-700 hover:text-white'
                }`}
              >
                <span>{tab.icon}</span>
                <span className="font-medium">{tab.label}</span>
              </button>
            ))}
          </div>
        </div>
      </div>
    </nav>
  )
}

export default Navigation
