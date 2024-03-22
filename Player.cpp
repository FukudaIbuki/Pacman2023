#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Debug.h"
#include "Stage.h"
#include "Gauge.h"

namespace 
{
	const float PLAYER_MOVE_SPEED{ 0.1f };
}

Player::Player(GameObject* parent)
	:GameObject(parent, "Player"),
	hModel_(-1), speed_(PLAYER_MOVE_SPEED), pStage_(nullptr), hpCrr_(100), hpMax_(100)
{
}

void Player::Initialize()
{
	hModel_ = Model::Load("Player.fbx");
	assert(hModel_ >= 0);
	transform_.position_.x = 0.5;
	transform_.position_.z = 1.5;
	pStage_ = (Stage *)FindObject("Stage");
	rec.SetRectCenter(transform_.position_.x, transform_.position_.z, 1.0, 1.0);
}

void Player::Update()
{
	enum Dir
	{
		UP, LEFT, DOWN, RIGHT, NONE,
	};
	int moveDir = Dir::NONE;

	XMVECTOR vFront = { 0,0,1,0 };
	XMVECTOR move{ 0,0,0,0 };

	if (Input::IsKey(DIK_UP))
	{
		move = XMVECTOR{ 0,0,1,0 };
		//moveDir = Dir::UP;
	}
	if (Input::IsKey(DIK_LEFT))
	{
		move = XMVECTOR{ -1,0,0,0 };
		//moveDir = Dir::LEFT;
	}
	if (Input::IsKey(DIK_DOWN))
	{
		move = XMVECTOR{ 0,0,-1,0 };
		//moveDir = Dir::DOWN;
	}
	if (Input::IsKey(DIK_RIGHT))
	{
		move = XMVECTOR{ 1,0,0,0 };
		//moveDir = Dir::RIGHT;
	}
	XMVECTOR pos = XMLoadFloat3(&(transform_.position_));
	XMVECTOR posTmp = XMVectorZero();//ゼロベクトルで初期化
	posTmp = pos + speed_ * move;

	int tx, ty;
	tx = (int)(XMVectorGetX(posTmp) + 1.0f);
	ty = pStage_->GetStageWidth() - (int)(XMVectorGetZ(posTmp) + 1.0f);
	PacRect trec;
	//if (!(pStage_->IsWall(tx, ty))) 
	//{
	//	pos = posTmp;
	//}
	//else
	//{
	//	hpCrr_ = hpCrr_ - 2;
	//	if (hpCrr_ < 0) hpCrr_ = 0;
	//}
	
	//posTmp.x posTmp.z => int tx, ty :配列のインデックス
	//仮にmapの配列をmap[][]とする
	//移動先がフロア(STAGE_OBJ::FLOOR => 0)だったら動く
	//if (map[ty][tx] == STAGE_OBJ::FLOOR) {
	//	pos = posTemp;
	//}
	//Debug::Log("(X, Z)=");
	//Debug::Log(XMVectorGetX(pos));
	//Debug::Log(",");
	//Debug::Log(XMVectorGetZ(pos), true);

	//Debug::Log("(iX, iZ)=");
	//Debug::Log(tx);
	//Debug::Log(",");
	//Debug::Log(ty);
	//Debug::Log(" : ");
	//Debug::Log(pStage_->IsWall(tx, ty));

	switch (moveDir)
	{
	case UP:
		tx = (int)(trec.centerx + 1);
		ty = pStage_->GetStageHeight() - (int)(trec.top) - 1;
		break;
	case DOWN:
		tx = (int)(trec.centerx + 1);
		ty = pStage_->GetStageHeight() - (int)(trec.bottom) - 1;
		break;
	case LEFT:
		tx = (int)(trec.left + 1);
		ty = pStage_->GetStageHeight() - (int)(trec.centery) - 1;
		break;
	case RIGHT:
		tx = (int)(trec.right + 1);
		ty = pStage_->GetStageHeight() - (int)(trec.centery) - 1;
		break;
	case NONE:
		tx = (int)(trec.centerx + 1);
		ty = pStage_->GetStageHeight() - (int)(trec.centery) - 1;
		break;
	defaulet:
		break;
	}

	if (!(pStage_->IsWall(tx, ty)))
	{
		pos = posTmp;
	}

	if (!XMVector3Equal(move, XMVectorZero())) {
		XMStoreFloat3(&(transform_.position_), pos);

		//XMMATRIX rot = XMMatrixRotationY(-XM_PIDIV2);
		//XMVECTOR modifiedVec = XMPlaneTransform(move, rot);
		//Debug::Log(XMVectorGetX(modifiedVec));
		//Debug::Log(",");
		//Debug::Log(XMVectorGetZ(modifiedVec), true);

		//float angle = atan2(XMVectorGetZ(modifiedVec), XMVectorGetX(modifiedVec));

		//Debug::Log("=>");
		//Debug::Log(XMConvertToDegrees(angle), true);
		//XMVECTOR vdot = XMVector3Dot(vFront, move);
		//assert(XMVectorGetX(vdot) <= 1 && XMVectorGetX(vdot) >= -1);
		//float angle = acos(XMVectorGetX(vdot));
		//float angle = atan2(XMVectorGetX(move), XMVectorGetZ(move));

		//XMVECTOR  vCross = XMVector3Cross(vFront, move);
		//if (XMVectorGetY(vCross) < 0)
		//{
		//	angle *= -1;
		//}
		float retangle = 0.0;
		retangle = atan2(XMVectorGetX(move), XMVectorGetZ(move));
		transform_.rotate_.y = XMConvertToDegrees(retangle);
	}
	//ゲージのインスタンスをFindObjectで見つけて、
	//ゲージ型にキャストして
	//メンバ変数を呼び出して、パックさんの今のHPを、ゲージに反映させる
	Gauge* pGauge = (Gauge*)FindObject("Gauge");
	pGauge->SetGaugeVal(hpCrr_, hpMax_);

	//float rotAngle[5]{ 0, 90, 180, -90, 0 };
	//transform_.rotate_.y = rotAngle[moveDir];
}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Player::Release()
{
}

PacRect::PacRect()
	: top(-1),bottom(-1),left(-1),right(-1),
	centerx(-1),centery(-1),width(-1),height(-1)
{
}

void PacRect::SetRectCenter(float _cx, float _cy, float _width, float _height)
{
	centerx = _cx;
	centery = _cy;
	width = _width;
	height = _height;
	top = centery + height / 2.0;
	bottom = centery - height / 2.0;
	left = centerx - width / 2.0;
	right = centerx + width / 2.0;

}

void PacRect::SetRectTopBottom(float _left, float _top, float _width, float _height)
{
	left = _left;
	top = _top;
	width = _width;
	height = _height;
	centery = top - height / 2.0;
	bottom = top - height;
	centerx = left + width / 2.0;
	right = left + width;
}

float PacRect::GetLeft()
{
	return left;
}

float PacRect::GetRight()
{
	return right;
}

float PacRect::GetTop()
{
	return top;
}

float PacRect::GetBottom()
{
	return bottom;
}

float PacRect::GetCenterX()
{
	return centerx;
}

float PacRect::GetCenterY()
{
	return centery;
}

float PacRect::GetWidth()
{
	return width;
}

float PacRect::GetHeight()
{
	return height;
}

