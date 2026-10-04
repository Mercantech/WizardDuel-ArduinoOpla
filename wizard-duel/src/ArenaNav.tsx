import './arena-nav.css'

type Active = 'select' | 'guide' | 'status' | 'bomber' | 'wizard' | 'tetris' | 'pong'

function detectActive(): Active {
  const path = window.location.pathname || ''
  if (/\/Bomberman/i.test(path)) return 'bomber'
  if (/\/Wizard/i.test(path)) return 'wizard'
  if (/\/Tetris/i.test(path)) return 'tetris'
  if (/\/Pong/i.test(path)) return 'pong'
  if (/\/guide/i.test(path)) return 'guide'
  if (/\/status/i.test(path)) return 'status'
  return 'select'
}

export default function ArenaNav({ active }: { active?: Active }) {
  const current = active ?? detectActive()

  return (
    <header className="arena-nav" role="navigation" aria-label="Mercantec Games">
      <a className="arena-nav-brand" href="/">
        <span className="arena-nav-mark">MERCANTEC</span>
        <span className="arena-nav-sub">GAMES · EST. ARENA</span>
      </a>
      <nav className="arena-nav-links">
        <a href="/" className={current === 'select' ? 'active' : undefined}>
          SELECT
        </a>
        <a href="/guide" className={current === 'guide' ? 'active' : undefined}>
          GUIDE
        </a>
        <a href="/status" className={current === 'status' ? 'active' : undefined}>
          STATUS
        </a>
        <a href="/Bomberman/" className={current === 'bomber' ? 'active' : undefined}>
          BOMBER
        </a>
        <a href="/Wizard/" className={current === 'wizard' ? 'active' : undefined}>
          WIZARD
        </a>
        <a href="/Tetris/" className={current === 'tetris' ? 'active' : undefined}>
          TETRIS
        </a>
        <a href="/Pong/" className={current === 'pong' ? 'active' : undefined}>
          PONG
        </a>
      </nav>
    </header>
  )
}
