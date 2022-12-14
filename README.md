# Unreal TPS Practice

언리얼 엔진 4 학습과 포토폴리오 목적으로 작성한 아레나 형식의 TPS 게임입니다.

- 엔진 : Unreal Engine 4 4.27.2
- 에디터 : Microsoft Visual Studio Community 2022 17.0.4
- 제작기간 : 2022. 07. 21 ~ 2022. 08. 18

# 플레이 영상 동영상

Youtube Link  :  https://youtu.be/yhGP_yIKe7U

Download Link : https://drive.google.com/file/d/1XUosqQoUH_OcdFbDZQOxslCjweAez0hm/view?usp=sharing
- Warning : 3.3GB
# 주요 기능

## UI

#### 스프레드 반응 크로스헤어

![Spread image](Documents/Imgs/spread.png)

- 디비전2의 크로스헤어를 참고하였습니다.
- 십자선 내부의 크기는 spread의 범위를 나타내며
- 십자선 내부에 원형의 점은 가장 최근 발사된 총알의 spread를 나타냅니다.

#### 3D UI

![](Documents/Imgs/3DUI_NORMAL.png)![](Documents/Imgs/3DUI_AIM.png)

- 좌측 : 비조준 시 3D
- 우측 : 조준 시 3D UI
- 디비전2의 UI를 참고하였습니다.
- 게임 플레이 시 필요한 정보를 나타내는 UI를 엔진에서 제공하는 Widget Component를 이용해 3D 상에서 렌더링하였습니다.

## 캐릭터

#### Inverse Kinetics 적용

- IK를 적용하여 캐릭터의 다리가 다양한 지면에 반응할 수 있습니다.

  - ![IK_LEG_STAIR](Documents/Imgs/IK_LEG_STAIR.png)![IK_LEG_BALLS](Documents/Imgs/IK_LEG_BALLS.png)![IK_LEG_SLOPE](Documents/Imgs/IK_LEG_SLOPE.png)

  - <img src="Documents/Imgs/IK_Stand2.gif" alt="IK_Stand2" style="zoom:75%;" /><img src="Documents/Imgs/IK_Walk.gif" alt="IK_Walk" width="30%" />

- IK를 적용하여 하나의 애니메이션으로 캐릭터의 왼손이 여러 총기의 형태에 맞게 파지할 수 있습니다.

  - ![IK_HAND_74U](Documents/Imgs/IK_HAND_74U.png)![IK_HAND_ASVAL](Documents/Imgs/IK_HAND_ASVAL.png)![IK_HAND_XM4](Documents/Imgs/IK_HAND_XM4.png)




## Etc.

원거리 적의 Deflection shooting

- ![AI_DeflectionShoot](Documents/Imgs/AI_DeflectionShoot.gif)

- 원거리 적의 격발 당시 위치벡터를 $E$, 플레이어의 위치벡터를 $P$, 플레이어의 속도벡터를 $v_p$, 투사체의 속력을 $V_E$, 투사체의 방향벡터를 $d_E$, 투사체가 플레이어와 충돌할 때까지의 시간을 $\Delta t$라 하면 다음의 수식으로 투사체의 방향벡터 $d_E$를 얻을 수 있습니다.
- ![](Documents/Imgs/DeflectionFormular.png)
