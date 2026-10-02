import './App.css'
import ArenaNav from './ArenaNav'
import WizardDuel from './WizardDuel'

export default function App() {
  return (
    <div className="arena-shell">
      <ArenaNav active="wizard" />
      <div className="arena-main">
        <WizardDuel />
      </div>
    </div>
  )
}
