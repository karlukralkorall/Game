#include <iostream> 
#include <windows.h>
#include <conio.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <cstdlib>

bool is_shut = false; //global, so bad!

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
	float sin;
	float cos;
	int radius;
	bool timeToDie;
	//std::vector<float> dist_to_bullet;
};

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
		

		float cosA = (float)((mouse.x - center.x) / sqrt(pow(mouse.x - center.x, 2) + pow(mouse.y - center.y, 2)));
		float sinA = (float)((mouse.y - center.y) / sqrt(pow(mouse.x - center.x, 2) + pow(mouse.y - center.y, 2)));

		if (sqrt(pow(mouse.x - center.x, 2) + pow(mouse.y - center.y, 2)) >= 20)
		{
			gun.x = center.x + 20 * cosA;
			gun.y = center.y + 20 * sinA;
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
				(*bul_it).x = (*bul_it).x + 1.05f * (*bul_it).cos;
				(*bul_it).y = (*bul_it).y + 1.05f * (*bul_it).sin;
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
		if (time_counter % 1000 == 0) //ADD enemyes
		{
			Enemy temp_enemy;
			temp_enemy.x = (float)(rand() % 1000);
			temp_enemy.y = (float)(rand() % 500);
			
			float ran_ang = (float)(rand() % 628)/10.f; 
			temp_enemy.sin = sinf(ran_ang);
			temp_enemy.cos = cosf(ran_ang);
			temp_enemy.timeToDie = false;
			temp_enemy.radius = 20;
			enemyes.push_back(temp_enemy);

		}//////////////////////////////

		if (enemyes.size() != 0)
		{
			en_it = enemyes.begin();
			while (en_it != enemyes.end())
			{
				////////////////////////////moving calculate
				if (time_counter % 7 == 0)
				{
					(*en_it).x = (*en_it).x + 3.5f*(*en_it).cos;
					(*en_it).y = (*en_it).y + 3.5f*(*en_it).sin;
					(*en_it).x = (float)((1000 + (int)(*en_it).x) % 1000);
					(*en_it).y = (float)((500 + (int)(*en_it).y) % 500);
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

						}
				

				if (push_child_anamy == true)
				{


					float father_angle = asinf((*en_it).sin);
					float child_angle = fmod((father_angle + 3.14f/6.f), 6.28);
					father_angle = fmod((father_angle - 3.14f / 6.f), 6.28);
					(*en_it).sin = sinf(father_angle);
					(*en_it).cos = cosf(father_angle);
					(*en_it).x += 10*(*en_it).cos;
					(*en_it).y += 10*(*en_it).sin;

					Enemy t_en1;
					t_en1.sin = sinf(child_angle);
					t_en1.cos = cosf(child_angle);
					t_en1.x = (*en_it).x + 10*t_en1.cos;
					t_en1.y = (*en_it).y + 10*t_en1.sin;
					t_en1.radius = 10;
					t_en1.timeToDie = false;

					child_en.push_back(t_en1);
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
				float dist = sqrt(pow(((*bulk_it).x - (*bulk_temp_it).x), 2) + pow(((*bulk_it).y - (*bulk_temp_it).y), 2));
				if ((*bulk_temp_it).radius == (*bulk_it).radius && (*bulk_it).radius == 10)
				{
					if (dist < 10)
					{
						float tm_cos = (*bulk_it).cos;
						float tm_sin = (*bulk_it).sin;
						(*bulk_it).cos = (*bulk_temp_it).cos;
						(*bulk_it).sin = (*bulk_temp_it).sin;
						(*bulk_temp_it).cos = tm_cos;
						(*bulk_temp_it).sin = tm_sin;

						(*bulk_temp_it).x += 10*(*bulk_temp_it).cos;
						(*bulk_temp_it).y += 10 * (*bulk_temp_it).sin;
						(*bulk_it).x += 10 * (*bulk_it).cos;
						(*bulk_it).y += 10 * (*bulk_it).sin;
					}
				}
				else if ((*bulk_temp_it).radius == (*bulk_it).radius && (*bulk_it).radius == 20)
				{
					if (dist < 40)
					{
						float tm_cos = (*bulk_it).cos;
						float tm_sin = (*bulk_it).sin;
						(*bulk_it).cos = (*bulk_temp_it).cos;
						(*bulk_it).sin = (*bulk_temp_it).sin;
						(*bulk_temp_it).cos = tm_cos;
						(*bulk_temp_it).sin = tm_sin;

						(*bulk_temp_it).x += 10 * (*bulk_temp_it).cos;
						(*bulk_temp_it).y += 10 * (*bulk_temp_it).sin;
						(*bulk_it).x += 10 * (*bulk_it).cos;
						(*bulk_it).y += 10 * (*bulk_it).sin;
					}
				}
				else if ((*bulk_temp_it).radius != (*bulk_it).radius)
				{
					if (dist < 30)
					{
						float tm_cos = (*bulk_it).cos;
						float tm_sin = (*bulk_it).sin;
						(*bulk_it).cos = (*bulk_temp_it).cos;
						(*bulk_it).sin = (*bulk_temp_it).sin;
						(*bulk_temp_it).cos = tm_cos;
						(*bulk_temp_it).sin = tm_sin;

						(*bulk_temp_it).x += 10 * (*bulk_temp_it).cos;
						(*bulk_temp_it).y += 10 * (*bulk_temp_it).sin;
						(*bulk_it).x += 10 * (*bulk_it).cos;
						(*bulk_it).y += 10 * (*bulk_it).sin;
					}
				}

				++bulk_temp_it;
			}
			++bulk_it;
		}

		cv::circle(screen, center, 10, white);
		cv::line(screen, center, gun, white);
		cv::imshow("1", screen);
		//cv::waitKey(1);
	}
	exit:
	return 0;
}