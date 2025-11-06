import { DeezerTrack } from '../services/deezer'
import { useState, useEffect, useRef } from 'react'

interface MusicPlayerProps {
  track: DeezerTrack | null
  isPlaying: boolean
  onPlayPause: () => void
  onClose: () => void
  audioElement: HTMLAudioElement | null
}

const MusicPlayer = ({ track, isPlaying, onPlayPause, onClose, audioElement }: MusicPlayerProps) => {
  const [progress, setProgress] = useState(0)
  const [currentTime, setCurrentTime] = useState(0)
  const [volume, setVolume] = useState(100)
  const [isMuted, setIsMuted] = useState(false)
  const [showVolumeSlider, setShowVolumeSlider] = useState(false)
  const progressBarRef = useRef<HTMLDivElement>(null)
  const volumeBarRef = useRef<HTMLDivElement>(null)

  useEffect(() => {
    if (!audioElement) return

    const updateProgress = () => {
      const current = audioElement.currentTime
      setCurrentTime(current)
      setProgress((current / 30) * 100)
    }

    audioElement.addEventListener('timeupdate', updateProgress)
    
    // Set initial volume
    audioElement.volume = volume / 100

    return () => {
      audioElement.removeEventListener('timeupdate', updateProgress)
    }
  }, [audioElement])

  useEffect(() => {
    if (audioElement) {
      audioElement.volume = isMuted ? 0 : volume / 100
    }
  }, [volume, isMuted, audioElement])

  useEffect(() => {
    if (!track) {
      setCurrentTime(0)
      setProgress(0)
    }
  }, [track])

  const handleProgressClick = (e: React.MouseEvent<HTMLDivElement>) => {
    if (!audioElement || !progressBarRef.current) return

    const rect = progressBarRef.current.getBoundingClientRect()
    const clickX = e.clientX - rect.left
    const percentage = clickX / rect.width
    const newTime = percentage * 30

    audioElement.currentTime = Math.max(0, Math.min(30, newTime))
    setCurrentTime(newTime)
    setProgress(percentage * 100)
  }

  const handleVolumeClick = (e: React.MouseEvent<HTMLDivElement>) => {
    if (!volumeBarRef.current) return

    const rect = volumeBarRef.current.getBoundingClientRect()
    const clickY = rect.bottom - e.clientY // Inverser car vertical
    const percentage = Math.max(0, Math.min(100, (clickY / rect.height) * 100))

    setVolume(percentage)
    setIsMuted(false)
  }

  const handleVolumeMouseMove = (e: React.MouseEvent<HTMLDivElement>) => {
    if (e.buttons !== 1 || !volumeBarRef.current) return // Vérifie si bouton gauche pressé

    const rect = volumeBarRef.current.getBoundingClientRect()
    const clickY = rect.bottom - e.clientY
    const percentage = Math.max(0, Math.min(100, (clickY / rect.height) * 100))

    setVolume(percentage)
    setIsMuted(false)
  }

  const toggleMute = () => {
    setIsMuted(!isMuted)
  }

  const getVolumeIcon = () => {
    if (isMuted || volume === 0) return '🔇'
    if (volume < 30) return '🔈'
    if (volume < 70) return '🔉'
    return '🔊'
  }

  const formatTime = (seconds: number) => {
    const mins = Math.floor(seconds / 60)
    const secs = Math.floor(seconds % 60)
    return `${mins}:${String(secs).padStart(2, '0')}`
  }

  if (!track) return null

  return (
    <div className="fixed bottom-0 left-0 right-0 bg-netflix-gray border-t border-gray-700 z-40">
      {/* Progress Bar */}
      <div
        ref={progressBarRef}
        onClick={handleProgressClick}
        className="h-1 bg-gray-800 cursor-pointer hover:h-2 transition-all group"
      >
        <div
          className="h-full bg-netflix-red transition-all group-hover:bg-red-500"
          style={{ width: `${progress}%` }}
        >
          <div className="opacity-0 group-hover:opacity-100 float-right -mt-1">
            <div className="w-3 h-3 bg-white rounded-full shadow-lg"></div>
          </div>
        </div>
      </div>

      {/* Player Controls */}
      <div className="container mx-auto px-4 py-3">
        <div className="flex items-center justify-between gap-4">
          {/* Track Info */}
          <div className="flex items-center gap-3 flex-1 min-w-0">
            <img
              src={track.album.cover_medium}
              alt={track.album.title}
              className="w-14 h-14 rounded shadow-lg"
              onError={(e) => {
                const target = e.target as HTMLImageElement
                target.src = 'https://via.placeholder.com/120?text=No+Image'
              }}
            />
            <div className="flex-1 min-w-0">
              <h3 className="font-semibold text-white truncate">{track.title}</h3>
              <p className="text-sm text-gray-400 truncate">{track.artist.name}</p>
            </div>
          </div>

          {/* Controls */}
          <div className="flex items-center gap-4">
            {/* Time */}
            <div className="hidden sm:flex items-center gap-2 text-sm text-gray-400">
              <span>{formatTime(currentTime)}</span>
              <span>/</span>
              <span>0:30</span>
            </div>

            {/* Play/Pause Button */}
            <button
              onClick={onPlayPause}
              className="w-12 h-12 flex items-center justify-center bg-netflix-red hover:bg-red-700 rounded-full transition-colors"
            >
              {isPlaying ? (
                <span className="text-2xl">⏸️</span>
              ) : (
                <span className="text-2xl">▶️</span>
              )}
            </button>

            {/* Volume Control */}
            <div 
              className="hidden md:flex items-center gap-2 relative"
              onMouseEnter={() => setShowVolumeSlider(true)}
              onMouseLeave={() => setShowVolumeSlider(false)}
            >
              <button
                onClick={toggleMute}
                className="w-10 h-10 flex items-center justify-center hover:bg-gray-700 rounded-full transition-colors"
                title={isMuted ? 'Réactiver le son' : 'Couper le son'}
              >
                <span className="text-xl">{getVolumeIcon()}</span>
              </button>
              
              {/* Volume Slider */}
              <div
                className={`absolute bottom-full left-1/2 -translate-x-1/2 mb-2 bg-netflix-gray border border-gray-600 rounded-lg p-3 shadow-lg transition-all ${
                  showVolumeSlider ? 'opacity-100 visible' : 'opacity-0 invisible'
                }`}
              >
                <div className="flex flex-col items-center gap-2">
                  <span className="text-xs text-gray-400 font-semibold">{Math.round(isMuted ? 0 : volume)}%</span>
                  <div
                    ref={volumeBarRef}
                    onClick={handleVolumeClick}
                    onMouseMove={handleVolumeMouseMove}
                    className="w-2 h-24 bg-gray-700 rounded-full cursor-pointer hover:bg-gray-600 relative"
                  >
                    <div
                      className="absolute bottom-0 w-full bg-netflix-red rounded-full transition-all pointer-events-none"
                      style={{ height: `${isMuted ? 0 : volume}%` }}
                    >
                      <div className="absolute top-0 left-1/2 -translate-x-1/2 -translate-y-1/2 w-3 h-3 bg-white rounded-full shadow-lg"></div>
                    </div>
                  </div>
                </div>
              </div>
            </div>

            {/* Close Button */}
            <button
              onClick={onClose}
              className="w-10 h-10 flex items-center justify-center hover:bg-gray-700 rounded-full transition-colors"
              title="Fermer"
            >
              <span className="text-xl">✕</span>
            </button>
          </div>
        </div>
      </div>

      {/* Preview Notice */}
      <div className="bg-blue-900 bg-opacity-20 border-t border-blue-700 px-4 py-2">
        <p className="text-xs text-blue-200 text-center">
          🎧 Aperçu de 30 secondes • Pour écouter la version complète, visitez Deezer
        </p>
      </div>
    </div>
  )
}

export default MusicPlayer
