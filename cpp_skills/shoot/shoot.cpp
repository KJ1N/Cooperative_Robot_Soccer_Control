#include "shoot.h"
#include "utils/maths.h"
//用户注意；接口需要如下声明
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);
bool is_getball(const point2f& player_pos, const point2f& ball_pos)
{

	float toballdist = (player_pos - ball_pos).length();
	if (toballdist < 15)
	{
		return 1;
	}
}
enum ball_near  //PenaltyArea
{

	outOfOrbit,
	onOrbit,
	shoot
};
int shoot_flag = 1;
PlayerTask player_plan(const WorldModel* model, int robot_id)
{
	PlayerTask task;
	const float pi = 3.1415926;
	const float& circleR = 10;
	const float& DetAngle = 0.6;
	const point2f& player_pos = model->get_our_player_pos(robot_id);      //获得球员坐标
	const point2f& ball_pos = model->get_ball_pos();                      //获得球的坐标
	const float& player_dir = model->get_our_player_dir(robot_id);        //获得球员朝向
	point2f shoot_position(300, 25);
	ball_near orbit;
	point2f shootPosOnOrbit = ball_pos + Maths::vector2polar(circleR, (ball_pos - shoot_position).angle());
	float toShootDir = fabs((player_pos - ball_pos).angle() - (ball_pos - shoot_position).angle());
	float toBallDist = (player_pos - ball_pos).length();
	const point2f& opp_goal = -FieldPoint::Goal_Center_Point;             //获取敌方球门坐标
	//偏向射门
	float toBallDir = (ball_pos - player_pos).angle();
	point2f robotBallAcrossCirclePoint = ball_pos + Maths::vector2polar(circleR, (player_pos - ball_pos).angle());
	point2f AntishootPosOnOrbit = ball_pos + Maths::vector2polar(circleR, (opp_goal - ball_pos).angle());
	point2f BallToRobot = player_pos - ball_pos;
	if (ball_pos.y < 0)
	{
		shoot_position.y = -shoot_position.y;
	}
	//if (toBallDist >circleR + 10)
	//orbit = outOfOrbit;
	//else if (toShootDir > 1)
	//orbit = onOrbit;
    //else
	//orbit = shoot;
	float b2r = BallToRobot.angle();
	float o2b = (ball_pos - shoot_position).angle();
	float bias_shootoppdir = (shoot_position - ball_pos).angle();         //偏向射门角度
	//bool add;

	//switch (orbit)
	//{
	//case outOfOrbit:
	//	task.target_pos = robotBallAcrossCirclePoint;
	//	task.orientate = bias_shootoppdir;
	//	break;
	//case onOrbit:
	//	if (b2r * o2b >0){
	//		if (b2r > 0){
	//			if (b2r > o2b)
	//				add = false;
	//			else
	//				add = true;
	//		}
	//		else{
	//			if (b2r > o2b)
	//				add = false;
	//			else
	//				add = true;
	//		}
	//	}
    //	else{
	//		if (b2r > 0)
	//			add = true;
	//		else
	//			add = false;
	//	}
	//	if (add)
	//	{
	//		//+
	//		task.target_pos = ball_pos + Maths::vector2polar(circleR, BallToRobot.angle() + DetAngle);
	//		task.orientate = bias_shootoppdir;
	//	}
	//	else
	//	{
	//		//-
	//		task.target_pos = ball_pos + Maths::vector2polar(circleR, BallToRobot.angle() - DetAngle);
	//	task.orientate = bias_shootoppdir;
	//	}
	//	break;
//	case shoot:
	task.target_pos = ball_pos - Maths::vector2polar(8, bias_shootoppdir);    //拿球
	task.orientate = bias_shootoppdir;                                         //射门角度
	task.needCb = true;
	if (is_getball(player_pos, ball_pos) && fabs(b2r - o2b) < 0.10)                                      //<10仿真软件不射门，11仿真软件射门
	{
		task.kickPower = 127;
		task.needKick = true;
		shoot_flag = 0;
	}
	//	   break;
	//}
	return task;
}
