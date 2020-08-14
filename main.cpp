#include <iostream> 
#include <windows.h>
#include <conio.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <cstdlib>
#define PI 3.1415926535f
#define CL_CO 10
bool is_shut = false; //global, so bad!


float get_radianAngle(float x, float y)
{
	if (x == 0.f || y == 0.f)
	{
		return x == 0.f ? (y > 0 ? 0 : PI) : (y == 0 ? (x > 0 ? PI / 2 : 3 * PI / 4) : 0);
	}
	else
	{
		if (x > 0.f && y > 0.f)
		{
			std::cout << "A\n";
			return atanf(x / y);
		}
		else if (x > 0.f && y < 0.f)
		{
			std::cout << "B\n";
			return atanf(x / y) + PI;
		}
		else if (x < 0.f && y > 0.f)
		{
			std::cout << "C\n";
			return atanf(x / y) + 2 * PI;
		}
		else if (x < 0.f && y < 0.f)
		{
			std::cout << "D\n";
			return atanf(x / y) + PI;
		}
	}
	return 0.f;
}
struct Bullet
{
	float x;
	float y;
	float sin;
	float cos;
	bool timeToDie; //not used
};

struct Enemy
{
	float x;
	float y;
	//float Vx;
	//float Vy;
	float V;
	float angle;
	//float sin;
	//float cos;
	int radius;
	bool timeToDie;

	bool clear;
	int clear_count;
	//std::vector<float> dist_to_bullet;
};

bool is_collision(const Enemy& a, const Enemy& b)
{
	//if (a.x > a.radius / 2 && a.x < (1000 - a.radius / 2) && a.y > a.radius / 2 && a.y < (500 - a.radius / 2) &&
	//	b.x > b.radius / 2 && b.x < (1000 - b.radius / 2) && b.y > b.radius / 2 && b.y < (500 - b.radius / 2))
	//{
		float dist = (float)a.radius + (float)b.radius;
		float real_dist;
		if (a.x > (float)a.radius  && a.x < (999.f - (float)a.radius ) && a.y >(float)a.radius  && a.y < (499.f - (float)a.radius ) &&
			b.x > (float)b.radius  && b.x < (999.f - (float)b.radius ) && b.y >(float)b.radius  && b.y < (499.f - (float)b.radius ))
		{
			real_dist = (pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
		}
		else 
		{
			float Ax, Ay, Bx, By;
			if (a.x < (float)a.radius && b.x >(999.f - (float)b.radius))
			{
				Ax = a.x + 999.f;
				Bx = b.x;
			}
			else if (a.x > (999.f - (float)a.radius) && b.x < (float)b.radius)
			{
				Ax = a.x - 999.f;
				Bx = b.x;
			}
			else
			{
				Ax = a.x;
				Bx = b.x;
			}

			if (a.y < (float)a.radius && b.y >(499.f - (float)b.radius))
			{
				Ay = a.y + 499.f;
				By = b.y;
			}
			else if (a.y > (499 - (float)a.radius) && b.y < (float)b.radius)
			{
				Ay = a.y - 499.f;
				By = b.y;
			}
			else
			{
				Ay = a.y;
				By = b.y;
			}


/*
			if (b.x < (float)b.radius&& a.x >(999.f - (float)a.radius))
				Bx = b.x + 999.f;
			else if (b.x >(999.f - (float)b.radius) && a.x < (float)a.radius)
				Bx = b.x - 999.f;
			else Bx = b.x;

			if (b.y < (float)b.radius && a.y >(499.f - (float)a.radius))
				By = b.y + 499.f;
			else if (b.y >(499.f - (float)b.radius) && a.y < (float)a.radius)
				By = b.y - 499.f;
			else By = b.y; */

			real_dist = (pow((Ax - Bx), 2) + pow((Ay - By), 2));
		}
		dist *= dist;
		return dist > real_dist ? true : false;
	//}

};

bool can_itBorn(float x, float y, float r, const std::vector<Enemy>& vec)
{
	bool born = true;
	auto it = vec.begin();
	while (it != vec.end())
	{
		if( (pow((x - (*it).x), 2) + pow((y - (*it).y), 2)) <= ((r + (*it).radius)*(r + (*it).radius)) )
			born = false;
		++it;
	}
	return born;
}

void collision(Enemy& a, Enemy& b)
{
	
	//if (a.clear == false && b.clear == false)
	//{
		float Teta;
		if (a.x > (float)a.radius  && a.x < (999.f - (float)a.radius) && a.y >(float)a.radius  && a.y < (499.f - (float)a.radius) &&
			b.x >(float)b.radius  && b.x < (999.f - (float)b.radius) && b.y >(float)b.radius  && b.y < (499.f - (float)b.radius))
		{
			Teta = /*get_radianAngle((float)b.x - (float)a.x, (float)b.y - (float)a.y);*/atanf(((float)b.x - (float)a.x)/((float)b.y - (float)a.y));
			std::cout << "Teta = " << Teta*180.f / PI << "\n";
		}
		else
		{
			float Ax, Ay, Bx, By;
			if (a.x < (float)a.radius && b.x >(999.f - (float)b.radius))
			{
				Ax = a.x + 999.f;
				Bx = b.x;
			}
			else if (a.x > (999.f - (float)a.radius) && b.x < (float)b.radius)
			{
				Ax = a.x - 999.f;
				Bx = b.x;
			}
			else
			{
				Ax = a.x;
				Bx = b.x;
			}

			if (a.y < (float)a.radius && b.y >(499.f - (float)b.radius))
			{
				Ay = a.y + 499.f;
				By = b.y;
			}
			else if (a.y > (499 - (float)a.radius) && b.y < (float)b.radius)
			{
				Ay = a.y - 499.f;
				By = b.y;
			}
			else
			{
				Ay = a.y;
				By = b.y;
			}


			/*if (b.x < (float)b.radius&& a.x >(999.f - (float)a.radius))
				Bx = b.x + 999.f;
			else if (b.x >(999.f - (float)b.radius) && a.x < (float)a.radius)
				Bx = b.x - 999.f;
			else Bx = b.x;

			if (b.y < (float)b.radius && a.y >(499.f - (float)a.radius))
				By = b.y + 499.f;
			else if (b.y >(499.f - (float)b.radius) && a.y < (float)a.radius)
				By = b.y - 499.f;
			else By = b.y;*/

			Teta = atanf((Bx - Ax)/ (By - Ay));/*get_radianAngle(Bx - Ax, By - Ay);*/
			std::cout << "!!!!!!!!!!!!!!!!Teta = " << Teta*180.f / PI << "\n";
		}
		float Alfa = a.angle;
		float Beta = b.angle;
		float m1 = (float)a.radius;// == 20 ? 500 : 100;//a.radius;
		float m2 = (float)b.radius;// == 20 ? 500 : 100;//b.radius;

		//float U1y = (2 * m2*b.V*cosf(PI + Teta - Beta) + a.V*cosf(Alfa - Teta)*(m1 - m2))*cosf(Teta) / (m1 + m2) + a.V*sinf(Alfa - Teta)*cosf(PI / 2 + Teta);
		//float U1x = (2 * m2*b.V*cosf(PI + Teta - Beta) + a.V*cosf(Alfa - Teta)*(m1 - m2))*sinf(Teta) / (m1 + m2) + a.V*sinf(Alfa - Teta)*sinf(PI / 2 + Teta);

		//float U2y = (2 * m1*a.V*cosf(Alfa - Teta) + b.V*cosf(PI + Teta - Beta)*(m2 - m1))*cosf(Teta) / (m1 + m2) + b.V*sinf(PI + Teta - Beta)*cosf(PI / 2 + Teta);
		//float U2x = (2 * m1*a.V*cosf(Alfa - Teta) + b.V*cosf(PI + Teta - Beta)*(m2 - m1))*sinf(Teta) / (m1 + m2) + b.V*sinf(PI + Teta - Beta)*sinf(PI / 2 + Teta);

		float U1y = (2 * m2*b.V*cosf(Beta - Teta) + a.V*cosf(Alfa - Teta)*(m1 - m2))*cosf(Teta) / (m1 + m2) + a.V*sinf(Alfa - Teta)*cosf((PI / 2) + Teta);
		float U1x = (2 * m2*b.V*cosf(Beta - Teta) + a.V*cosf(Alfa - Teta)*(m1 - m2))*sinf(Teta) / (m1 + m2) + a.V*sinf(Alfa - Teta)*sinf((PI / 2) + Teta);
		std::cout << "U1y = " << U1y << " U1x = " << U1x << "\n";

		float U2y = (2 * m1*a.V*cosf(Alfa - Teta) + b.V*cosf(Beta - Teta)*(m2 - m1))*cosf(Teta) / (m1 + m2) + b.V*sinf(Beta - Teta)*cosf(PI / 2 + Teta);
		float U2x = (2 * m1*a.V*cosf(Alfa - Teta) + b.V*cosf(Beta - Teta)*(m2 - m1))*sinf(Teta) / (m1 + m2) + b.V*sinf(Beta - Teta)*sinf(PI / 2 + Teta);
		std::cout << "U2y = " << U2y << " U2x = " << U2x << "\n";

		std::cout << "a.angle = " << a.angle*180.f / PI << "  ";
		std::cout << "b.angle = " << b.angle*180.f / PI << "\n";
		std::cout << "---------------------"<< "\n";
		//a.angle = atanf(U1x / U1y); std::cout << "a.angle = " << a.angle*180.f/PI << "  ";
		//b.angle = atanf(U2x / U2y); std::cout << "b.angle = " << b.angle*180.f/PI << "\n";


		a.V = sqrt(pow(U1x, 2) + pow(U1y, 2)); std::cout << "a.V = " << a.V << "  ";
		b.V = sqrt(pow(U2x, 2) + pow(U2y, 2)); std::cout << "b.V = " << b.V << "\n";

		a.angle = get_radianAngle(U1x, U1y); std::cout << "a.angle = " << a.angle*180.f / PI << "  ";
		b.angle = get_radianAngle(U2x, U2y); std::cout << "b.angle = " << b.angle*180.f / PI << "\n";

	//	if (can_itBorn(a.x + 8.f*a.V*sinf(a.angle), a.y + 8.f*a.V*cosf(a.angle), a.radius, enem))
		//{
			a.x += a.V*sinf(a.angle);
			a.y += a.V*cosf(a.angle);
		//}
		//else std::cout << "ERROR\n";
		//if (can_itBorn(b.x + 8.f*b.V*sinf(b.angle), b.y + 8.f*b.V*cosf(b.angle), b.radius, enem))
		//{
			b.x += b.V*sinf(b.angle);
			b.y += b.V*cosf(b.angle);
		//}
		//else std::cout << "ERROR\n";
		a.clear = true;
		b.clear = true;
		std::cout << "=======================" << "\n";
	//}



}

void mouseCall(int event, int x, int y, int flags, void* userdata)
{
	switch (event)
	{
		case CV_EVENT_MOUSEMOVE :
			(*(cv::Point*)userdata).x = x;
			(*(cv::Point*)userdata).y = y;
			break;
		case CV_EVENT_LBUTTONDOWN :
			is_shut = true;
			break;
		case CV_EVENT_RBUTTONDOWN :
			std::cout << "mouse angle is " << atanf((float)x / (float)y)*180.f/PI << " degrees\n";
			break;
	}
}



int main()
{
	cv::Scalar black(0, 0, 0);
	cv::Scalar white(255, 255, 255);
	int rx = rand() % 1000;
	int ry = rand() % 500;
	cv::Point center(rx, ry);
	cv::Point mouse(0, 0);
	cv::Point2f gun(0.f, 0.f);
	cv::Mat screen(500, 1000, CV_8UC3, black);
	std::vector<Bullet> bullets;
	auto bul_it = bullets.begin();
	std::vector<Enemy> enemyes;
	auto en_it = enemyes.begin();
	unsigned int time_counter = 0;//hmm

	bool push_child_anamy = false;
	//std::vector<Enemy> child_en;
	std::vector<Enemy> child_en;
	auto ch_it = child_en.begin();

	int score = 0;

	int char_number = 0;
	while (true) //MAIN LOOP
	{


		++time_counter;
		if (time_counter == 1000000)
			time_counter = 0;
		screen.setTo(black);

    		char_number = cv::waitKey(1); //_getch();

			switch (char_number) //
			{
			case 119: //„r„r„u„‚„‡
				center.y -= 3;
				break;
			case 115: //„r„~„y„x
				center.y += 3;
				break;
			case 97: //„r„|„u„r„€ 
				center.x -= 3;
				break;
			case 100: //„r„„‚„p„r„€
				center.x += 3;
				break;
			case 27: goto exit;
			}

		center.x = (1000 + center.x) % 1000;
		center.y = (500 + center.y) % 500;

		cvSetMouseCallback("1", mouseCall, &mouse);
		

		float sinA = /*(float)*/((mouse.x - center.x) / sqrt(pow(mouse.x - center.x, 2) + pow(mouse.y - center.y, 2)));
		float cosA = /*(float)*/((mouse.y - center.y) / sqrt(pow(mouse.x - center.x, 2) + pow(mouse.y - center.y, 2)));

		if (sqrt(pow(mouse.x - center.x, 2) + pow(mouse.y - center.y, 2)) >= 20)
		{
			gun.y = center.y + 20.f * cosA;
			gun.x = center.x + 20.f * sinA;
		}
		else
		{
			gun.x = (float)(center.x + 20);
			gun.y = (float)(center.y);
		}

		if (is_shut)
		{
			Bullet temp_bullet;
			temp_bullet.x = gun.x;
			temp_bullet.y = gun.y;
			temp_bullet.sin = sinA;
			temp_bullet.cos = cosA;
			temp_bullet.timeToDie = false;
			if (temp_bullet.x >= 0 && temp_bullet.x < 1000 && temp_bullet.y >= 0 && temp_bullet.y < 500)
				bullets.push_back(temp_bullet);
			std::cout << "SHUT!\n";
			is_shut = false;
		}
		if (bullets.size() != 0)
		{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			bul_it = bullets.begin(); //buulets
			while (bul_it != bullets.end())
			{
				(*bul_it).x = (*bul_it).x + 1.05f * (*bul_it).sin;
				(*bul_it).y = (*bul_it).y + 1.05f * (*bul_it).cos;
				if ((*bul_it).x == 0 || (*bul_it).x == 999 || (*bul_it).y == 0 || (*bul_it).y == 499)
				{

					(*bul_it).timeToDie = true;
				}
				if ((*bul_it).timeToDie == true)
				{

					bul_it = bullets.erase(bul_it);
				}
				else
				{
					cv::circle(screen, cv::Point((int)(*bul_it).x, (int)(*bul_it).y), 1, white);
				}
				if (bul_it != bullets.end())
				{
					++bul_it;
				}
			}

////////////////////////////////////////////////////////////////////////////////////////////////////////
		}

		//Enemyes
		if (time_counter % 1500 == 0) //ADD enemyes
		{
			Enemy temp_enemy;
			temp_enemy.x = (float)(rand() % 1000);
			temp_enemy.y = (float)(rand() % 500);
			
			int ran_deg = rand() % 360;//(float)(rand() % 628)/10.f; 
			float ran_ang = ((float)ran_deg*PI) / 180.f;
			//ran_ang = fmod(ran_ang, PI);
			std::cout << "ran_ang = " << ran_ang << "\n";
			//temp_enemy.sin = sinf(ran_ang);
			//temp_enemy.cos = cosf(ran_ang);
			temp_enemy.angle = ran_ang;
			//temp_enemy.Vx = 3.f*cosf(temp_enemy.angle);
			//temp_enemy.Vy = 3.f*sinf(temp_enemy.angle);
			temp_enemy.V = 3.f;
			temp_enemy.timeToDie = false;
			temp_enemy.radius = 20;
			temp_enemy.clear = false;
			temp_enemy.clear_count = CL_CO;

			bool born = true;
			auto born_it = enemyes.begin();
			while (born_it != enemyes.end())
			{
				if (is_collision(*born_it, temp_enemy))
				{
					born = false;
					std::cout << "COLLISION\n";
					break;
				}
				++born_it;
			}
			if (born == true) enemyes.push_back(temp_enemy);

		}//////////////////////////////

		if (enemyes.size() != 0)
		{
			en_it = enemyes.begin();
			while (en_it != enemyes.end())
			{
				////////////////////////////moving calculate
				if (time_counter % 7 == 0)
				{
					(*en_it).x += (*en_it).V*sinf((*en_it).angle);//(*en_it).x + (*en_it).Vx;//3.5f*(*en_it).cos;
					(*en_it).y += (*en_it).V*cosf((*en_it).angle);//(*en_it).y + (*en_it).Vy;//3.5f*(*en_it).sin;
					if((*en_it).x > 0)
						(*en_it).x = (float)((1000 + (int)(*en_it).x) % 1000);
					else if ((*en_it).x < 0)
						(*en_it).x = (float)((1000 - 1) % 1000);
					if((int)(*en_it).y > 0)
						(*en_it).y = (float)((500 + (int)(*en_it).y) % 500);
					else if((int)(*en_it).y < 0)
						(*en_it).y = (float)((500 + - 1) % 500);
				}
				///////////////////////////////
				//////////////////////////////
				if ((*en_it).clear == true)
				{
					--(*en_it).clear_count;
					if ((*en_it).clear_count == 0)
					{
						(*en_it).clear = false;
						(*en_it).clear_count = CL_CO;
					}
				}


				///////////////////////////////

				auto temp_it = bullets.begin();
				while (temp_it != bullets.end())
				{
					float b_dist = sqrt(pow(((*en_it).x - (*temp_it).x), 2) + pow(((*en_it).y - (*temp_it).y), 2)); //chek distance

					if (b_dist < ((*en_it).radius + 5)) // is shot
					{
					
						if ((*en_it).radius == 20)
						{
							push_child_anamy = true;
						}

						(*temp_it).timeToDie = true;//bullet die

						(*en_it).radius -= 10; // become litle

						
					}
					if (temp_it != bullets.end())
					{
						++temp_it;
					}
				}
				

						if ((*en_it).radius == 0)
						{
							
							(*en_it).timeToDie = true;
							++score;

						}
				

				if (push_child_anamy == true)
				{


					//float father_angle = asinf((*en_it).sin);
					//float child_angle = fmod((father_angle + 3.14f/6.f), 6.28);
					float fan = 45.f * PI / 180.f;
					float chan = (-45.f) * PI / 180.f;
					std::cout << "(*en_it).angle = " << (*en_it).angle << "\n";
					(*en_it).angle += fan;
					std::cout << "(*en_it).angle = " << (*en_it).angle << "\n";
					//(*en_it).Vx = 3*cosf((*en_it).angle);
					//(*en_it).Vy = 3*sinf((*en_it).angle);
					//(*en_it).sin = sinf(father_angle);
					//(*en_it).cos = cosf(father_angle);
					(*en_it).x += 7.f * (*en_it).V * sinf((*en_it).angle);
					(*en_it).y += 7.f * (*en_it).V * cosf((*en_it).angle);

					Enemy t_en1;
					//t_en1.sin = sinf(child_angle);
					//t_en1.cos = cosf(child_angle);
					t_en1.angle = (*en_it).angle - fan + chan;
					std::cout << "t_en1.angle = " << t_en1.angle << "\n";
					//t_en1.Vx = 3*cosf(t_en1.angle);
					//t_en1.Vy = 3*sinf(t_en1.angle);
					t_en1.V = (*en_it).V;
					t_en1.x = (*en_it).x - 7.f * (*en_it).V * sinf((*en_it).angle) + 7.f * t_en1.V * sinf(t_en1.angle);
					t_en1.y = (*en_it).y - 7.f * (*en_it).V * cosf((*en_it).angle) + 7.f * t_en1.V * cosf(t_en1.angle); //7 is minimu for deete collision
					t_en1.radius = 10;
					t_en1.timeToDie = false;
					t_en1.clear = false;
					t_en1.clear_count = CL_CO;
						 
///////////////
					bool bornch = true;
					auto bornch_it = enemyes.begin();
					while (bornch_it != enemyes.end())
					{
						if (is_collision(*bornch_it, t_en1))
						{
							bornch = false;
							std::cout << "COLLISION\n";
							break;
						}
						++bornch_it;
					}
					if (bornch == true) child_en.push_back(t_en1);
///////////////

					//child_en.push_back(t_en1);
					//enemyes.push_back(t_en1);
					std::cout << "PUSH CHILD \n";
					push_child_anamy = false;
				}
				
				if ((*en_it).timeToDie == true)
				{
					
					en_it = enemyes.erase(en_it);
				}
				else
				{

					cv::circle(screen, cv::Point((int)(*en_it).x, (int)(*en_it).y), (*en_it).radius, white);
					//++en_it;
				}
				if (en_it != enemyes.end())
				{
					++en_it;
				}

			}
		} // if Enemies size != 0


		ch_it = child_en.begin();
		while (ch_it != child_en.end())
		{
			enemyes.push_back(*ch_it);
			++ch_it;
		}
		child_en.clear();

		auto bulk_it = enemyes.begin();
		while(bulk_it != enemyes.end())
		{
			auto bulk_temp_it = bulk_it + 1;
			while (bulk_temp_it != enemyes.end())
			{


				if (is_collision(*bulk_temp_it, *bulk_it))
					collision(*bulk_temp_it, *bulk_it);
				++bulk_temp_it;
			}
			++bulk_it;
		}

		cv::putText(screen, std::to_string(score), cv::Point(10,10), 1, 1, cv::Scalar(255,255,255));
		cv::circle(screen, center, 10, white);
		cv::line(screen, center, gun, white);
		cv::imshow("1", screen);
		//cv::waitKey(1);
	}
	exit:
	return 0;
}