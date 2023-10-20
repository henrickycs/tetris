#pragma once
#include "grid.h"
#include "blocks.cpp"


class Game {
public:
	Game();
	~Game();
	void Draw();
	void HandleInput();
	void MoveBlockDown();
	bool gameOver;
	int score;
	Music music;

private:
	Block GetRandomBlock();
	std::vector<Block> GetAllBlocks();
	void MoveBlockLeft();
	void MoveBlockRight();
	bool isBlockOutside();
	void RotateBlock();
	void LockBlock();
	bool BlockFit();
	void Reset();
	void UpdateScore(int linesCleared, int moveDownPoints);
	Grid grid;
	std::vector<Block> blocks;
	Block currentBlock;
	Block nextBlock;
	Sound rotateSound;
	Sound clearSound;
};
