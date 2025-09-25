#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
BIN_DIR="$ROOT_DIR/bin"
SRC_DIR="$ROOT_DIR/src"
TEST_DIR="$ROOT_DIR/tests"
DATA_DIR="$ROOT_DIR/data"

mkdir -p "$BIN_DIR" "$DATA_DIR"

# Seed genres if missing (non-destructive)
if [ ! -f "$DATA_DIR/genres.csv" ]; then
	cat > "$DATA_DIR/genres.csv" <<'EOF'
Pop
Rock
Hip-Hop
R&B
Jazz
Blues
Classical
Country
Folk
Reggae
Metal
Punk
Electronic
House
Techno
Trance
Drum & Bass
Dubstep
K-Pop
J-Pop
Latin
Afrobeats
Soul
Gospel
Indie
EOF
fi

# Expect tracks.csv to be committed in the repo; do not generate here
if [ ! -f "$DATA_DIR/tracks.csv" ]; then
	echo "ERROR: data/tracks.csv not found. Please add it to the repo (or run a separate seed script)." >&2
	exit 1
fi

# Expect artists.csv to be present in the repo as well
if [ ! -f "$DATA_DIR/artists.csv" ]; then
	echo "ERROR: data/artists.csv not found. Please add it to the repo (or derive it offline)." >&2
	exit 1
fi

# Ensure headered CSVs exist
[ -f "$DATA_DIR/users.csv" ] || :
[ -f "$DATA_DIR/preferences.csv" ] || echo "user,genres,artists,moods" > "$DATA_DIR/preferences.csv"
[ -f "$DATA_DIR/playlists.csv" ] || echo "user,playlist_name,tracks" > "$DATA_DIR/playlists.csv"

printf "Building app...\n"
/usr/bin/env g++ -std=c++17 -O3 -DNDEBUG -o "$BIN_DIR/app" "$SRC_DIR/main.cpp"

if [ -f "$TEST_DIR/test_csv.cpp" ]; then
	printf "Building tests...\n"
	/usr/bin/env g++ -std=c++17 -O3 -DNDEBUG -o "$BIN_DIR/test_csv" "$TEST_DIR/test_csv.cpp"
	printf "Running tests...\n"
	"$BIN_DIR/test_csv"
fi

printf "Build complete. Binary: %s\n" "$BIN_DIR/app"

if [ "${1-}" = "--run" ]; then
	printf "\nRunning app...\n\n"
	"$BIN_DIR/app"
fi 