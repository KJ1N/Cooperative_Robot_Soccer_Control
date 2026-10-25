#include "getball.h"
#include "utils/maths.h"

//用户注意；接口需要如下声明
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);

enum ball_near  
{
	outOfOrbit,
	onOrbit,
	shoot
};

PlayerTask player_plan(const WorldModel* model, int robot_id){
	PlayerTask task;
	int   receiver= -1;
	for (int i = 0; i < 6; i++){
		if (i == robot_id || i == model->get_our_goalie())
			continue;
		if (model->get_our_exist_id()[i])
			receiver = i;
	}
	point2f receiver_pos(300, 25);
	const point2f& kicker_pos = model->get_our_player_pos(robot_id);
	const float& circleR = 30;
	const float& DetAngle = 0.6;
	const point2f& ball = model->get_ball_pos();
	const float& dir = model->get_our_player_dir(robot_id);
	ball_near orbit;
	point2f shootPosOnOrbit = ball + Maths::vector2polar(circleR, (ball - receiver_pos).angle());
	float toShootDir = fabs((kicker_pos - ball).angle() - (ball - receiver_pos).angle());
	float toOppGoalDir = (receiver_pos - kicker_pos).angle();
	float toBallDist = (kicker_pos - ball).length();
	float rec2kickDir = (receiver_pos - kicker_pos).angle();
	float toBallDir = (ball - kicker_pos).angle();
	point2f robotBallAcrossCirclePoint = ball + Maths::vector2polar(circleR, (kicker_pos - ball).angle());
	point2f BallToRobot = kicker_pos - ball;
	if (ball.y < 0)
	{
		receiver_pos.y = -receiver_pos.y;
	}
	if (toBallDist >circleR + 10)
		orbit = outOfOrbit;
	else if (toShootDir > 1)
		orbit = onOrbit;
	else  
		orbit = shoot;
	bool getBall = toBallDist < 10;
	float diffdir_onorbit = 0;
	float b2r = BallToRobot.angle();
	float o2b = (ball - receiver_pos).angle();
	bool add;
	switch (orbit)
	{
	case outOfOrbit:
		task.target_pos = robotBallAcrossCirclePoint;
		task.orientate = rec2kickDir;
		break;
	case onOrbit:
		if (b2r * o2b >0){
			if (b2r > 0){
				if (b2r > o2b)
					add = false;
				else
					add = true;
			}
			else{
				if (b2r > o2b)
					add = false;
				else
					add = true;
			}
		}
		else{
			if (b2r > 0)
				add = true;
			else
				add = false;
		}
		if (add)
		{
			task.target_pos = ball + Maths::vector2polar(circleR, BallToRobot.angle() + DetAngle);
			task.orientate = rec2kickDir;
		}
		else
		{
			task.target_pos = ball + Maths::vector2polar(circleR, BallToRobot.angle() - DetAngle);
			task.orientate = rec2kickDir;
		}
		break;
	case shoot:
		task.target_pos = ball + Maths::vector2polar(7, (ball - receiver_pos).angle());
		task.orientate = rec2kickDir;
		break;
	}
	return task;
}