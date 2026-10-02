import './arena-nav.css'

type Active = 'select' | 'guide' | 'bomber' | 'wizard'

function detectActive(): Active {
  const path = window.location.pathname || ''
  if (/\/Bomberman/i.test(path)) return 'bomber'
  if (/\/Wizard/i.test(path)) return 'wizard'
  if (/\/guide/i.test(path)) return 'guide'
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
        <a href="/Bomberman/" className={current === 'bomber' ? 'active' : undefined}>
          BOMBER
        </a>
        <a href="/Wizard/" className={current === 'wizard' ? 'active' : undefined}>
          WIZARD
        </a>
      </nav>
    </header>
  )
}
