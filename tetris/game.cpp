#include "game.h"
#include <random>

Game::Game() {
	grid = Grid();
	blocks = GetAllBlocks();
	currentBlock = GetRandomBlock();
	nextBlock = GetRandomBlock();
	gameOver = false;
	score = 0;
	InitAudioDevice();
	music = LoadMusicStream("../sounds/music.mp3");
	PlayMusicStream(music);
	rotateSound = LoadSound("../sounds/rotate.mp3");
	clearSound = LoadSound("../sounds/clear.mp3");
}

Game::~Game() {
	UnloadSound(rotateSound);
	UnloadSound(clearSound);
	UnloadMusicStream(music);
	CloseAudioDevice();
}

Block Game::GetRandomBlock() {
	if (blocks.empty()) {
		blocks = GetAllBlocks();
	}
	int randIndex = rand() % blocks.size();
	Block block = blocks[randIndex];
	blocks.erase(blocks.begin() + randIndex);
	return block;
}

std::vector<Block> Game::GetAllBlocks() {
	return { LBlock(), JBlock(), IBlock(), OBlock(), SBlock(), TBlock(), ZBlock() };
}

void Game::Draw() {
	grid.Draw();
	currentBlock.Draw(11, 11);
	switch (nextBlock.id) {
		case 3:
			nextBlock.Draw(255, 290);
			break;
		case 4:
			nextBlock.Draw(255, 280);
			break;
		default:
			nextBlock.Draw(270,270);
			break;
	}
}

void Game::HandleInput(){
	int keyPressed = GetKeyPressed();
	if (gameOver && keyPressed != 0) {
		gameOver = false;
		Reset();
	}
	switch (keyPressed) {
		case KEY_LEFT:
			MoveBlockLeft();
			break;
		case KEY_RIGHT:
			MoveBlockRight();
			break;
		case KEY_DOWN:
			MoveBlockDown();
			UpdateScore(0, 1);
			break;
		case KEY_UP:
			RotateBlock();
			break;
	}
}

void Game::MoveBlockLeft() {
	if (!gameOver) {
		currentBlock.Move(0, -1);
		if (isBlockOutside() || BlockFit() == false) {
			currentBlock.Move(0, 1);
		}
	}
}

void Game::MoveBlockRight() {
	if (!gameOver) {
		currentBlock.Move(0, 1);
		if (isBlockOutside() || BlockFit() == false) {
			currentBlock.Move(0, -1);
		}
	}
}

void Game::MoveBlockDown() {
	if (!gameOver) {
		currentBlock.Move(1, 0);
		if (isBlockOutside() || BlockFit() == false) {
			currentBlock.Move(-1, 0);
			LockBlock();
		}
	}
}

bool Game::isBlockOutside(){
	std::vector<Position> tiles = currentBlock.GetCellPositions();
	for (Position item : tiles) {
		if (grid.isCellOutside(item.row, item.column)) {
			return true;
		}
	}
	return false;
}

void Game::RotateBlock(){
	if (!gameOver) {
		currentBlock.Rotate();
		if (isBlockOutside() || BlockFit() == false) {
			currentBlock.UndoRotation();
		}
		else {
			PlaySound(rotateSound);
		}
	}
}

void Game::LockBlock(){
	std::vector<Position> tiles = currentBlock.GetCellPositions();
	for (Position item : tiles) {
		grid.grid[item.row][item.column] = currentBlock.id;
	}
	currentBlock = nextBlock;
	if (BlockFit() == false) {
		gameOver = true;
	}
	nextBlock = GetRandomBlock();
	int rowsCleared = grid.ClearFullRows();
	if (rowsCleared > 0) {
		PlaySound(clearSound);
	}
	UpdateScore(rowsCleared, 0);
}

bool Game::BlockFit(){
	std::vector<Position> tiles = currentBlock.GetCellPositions();
	for (Position item : tiles) {
		if (grid.isCellEmpty(item.row, item.column) == false) {
			return false;
		}
	}
	return true;
}

//Reset the game after the game over
//only resets if the player press any button
void Game::Reset(){
	grid.Initialize();
	blocks = GetAllBlocks();
	currentBlock = GetRandomBlock();
	nextBlock = GetRandomBlock();
	score = 0; 
}

void Game::UpdateScore(int linesCleared, int moveDownPoints){
	//100 points for a single clear
	//300 points for a double line clear
	//500 points for a triple line clear
	//1 point for each move down by the player
	switch (linesCleared) {
		case 1:
			score += 100;
			break;
		case 2:
			score += 300;
			break;
		case 3:
			score += 500;
			break;
		default:
			break;
	}
	score += moveDownPoints;
}
