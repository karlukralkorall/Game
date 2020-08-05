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

		bul_it = bullets.begin();
		while (bul_it != bullets.end())
		{
			(*bul_it).x = (*bul_it).x + 1.05f * (*bul_it).cos;
			(*bul_it).y = (*bul_it).y + 1.05f * (*bul_it).sin;
			if ((*bul_it).x == 0 || (*bul_it).x == 999 || (*bul_it).y == 0 || (*bul_it).y == 499)
			{
				bullets.erase(bul_it);
				//bul_it = bullets.begin();
				break;
			}
			else
				cv::circle(screen, cv::Point((int)(*bul_it).x, (int)(*bul_it).y), 1, white);

			++bul_it;
		}

		//Enemyes
		if (time_counter % 1000 == 0)
		{
			Enemy temp_enemy;
			temp_enemy.x = rand() % 1000;
			temp_enemy.y = rand() % 500;
			temp_enemy.sin = (rand() % 10) / 10;
			temp_enemy.cos = (rand() % 10) / 10;
			enemyes.push_back(temp_enemy);
		}
		en_it = enemyes.begin();
		while (en_it != enemyes.end())
		{
			cv::circle(screen, cv::Point((int)(*en_it).x, (int)(*en_it).y), 10, white);
			++en_it;
		}
		

		cv::circle(screen, center, 10, white);
		cv::line(screen, center, gun, white);
		cv::imshow("1", screen);
		//cv::waitKey(1);
	}
	exit:
	return 0;
}