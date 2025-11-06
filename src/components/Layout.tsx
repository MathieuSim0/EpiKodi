import { ReactNode } from 'react'
import Navigation from './Navigation'
import { TabType } from '../App'

interface LayoutProps {
  children: ReactNode
  activeTab: TabType
  setActiveTab: (tab: TabType) => void
}

const Layout = ({ children, activeTab, setActiveTab }: LayoutProps) => {
  return (
    <div className="min-h-screen bg-netflix-black text-white">
      <Navigation activeTab={activeTab} setActiveTab={setActiveTab} />
      <main className="container mx-auto px-4 py-8">
        {children}
      </main>
    </div>
  )
}

export default Layout
