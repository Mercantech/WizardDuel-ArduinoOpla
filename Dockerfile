# Build React frontend
FROM node:20-alpine AS frontend
WORKDIR /app/wizard-duel
COPY wizard-duel/package*.json ./
RUN npm ci
COPY wizard-duel/ ./
ARG VITE_BASE_PATH=/Wizard/
ENV VITE_BASE_PATH=$VITE_BASE_PATH
RUN npm run build

# Runtime: bridge + static
FROM node:20-alpine
WORKDIR /app

COPY opla-wizard-bridge/package*.json ./
RUN npm ci --omit=dev

COPY opla-wizard-bridge/ ./
COPY --from=frontend /app/wizard-duel/dist ./public

ENV PORT=3000
ENV PUBLIC_DIR=/app/public
EXPOSE 3000

CMD ["node", "bridge.js"]
