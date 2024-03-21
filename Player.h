#pragma once
#include "Engine/GameObject.h"

class Stage;

class PacRect
{
public:
	PacRect();
	void SetRectCenter(float _cx, float _cy, float _width, float _height);
	void SetRectTopBottom(float _left, float _top, float _width, float _height);
	~PacRect() {};
	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();
	float GetCenterX();
	float GetCenterY();
	float GetWidth();
	float GetHeight();
	float top;
	float bottom;
	float left;
	float right;
	float centerx;
	float centery;
	float width;
	float height;
};

class Player :
    public GameObject
{
	int hModel_;
	float speed_;
	Stage* pStage_;
	int hpMax_;//最大HP
	int hpCrr_;//HP
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Player(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

