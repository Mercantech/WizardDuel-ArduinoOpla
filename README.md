# ⚡ Wizard Duel - Arduino Opla Edition

Et multiplayer troldmandskamp-spil hvor 2-4 spillere kæmper mod hinanden med magi! Spillerne styrer deres wizard med en Arduino Opla IoT Kit, mens kampen vises live på en webbaseret arena.

**Live:** [https://games.mercantec.tech/Wizard](https://games.mercantec.tech/Wizard)  
Portal: [https://games.mercantec.tech/](https://games.mercantec.tech/)

## Hosting

Én Docker-container serverer både React-frontend og bridge (HTTP + WebSocket), ligesom Bomberman.

```bash
# Produktion (Dokploy / Traefik PathPrefix /Wizard)
docker compose up -d --build

# Lokalt med host-port
docker compose -f docker-compose.yml -f docker-compose.local.yml up --build
```

Lokal lab uden Docker: `START_WIZARD_DUEL.bat` (bridge :3000 + Vite :5173).

## Arduino controller

Samme kontrakt som Bomberman – se `ArduinoKode/WizardDuel_Student/` og [games.mercantec.tech](https://games.mercantec.tech/) for overblik.

| Variabel | Værdi |
|---|---|
| `SERVER_HOST` | `games.mercantec.tech` |
| `GAME_BASE_PATH` | `/Wizard` |
| `USE_HTTPS` | `1` |

Endpoints (efter StripPrefix ser serveren root `/api/...`):

- `POST /api/controller/join`
- `POST /api/controller/heartbeat`
- `POST /api/controller/action` med `"action":"cast"`

## Spells

| Spell | Skade | Mana | Type |
|-------|-------|------|------|
| Fireball | 10 dmg | 20 | Single target |
| Lightning Storm | 10 dmg | 50 | Alle modstandere |
| Shield | - | 25 | Halverer skade i 5 sek |
| Heal | +20 HP | 40 | Self |
| Power Boost | - | 40 | +50% skade i 10 sek |
| Death Ray | 40 dmg | 80 | Single target |

## Licens

MIT License
