#include "passball.h"
#include "utils/maths.h"

//用户注意；接口需要如下声明
extern "C"_declspec(dllexport) PlayerTask player_plan(const WorldModel* model, int robot_id);

namespace{
	float head_len = 7.0;
#define fast_pass 7
}
bool is_getball(const point2f& player_pos, const point2f& ball_pos)
{

	float toballdist = (player_pos - ball_pos).length();
	if (toballdist < 15)
	{
		return 1;
	}
}
//判断是否可以传球
bool is_ready_pass(const point2f& ball, const point2f& passer, const point2f& receiver){
	float receiver_to_ball = (ball - receiver).angle();
	float ball_to_passer = (passer - ball).angle();
	//两个矢量角度之差小于某个值，判断是否可以传球
	bool pass;
	if ((passer - receiver).length() > 80)
		pass = fabs(receiver_to_ball - ball_to_passer) < 0.25;
	else
		pass = fabs(receiver_to_ball - ball_to_passer) < 0.28;
	return pass;
}
int shoot_flag = 1;
PlayerTask player_plan(const WorldModel* model, int robot_id){
	PlayerTask task;
	//获取球的坐标
	const point2f& ball = model->get_ball_pos();
	
	int robot1 = -7, receiver = -7;
	for (int i = 0; i < 6; i++){
		if (i == robot_id || i == model->get_our_goalie())
			continue;
		if (model->get_our_exist_id()[i])
			robot1 = i;
	}
	
	//获取我方球员坐标
	const point2f& robot1_pos = model->get_our_player_pos(robot1);

		receiver = robot1;
	//获取接球者朝向
	float rece_dir = model->get_our_player_dir(receiver);
	//获取传球者朝向
	float excute_dir = model->get_our_player_dir(robot_id);
	//获取接球者位置
	const point2f& recieve_player_pos = model->get_our_player_pos(receiver);
	//获取传球者位置
	const point2f& pass_player_pos = model->get_our_player_pos(robot_id);
	//获取接球者到球距离
	float r2bdist = (recieve_player_pos - ball).length();
	float rece_to_ball = (ball - recieve_player_pos).angle();    //接球者朝向球的夹角
	point2f receive_head = recieve_player_pos + Maths::vector2polar(head_len, rece_dir);
	float pass_dir = (receive_head - ball).angle();
	bool get_ball = (ball - pass_player_pos).length() < get_ball_threshold - 1.5 && (fabs(anglemod(excute_dir - (ball - pass_player_pos).angle())) < PI / 4);
	point2f BallToRobot = pass_player_pos - ball;
	//判断并执行传球 
	float bias_shootoppdir = (ball - pass_player_pos).angle();
	float b2r = BallToRobot.angle();
	float o2b = (ball - recieve_player_pos).angle();
	task.needCb = true;
	/*if (is_ready_pass(ball, pass_player_pos, recieve_player_pos)){
		if (get_ball){*/
	/*if (is_getball(pass_player_pos, ball) && fabs(b2r - o2b) < 0.25){
		if (get_ball){
			task.needKick = true;
			if (r2bdist < 80)
				task.kickPower = 5;
			else if (r2bdist < 150 )
				task.kickPower = 15;
			else if (r2bdist < 300)
				task.kickPower = 20;
			else 
				task.kickPower = 30;
		}
		task.target_pos = ball - Maths::vector2polar(fast_pass, rece_to_ball);
		task.needCb = true;
	}
	else
	{
		task.target_pos = ball - Maths::vector2polar(BALL_SIZE / 2 + MAX_ROBOT_SIZE + 8, rece_to_ball);
		task.needCb = true;
	}
	task.orientate = pass_dir;
	task.flag = 1;*/

	task.target_pos = ball - Maths::vector2polar(8, bias_shootoppdir);    //拿球
	task.orientate = bias_shootoppdir;                                         //射门角度
	task.needCb = true;
	if (is_getball(pass_player_pos, ball) && fabs(b2r - o2b) < 0.10)                                      //<10仿真软件不射门，11仿真软件射门
	{
		task.kickPower = 30;
		task.needKick = true;
		shoot_flag = 0;
	}
	return task;
}
