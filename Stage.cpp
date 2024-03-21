#include "Stage.h"
#include "Engine/model.h"
#include "Engine/Camera.h"
#include "Engine/CsvReader.h"

namespace {
	//const int STAGE_X{ 15 };
	//const int STAGE_Y{ 15 };	
}

bool Stage::IsWall(int _x, int _y)
{
	if (stageData_[_y][_x] == STAGE_OBJ::WALL)
		return true;
	else
		return false;
}

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage")
{
	CsvReader csv;
	csv.Load("map.csv");

	stageWidth_ = csv.GetWidth();
	stageHeight_ = csv.GetHeight();

	for (int i = 0; i < stageHeight_; i++)
	{
		vector<int> sdata(stageWidth_, 0);//15ŒÂ‚Ì”z—ñ‚ğ0‚Å‰Šú‰»
		stageData_.push_back(sdata);
	}
	//vector<vector<int>> stageData_(STAGE_Y, vector<int>(STAGE_X, 0));

	for (int j = 0; j < stageHeight_; j++)
	{
		for (int i = 0; i < stageWidth_; i++)
		{
			stageData_[j][i] = csv.GetValue(i, j);
		}
	}
}

void Stage::Initialize()
{
	hFloor_ = Model::Load("floor.fbx");
	assert(hFloor_ >= 0);
	hBlock_ = Model::Load("wall.fbx");
	assert(hBlock_ >= 0);
	Camera::SetPosition({ 6.5, 10, -5 });
	Camera::SetTarget({ 6.5, 0, 5.5 });
}

void Stage::Update()
{

}

void Stage::Draw()
{
	Transform floorTrans;
	floorTrans.position_ = {0, 0, 0};

	for (int z = 0; z < 15; z++){
		for (int x = 0; x < 15; x++) {
			floorTrans.position_ = { (float)x, 0, (float)(14 - z) };
			if (stageData_[z][x] == 1) {
				Model::SetTransform(hBlock_, floorTrans);
				Model::Draw(hBlock_);
			}
			else {
				Model::SetTransform(hFloor_, floorTrans);
				Model::Draw(hFloor_);
			}
		}
	}
	
}

void Stage::Release()
{
	for (int i = 0; i < stageHeight_; i++) 
	{
		stageData_[i].clear();
	}
	stageData_.clear();
}
