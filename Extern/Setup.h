#pragma once

enum class PlayerMoveType {
	IDLE,
	MOVING,
	SHOOTING,
	DEAD
};
enum class Diff {
	INSANE,
	IMPOSSIBLE,
	HARD,
	NORMAL
};
enum {
	CH_MUSIC,
	CH_PLAYER_HIT,
	CH_ENEMY_THROW,
	CH_FIRE,
	CH_MELT,
	CH_SNOWBALL,
	CH_SNOWMAN_HIT,
	CH_UI,
	CH_COIN
};
