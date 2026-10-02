import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'

// Production under Traefik PathPrefix /Wizard (StripPrefix til backend).
// Lokal: VITE_BASE_PATH=/ npm run build  eller default /
const base = process.env.VITE_BASE_PATH || '/'

export default defineConfig({
  plugins: [react()],
  base,
})
