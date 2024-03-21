#pragma once
#include "Engine/GameObject.h"

class Gauge :
    public GameObject
{
	int hGaugeBar_;//ゲージの画像ハンドル
	int hGaugeFrame_;//フレームの画像ハンドル

	int gaugeMaxVal_;//ゲージの最大値
	int gaugeCrrVal_;//カレントの値（現在の値）current value

public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Gauge(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	void SetGaugeVal(int _crr, int _max)
	{
		gaugeMaxVal_ = _max;
		gaugeCrrVal_ = _crr;
	}
};

